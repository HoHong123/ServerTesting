#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>

using namespace std;
#pragma comment (lib, "Ws2_32.lib") // cpp

void CALLBACK RecvCallBackFunc(DWORD _error,
	DWORD _recvLen,
	LPWSAOVERLAPPED _overlapped,
	DWORD _inflags)
{
	cout << "받았다" << endl;
}

int main()
{
	//===============================================
	//	WSAStartup
	WSADATA	wsa;
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup 에러" << endl;
		return -1;
	}

	//===============================================
	//	소켓 생성
	SOCKET	mySocket = WSASocket(
		AF_INET,				// 인터넷 IPv4 주소 체계 선택
		SOCK_STREAM,			// TCP 통신 프로토콜 선택
		0,						// 사용할 프로토콜 = 0이면 적절한 자동 할당
		nullptr,				// 연결된 클라이언트의 정보를 저장할 WSAPROTOCOL_INFO 구조체 포인터
		0,						// 소켓 그룹 ID = GID 같은 개념
		WSA_FLAG_OVERLAPPED);	// 추가 동작에 대한 소켓 특성 지정
	// WSA_FLAG_OVERLAPPED : 비동기 소켓 작업 수행 플레그로 IOCP 서버와 함께 사용됌

	if (mySocket == INVALID_SOCKET)
	{
		cout << "소켓 생성 에러" << endl;
		WSACleanup();
		return -1;
	}

	//===============================================
	//	바인딩
	sockaddr_in		recviverAddr = {};
	recviverAddr.sin_family = AF_INET;		// IPv4 주소 체계 설정
	recviverAddr.sin_port = htons(12345);	// 포트 선택 및 호스트 IP 포트 번호를 네트워크 IP 포트 번호로 전환
	recviverAddr.sin_addr.s_addr = INADDR_ANY;	// 해당 포트에 접근하는 모든 IP 허용

	result = bind(
		mySocket,					// 포트와 연결할 소켓 생성
		(sockaddr*)&recviverAddr,	// 포트를 통해 받는 정보를 저장할 구조체 포인터
		sizeof(recviverAddr));		// 정보를 저장할 구조체의 크기 설정

	if (result != 0)
	{
		cout << "바인딩 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	//===============================================
	//	리슨
	// mySocket 소켓에 대한 연결 대기 큐를 생성하고 클라이언트 연결 요청을 받아들이는 상태로 선언
	if (listen(mySocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "리슨 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
	}


	// 클라이언트 소켓 선언
	SOCKET clientSocket;

	while (true)
	{
		cout << "연결 대기 시작" << endl;

		//===============================================
		//	어셉
		// 클라이언트 요청을 받고 소켓에 할당
		// mySocket은 비동기 처리 동작을 수행하는 소켓이기에 연결이 된다면 비동기로 처리된다.
		// 하여 클라이언트와 연결되더라도 비동기적 수신을 한다.
		clientSocket = accept(mySocket,		// listen 함수로 설정된 서버 소켓 핸들러
							nullptr,		// 연결된 클라이언트 주소 정보를 저장할 구조체 포인터
							nullptr);		// 클라이언트 주소 정보 구조체의 크기

		if (clientSocket == INVALID_SOCKET)
		{
			cout << "어셉 에러" << endl;
			closesocket(mySocket);
			WSACleanup();
			return -1;
		}

		cout << "클라 연결 성공" << endl;

		char buffer[512] = "";

		WSABUF	dataBuf;
		dataBuf.buf = buffer;
		dataBuf.len = sizeof(buffer);

		DWORD		recvByte = 0;
		DWORD		flags = 0;

		// 
		WSAOVERLAPPED	overlapped = {};

		//===============================================
		//	수신 대기
		// 비동기 소켓 데이터 수신 함수
		result = WSARecv(
			clientSocket,		// 데이터를 받은 소켓
			&dataBuf,			// 데이터를 저장할 버퍼 배열
			1,					// 버퍼 배열 원소 개수
			&recvByte,			// 실제 수신된 데이터와 바이트 수를 나타낸 변수 데이터
			&flags,				// 소켓 플래그
			&overlapped,		// 비동기 작업에 사용되는 오버랩 구조체 포인터
			RecvCallBackFunc);	// 비동기 작업 완료 시 호출되는 콜백 함수

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

	//===============================================
	//	해제들
	closesocket(clientSocket);
	closesocket(mySocket);
	WSACleanup();

	return 0;
}