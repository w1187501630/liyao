#pragma once
#ifndef WRITE2SQL
#define WRITE2SQL
/*SQLINTEGER num1, age1;
// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。*/
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
//TEST ON Github
//SQLINTEGER num1, age1;
//SQLCHAR frameID[20];
char entity[255];
char exm[1024];/*
			   struct worker
			   {
			   char sno[255];//工号
			   unsigned char sname[255];//姓名
			   char sex[255];//性别
			   char bdate[255];//年龄
			   char dept[255];//专业
			   unsigned short classno;//班级
			   };
struct msg
{
	char frameID[5];	//帧标识5.29取消unsigned
	unsigned short frameLength;	//帧长度
	unsigned short ManuName;	//厂商标识
	char Ver;					//协议版本号
	//	unsigned
	char AK[32];				//在百度开放平台申请的 access key
	unsigned long ServiceID;	//服务ID
	//unsigned
unsigned char EntityLength;	//终端标识符长度
char* EntityName;			//终端标识符
int Longitude;				//经度
int Latitude;				//纬度
char Coordinate[1];			//坐标类型
unsigned short Velocity;	//速度
char Direction[1];			//方向
unsigned int Time;			//时间//5.29取消unsign
unsigned short CityID;		//城市编号
unsigned char Passenger;	//载客
unsigned short ExMsgLength;	//扩展信息长度
char* ExMsg;				//扩展信息*/
//};*/
//msg msg2;*/
void workertianjiashuju(Message ms)
{
	SQLRETURN ret; //DESKTOP - QILGJO2\MSSQLSERVER1
	SQLHENV henv;//SQLHANDLE henv 
	SQLHDBC hdbc;//SQLHANDLE hdbc 
	SQLHSTMT hstmt;//SQLHANDLE hstmt 

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境句柄 
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境属性 
	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接句柄 
	ret = SQLConnect(hdbc, (SQLCHAR*)"local1", SQL_NTS, (SQLCHAR*)"sa", SQL_NTS, (SQLCHAR*)"123", SQL_NTS);//连接数据库 
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		cout << "数据库连接成功" << endl;
		ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请SQL语句句柄 
		SQLCHAR sql[] = "INSERT INTO car_copy VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";//SQL操作语句
		//SQLCHAR sql[] = "INSERT INTO student_copy VALUES (?,?,?,?,?,?);";//SQL操作语句
		SQLLEN P = SQL_NTS;
		ret = SQLPrepare(hstmt, sql, SQL_NTS);
		/*ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//绑定参数
		ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//绑定参数
		ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//绑定参数
		ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);//绑定参数
		ret = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, &a, 255, &P);
		ret = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT,0 , 0, &wo.sno, 0, &P);//*/
		ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, &ms.frameID, 20, &P);//绑定帧标识参数
		ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.frameLength, 0, &P);//绑定帧长参数
		ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.ManuName, 0, &P);//绑定厂商标识参数 
		ret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 255, 0, &ms.Ver, 255, &P);//绑定协议版本号参数
		ret = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, ms.AK, 255, &P);//绑定access key参数 
		ret = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 32, 0, &ms.ServiceID, 32, &P);//绑定ServiceId参数
		ret = SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 20, 0, &ms.EntityLength, 20, &P);//绑定帧标识参数
		//cout << "memcpy1zhiqian" << endl;
		memmove(entity, ms.EntityName, 255);
		//cout << "扩展信息1" << entity << endl;
		free(ms.EntityName);
		//cout << "memcpy1zhiqian" << endl;
		ret = SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, entity, 255, &P);//绑定终端标识符参数
		//cout << "memcpy1zhiqian" << endl;
		ret = SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ms.Longitude, 0, &P);//绑定经度参数 
		ret = SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ms.Latitude, 0, &P);//绑定纬度参数
		ret = SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 10, 0, &ms.Coordinate, 10, &P);//绑定坐标类型参数
		ret = SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.Velocity, 0, &P);//绑定速度参数
		ret = SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 1, 0, &ms.Direction, 1, &P);//绑定方向参数 
		ret = SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ms.Time, 0, &P);//绑定时间参数
		ret = SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.CityID, 0, &P);//绑定城市编号参数 
		ret = SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 20, 0, &ms.Passenger, 20, &P);//绑定载客参数
		ret = SQLBindParameter(hstmt, 17, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &ms.ExMsgLength, 0, &P);//绑定扩展信息长度参数
		//cout << "memcpy2zhiqian" << endl;
		memmove(exm, ms.ExMsg, 1024);
		//cout << "扩展信息2" << exm << endl;
		free(ms.ExMsg);
		ret = SQLBindParameter(hstmt, 18, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 1024, 0, exm, 1024, &P);//绑定扩展信息参数
		ret = SQLExecute(hstmt);//直接执行SQL语句 
		//cout << "zhixingwanzhiqiandadadada大大大" << endl;
		if (ret == SQL_ERROR) {
			cout << "数据库错误" << endl;
			system("pause");
		}
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {

			/*****************测试结构体数组********************/
			cout << ">>>>>>>>>>>>>>往数据库添加信息成功" << endl;

			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句句柄 
		}
		else printf("添加数据库操作失败！\n");
		SQLDisconnect(hdbc);//断开与数据库的连接 
	}
	else
	{
		printf("连接数据库失败!\n");
		free(ms.EntityName);
		free(ms.ExMsg);
	}
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接句柄 
	SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境句柄

}
#endif
/*int main(){
	msg msg1;
	char *a = "0";
	strcpy(msg1.frameID, "000");
	msg1.EntityLength = 0x00;
	//msg1.Passenger = 0x44;
	msg1.frameLength = 0000;
	msg1.Ver = '0';//初始化；
	msg1.Time = 0;//初始化；
	memcpy(msg1.AK,a,32);//初始化；
	msg1.EntityName = "0000";
	msg1.ExMsg="0000";
	//memcpy(msg1.AK, a, strlen(a) + 1);
	msg1.ServiceID = 000000000;
	//memcpy(msg1.ExMsg, a, strlen(a)+1);
	workertianjiashuju(msg1);
	return 0;

}*/
