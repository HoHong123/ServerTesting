#include <iostream>
#include <ws2tcpip.h>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	//===============================================
	//	WSAStartup
	WSADATA wsa;
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup 에러" << endl;
		return -1;
	}

	//===============================================
	//	소켓 생성
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);

	if (mySocket == INVALID_SOCKET)
	{
		cerr << "socket 생성 에러" << endl;
		WSACleanup();

		return -1;
	}

	//===============================================
	//	바인딩
	sockaddr_in     receiverAddr = {};
	receiverAddr.sin_family = AF_INET;					//	수신 프로토콜
	receiverAddr.sin_port = htons(12345);				//	수신자의 포트 번호
	receiverAddr.sin_addr.s_addr = INADDR_ANY;			//	수신할 IP

	//	bind
	result = bind(mySocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	if (result != 0)
	{
		cerr << "binding 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	//===============================================
	//	리슨
	// mySocket 소켓에 대한 연결 대기 큐를 생성하고 클라이언트 연결 요청을 받아들이는 상태로 선언
	if (listen(mySocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cerr << "listen 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	cout << "연결 대기 시작" << endl;

	//===============================================
	//	어셉
	// accept 함수는 클라이언트의 연결 소켓이 전달될 때까지 동기상태로 대기(Hold)한다.
	auto clientSocket = accept(mySocket, nullptr, nullptr);

	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "accept 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	cout << "클라 연결 성공" << endl;


	//	recv send
	while (true)
	{
		// 연결 성공 후 데이터 입력을 위한 버퍼 선언
		char buffer[512] = "";
		// clientSocket에서 받은 데이터를 추출하며 결과값 반환
		int recvBytes = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0)
		{
			cout << "클라 연결 문제" << endl;
			break;
		}

		// 서버 콘솔에 클라이언트에게 받은 데이터를 출력
		cout << "서버에서 수신한 메시지  " << buffer << endl;
		// 
		send(clientSocket, buffer, strlen(buffer), 0);
	}


	closesocket(clientSocket);
	closesocket(mySocket);
	WSACleanup();
	return 1;
}