/************************************************************************ 
* �ļ�����    log.h 
* �ļ�������  ���ڼ�¼��־����CLog  
* �汾�ţ�    2.0 
* ��־��ʶ��  0085
************************************************************************/ 

#ifndef _LOG_H
#define _LOG_H

#include "boost/thread.hpp"

class CLog
{
public:
	CLog();
	~CLog();

public:
	void InitLog(const char* lpszLogPath = "./");		
	void Add(const char* fmt, ...);		//������֣������͸�printfһ��
	
protected:

	enum {BUFSIZE = 4096};  //����������
	char  m_tBuf[BUFSIZE];
	char m_strLogPath[256];
	boost::mutex mutex_;
};

#endif