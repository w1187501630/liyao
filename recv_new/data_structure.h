#pragma once
#include "write2sql.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include<iostream>
#include "winsock.h"
#pragma comment(lib, "wsock32")

typedef struct Message		//发收信息结构体
{
	unsigned char frameID[5];	//帧标识
	unsigned short frameLength;	//帧长度
	unsigned short ManuName;	//厂商标识
	char Ver;					//协议版本号
//	unsigned
	char AK[32];				//在百度开放平台申请的 access key
	unsigned long ServiceID;	//服务ID
	unsigned char EntityLength;	//终端标识符长度
	char* EntityName;			//终端标识符
	int Longitude;				//经度
	int Latitude;				//纬度
	char Coordinate[1];			//坐标类型
	unsigned short Velocity;	//速度
	char Direction[1];			//方向
	unsigned int Time;			//时间
	unsigned short CityID;		//城市编号
	unsigned char Passenger;	//载客
	unsigned short ExMsgLength;	//扩展信息长度
	char* ExMsg;				//扩展信息
}Message;


typedef struct ResponseMessage	//返回信息结构体
{
	unsigned char frameID[5] = { 0xFF, 'T', 'R', 'C', 'K' };	//帧标识
	unsigned short frameLength;	//帧长度
	unsigned char EntityLength;	//终端标识符长度
	char* EntityName;			//终端标识符
	unsigned int Time;			//时间
	unsigned char Status;		//状态	0：成功 1：厂商标识错误 2：解密失败 3：AK 错误 4：ServiceID 错误
	//5：EntityName 为空 6：坐标类型错误 7：消息长度错误 8：消息处理失败
}ResponseMessage;
