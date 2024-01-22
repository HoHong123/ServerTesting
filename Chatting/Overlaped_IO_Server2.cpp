#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>

using namespace std;
#pragma comment (lib, "Ws2_32.lib") // cpp

/*
*	기본 자료형
	int, float, bool,
*/
/*
	사용자정의 자료형

	struct 구조체명
	{
		자료형 멤버변수명;
		자료형 멤버변수명2;
	};

	int num;

	구조체명 변수명;

	변수명.멤버변수명 = 값;
*/
struct ClientInfo
{
	WSAOVERLAPPED	overlapped;

	SOCKET			socket;
	char			buffer[512];
	WSABUF			dataBuf;
	DWORD			recvByte;
	DWORD			flags;
};

//	이런 형태의 함수가 어딘가 있다.
void CALLBACK RecvCallBackFunc(DWORD _error, DWORD _recvLen, LPWSAOVERLAPPED _overlapped, DWORD _inflags);

void CALLBACK SendCallBackFunc(DWORD _error, DWORD _recvLen, LPWSAOVERLAPPED _overlapped, DWORD _inflags)
{
	ClientInfo* info = (ClientInfo*)_overlapped;

	//	수신 대기
	int result = WSARecv(
		info->socket,
		&info->dataBuf,
		1,
		&info->recvByte,
		&info->flags,
		&info->overlapped,
		RecvCallBackFunc);

	if (result == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			cout << "비동처리 정상 완료" << endl;
		}
	}
}

void CALLBACK RecvCallBackFunc(DWORD _error, DWORD _recvLen, LPWSAOVERLAPPED _overlapped, DWORD _inflags)
{
	ClientInfo* info = (ClientInfo*)_overlapped;

	cout << "받았다" << endl;

	if (_recvLen == 0)
	{
		cout << "클라 연결 종료" << endl;
		closesocket(info->socket);
		delete info;

		return;
	}

	if (_error == 0)
	{
		cout << "수신 내용 : " << info->buffer << endl;
		//	핑퐁...
		WSASend(
			info->socket,
			&info->dataBuf,
			1,
			&info->recvByte,
			0,
			&info->overlapped,
			SendCallBackFunc);
	}
}

int main()
{
	//	WSAStartup
	WSADATA	wsa;
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup 에러" << endl;
		return -1;
	}

	//	소켓 생성
	SOCKET	mySocket = WSASocket(
		AF_INET,
		SOCK_STREAM,
		0,
		nullptr,
		0,
		WSA_FLAG_OVERLAPPED);

	if (mySocket == INVALID_SOCKET)
	{
		cout << "소켓 생성 에러" << endl;
		WSACleanup();
		return -1;
	}

	//	바인딩
	sockaddr_in		recviverAddr = {};
	recviverAddr.sin_family = AF_INET;
	recviverAddr.sin_port = htons(12345);
	recviverAddr.sin_addr.s_addr = INADDR_ANY;

	result = bind(
		mySocket,
		(sockaddr*)&recviverAddr,
		sizeof(recviverAddr));

	if (result != 0)
	{
		cout << "바인딩 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	//	리슨
	if (listen(mySocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "리슨 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
	}

	SOCKET clientSocket;

	while (true)
	{
		cout << "연결 대기 시작" << endl;

		//	어셉
		clientSocket = accept(mySocket, nullptr, nullptr);

		if (clientSocket == INVALID_SOCKET)
		{
			cout << "어셉 에러" << endl;
			closesocket(mySocket);
			WSACleanup();
			return -1;
		}

		cout << "클라 연결 성공" << endl;

		ClientInfo* info = new ClientInfo;
		info->socket = clientSocket;
		info->dataBuf.buf = info->buffer;
		info->dataBuf.len = 512;
		info->flags = 0;
		info->recvByte = 0;

		//	수신 대기
		result = WSARecv(
			info->socket,
			&info->dataBuf,
			1,
			&info->recvByte,
			&info->flags,
			&info->overlapped,
			RecvCallBackFunc);

		if (result == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
			{
				cout << "비동처리 정상 완료" << endl;
			}
			else
			{
				break;
			}
		}
	}

	//	해제들
	closesocket(clientSocket);
	closesocket(mySocket);
	WSACleanup();

	return 0;
}