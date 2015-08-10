#ifndef _VNMP_NET_SDK_2014_08_05
#define _VNMP_NET_SDK_2014_08_05

#ifdef	WIN32
#define  VIDDEVSDK	_declspec(dllexport) 
#else
#define  VIDDEVSDK	
#endif

#ifndef WIN32
#ifndef __stdcall
#define __stdcall __attribute__((__stdcall__))
#endif
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

/************************************************************************/
/*				��Ƶ�豸���ݽṹ				                        */
/************************************************************************/
typedef struct tagVIDEODEVICE
{
	//�豸��¼��Ϣ���û��ṩ��SDK����Ϣ��
	char		ip[64];			/*�豸IP��ַ*/
	int			port;			/*�豸�˿ں�*/
	char		username[64];	/*�û���¼����*/
	char		password[64];	/*�û���¼����*/
	
	//������ʶ��Ƶ��Ϣ
	char		channel[256];		/*��ƵԴ��Ӧ��ͨ����*/
	
	//��SDK�ڲ�ʹ��(�洢��¼������������)
	int			handle;			/*�豸���*/
	int			devnum;			/*�豸���*/
	
	//�����ֶΣ�������չ����(�ڲ�ʹ�ã���Э��ʹ��)
	int			reuse;			/*�����ֶ�*/
	char		reserve[256];	/*�����ֶ�*/
}VIDEODEVICE;

/************************************************************************/
/*					�ص�����                                            */
/************************************************************************/

//��Ƶͼ�񻺳����������ݵĻ�ȡ
typedef void (__stdcall *VIDEO_CAPTURE_CALLBACK)(unsigned long dwDataType, unsigned long bufsize, unsigned char *buffer, unsigned long user);

typedef bool (__stdcall *VIDEO_CAPTURE_CALLBACKEx)(unsigned long dwDataType, unsigned long bufsize, unsigned char *buffer,  void* user);

//�л��ɹ����µ���Ƶ�������������Ϣ�ص�����ԭ��
typedef void (__stdcall *VIDEO_SWITCH_CAMERA_UPDATE_CALLBACK)(unsigned long dwOldCameraID,unsigned long dwNewCameraID, unsigned char *NewCameraName, unsigned long user);

/********************Ԥ���ص�����*********************/
#define NET_DVR_SYSHEAD         1   //ϵͳͷ����
#define NET_DVR_STREAMDATA      2   //��Ƶ�����ݣ�����������������Ƶ�ֿ�����Ƶ�����ݣ�
#define NET_DVR_AUDIOSTREAMDATA 3   //��Ƶ������
#define NET_DVR_STD_VIDEODATA   4   //��׼��Ƶ������
#define NET_DVR_STD_AUDIODATA   5   //��׼��Ƶ������

/*************��������Э�鶨�� ..capturing_start VIDEODEVICE.reuse********/
#define VNMP_TRANSPORT_UDP		0x1000   //RTP over UDP      0x1000
#define VNMP_TRANSPORT_TCP		0x1001   //RTP over TCP	     0x1001
#define VNMP_TRANSPORT_RAW		0x1002   //RAW+12 over TCP	 0x1002 //Ĭ��

/************************************************************************/
/*					          �����붨��                                */
/************************************************************************/
#define ERROR_PROCESS_SUCCESS		0		//�ɹ�

#define ERROR_CAMERA_INVALID		-11		//��Ч�����ID
#define ERROR_CAMERA_NOLEVEL		-12		//�Բ�������Ȩ������ͼ��
#define ERROR_CODEC_FULL			-13		//�޿���ͨ��
#define ERROR_CAMERA_DISLINE		-14		//�����������
#define ERROR_CAMERA_FAILURE		-15		//��ƵԴ����ʧ��

#define ERROR_CAMERA_NOOPEN			-20		//�����δ��
#define ERROR_CAMERA_NOCONTROL		-21		//��������ܿ���
#define ERROR_CONTROL_NOLEVEL		-22		//��Ȩ���ƴ������
#define ERROR_SWITCH_REOPEN			-23		//�л�ʧ�ܣ������´�
#define ERROR_SWITCH_REOPEN_FIALED	-24		//�л�ʧ��
#define ERROR_SWITCH_NOLEVEL		-31		//�û�Ȩ�޲������е����û������,�޷��л�
#define ERROR_SWITCH_ERROR			-32		//�л���ʧ��
#define ERROR_NEWCAMERA_NOLEVEL		-33		//�û���Ȩ����Ҫ�л��������

#define ERROR_REPLAY_ERROR			-41		//������޷��ط�
#define ERROR_REPLAY_NOLEVEL		-42		//�û���Ȩ�ط�

#define ERROR_SYSTEM_ERROR			-100	//ϵͳ����

#define ERROR_SOCKET_CRAETE			-50		//SOCKET����ʧ��,������Դ����,�����ǿͻ��˹���
#define ERROR_SOCKET_SEND			-51		//��������ʧ�ܣ��������֮����������
#define ERROR_SOCKET_RECV			-52		//û����Ӧ�����������֮����������
#define ERROR_SOCKET_CONNECT		-53		//��������ʧ�ܣ��������֮����������
#define ERROR_SOCKET_OVERTIME		-54		//������Ӧ��ʱ���������֮����������

#define ERROR_GIVEUP_SELECT			-61		//����ѡ��ռ��������Ĵ�	
#define ERROR_PARAMETER_INVALID		-62		//��������	
#define ERROR_USER_OVERNUM			-65		//����dll֧�ֵ�����û�������

#define ERROR_NET_FAILED            -201    //�������
#define ERROR_OPEN_CAMERA           -202    //��ʧ��

/************************************************************************/
/*					�ӿڵĳ�ʼ�����ͷ�                                  */
/************************************************************************/
VIDDEVSDK int __stdcall  video_device_init();
VIDDEVSDK int __stdcall  video_device_destroy();

/************************************************************************/
/*					��ý���������¼��ע������                          */
/************************************************************************/
VIDDEVSDK int __stdcall video_device_login(VIDEODEVICE *videodev);
VIDDEVSDK int __stdcall video_device_logout(VIDEODEVICE *videodev);

/************************************************************************/
/*					�л�ʱ��������Ƹ��»ص�����ע��			        */
/************************************************************************/
VIDDEVSDK void __stdcall video_device_set_camera_switch_update(VIDEODEVICE *videodev,
															   VIDEO_SWITCH_CAMERA_UPDATE_CALLBACK camera_switch_update_callback, unsigned long user);

/************************************************************************/
/*					��Ƶ���俪ʼ��ֹͣ����				                */
/************************************************************************/
VIDDEVSDK int __stdcall video_device_capturing_start(VIDEODEVICE *videodev,
													 unsigned long user, VIDEO_CAPTURE_CALLBACK capture_callback);

VIDDEVSDK int __stdcall video_device_capturing_startEx(VIDEODEVICE *videodev, void* user, VIDEO_CAPTURE_CALLBACKEx capture_callback);

VIDDEVSDK int __stdcall video_device_capturing_stop(VIDEODEVICE *videodev);

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
//#define CAMERA_COMMAND_MONITOR_CAMERA_SELECT 43 //������л���MONITOR_CAMERA��ѡ��

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
VIDDEVSDK int __stdcall video_device_cameracontrol(VIDEODEVICE *videodev, int ControlType, int ControlSpeed);


/************************************************************************/
/*							��Ƶ�ط����								*/
/************************************************************************/

//���ݽṹ
typedef struct{
	unsigned long dwYear;		//��
	unsigned long dwMonth;		//��
	unsigned long dwDay;		//��
	unsigned long dwHour;		//ʱ
	unsigned long dwMinute;		//��
	unsigned long dwSecond;		//��
}REPALY_TIME, *LPREPLAY_TIME;

//ʱ����
typedef struct tagTimeSlice{
	REPALY_TIME sBeginTime;
	REPALY_TIME sEndTime;
}TIME_SLICE;

//��Ƶͼ�񻺳����������ݵĻ�ȡ ���лط���dwParamΪʱ�䵥λΪ�� /*������dwParamΪ���ص��ļ���С��λΪ�ֽ�*/
typedef void (__stdcall *VIDEO_PLAYBACK_CALLBACK)(unsigned long dwParam,unsigned long dwDataType, unsigned long bufsize,
	unsigned char *buffer, unsigned long user);

//��ѯָ��ʱ��¼��
VIDDEVSDK int __stdcall video_device_FindPlayBackByTime(
	VIDEODEVICE *videodev, 
	IN TIME_SLICE* pRecordTimeSlice,  /*¼���ļ���Ϣ*/
	char* pPlayBackInfo,
	unsigned long PlayBackInfoSize);

//�ط�ָ��ʱ��¼��
VIDDEVSDK int __stdcall video_device_PlayBackByTime(
	VIDEODEVICE *videodev, 
	IN TIME_SLICE* pRecordTimeSlice,  /*¼���ļ���Ϣ*/
	VIDEO_PLAYBACK_CALLBACK playback,       
	unsigned long dwUser);

//����ָ��ʱ��¼��
VIDDEVSDK int __stdcall video_device_DownloadByTime(
	VIDEODEVICE *videodev, 
	IN TIME_SLICE* pRecordTimeSlice,  /*¼���ļ���Ϣ*/
	VIDEO_PLAYBACK_CALLBACK playback,       
	unsigned long dwUser);

//�طſ�����ز���
#define PLAY_BACK_START 		1//��ʼ����
#define PLAY_BACK_STOP			2//ֹͣ����
#define PLAY_BACK_PAUSE 		3//��ͣ����
#define PLAY_BACK_RESTART		4//�ָ�����
#define PLAY_BACK_FAST 			5//���
#define PLAY_BACK_SLOW 			6//����
#define PLAY_BACK_SKIPTIME 		7//ѡ��ʱ��

//�طſ���
VIDDEVSDK int __stdcall video_device_PlayBackControl(
	VIDEODEVICE *videodev,
	unsigned long dwControlCode,          /*��������*/
	unsigned long dwParam);               /*���Ʋ���*/

//ֹͣ�ط�
VIDDEVSDK int __stdcall video_device_StopPlayBack(VIDEODEVICE *videodev);

//ֹͣ����
VIDDEVSDK int __stdcall video_device_StopDownload(VIDEODEVICE *videodev);
#endif //_VNMP_NET_SDK_2014_08_05
