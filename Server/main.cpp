
#include "NetClientSession.h"
#include "NetServer.h"
#include "FileOpr.h"
#include "RtspCamera.h"
#include "CameraManage.h"
#include "NetClient.h"

T_ServerParam SerParam;
NetServer NetWorkServer;
CFileOpr fileOpr;
CRtspCamera rtspCamera;
ManageCamera ManCamera;
NetClient  NetWorkClient;

void string2char(string str, char*buf)
{
	memcpy(buf, str.c_str(),str.length());
}

int main(int argc ,char **argv)
{
	string a ;
	int num= 0;
	char buffer[64] ={0};

	fileOpr.read_server_config(SerParam);

	NetWorkServer.InitNetServer(SerParam.ServerPort);
	NetWorkServer.Start();
	NetWorkServer.Run();
	
	NetWorkClient.start(SerParam.AlarmIp,SerParam.AlarmPort);
	NetWorkClient.Run();

	ManCamera.AddServer(&NetWorkServer); // as server
	ManCamera.AddClient(&NetWorkClient); // as client
	ManCamera.InitFromDB();

	NetWorkServer.AddManaCamera(&ManCamera);
	NetWorkClient.AddManaCamera(&ManCamera);

	StartRTSPServer(&rtspCamera);

	cout<<"everthing is already"<<endl;
	
//	while(1)  //��������
//	{
//		sleep(5);
//	}

	while(cin>> a){ // ����
			memset(buffer, 0 ,64);
	    	string2char(a,buffer);
	    	num =  atoi(buffer);
	    	switch(num){
	    	case 1:
	    		NetWorkServer.GetClient();
	    		break;
	    	case 2:
	    		ManCamera.m_CamList.Get_All_Camera();
	    		break;
	    	case 3:
	    		ManCamera.Get_Rest_Camlist();
	    		break;
	    	case 4:
	    		ManCamera.Get_Rest_SingleCamlist();
	    		break;
			case 11:
				ManCamera.Server->SendBufferToMCUClient(600300001,1);
				break;
			case 12:
				ManCamera.Server->SendBufferToMCUClient(600300002,1);
				break;
			case 13:
				ManCamera.Server->SendBufferToMCUClient(600300003,1);
				break;
			case 14:
				ManCamera.Server->SendBufferToMCUClient(600300004,1);
				break;
			case 21:
				NetWorkClient.SendToServer("hello!",7);
				printf("----\n");
				break;
			default :break;
	    	}
	}

	return 0;
}


