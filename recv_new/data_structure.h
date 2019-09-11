#pragma once
#include "write2sql.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include<iostream>
#include "winsock.h"
#pragma comment(lib, "wsock32")

typedef struct Message		//������Ϣ�ṹ��
{
	unsigned char frameID[5];	//֡��ʶ
	unsigned short frameLength;	//֡����
	unsigned short ManuName;	//���̱�ʶ
	char Ver;					//Э��汾��
//	unsigned
	char AK[32];				//�ڰٶȿ���ƽ̨����� access key
	unsigned long ServiceID;	//����ID
	unsigned char EntityLength;	//�ն˱�ʶ������
	char* EntityName;			//�ն˱�ʶ��
	int Longitude;				//����
	int Latitude;				//γ��
	char Coordinate[1];			//��������
	unsigned short Velocity;	//�ٶ�
	char Direction[1];			//����
	unsigned int Time;			//ʱ��
	unsigned short CityID;		//���б��
	unsigned char Passenger;	//�ؿ�
	unsigned short ExMsgLength;	//��չ��Ϣ����
	char* ExMsg;				//��չ��Ϣ
}Message;


typedef struct ResponseMessage	//������Ϣ�ṹ��
{
	unsigned char frameID[5] = { 0xFF, 'T', 'R', 'C', 'K' };	//֡��ʶ
	unsigned short frameLength;	//֡����
	unsigned char EntityLength;	//�ն˱�ʶ������
	char* EntityName;			//�ն˱�ʶ��
	unsigned int Time;			//ʱ��
	unsigned char Status;		//״̬	0���ɹ� 1�����̱�ʶ���� 2������ʧ�� 3��AK ���� 4��ServiceID ����
	//5��EntityName Ϊ�� 6���������ʹ��� 7����Ϣ���ȴ��� 8����Ϣ����ʧ��
}ResponseMessage;
