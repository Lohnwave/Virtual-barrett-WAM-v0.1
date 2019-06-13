/*
Last Revised: 2019-06-10
 获取Leap motion 采集的手平移、姿态，并通过以太网传给服务器
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WINSOCK2.H>   
#include <stdio.h>     
#include <string>
#include<iostream>
#include<sstream>
#include <cstring>
#include "Leap.h"
#include <Windows.h>
//定义程序中使用的常量      
#define SERVER_ADDRESS "127.0.0.1" //服务器端IP地址      
#define PORT           3000       //服务器的端口号      
   
#pragma comment(lib, "ws2_32.lib")      
using namespace std;
using namespace Leap;
string generateCtrlCmd(float parm1, float parm2, float parm3, float parm4, float parm5, float parm6);
string int2str(const float &int_temp);
SOCKET initialize();

class SampleListener : public Listener {
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);
	virtual void onServiceChange(const Controller&);
	virtual void onDeviceFailure(const Controller&);
	virtual void onLogMessage(const Controller&, MessageSeverity severity, int64_t timestamp, const char* msg);
	Hand AcquireHand()const;
	Vector AcqureNormal()const;
	Vector AcqureDirection()const;
	Vector AcqurePosition()const;
	bool HasData();
private:
	Vector position;
	Vector normal;
	Vector direction;
};

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };

Vector SampleListener::AcqureNormal()const {
	return normal;
}

Vector SampleListener::AcqureDirection()const {
	return direction;
}

Vector SampleListener::AcqurePosition()const {
	return position;
}

void SampleListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		position = hand.palmPosition();
		normal = hand.palmNormal();
		direction = hand.direction();
	}
}

void SampleListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
		std::cout << "  isSmudged:" << (devices[i].isSmudged() ? "true" : "false") << std::endl;
		std::cout << "  isLightingBad:" << (devices[i].isLightingBad() ? "true" : "false") << std::endl;
	}
}

void SampleListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

void SampleListener::onServiceChange(const Controller& controller) {
	std::cout << "Service Changed" << std::endl;
}

void SampleListener::onDeviceFailure(const Controller& controller) {
	std::cout << "Device Error" << std::endl;
	const Leap::FailedDeviceList devices = controller.failedDevices();

	for (FailedDeviceList::const_iterator dl = devices.begin(); dl != devices.end(); ++dl) {
		const FailedDevice device = *dl;
		std::cout << "  PNP ID:" << device.pnpId();
		std::cout << "    Failure type:" << device.failure();
	}
}

void SampleListener::onLogMessage(const Controller&, MessageSeverity s, int64_t t, const char* msg) {
	switch (s) {
	case Leap::MESSAGE_CRITICAL:
		std::cout << "[Critical]";
		break;
	case Leap::MESSAGE_WARNING:
		std::cout << "[Warning]";
		break;
	case Leap::MESSAGE_INFORMATION:
		std::cout << "[Info]";
		break;
	case Leap::MESSAGE_UNKNOWN:
		std::cout << "[Unknown]";
	}
	std::cout << "[" << t << "] ";
	std::cout << msg << std::endl;
}

int main()
{
	float x = 0.f, y = 0.f, z = 0.f, a = 0.f, b = 0.f, c = 0.f;

	SOCKET sClient = initialize();

	string 	input="";

	//Sleep(1000);
	SampleListener listener;
	Controller controller;
	controller.addListener(listener);

	Vector HandPosition_pre, HandNormal_pre, HandDirection_pre; //上一帧的手的数据

	Vector HandPosition, HandNormal, HandDirection; //当前帧的手的数据

	Vector HandPosition_R, Eulerangel_R; //前后两帧的手的位姿变化量

	float x0= 0.f, y0= 0.f, z0= 0.f, a0= 0.f, b0= 0.f, c0= 0.f; //初始位姿

	HandPosition.x = 0;
	HandPosition.y = 0;
	HandPosition.z = 0;
	HandPosition_pre = HandPosition;
	/*HandDirection.x = 0;
	HandDirection.y = 0;
	HandDirection.z = 0;
	HandNormal.x = 0;
	HandNormal.y = 0;
	HandNormal.z = 0;	*/

	while (1)
	{
		Sleep(500);
		if (HandPosition_pre.x == 0 && HandPosition_pre.y == 0 && HandPosition_pre.z == 0)
		{
			HandPosition_pre = listener.AcqurePosition();
			HandNormal_pre = listener.AcqureNormal();
			HandDirection_pre = listener.AcqureDirection();
			x0 = HandPosition_pre.z;
			y0 = HandPosition_pre.x;
			z0 = HandPosition_pre.y;
			a0 = HandDirection_pre.pitch();
			b0 = HandNormal_pre.roll();
			c0 = HandDirection_pre.yaw();
		}
		else
		{
			HandPosition = listener.AcqurePosition();
			HandNormal = listener.AcqureNormal();
			HandDirection = listener.AcqureDirection();
			HandPosition_R.x = HandPosition.x - HandPosition_pre.x; 
			HandPosition_R.y = HandPosition.y - HandPosition_pre.y;
			HandPosition_R.z = HandPosition.z - HandPosition_pre.z;
			
			Eulerangel_R.x = HandDirection.pitch() - HandDirection_pre.pitch(); //弧为单位
			Eulerangel_R.y = HandNormal.roll() - HandNormal_pre.roll();
			Eulerangel_R.z = HandDirection.yaw() - HandDirection_pre.yaw();
		
		
			// 此处以毫米 弧度为单位
			if (fabs(HandPosition_R.x) > 10 || fabs(HandPosition_R.y) > 10 || fabs(HandPosition_R.z) > 10 || 
				(fabs(Eulerangel_R.x) > 0.2) || 
				(fabs(Eulerangel_R.y) > 0.2) || 
				(fabs(Eulerangel_R.z) > 0.2)) {
				// 相较于传递增量，可以避免累计误差
				x = HandPosition.z-x0;
				y = HandPosition.x-y0;
				z = HandPosition.y-z0;
				// 防止姿态变化过大影响逆运动学求解
				if (fabs(Eulerangel_R.x) < 0.5 && fabs(Eulerangel_R.y) < 0.5 && fabs(Eulerangel_R.z) < 0.5) {
					a = HandDirection.pitch() - a0;
					b = HandNormal.roll() - b0;
					c = HandDirection.yaw() - c0;
				}


				HandPosition_pre = HandPosition;
				HandNormal_pre = HandNormal;
				HandDirection_pre = HandDirection;

				input = generateCtrlCmd(x, y, z, a, b, c);
                std::cout << std::string(2, ' ') << "X方向上手的位移为: " << x << "    Y方向上手的位移为: " <<y<< "    Z方向上手的位移为: " << z << std::endl;
				std::cout << std::string(2, ' ') << "pitch变化量为: " << a << " 弧度, "<< "roll变化量为: " << b << " 弧度, "<< "yaw变化量为: " << c << " 弧度" << std::endl;
				int flag = send(sClient, input.c_str(), 60, 0);
				if (flag == SOCKET_ERROR)
				{
					cout << "发送失败" << endl;
				}
				else
				{

					cout << "发送成功" << endl;
				}
				
			}
		}
	}

	closesocket(sClient);
	WSACleanup();
	// Remove the sample listener when done
	controller.removeListener(listener);

	return 0;
}

string generateCtrlCmd(float parm1, float parm2, float parm3, float parm4, float parm5, float parm6)
{
	string instructions = int2str(parm1);
	instructions += ",";
	instructions += int2str(parm2);
	instructions += ",";	
	instructions += int2str(parm3);
	instructions += ",";	
	instructions += int2str(parm4);
	instructions += ",";
	instructions += int2str(parm5);
	instructions += ",";
	instructions += int2str(parm6);
	instructions += ",";

	return instructions;
}

string int2str(const float &int_temp)
{
	string string_temp;
	stringstream stream;
	stream << int_temp;
	string_temp = stream.str();   //此处也可以用 stream>>string_temp  
	return string_temp;
}
SOCKET initialize() {

	WSADATA wsaData;
	//连接所用套节字      
	SOCKET sClient;
	//保存远程服务器的地址信息      
	SOCKADDR_IN server;
	//收发缓冲区      


	// Initialize Windows socket library      
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("初始化Winsock失败");
		return 0;
	};

	// 创建客户端套节字      
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET指明使用TCP/IP协议族；      
														 //SOCK_STREAM, IPPROTO_TCP具体指明使用TCP协议      
														 // 指明远程服务器的地址信息(端口号、IP地址等)      
	memset(&server, 0, sizeof(SOCKADDR_IN)); //先将保存地址的server置为全0      
	server.sin_family = PF_INET; //声明地址格式是TCP/IP地址格式      
	server.sin_port = htons(PORT); //指明连接服务器的端口号，htons()用于 converts values between the host and network byte order      
	server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);      
	int iRetVal = connect(sClient, (struct sockaddr *) &server, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == iRetVal)
	{
		 printf("服务器连接失败！\n");
	}
	else
	{
		printf("服务器连接成功！\n");
	}
	return sClient;

}