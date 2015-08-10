#pragma once


#ifdef	WIN32
#define  MSDLL	_declspec(dllexport) 
#else
#define  MSDLL	
#endif

#include "ICamera.h"

MSDLL int __stdcall StartRTSPServer(ICamera* pICamera);

MSDLL void __stdcall StopRTSPServer();

////////////////////////////��չ�ӿ�///////////////////////////////////////////////////////
//���ʹ�����Ϣ���ͻ���
MSDLL int  __stdcall SendCameraError(int user,const char* chErrorInfo);
//����ת��ʱ�л�����
MSDLL int  __stdcall UpdateSwitchCameraInfo(int oldCameraID, int iNewCameraID);
//����������չ�ӿ�
MSDLL void __stdcall SetIDevice(IDevice* pIDevice);
////////////////////////////////////////////////////////////////////////////////////////////

#define ERROR_LOADMSDB_FAILED              -101          //����MS���ݿ�ʧ��
#define ERROR_RTSPPORT_CONFIG_FAILED       -102          //����rtsp�˿� ����
#define ERROR_STARTLISTEN_RTSPPORT_FAILED  -103			 //����rtsp�˿�ʧ��
#define ERROR_VIDEOPATH_CONFIG_FAILED      -104          //¼��·�� ���ô���
#define ERROR_MEDIASAVE_INIT_FAILED        -105          //¼��ģ������ʧ��
#define ERROR_LOADMRDB_FAILED			   -106          //���ش洢���ݿ�ʧ��


/************************************************************************/
/*					�������̨���Ƶ�֧��								*/
/************************************************************************/
//��̨������ز���
#define CAMERA_COMMAND_STOP				0	    //ֹͣ
#define CAMERA_COMMAND_UP				1	    //��̨��
#define CAMERA_COMMAND_UPSTOP           10001	//��̨��ֹͣ
#define CAMERA_COMMAND_DOWN				2		//��̨��
#define CAMERA_COMMAND_DOWNSTOP		    10002   //��̨��ֹͣ
#define CAMERA_COMMAND_LEFT				3		//��̨��
#define CAMERA_COMMAND_LEFTSTOP		    10003	//��̨��ֹͣ
#define CAMERA_COMMAND_RIGHT			4		//��̨��
#define CAMERA_COMMAND_RIGHTSTOP	    10004	//��̨��ֹͣ

#define CAMERA_COMMAND_ZOOM_IN			5		//��ͷ��
#define CAMERA_COMMAND_ZOOM_INSTOP	    10005	//��ͷ��ֹͣ
#define CAMERA_COMMAND_ZOOM_OUT			6		//��ͷԶ
#define CAMERA_COMMAND_ZOOM_OUTSTOP	    10006	//��ͷԶֹͣ

#define CAMERA_COMMAND_FOCUS_NEAR		7		//�����
#define CAMERA_COMMAND_FOCUS_NEARSTOP	10007	//�����ֹͣ
#define CAMERA_COMMAND_FOCUS_FAR		8		//����Զ
#define CAMERA_COMMAND_FOCUS_FARSTOP	10008	//����Զֹͣ

#define CAMERA_COMMAND_LIGHT_CLOSE		9		//��Ȧ��
#define CAMERA_COMMAND_LIGHT_CLOSESTOP	10009	//��Ȧ��ֹͣ
#define CAMERA_COMMAND_LIGHT_OPEN		10		//��Ȧ��
#define CAMERA_COMMAND_LIGHT_OPENSTOP	100010	//��Ȧ��ֹͣ

#define CAMERA_COMMAND_BRUSH_CLOSE		11		//��ˢ��
#define CAMERA_COMMAND_BRUSH_OPEN		12		//��ˢ��
#define CAMERA_COMMAND_RAY_CLOSE		13		//�ƹ��
#define CAMERA_COMMAND_RAY_OPEN		    14		//�ƹ⿪

#define CAMERA_COMMAND_SET_PRESET		15		//Ԥ��λ����
#define CAMERA_COMMAND_GOTO_PRESET		16		//Ԥ��λ����
#define CAMERA_COMMAND_DEL_PRESET		17		//Ԥ��λɾ��

#define CAMERA_COMMAND_CAMERA_SELECT	21		//�����ѡ��
#define CAMERA_COMMAND_CAMERA_LOCK		22		//���������
#define CAMERA_COMMAND_CAMERA_UNLOCK	23		//���������

//#define CAMERA_COMMAND_MONITOR_SELECT	31		//������ѡ��
#define CAMERA_COMMAND_MONITOR_CAMERA_SELECT 43 //������л���MONITOR_CAMERA��ѡ��

#define CAMERA_COMMAND_UP_LEFT			51	    //��̨����
#define CAMERA_COMMAND_UP_LEFTSTOP      10051	//��̨����ֹͣ
#define CAMERA_COMMAND_UP_RIGHT			52	    //��̨����
#define CAMERA_COMMAND_UP_RIGHTSTOP     10052	//��̨����ֹͣ
#define CAMERA_COMMAND_DOWN_LEFT		53		//��̨����
#define CAMERA_COMMAND_DOWN_LEFTSTOP	10053   //��̨����ֹͣ
#define CAMERA_COMMAND_DOWN_RIGHT		54		//��̨����
#define CAMERA_COMMAND_DOWN_RIGHTSTOP	10054   //��̨����ֹͣ

//Ѳ������ʱparam1Ϊ������� 
//Ѳ������=61 ����Ԥ�õ����Ѳ������ʱparam2����Ϊ��
//��һ���ֽڣ�Ԥ�õ�
//�ڶ����ֽڣ�ͣ��ʱ��
//�������ֽڣ��ٶ�
#define CAMERA_COMMAND_CRUISE_ADD	    61		//��Ԥ�õ����Ѳ������
#define CAMERA_COMMAND_CRUISE_DEL		62		//��Ԥ�õ��Ѳ��������ɾ��
#define CAMERA_COMMAND_CRUISE_CALL	    63		//��ʼѲ��
#define CAMERA_COMMAND_CRUISE_CALLSTOP	64		//ֹͣѲ��

//ControlSpeed ��Χ��1-7�� ControlType Ϊ������̨����
#define PLAY_BACK_GETTIME 		        208//��ȡ��ǰ�ط�ʱ��