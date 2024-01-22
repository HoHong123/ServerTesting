#include <iostream>			// �����
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <map>
using namespace std;

//	Ws2_32.lib ��� ���̺귯�� ������ �����ض�.
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA	wsa;
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup ����" << endl;
		return -1;
	}

	////
	SOCKET	mySocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (mySocket == SOCKET_ERROR)
	{
		cout << "socket ���� ����" << endl;

		WSACleanup();
		return -1;
	}
	///
	//////////////////////////////////////////////////////////////////////////
	sockaddr_in	recvAddr		= {};
	recvAddr.sin_family			= AF_INET;
	recvAddr.sin_port			= htons(12345);
	recvAddr.sin_addr.s_addr	= INADDR_ANY;		//	������ IP�� ��ǻ�Ϳ� �ٿ��� ��� IP

	result = bind(mySocket, (sockaddr*)&recvAddr, sizeof(recvAddr));

	if (result != 0)
	{
		cout << "���ε� ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	sockaddr_in	destAddr = {};
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(12345);

	//#include <WS2tcpip.h> �ʿ�
	inet_pton(AF_INET, "127.0.0.1", &destAddr.sin_addr);

	cout << "���� ����" << endl;

	map<string, sockaddr_in>	userList;

	while (true)
	{
		/////////////////////////
		char buffer[1024] = "";
		sockaddr_in senderAddr;
		socklen_t   senderAddrLen = sizeof(senderAddr);

		int receivedBytes = recvfrom(
			mySocket,
			buffer,
			sizeof(buffer),
			0,
			(sockaddr*)&senderAddr,	// ������� ����
			&senderAddrLen);

		//	IP:Port �� ���ڿ� �����
		char senderIP[256] = "";
		inet_ntop(AF_INET, &senderAddr.sin_addr, senderIP, sizeof(senderIP));
		//	���ο� ���� ���� �����ϱ�
		char userInfo[256]; // "127.0.0.1:12345"
		sprintf_s(userInfo, "%s:%d", senderIP, senderAddr.sin_port);

		//	<STL>
		//	map�� �ִ� ������ Ȯ���� �� ������ �߰�
		//auto finder = userList.find(userInfo);

		////	�ڷᱸ�� �ȿ� �ش� ���� ������ ����.
		//if (finder == userList.end())
		//{
		//	userList.insert(make_pair(userInfo, senderAddr));
		//}
		userList[userInfo] = senderAddr;

		// ���ŵ� �ּҿ� ������ ���
		cout << "���ŵ� ���� : " << buffer << endl;

		//	map �ִ� ��� ������� ����
		for (auto i = userList.begin(); i != userList.end(); i++)
		{
			sendto(
				mySocket,
				buffer,
				strlen(buffer),
				0,						//	�÷���
				(sockaddr*)&i->second,	//	���� ���
				sizeof(i->second));		//	destAddr�� ����Ʈ
		}
		
		//	quit �Է½� �ݺ��� ����
		//	buffer == "ko : quit"
		if (strstr(buffer, "quit") != nullptr )
		{
			break;
		}
	}
	////
	closesocket(mySocket);
	WSACleanup();

	return 0;
}