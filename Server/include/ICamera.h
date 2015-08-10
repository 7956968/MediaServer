#pragma once
#include <memory.h>

class IData
{
public:
#define STR_LEN 64
	typedef struct _DBCAMERAINFO_
	{
		int		CameraID;
		char	CameraName[STR_LEN];
		char	UnitName[STR_LEN];
		char	Input[256];
		char	Remark[STR_LEN];
		int		DevID;
		int		DevType;
		char	sControlIP[STR_LEN];
		int     CameraType;
		int     isMatrix;
		_DBCAMERAINFO_()
		{
			CameraID	= 0;
			memset(CameraName, 0, STR_LEN);
			memset(UnitName, 0, STR_LEN);
			memset(Remark, 0, STR_LEN);
			DevID		= 0;
			DevType		= 0;
			CameraType = 0;
			memset(sControlIP, 0, STR_LEN);
			memset( Input, 0, 256 );
			isMatrix = 0;
		}
	} DBCAMERAINFO;

	typedef struct _DBDEVINFO_
	{
		int		DevID;
		char	IPAddr[STR_LEN];
		int		Port;
		char	User[STR_LEN];
		char	Pwd[STR_LEN];
		int		DevType;

		_DBDEVINFO_()
		{
			DevID	= 0;
			memset(IPAddr, 0, STR_LEN);
			Port	= 0;
			memset(User, 0, STR_LEN);
			memset(Pwd, 0, STR_LEN);
			DevType	= 0;
		}

	} DBDEVINFO;

	typedef struct _DBCODERINFO_ 
	{
		int		CoderID;
		char	IPAddr[STR_LEN];
		int		Port;
		char	User[STR_LEN];
		char	Pwd[STR_LEN];
		int     Input;
		int		OutPut;
		int		MatrixID;
		_DBCODERINFO_()
		{
			CoderID	= 0;
			memset(IPAddr, 0, STR_LEN);
			Port	= 0;
			memset(User, 0, STR_LEN);
			memset(Pwd, 0, STR_LEN);
			OutPut	= 0;
			MatrixID= 0;
		}
	} DBCODERINFO;

	typedef struct _DBMATRIXINFO_
	{
		int MatrixID;
		int MatrixType;
		int	Baud;
		int DataBit;
		int StartBit;
		int StopBit;
		int Parity;
		int FlowControl;
		char SoftConIPAddr[STR_LEN];//�������IP��ַ
		DBCODERINFO ControlCoder;
		_DBMATRIXINFO_()
		{
			MatrixID	= 0;
			MatrixType	= 0;
			Baud		= 0;
			DataBit		= 0;
			StartBit	= 0;
			StopBit		= 0;
			Parity		= 0;
			FlowControl	= 0;
			memset(SoftConIPAddr, 0, STR_LEN);
		}
	} DBMATRIXINFO;

public:  
	virtual ~IData() {}
	virtual int getCameraInfo(int cameraid, DBCAMERAINFO* camera) = 0;     //��ȡ�������Ϣ
	virtual int getCoderInfo(int CoderID, DBCODERINFO* coder) = 0;	       //��ȡ������ͨ����Ϣ
	virtual int getMatrixInfo(int MatrixID, DBMATRIXINFO* MatrixInfo) = 0; //��ȡ������Ϣ
	virtual int getDevInfo(int DevID, DBDEVINFO* dev) = 0;	               //��ȡ�豸��Ϣ
};

typedef struct tagResultParam
{
	int resultType;
	union tagResultInfo{
		int fullCameraID[63];//�Խ�ƽ̨ʱ����Ϊͨ����ʱ���������ڴ򿪵������ID��
		char strResult[252];//openCamera��switchCamera���ش���ʱ���洢������Ϣ�����ݸ��ͻ�����ʾ
	}unResultInfo;
}RESULT_PARAM_S;

#ifndef WIN32
#ifndef __stdcall
#define __stdcall __attribute__((__stdcall__))
#endif
#endif

typedef bool (__stdcall *VIDEO_CAPTURE_CALLBACKEX)(unsigned long dwDataType, unsigned long bufsize, unsigned char *buffer, int user);

class ICamera
{
public:
	ICamera(): m_pIData(NULL){} //���캯����m_pIData��δ��ֵ����ʹ��,���ڵ���StartRTSPServer֮��ʹ��
	virtual ~ICamera() {}
	void SetIData( IData* pIData ){ m_pIData = pIData; } //�ڵ���StartRTSPServer������m_pIData����
	virtual int openCamera(int CameraID, int ChannelID, int user, VIDEO_CAPTURE_CALLBACKEX capture_callback, RESULT_PARAM_S *resultParam) = 0;
	virtual int closeCamera(int CameraID) = 0;
	virtual int switchCamera(int CameraID, int oldCameraID, int ChannelID, int user, VIDEO_CAPTURE_CALLBACKEX capture_callback) = 0;
	virtual int controlCamera(int CameraID, int ChannelID, int ControlType, int ControlSpeed) = 0;
protected:
	IData* m_pIData;
};

typedef struct _XMLCAMERAINFO_
{
	char chCameraName[64];	   //���������
	char chCameraInput[256];   //�����Ψһ��ʶ
	int  iOnline;              //0-������ 1-���ߣ�
	int  iStatus;              //0-���� 1-�޷��� 2-�����������
	char chSmX[64];			   //����
	char chSmY[64];			   //γ��
	int  iUnitID;			   //iUnitID<=0ʱ Ĭ��ȡDevID��ǰ��λ ����㵥λID=AccessPointID
	char chRemark[64];         //��ע MS����ʱ��ʽ�������ID&21����&�Ƿ�ɿ�&����
	_XMLCAMERAINFO_()
	{
		memset(chCameraName, 0, 64);
		memset(chCameraInput, 0, 256);
		iOnline		= 1;
		iStatus		= 0;
		memset(chSmX, 0, 64);
		memset(chSmY, 0, 64);
		iUnitID = -1;
		memset(chRemark, 0, 64);
	}
}STCAMERAINFO;

class IDevice
{
public:
	IDevice(){} //���캯����m_pIData ��δ��ֵ ����ʹ��
	virtual ~IDevice() {}
	void SetIData( IData* pIData ){ m_pIData = pIData; } //�ڵ���StartRTSPServer������m_pIData����

	//outResultΪMSDLL�ڲ������ڴ��СΪ65536
	//���ʽӿڷ�ʽ��http://IP:PORT/getDeviceInfo?AAA&BBBB ����IPΪMSIP PORTΪMSRTSPPORT pParamΪ AAA&BBBB
	virtual int getDeviceInfo(const char* pParam, char* outResult) = 0;
protected:
	IData* m_pIData;
};