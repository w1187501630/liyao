#pragma once
#ifndef WRITE2SQL
#define WRITE2SQL
/*SQLINTEGER num1, age1;
// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣*/
#include "data_structure.h"
#include <stdio.h>    
#include <string.h>
#include <windows.h>    
#include <sql.h>   
#include <sqlext.h>    
#include <sqltypes.h>    
#include <odbcss.h>
#include <iostream>
#include <string>
#include <time.h>
#include <cstring>

using namespace std;
#pragma warning( disable : 4996)
#define MAXSTRSIZE 1024

//SQLINTEGER num1, age1;
//SQLCHAR frameID[20];
char entity[255];
char exm[1024];/*
			   struct worker
			   {
			   char sno[255];//����
			   unsigned char sname[255];//����
			   char sex[255];//�Ա�
			   char bdate[255];//����
			   char dept[255];//רҵ
			   unsigned short classno;//�༶
			   };
struct msg
{
	char frameID[5];	//֡��ʶ5.29ȡ��unsigned
	unsigned short frameLength;	//֡����
	unsigned short ManuName;	//���̱�ʶ
	char Ver;					//Э��汾��
	//	unsigned
	char AK[32];				//�ڰٶȿ���ƽ̨����� access key
	unsigned long ServiceID;	//����ID
	//unsigned
unsigned char EntityLength;	//�ն˱�ʶ������
char* EntityName;			//�ն˱�ʶ��
int Longitude;				//����
int Latitude;				//γ��
char Coordinate[1];			//��������
unsigned short Velocity;	//�ٶ�
char Direction[1];			//����
unsigned int Time;			//ʱ��//5.29ȡ��unsign
unsigned short CityID;		//���б��
unsigned char Passenger;	//�ؿ�
unsigned short ExMsgLength;	//��չ��Ϣ����
char* ExMsg;				//��չ��Ϣ*/
//};*/
//msg msg2;*/
void workertianjiashuju(Message ms)
{
	SQLRETURN ret; //DESKTOP - QILGJO2\MSSQLSERVER1
	SQLHENV henv;//SQLHANDLE henv 
	SQLHDBC hdbc;//SQLHANDLE hdbc 
	SQLHSTMT hstmt;//SQLHANDLE hstmt 

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//���뻷����� 
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//���û������� 
	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//�������ݿ����Ӿ�� 
	ret = SQLConnect(hdbc, (SQLCHAR*)"local1", SQL_NTS, (SQLCHAR*)"sa", SQL_NTS, (SQLCHAR*)"123", SQL_NTS);//�������ݿ� 
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		cout << "���ݿ����ӳɹ�" << endl;
		ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//����SQL����� 
		SQLCHAR sql[] = "INSERT INTO car_copy VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";//SQL�������
		//SQLCHAR sql[] = "INSERT INTO student_copy VALUES (?,?,?,?,?,?);";//SQL�������
		SQLLEN P = SQL_NTS;
		ret = SQLPrepare(hstmt, sql, SQL_NTS);
		/*ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//�󶨲���
		ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//�󶨲���
		ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//�󶨲���
		ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//�󶨲���
		ret = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);
		ret = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT,0 , 0, &wo.sno, 0, &P);//*/
		ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, &ms.frameID, 20, &P);//��֡��ʶ����
		ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.frameLength, 0, &P);//��֡������
		ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.ManuName, 0, &P);//�󶨳��̱�ʶ���� 
		ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 255, 0, &ms.Ver, 255, &P);//��Э��汾�Ų���
		ret = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, ms.AK, 255, &P);//��access key���� 
		ret = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 32, 0, &ms.ServiceID, 32, &P);//��ServiceId����
		ret = SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 20, 0, &ms.EntityLength, 20, &P);//��֡��ʶ����
		//cout << "memcpy1zhiqian" << endl;
		memmove(entity, ms.EntityName, 255);
		//cout << "��չ��Ϣ1" << entity << endl;
		free(ms.EntityName);
		//cout << "memcpy1zhiqian" << endl;
		ret = SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, entity, 255, &P);//���ն˱�ʶ������
		//cout << "memcpy1zhiqian" << endl;
		ret = SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ms.Longitude, 0, &P);//�󶨾��Ȳ��� 
		ret = SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ms.Latitude, 0, &P);//��γ�Ȳ���
		ret = SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 10, 0, &ms.Coordinate, 10, &P);//���������Ͳ���
		ret = SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.Velocity, 0, &P);//���ٶȲ���
		ret = SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 1, 0, &ms.Direction, 1, &P);//�󶨷������ 
		ret = SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ms.Time, 0, &P);//��ʱ�����
		ret = SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.CityID, 0, &P);//�󶨳��б�Ų��� 
		ret = SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 20, 0, &ms.Passenger, 20, &P);//���ؿͲ���
		ret = SQLBindParameter(hstmt, 17, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.ExMsgLength, 0, &P);//����չ��Ϣ���Ȳ���
		//cout << "memcpy2zhiqian" << endl;
		memmove(exm, ms.ExMsg, 1024);
		//cout << "��չ��Ϣ2" << exm << endl;
		free(ms.ExMsg);
		ret = SQLBindParameter(hstmt, 18, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 1024, 0, exm, 1024, &P);//����չ��Ϣ����
		ret = SQLExecute(hstmt);//ֱ��ִ��SQL��� 
		//cout << "zhixingwanzhiqiandadadada����" << endl;
		if (ret == SQL_ERROR) {
			cout << "���ݿ����" << endl;
			system("pause");
		}
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {

			/*****************���Խṹ������********************/
			cout << ">>>>>>>>>>>>>>�����ݿ������Ϣ�ɹ�" << endl;

			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//�ͷ������ 
		}
		else printf("������ݿ����ʧ�ܣ�\n");
		SQLDisconnect(hdbc);//�Ͽ������ݿ������ 
	}
	else
	{
		printf("�������ݿ�ʧ��!\n");
		free(ms.EntityName);
		free(ms.ExMsg);
	}
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//�ͷ����Ӿ�� 
	SQLFreeHandle(SQL_HANDLE_ENV, henv);//�ͷŻ������

}
#endif
/*int main(){
	msg msg1;
	char *a = "0";
	strcpy(msg1.frameID, "000");
	msg1.EntityLength = 0x00;
	//msg1.Passenger = 0x44;
	msg1.frameLength = 0000;
	msg1.Ver = '0';//��ʼ����
	msg1.Time = 0;//��ʼ����
	memcpy(msg1.AK,a,32);//��ʼ����
	msg1.EntityName = "0000";
	msg1.ExMsg="0000";
	//memcpy(msg1.AK, a, strlen(a) + 1);
	msg1.ServiceID = 000000000;
	//memcpy(msg1.ExMsg, a, strlen(a)+1);
	workertianjiashuju(msg1);
	return 0;

}*/