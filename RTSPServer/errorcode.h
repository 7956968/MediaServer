/*
 * $Id: errorcode.h,v 2.0.0.0 2008/07/07 litianshun Exp $
 *
 * errorcode.h
 *
 * error code defination
 *
 * intervideo manager platform
 *
 * Copyright (c) 2006-2007
 *
 * The Initial Developer of the Original Code is qu licheng.
 *
 * Contributor(s): 
 *
 * $Log: errorcode.h,v $
 * Revision 2.0.0.0  2008/07/07 litianshun
 * ����������
 *
 * Revision 1.1.1.2  2007/08/03 02:54:26  jijing
 * ���Ӵ������ ERROR_USER_DOMAIN�����Ǳ����û�
 *
 * Revision 1.1.1.1  2007/07/31 07:36:47  qulicheng
 * initial import
 */

#ifndef _ERROR_CODE_
#define _ERROR_CODE_

#ifdef __cplusplus
extern "C" {
#endif

#define ERROR_PROCESS_SUCCESS		0		//�ɹ�

#define ERROR_VER_TOKEN				-1		//������Ҫ��İ汾���ʶ
#define ERROR_COMMAND_INVALID		-2		//��Ч������
#define ERROR_USER_LOGIN_ALREADY	-3		//���û��ѵ�¼
#define ERROR_USER_NOLOGINED		-4		//�û�δ��¼
#define ERROR_USER_PASSWORD			-5		//�û��������

#define ERROR_CAMERA_END			-10		//������б������
#define ERROR_CAMERA_INVALID		-11		//��Ч�����ID
#define ERROR_CAMERA_NOLEVEL		-12		//�Բ�������Ȩ������ͼ��
#define ERROR_CODEC_FULL			-13		//�޿���ͨ��
#define ERROR_CAMERA_DISLINE		-14		//�����������
#define ERROR_CAMERA_FAILURE		-15		//��ƵԴ����ʧ��
#define ERROR_CAMERA_ERR			-16		//����������б����

#define ERROR_CAMERA_NOOPEN			-20		//�����δ��
#define ERROR_CAMERA_NOCONTROL		-21		//��������ܿ���
#define ERROR_CONTROL_NOLEVEL		-22		//��Ȩ���ƴ������
#define	ERROR_CAMERA_REOPEN         -23     //��ʾ�ͻ����ȹر�,���

#define ERROR_SWITCH_NOLEVEL		-31		//�û�Ȩ�޲������е����û������,�޷��л�
#define ERROR_SWITCH_ERROR			-32		//�л�ʧ��
#define ERROR_NEWCAMERA_NOLEVEL		-33		//�û���Ȩ����Ҫ�л��������

#define ERROR_REPLAY_ERROR			-41		//������޷��ط�
#define ERROR_REPLAY_NOLEVEL		-42		//�û���Ȩ�ط�


#define ERROR_SENTBL_FULL			-90		//���ͱ���

#define ERROR_SYSTEM_ERROR			-100	//ϵͳ����

#define ERROR_NET_FAILED            -201    //�������
#define ERROR_OPEN_CAMERA           -202    //�����������

#ifdef __cplusplus
}
#endif

#endif	//_ERROR_CODE_

