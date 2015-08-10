#include "PreAllocateDisk.h"

#include <boost/filesystem.hpp>

#ifdef WIN32
#include "windows.h"
#else
#include <sys/statfs.h>
#endif

namespace fs = boost::filesystem;

namespace MediaSave
{

CPreAllocateDisk::CPreAllocateDisk(void)
	:m_bPreAllocDisking(false),
	m_isPTZAutoBack(false),
	m_bCacheIsHaveEnoughDisk(true)
{
}

CPreAllocateDisk::~CPreAllocateDisk(void)
{
}

bool CPreAllocateDisk::InitStorageDirArray(const char* storagePath)
{
	// read database
	m_storagePath = storagePath;
	m_log.InitLog("./log/MR-");
	m_log.Add("CPreAllocateDisk::InitStorageDirArray...");
	bool bret = storagedir_array[0].InitStorageDir(0, 0, storagePath, this);
	if(bret)
		return storagedir_array[0].InitPackageArray();
	return bret;
}

bool CPreAllocateDisk::IsHaveEnoughDisk(CStorageDir& storagedir_)
{
	//�������������ж��Ƿ���Ҫ����Ӳ�̿ռ�
	if( !m_bCacheIsHaveEnoughDisk )return m_bCacheIsHaveEnoughDisk;

#ifdef WIN32
	unsigned __int64  i64FreeBytesToCaller;
	unsigned __int64  i64TotalBytes;
	unsigned __int64  i64FreeBytes;
	float FreeGB = 0;
	float TotalGB = 0;
	BOOL  fResult = GetDiskFreeSpaceEx (m_storagePath.substr(0,3).c_str(),
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);
	if(fResult){
		FreeGB = (float)((UINT)(i64FreeBytesToCaller/1024/1024))/1024;
		if( FreeGB < 10.0 ) {
			m_bCacheIsHaveEnoughDisk = false;
			return false;
		}
	}
#else
	struct statfs diskInfo; 
	statfs(m_storagePath.c_str(), &diskInfo); 

	unsigned long long blocksize = diskInfo.f_bsize; //ÿ��block��������ֽ��� 
	unsigned long long totalsize = blocksize * diskInfo.f_blocks; //�ܵ��ֽ�����f_blocksΪblock����Ŀ 

	unsigned long long freeDisk = diskInfo.f_bfree * blocksize; //ʣ��ռ�Ĵ�С 
	unsigned long long availableDisk = diskInfo.f_bavail * blocksize; //���ÿռ��С 
	double FreeGB = availableDisk>>30;
	if( FreeGB < 10.0 ){
		m_bCacheIsHaveEnoughDisk = false;
		return false;
	}
	//printf("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n", 
	//	freeDisk>>20, freeDisk>>30, availableDisk>>20, availableDisk>>30); 
#endif
	return true;
}

bool CPreAllocateDisk::ApplyPreAllocateDisk()
{
	if ( IsHaveEnoughDisk(storagedir_array[0]) 
		&& !m_bPreAllocDisking )
	{
		m_bPreAllocDisking = true;
		bool  bret = storagedir_array[0].CreatePackageFiles();
		m_bPreAllocDisking = false;
		m_fileIndexList.SetCacheHaveFreeFile();
		return bret;
	}
	return false;
}

CFileIndexList::CFileIndexPtr CPreAllocateDisk::GetFreeFileIndex()
{
	if( !m_fileIndexList.IsHaveFreeFiles() )
		ApplyPreAllocateDisk();
	return m_fileIndexList.GetFreeFileIndex();
}

void CPreAllocateDisk::PrintPackageArray()
{
	storagedir_array[0].PrintPackageArray();
}

}// end namespace
