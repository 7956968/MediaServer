
#include "NetClientSession.h"
#include "Xmlparser.h"

extern T_ServerParam SerParam;

NetClientSession::NetClientSession(NetServer* ourServer,unsigned int sessionId)
:fOurServer(ourServer),fOurSessionId(sessionId),
fSessionIsActive(true), client_socket_(ourServer->Get_io_service()),
stop_(true)
{
	memset(fAcceptBuffer, 0 ,BUFFER_SIZE);
	memset(fBuffer, 0 ,BUFFER_SIZE_MIN);
	PKGNUM = 0;
	totalRecive = 0;
	m_DeivceType = 0;
}

NetClientSession::~NetClientSession()
{
	boost::system::error_code ec;
	client_socket_.shutdown(boost::asio::socket_base::shutdown_both,ec);
	client_socket_.close(ec);
	fOurServer->m_log.Add("NetClientSession::~NetClientSession fOurSessionId = %d", fOurSessionId);
}

int NetClientSession::Start()
{
	stop_ = false;
	int iret = -1;
	incomingAcceptHandler();
	iret = 0;

	fOurServer->m_log.Add("NetClientSession::Start fOurSessionId = %d", fOurSessionId);
	return iret;
}

void NetClientSession::incomingAcceptHandler()
{
	try {
		  client_socket_.async_read_some(
			boost::asio::buffer(fBuffer,1024),
			boost::bind(&NetClientSession::HandleRecvReponse,
			shared_from_this(),boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	catch (...){  }

}

void NetClientSession::mcu_register(char* buffer ,int size)
{
	T_MCU_VDCS_REGISTER  t_register;
	memcpy(&t_register,buffer,sizeof(T_MCU_VDCS_REGISTER));
	m_DeivceType = t_register.ClientType;
}

void NetClientSession::mcu_register_ack()
{	
	T_PacketHead			 t_PackHeadRegAck;
	T_VDCS_MCU_REGISTER_ACK  t_register_ack;

	char buff[28+23] = {0};
	
	t_PackHeadRegAck.magic			 =  T_PACKETHEAD_MAGIC;
	t_PackHeadRegAck.cmd			 =  SM_VDCS_MCU_REGISTER_ACK;
	t_PackHeadRegAck.UnEncryptLen	 =  sizeof(T_VDCS_MCU_REGISTER_ACK);

	memcpy(buff, &t_PackHeadRegAck,sizeof(T_PacketHead));
	
	t_register_ack.Ack = 0;
	memcpy(buff+sizeof(T_PacketHead),&t_register_ack,sizeof(T_VDCS_MCU_REGISTER_ACK));
	SendMessage(buff,sizeof(buff));
}

void NetClientSession::mcu_operate_alarm(uint32 ID ,uint8 flag)
{
	T_PacketHead			 t_PackHeadOpr;
	T_VDCS_MCU_OPERATE_TERM  t_opreate;
	char buff[28+45] = {0};
	
	t_PackHeadOpr.magic			 =  T_PACKETHEAD_MAGIC;
	t_PackHeadOpr.cmd			 =  SM_VDCS_MCU_OPERATE_TERM;
	t_PackHeadOpr.UnEncryptLen	 =  sizeof(T_VDCS_MCU_OPERATE_TERM);
	
	memcpy(buff, &t_PackHeadOpr,sizeof(T_PacketHead));

	t_opreate.TermType = 	(uint8)DeviceTypeLock;
	t_opreate.port     =	(uint8)(ID - 600300000);
	t_opreate.OpFlag   =	 flag;
	
	memcpy(buff+sizeof(T_PacketHead),&t_opreate,sizeof(T_VDCS_MCU_OPERATE_TERM));

	SendMessage(buff,sizeof(buff));
	
}

void NetClientSession::mcu_operate_alarm_ack(char* buffer ,int size)
{
	T_MCU_VDCS_OPERATE_TERM_ACK t_operate_ack;
	memcpy(&t_operate_ack, buffer,sizeof(T_MCU_VDCS_OPERATE_TERM_ACK));
	if(t_operate_ack.Ack == 0)
	{
		printf("mcu operate success!\n");
	}
	else
	{
		printf("mcu operate fail!\n");
	}
}

int NetClientSession::client_register_ack()
{
	T_PacketHead						 t_PackHeadRegAck;
	T_ANAY_VDCS_REGISTER_ACK    		 t_RegAck;
	char  RegAckBuff[28 + 20] ={0};

	t_PackHeadRegAck.magic			  =  T_PACKETHEAD_MAGIC;
	t_PackHeadRegAck.cmd			  =  SM_VDCS_ANAY_REGISTER_ACK;
	t_PackHeadRegAck.UnEncryptLen	 =  sizeof(T_ANAY_VDCS_REGISTER_ACK);
	memcpy(RegAckBuff,&t_PackHeadRegAck,sizeof(T_PacketHead));
	
	t_RegAck.ServerID = SerParam.ServerID;
	memcpy(t_RegAck.Serverip ,SerParam.Serverip,IP_LEN_16);
	memcpy(RegAckBuff+sizeof(T_PacketHead),&t_RegAck,sizeof(T_ANAY_VDCS_REGISTER_ACK));
	SendMessage(RegAckBuff,sizeof(RegAckBuff));
  	return 0;
}

void NetClientSession::push_camera_data_ack1(ST_VDCS_VIDEO_PUSH_CAM & AddCamera)
{
	T_PacketHead  				t_PackHeadAddAck;
	T_ANAY_VDCS_PUSH_CAM_ACK  	t_CamAddAck;

	char AddAckBuff[28 + 1 +16+128+128] ={0};

	t_PackHeadAddAck.magic	    	=  T_PACKETHEAD_MAGIC;
	t_PackHeadAddAck.cmd			=  SM_VDCS_ANAY_PUSH_CAMERA_ACK;
	t_PackHeadAddAck.UnEncryptLen	=  sizeof(T_ANAY_VDCS_PUSH_CAM_ACK);
	memcpy(AddAckBuff,&t_PackHeadAddAck,sizeof(T_PacketHead));

	t_CamAddAck.ack   = 0;
	memcpy(t_CamAddAck.ip ,AddCamera.ip,IP_LEN_16);
	memcpy(t_CamAddAck.CameUrL,AddCamera.CameUrL,SINGLE_URL_LEN_128);
	memcpy(AddAckBuff+sizeof(T_PacketHead),&t_CamAddAck,sizeof(T_ANAY_VDCS_PUSH_CAM_ACK));
	SendMessage(AddAckBuff,sizeof(AddAckBuff));
}

void NetClientSession::push_camera_data_ack2(ST_VDCS_VIDEO_PUSH_CAM & AddCamera,string &url)
{
	T_PacketHead  t_PackHeadAddAck;
	T_ANAY_VDCS_PUSH_CAM_ACK  t_CamAddAck;
	char AddAckBuff[28 + 1 +16+128+128] ={0};

	t_PackHeadAddAck.magic	    =  T_PACKETHEAD_MAGIC;
	t_PackHeadAddAck.cmd			  = SM_VDCS_ANAY_PUSH_CAMERA_ACK;
	t_PackHeadAddAck.UnEncryptLen	 =  sizeof(T_ANAY_VDCS_PUSH_CAM_ACK);
	memcpy(AddAckBuff,&t_PackHeadAddAck,sizeof(T_PacketHead));

	t_CamAddAck.ack   = 1;
	memcpy(t_CamAddAck.ip ,AddCamera.ip,IP_LEN_16);
	memcpy(t_CamAddAck.CameUrL,AddCamera.CameUrL,SINGLE_URL_LEN_128);
	memcpy(t_CamAddAck.RtspUrL,url.c_str(),url.length());
	memcpy(AddAckBuff+sizeof(T_PacketHead),&t_CamAddAck,sizeof(T_ANAY_VDCS_PUSH_CAM_ACK));
	SendMessage(AddAckBuff,sizeof(AddAckBuff));
}

int NetClientSession::push_camera_data(char* buffer ,int size)
{
	int iRet = -1;
	
	ST_VDCS_VIDEO_PUSH_CAM t_add_camera;

	memcpy(&t_add_camera,buffer,sizeof(ST_VDCS_VIDEO_PUSH_CAM));
	if('r' == t_add_camera.CameUrL[0])
	{
	    iRet = fOurServer->ManCam->try_to_open(t_add_camera.ip,t_add_camera.CameUrL);
	    if(iRet < 0){
			fOurServer->m_log.Add("open %s failed!", t_add_camera.CameUrL);
			push_camera_data_ack1(t_add_camera);
			return -1;
	    }
	}
	string url;
	url = fOurServer->ManCam->Create_or_Renew_Camera(t_add_camera);
	push_camera_data_ack2(t_add_camera,url);
	return 0;
}

void NetClientSession::push_camera_param_ack(int ret)
{
	T_PacketHead  t_PackHeadAck;
	T_ANAY_VDCS_PUSH_CAM_PARAM_ACK  t_CamAck;
	char PushParamAckBuff[28 + 1] ={0};

	t_PackHeadAck.magic	    =  T_PACKETHEAD_MAGIC;
	t_PackHeadAck.cmd			  =  SM_VDCS_ANAY_PUSH_CAMERA_PARAM_ACK;
	t_PackHeadAck.UnEncryptLen	 =  sizeof(T_ANAY_VDCS_PUSH_CAM_ACK);
	memcpy(PushParamAckBuff,&t_PackHeadAck,sizeof(T_PacketHead));
	if(ret < 0)
		t_CamAck.ack   = 0;
	else
		t_CamAck.ack  = 1;
	memcpy(PushParamAckBuff+sizeof(T_PacketHead),&t_CamAck,sizeof(T_ANAY_VDCS_PUSH_CAM_PARAM_ACK));
	
	SendMessage(PushParamAckBuff,sizeof(PushParamAckBuff));
}

int NetClientSession::push_camera_param(char * buffer , int size)
{
	int iRet = -1;
	uint8    Num = 0;
	vector <VIDEO_DRAW>  DrawPkg;
	T_VDCS_VIDEO_CAMERA_PARAM t_CameraParam;
	
	memcpy(&t_CameraParam,buffer,sizeof(T_VDCS_VIDEO_CAMERA_PARAM));

	if(t_CameraParam.PkgNum == 0)
	{
		fOurServer->m_log.Add(" %d client push_camera_param pkgnum is 0", fOurSessionId);
		return -1;
	}
	
	for(Num=0; Num  < t_CameraParam.PkgNum; Num++)
	{
		VIDEO_DRAW tmp;
		memcpy(&tmp,buffer+sizeof(T_VDCS_VIDEO_CAMERA_PARAM)+sizeof(VIDEO_DRAW) *Num,sizeof(VIDEO_DRAW));
		DrawPkg.push_back(tmp);
	}
	
	iRet = fOurServer->ManCam->Set_or_Renew_Camera_Param(&t_CameraParam,DrawPkg);
	if(iRet < 0)
	{
		fOurServer->m_log.Add(" %d client push_camera_param failure", fOurSessionId);
		push_camera_param_ack(iRet);
		return -1;
	}
	push_camera_param_ack(iRet);
	return 0;
}

int NetClientSession::delete_camera_ack(T_VDCS_VIDEO_CAMERA_DELETE* pt_CamDel,int ret)
{
	T_PacketHead  						t_PackHeadDelAck;
	T_VDCS_VIDEO_CAMERA_DELETE_ACK  t_CamDelAck;
	char DelAckBuff[28 + 145] ={0};

	t_PackHeadDelAck.magic		  =  T_PACKETHEAD_MAGIC;
	t_PackHeadDelAck.cmd			  =  SM_VDCS_ANAY_DELETE_CAMERA_ACK;
	t_PackHeadDelAck.UnEncryptLen	  =	sizeof(T_VDCS_VIDEO_CAMERA_DELETE_ACK);
	memcpy(DelAckBuff,&t_PackHeadDelAck,sizeof(T_PacketHead));

	if(ret ==  1){
		t_CamDelAck.Ack   = 1;
	}else{
		t_CamDelAck.Ack   = 0;
	}
	
	memcpy(t_CamDelAck.ip ,pt_CamDel->ip,IP_LEN_16);
	memcpy(t_CamDelAck.CameUrl,pt_CamDel->CameUrL,SINGLE_URL_LEN_128);
	
	memcpy(DelAckBuff+sizeof(T_PacketHead),&t_CamDelAck,sizeof(T_VDCS_VIDEO_CAMERA_DELETE_ACK));
	SendMessage(DelAckBuff,sizeof(DelAckBuff));
	printf("delete_camera_ack ret is %d\n",ret);
	return 0;
}

int NetClientSession::delete_camera(char * buffer , int size)
{
	int iRet = -1;
	uint32 ID = 0;
	SingleCamPtr  tmpCamPtr;

	T_VDCS_VIDEO_CAMERA_DELETE  t_CamDel;

	memcpy(&t_CamDel,buffer,sizeof(T_VDCS_VIDEO_CAMERA_DELETE));

	tmpCamPtr = fOurServer->ManCam->search_camera_by_url(t_CamDel.CameUrL);
	if(tmpCamPtr != NULL)
	{
		tmpCamPtr->TimeThread->quit();
		tmpCamPtr->TimeThread->resume();
		tmpCamPtr->Ana1Thread->quit();
		tmpCamPtr->Ana1Thread->resume();
		tmpCamPtr->Ana2Thread->quit();
		tmpCamPtr->Ana2Thread->resume();
		tmpCamPtr->ReadThread->quit();
		tmpCamPtr->ReadThread->resume();
	
		usleep(200*1000);
		ID = tmpCamPtr->CameraID;
		if(ID >0)
		{
			iRet = fOurServer->ManCam->remove_camera_by_id(ID);
			if(iRet < 0) {
				fOurServer->m_log.Add(" %d remove_camera_by_id fail", fOurSessionId);
				delete_camera_ack(&t_CamDel,0);
				return -1;
			}
		
			iRet = fOurServer->ManCam->resume_cameraID_in_list(ID);
			if(iRet < 0) {
				fOurServer->m_log.Add(" %d resume_cameraID_in_list fail", fOurSessionId);
				delete_camera_ack(&t_CamDel,0);
				return -1;
			}

			iRet = fOurServer->ManCam->remove_camera_from_db_by_id(ID);
			if(iRet !=0) {
				fOurServer->m_log.Add(" %d remove_camera_from_db_by_id fail", fOurSessionId);
				delete_camera_ack(&t_CamDel,0);
				return -1;
			}
		}else{
			fOurServer->m_log.Add(" %d get camera ID fail", fOurSessionId);
			delete_camera_ack(&t_CamDel,0);
			return -1;
		}
		
	}else{
		fOurServer->m_log.Add(" %d get camera ptr fail", fOurSessionId);
		delete_camera_ack(&t_CamDel,0);
		return -1;
	}
	delete_camera_ack(&t_CamDel,1);
	return 0;
}

int NetClientSession::device_status_ack(char * buffer,int size)
{
	T_VDCS_ANAY_DEVICE_STATUS_ACK t_AnaDeviceAck;
	uint8 ack;

	memcpy(&t_AnaDeviceAck,buffer,sizeof(T_VDCS_ANAY_DEVICE_STATUS_ACK));
	
	ack = t_AnaDeviceAck.Ack;
	if(ack == 1){
			dbgprint("ipCamera %s deivce break report  sucess!\n",t_AnaDeviceAck.CameUrl);
	}else{
		dbgprint("ipCamera %s deivce break reporterror!\n",t_AnaDeviceAck.CameUrl);
	}
	return 0;
}

int NetClientSession::warn_info_ack(char * buffer,int size)
{
	T_VDCS_ANAY_WARN_INFO_ACK  t_AnayWarnAck;
	uint8 ack;
	
	memcpy(&t_AnayWarnAck,buffer,sizeof(T_VDCS_ANAY_WARN_INFO_ACK));
		
	ack = t_AnayWarnAck.Ack;
	if(ack == 1){
			dbgprint("ipCamera %s warninfo  analyze  %x report sucess!\n",t_AnayWarnAck.CameUrL,t_AnayWarnAck.AnalyzeType);
	}else{
		dbgprint("ipCamera %s warninfo analyze  %x report error!\n",t_AnayWarnAck.CameUrL,t_AnayWarnAck.AnalyzeType);
	}
	return 0;
}

int NetClientSession::ReciveData_GetParam(char* buffer ,int size)
{
	uint16 cmd;
	T_PacketHead   t_packet_head;
	if(size  < PACKET_HEAD_LEN )
	{
		printf("client recive buffer err!\n");
		return -1;
	}
	memcpy(&t_packet_head,buffer,PACKET_HEAD_LEN);
	cmd = t_packet_head.cmd;
	
	switch (cmd ){
		case SM_MCU_VDCS_REGISTER:
			mcu_register(buffer+PACKET_HEAD_LEN,size-PACKET_HEAD_LEN);
			mcu_register_ack();
			break;
		case SM_MCU_VDCS_OPERATE_TERM_ACK:
			mcu_operate_alarm_ack(buffer+PACKET_HEAD_LEN,size-PACKET_HEAD_LEN);
			break;
		case SM_ANAY_VDCS_REGISTER:
			fOurServer->m_log.Add("%d client_register_ack", fOurSessionId);
			client_register_ack();
			break;
		case SM_VDCS_ANAY_DEVICE_STATUS_ACK:
			fOurServer->m_log.Add("%d device status ack", fOurSessionId);
			device_status_ack(buffer+PACKET_HEAD_LEN,size-PACKET_HEAD_LEN);
			break;
		case SM_VDCS_ANAY_WARN_INFO_ACK:
			fOurServer->m_log.Add("%d warn info ack", fOurSessionId);
			warn_info_ack(buffer+PACKET_HEAD_LEN,size-PACKET_HEAD_LEN);
			break;
		case SM_VDCS_ANAY_PUSH_CAMERA:
			fOurServer->m_log.Add("%d client push camera", fOurSessionId);
			push_camera_data(buffer+PACKET_HEAD_LEN,size-PACKET_HEAD_LEN);
			break;
		case SM_VDCS_ANAY_PUSH_CAMERA_PARAM:
			fOurServer->m_log.Add("%d client push camera param", fOurSessionId);
			push_camera_param(buffer+PACKET_HEAD_LEN,size-PACKET_HEAD_LEN);
			break;
		case SM_VDCS_ANAY_DELETE_CAMERA:
			fOurServer->m_log.Add("%d client delete camera ", fOurSessionId);
			delete_camera(buffer+PACKET_HEAD_LEN,size-PACKET_HEAD_LEN);
			break;
		default: break;
	}
	//dbgprint("client cmd = %x\n", cmd);
	return 0;
}

void NetClientSession::HandleRecvReponse(const boost::system::error_code& error, int recvsizes)
{
	if(stop_)return;
	if (!error)
	{
		totalRecive += recvsizes;
		if(recvsizes == BUFFER_SIZE_MIN)
		{
			memcpy(fAcceptBuffer+BUFFER_SIZE_MIN*PKGNUM,fBuffer,BUFFER_SIZE_MIN);
			PKGNUM++;
			memset(fBuffer, 0 ,BUFFER_SIZE_MIN);
			incomingAcceptHandler();
		}
		else if(recvsizes < BUFFER_SIZE_MIN)
		{
			memcpy(fAcceptBuffer+BUFFER_SIZE_MIN*PKGNUM,fBuffer,recvsizes);
			std::cout<<"client send data length "<<totalRecive<<std::endl;
			ReciveData_GetParam(fAcceptBuffer,totalRecive);
			memset(fBuffer, 0 ,BUFFER_SIZE_MIN);
			PKGNUM = 0;
			totalRecive = 0;
			incomingAcceptHandler();
		}
	}
	else{
		std::cerr<< "NetClientSession::HandleRecvReponse : " << error.message() << std::endl;
		fOurServer->RemoveNetClientBySessionID(fOurSessionId);
	}

}
int NetClientSession::SendMessage(char *buff,int size)
{
  try {
		client_socket_.async_send(
			boost::asio::buffer(buff,size),
			boost::bind(&NetClientSession::HandleSendResponse,shared_from_this(),
			boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));

		return 0;

	}catch (...){  }

	return 0;
}

void NetClientSession::HandleSendResponse(const boost::system::error_code& error, int sendsizes)
{
	if (!error){
		std::cout<<"sendsizes:"<<sendsizes<<std::endl;
	}
	else{

	}
}
