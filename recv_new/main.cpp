#define _CRT_SECURE_NO_WARNINGS
#pragma pack(1)

#include "write2sql.h"
#include "data_structure.h"
#include <openssl/aes.h>
#include "process.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include<iostream>
#pragma comment(lib, "wsock32")


#define num_subthread 1
#define num_sendthread 1

using namespace std;

class RingBuff
{
private:
	char *begin;
	char *end;
	char *write;
	char *read;
	CRITICAL_SECTION csLock;
	int count;

public:
	RingBuff(int buff_size);
	~RingBuff();
	int put_data(int datain_length, char *addrin);
	int pop_data(int dataout_length, char *addrout);
	int get_capcity();
	int get_count();
	void clear_buff();
	void lock();
	void unlock();
};

char key0[] = "4fa8903e5acd39b13d7cc127d3a824f2";
char vi[] = "00000000000000000000000000000000";

typedef struct receive_data_args
{
	RingBuff *rb;
	SOCKET *cSocket;
}REARGS;

typedef struct subcontract_args
{
	RingBuff *rb_r;
	RingBuff *rb_s;
}SUBARGS;

SOCKET create_socket();
void receive_data(SOCKET *cSocket, RingBuff *rb);
unsigned int _stdcall send_data(LPVOID lpParam);
unsigned int _stdcall subcontract(LPVOID lpParam);
unsigned char* str2hex(char *str);
char *padding_buf(char *buf, int size, int *final_size);
void printf_buff(char *buff, int size);
void encrpyt_buf(const unsigned char *raw_buf, unsigned char **encrpy_buf, int len);
void decrpyt_buf(const unsigned char *raw_buf, unsigned char **encrpy_buf, int len);


int main()
{
	SOCKET client_socket;
	RingBuff rb_recv = RingBuff(102400);
	RingBuff rb_send = RingBuff(10240);

	client_socket = create_socket();
	REARGS re_args = { &rb_recv, &client_socket };
	REARGS send_args = { &rb_send, &client_socket };
	SUBARGS sub_args = { &rb_recv, &rb_send };

	HANDLE sub_thread[num_subthread];
	HANDLE send_thread[num_sendthread];


	for (size_t j = 0; j < num_subthread; j++)
	{
		sub_thread[j] = (HANDLE)_beginthreadex(NULL, 0, subcontract, &sub_args, 0, NULL);
	}

	for (size_t k = 0; k < num_sendthread; k++)
	{
		send_thread[k] = (HANDLE)_beginthreadex(NULL, 0, send_data, &send_args, 0, NULL);
	}
	
	receive_data(&client_socket, &rb_recv);

	/*for (size_t j = 0; j < 4; j++)
	{
		CloseHandle(sub_thread[j]);
	}

	for (size_t k = 0; k < 4; k++)
	{
		CloseHandle(send_thread[k]);
	}*/

	closesocket(client_socket);
	WSACleanup();
	return 0;
}







RingBuff::RingBuff(int buff_size)
{
	InitializeCriticalSection(&csLock);
	begin = new char[buff_size];
	end = begin + buff_size;
	write = begin;
	read = begin;
	count = 0;
}

RingBuff::~RingBuff()
{
	DeleteCriticalSection(&csLock);
	delete[] begin;
}

void RingBuff::lock()
{
	EnterCriticalSection(&csLock);
}

void RingBuff::unlock()
{
	LeaveCriticalSection(&csLock);
}

int RingBuff::get_count()
{
	return count;
}

void RingBuff::clear_buff()
{
	write = begin;
	read = begin;
	count = 0;
}

int RingBuff::get_capcity()
{
	if (write == read)
		return (end - begin - 1);
	else if (write > read)
		return ((end - write) + (read - begin)-1);
	else
		return (read - write-1);
}

int RingBuff::put_data(int datain_length, char *addrin)
{
	if (datain_length >= get_capcity())
	{
		clear_buff();
		return 0;
	}

	if (write < read)
	{
		memcpy(write, addrin, datain_length);
		write = write + datain_length;
		count = count + datain_length;
		return datain_length;
	}
	else
	{
		if ((end - write) >= datain_length)
		{
			memcpy(write, addrin, datain_length);
			write = write + datain_length;
			count = count + datain_length;
			return datain_length;
		}
		else
		{
			memcpy(write, addrin, end - write);
			memcpy(begin, addrin + (end - write), datain_length - (end - write));
			write = begin + datain_length - (end - write);
			count = count + datain_length;
			return datain_length;
		}
	}
}

int RingBuff::pop_data(int dataout_length, char *addrout)
{
	if (dataout_length >= get_count())
	{
		return 0;
	}
	else 
	{
		if (write > read)
		{
			memcpy(addrout, read, dataout_length);
			read = read + dataout_length;
			count = count - dataout_length;
			return dataout_length;
		}
		else
		{
			if ((end - read) > dataout_length)
			{
				memcpy(addrout, read, dataout_length);
				read = read + dataout_length;
				count = count - dataout_length;
				return dataout_length;
			}
			else
			{
				memcpy(addrout, read, end - read);
				memcpy(addrout + (end - read), begin, dataout_length - (end - read));
				read = begin + (dataout_length - (end - read));
				count = count - dataout_length;
				return dataout_length;
			}
		}
	}
}

unsigned char* str2hex(char *str) {//字串转换函数 注释1
	unsigned char *ret = NULL;
	int str_len = strlen(str);
	int i = 0;
	assert((str_len % 2) == 0);
	ret = (unsigned char *)malloc(str_len / 2);
	for (i = 0; i < str_len; i = i + 2) {
		sscanf(str + i, "%2hhx", &ret[i / 2]);
	}
	return ret;
}

char *padding_buf(char *buf, int size, int *final_size) {//padding算法 注释2
	char *ret = NULL;
	int pidding_size = AES_BLOCK_SIZE - (size % AES_BLOCK_SIZE);
	int i;
	*final_size = size + pidding_size;
	ret = (char *)malloc(size + pidding_size);
	memcpy(ret, buf, size);
	if (pidding_size != 0) {
		for (i = size; i < (size + pidding_size); i++) {
			ret[i] = pidding_size;
		}
	}
	return ret;
}

void printf_buff(char *buff, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		printf("%02X ", (unsigned char)buff[i]);
		if ((i + 1) % 8 == 0) {
			printf("\n");
		}
	}
	printf("\n\n\n\n");
}

void encrpyt_buf(const unsigned char *raw_buf, unsigned char **encrpy_buf, int len) {
	AES_KEY aes;
	unsigned char *key = str2hex(key0);
	unsigned char *iv = str2hex(vi);
	AES_set_encrypt_key(key, 128, &aes);//函数接口 注释3
	AES_cbc_encrypt(raw_buf, *encrpy_buf, len, &aes, iv, AES_ENCRYPT);
	free(key);
	free(iv);
}

void decrpyt_buf(const unsigned char *raw_buf, unsigned char **encrpy_buf, int len) {
	AES_KEY aes;
	unsigned char *key = str2hex(key0);
	unsigned char *iv = str2hex(vi);
	AES_set_decrypt_key(key, 128, &aes);
	AES_cbc_encrypt(raw_buf, *encrpy_buf, len, &aes, iv, AES_DECRYPT);
	free(key);
	free(iv);
}

SOCKET create_socket()
{
	int ret = 0;
	WSADATA wsadata;
	WORD version = MAKEWORD(2, 0);
	ret = WSAStartup(version, &wsadata);

	//服务器端口创建
	SOCKET m_hServerSocket;
	m_hServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_hServerSocket)
	{
		cout << "服务器socket创建失败!" << endl;
	}
	else
	{
		cout << "服务器socket创建成功" << endl;
	}

	//服务器端口绑定
	SOCKADDR_IN m_addr;
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_addr.sin_port = htons(8042);
	ret = bind(m_hServerSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));

	//服务器端口开始监听
	ret = listen(m_hServerSocket, 20);//第二个参数指定最大申请的连接数
	//每当有一个客户端连接申请，则服务器端创建一个线程对其进行处理,模拟数据库服务器的处理方式
	SOCKET com_Sock;
	SOCKADDR_IN clntaddr;
	int clnlen = sizeof(clntaddr);
	cout << "waiting......" << endl;
	com_Sock = accept(m_hServerSocket, (SOCKADDR *)&clntaddr, &clnlen);
	cout << "Connected from IP address: " << inet_ntoa(clntaddr.sin_addr) << endl;
	return com_Sock;
}

void receive_data(SOCKET *cSocket, RingBuff *rb)
{
	char bf[1024];
	int ret;
	while (true)
	{
		ret = recv(*cSocket, bf, 1024, 0);
		while (ret > 0)
		{
			(*rb).lock();
			if ((*rb).put_data(ret, bf))
			{
				(*rb).unlock();
				break;
			}
			(*rb).unlock();
		}
		//cout << "re thread rb count " << (*(parg->rb)).get_count() << endl;
	}
}

unsigned int _stdcall send_data(LPVOID lpParam)
{
	REARGS *parg = (REARGS*)lpParam;
	char bf[112];
	while (true)
	{
		if ((*(parg->rb)).get_count() < 112)
			continue;
		(*(parg->rb)).lock();
		if ((*(parg->rb)).pop_data(112, bf))
		{
			(*(parg->rb)).unlock();
			send(*(parg->cSocket), bf, 112, 0);
			continue;
		}
		(*(parg->rb)).unlock();
		
		//cout << "send buff rb count " << (*(parg->rb)).get_count() << endl;
	}
	return 0;
}

unsigned int _stdcall subcontract(LPVOID lpParam)
{
	SUBARGS *subarg = (SUBARGS*)lpParam;
	char ID[10];
	char recv_msg[255] = { 0 };
	Message msg2;
	ResponseMessage ReMsg;
	int ReLen;

	while (true)
	{
		cout << "receive buff count " << (*(subarg->rb_r)).get_count() << endl;
		cout << "send buff count " << (*(subarg->rb_s)).get_count() << endl;
		if ((*(subarg->rb_r)).get_count() < 64)
			continue;
		(*(subarg->rb_r)).lock();
		(*(subarg->rb_r)).pop_data(10, ID);

		if (ID[0] != -1 || ID[1] != 0x54 || ID[2] != 0x52 || ID[3] != 0x43 || ID[4] != 0x4B)
		{
			(*(subarg->rb_r)).unlock();
			continue;
		}
		unsigned short frameLength = 0;
		memcpy(&frameLength, &ID[5], 2);

		int befor_encry = ntohs(frameLength) - 3;
		int padding_size = 16 - (befor_encry % 16);
		int after_padding_size = befor_encry + padding_size;

		(*(subarg->rb_r)).pop_data(after_padding_size, &recv_msg[10]);
		(*(subarg->rb_r)).unlock();





		memcpy(recv_msg, ID, 10);

		msg2.frameLength = ntohs(frameLength);

		unsigned char* recv_buf = (unsigned char*)malloc(after_padding_size * sizeof(unsigned char));
		memcpy(recv_buf, &recv_msg[10], after_padding_size);

		char* decrypt_buf = (char *)malloc(after_padding_size * sizeof(char));
		decrpyt_buf((const unsigned char*)recv_buf, (unsigned char**)&decrypt_buf, after_padding_size);

		memcpy(&recv_msg[10], decrypt_buf, befor_encry);

		memcpy(msg2.frameID, recv_msg, 5);

		unsigned short ManuName;
		memcpy(&ManuName, &recv_msg[7], 2);
		msg2.ManuName = ntohs(ManuName);

		memcpy(&msg2.Ver, &recv_msg[9], 1);

		memcpy(&msg2.AK, &recv_msg[10], 32);

		unsigned long ServiceID;
		memcpy(&ServiceID, &recv_msg[42], 8);
		msg2.ServiceID = ntohl(ServiceID);

		memcpy(&msg2.EntityLength, &recv_msg[50], 1);
		msg2.EntityName = (char*)malloc((int)(msg2.EntityLength + 1) * sizeof(char));
		memcpy(msg2.EntityName, &recv_msg[51], msg2.EntityLength);
		msg2.EntityName[msg2.EntityLength] = '\0';

		int Longitude;
		memcpy(&Longitude, &recv_msg[51 + msg2.EntityLength], 4);
		msg2.Longitude = ntohl(Longitude);

		int Latitude;
		memcpy(&Latitude, &recv_msg[55 + msg2.EntityLength], 4);
		msg2.Latitude = ntohl(Latitude);

		memcpy(&msg2.Coordinate, &recv_msg[59 + msg2.EntityLength], 1);

		unsigned short Velocity;
		memcpy(&Velocity, &recv_msg[60 + msg2.EntityLength], 2);
		msg2.Velocity = ntohs(Velocity);

		memcpy(&msg2.Direction, &recv_msg[62 + msg2.EntityLength], 1);

		unsigned int Time;
		memcpy(&Time, &recv_msg[63 + msg2.EntityLength], 4);
		msg2.Time = ntohl(Time);

		unsigned short CityID;
		memcpy(&CityID, &recv_msg[67 + msg2.EntityLength], 2);
		msg2.CityID = ntohs(CityID);

		memcpy(&msg2.Passenger, &recv_msg[69 + msg2.EntityLength], 1);

		unsigned short ExMsgLength;
		memcpy(&ExMsgLength, &recv_msg[70 + msg2.EntityLength], 2);
		msg2.ExMsgLength = ntohs(ExMsgLength);

		msg2.ExMsg = (char*)malloc((int)(msg2.ExMsgLength + 1) * sizeof(char));
		memcpy(msg2.ExMsg, recv_msg + 72 + (int)msg2.EntityLength, msg2.ExMsgLength);
		msg2.ExMsg[msg2.ExMsgLength] = '\0';

		cout << "帧标识: ";
		for (int i = 0; i < 5; i++)
			printf("%c", msg2.frameID[i]);
		cout << "\t帧长:" << msg2.frameLength << "\t\t厂商:" << msg2.ManuName << "\t协议版本:" << msg2.Ver << endl;
		cout << "AK:" << msg2.AK;
		cout << "\t服务ID:" << msg2.ServiceID << "\t终端标识长度:" << (int)(msg2.EntityLength) << "\t终端名:" << msg2.EntityName;

		cout << "\n经度:" << (float)(msg2.Longitude) << "\t纬度:" << (float)msg2.Latitude << "\t\t坐标类型:" << msg2.Coordinate[0]
			<< "\t速度:" << (float)(msg2.Velocity) << "\t\t方向:" << msg2.Direction[0] << endl;
		cout << "时间:" << msg2.Time << "\t\t城市编号:" << msg2.CityID << "\t载客:" << (int)msg2.Passenger;
		cout << "\t\t扩展长度:" << msg2.ExMsgLength << "\t扩展信息:" << msg2.ExMsg << endl << endl;
		workertianjiashuju(msg2);

		ReMsg.EntityLength = msg2.EntityLength;
		ReMsg.EntityName = msg2.EntityName;
		ReMsg.Time = htonl(msg2.Time);
		ReMsg.Status = 0;
		ReMsg.frameLength = htons(6 + ReMsg.EntityLength);

		ReLen = 13 + ReMsg.EntityLength;

		char* sendData = (char*)malloc(ReLen * sizeof(char));
		memcpy(sendData, &ReMsg, 8);
		memcpy(&sendData[8], ReMsg.EntityName, ReMsg.EntityLength);
		memcpy(&sendData[8 + ReMsg.EntityLength], &ReMsg.Time, 5);

		(*(subarg->rb_s)).lock();
		(*(subarg->rb_s)).put_data(ReLen, sendData);
		(*(subarg->rb_s)).unlock();

		free(sendData);
		free(decrypt_buf);
		free(recv_buf);
	}
	return 0;
}
