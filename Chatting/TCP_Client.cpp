#include <iostream>
#include <string>
#include <ws2tcpip.h>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	//	WSAStartup	
	WSADATA wsa;
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup 에러" << endl;
		return -1;
	}

	//	socket
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);

	if (mySocket == INVALID_SOCKET)
	{
		cerr << "socket 생성 에러" << endl;
		WSACleanup();

		return -1;
	}

	// 수신자의 주소 설정
	sockaddr_in     receiverAddr = {};
	receiverAddr.sin_family = AF_INET;					//	수신 프로토콜
	receiverAddr.sin_port = htons(12345);				//	수신자의 포트 번호
	inet_pton(AF_INET, "127.0.0.1", &receiverAddr.sin_addr);

	//	연결 시도
	result = connect(mySocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	if (result == SOCKET_ERROR)
	{
		cerr << "연결 에러" << endl;
		closesocket(mySocket);
		WSACleanup();

		return -1;
	}

	cout << "서버에 연결됨" << endl;

	while (true)
	{
		string msg;

		cout << "입력 : ";
		getline(cin, msg);

		if (msg == "exit")
		{
			break;
		}

		send(mySocket, msg.c_str(), msg.length(), 0);

		char buffer[512] = "";
		int recvBytes = recv(mySocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0)
		{
			cout << "연결 문제" << endl;
			break;
		}

		cout << "수신 메시지 : " << buffer << endl;
	}

	//	recv send
	closesocket(mySocket);
	WSACleanup();
	return 1;
}