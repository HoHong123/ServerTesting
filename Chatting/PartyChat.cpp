#include <iostream>			// 입출력
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <map>
#include <Ws2def.h>

using namespace std;

//	Ws2_32.lib 라는 라이브러리 파일을 포함해라.
#pragma comment(lib, "Ws2_32.lib")


// UTF8, 16, 32, UNICODE, ANSI, ASCII
// UTF-8 => ANSI로 변경하여 한글을 출력하라.
void UTF8to_ANSI(char buffer[], int _byteSize)
{
	// 받은 데이터를 wide character로 변환

	// 문자열 크기 측정
	int wideBufferSize = MultiByteToWideChar(CP_UTF8, 0, buffer, _byteSize, nullptr, 0);
	// 크기만큼의 유니코드형 문자열 동적 할당
	wchar_t* wideBuffer = new wchar_t[wideBufferSize + 1];

	// UTF8 -> UNICODE
	MultiByteToWideChar(CP_UTF8, 0, buffer, _byteSize, wideBuffer, wideBufferSize);
	// NULL 문자
	wideBuffer[wideBufferSize] = L'\0';

	// wide char 
	// 유니코드로 바꾼 문자열의 크기 측정
	int ansiLength = WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, NULL, 0, NULL, NULL);
	// 그것을 buffer에 변환하여 사용 ( UNICODE -> ANSI )
	WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, buffer, ansiLength, NULL, NULL);
	// NULL 문자 처리
	buffer[ansiLength] = '\0';

	// 동적할당 해제
	delete[] wideBuffer;
}	

int main()
{
	WSADATA	wsa;
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup 에러" << endl;
		return -1;
	}

	////
	SOCKET	mySocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (mySocket == SOCKET_ERROR)
	{
		cout << "socket 생성 에러" << endl;

		WSACleanup();
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	sockaddr_in	recvAddr = {};
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(12346);
	recvAddr.sin_addr.s_addr = INADDR_ANY;		//	수신할 IP는 컴퓨터에 붙여진 모든 IP

	result = bind(mySocket, (sockaddr*)&recvAddr, sizeof(recvAddr));

	if (result != 0)
	{
		cout << "바인딩 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	sockaddr_in	destAddr = {};
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(12346);

	//#include <WS2tcpip.h> 필요
	inet_pton(AF_INET, "192.168.0.114", &destAddr.sin_addr);

	cout << "서버 시작" << endl;

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
			(sockaddr*)&senderAddr,	// 보낸사람 정보
			&senderAddrLen);

		//	IP:Port 로 문자열 만들고
		char senderIP[256] = "";
		inet_ntop(AF_INET, &senderAddr.sin_addr, senderIP, sizeof(senderIP));
		//	새로운 유저 정보 저장하기
		char userInfo[256]; // "127.0.0.1:12345"
		sprintf_s(userInfo, "%s:%d", senderIP, senderAddr.sin_port);

		// 수신된 주소와 내용을 출력
		cout << "수신된 내용 : " << buffer << endl;

		
		// 데이터를 전달하고 돌려받을 때, 오류가 발생한다면 응답 코드를 넣어 반환한다.
		if (receivedBytes < 1)
		{
			// 0 = 데이터가 없음 오류
			// 음수 = 데이터 오류
			int errorCode = WSAGetLastError();

			// 강제 종료
			if (errorCode == WSAECONNRESET) 
			{
				userList.erase(userInfo);

				cout << userInfo << " : 연결해제" << endl;

				continue;
			}
		}
		else {
			//	<STL>
			//	map에 있는 것인지 확인한 후 없으면 추가
			//userList[userInfo] = senderAddr;
			auto finder = userList.find(userInfo);

			//	자료구조 안에 해당 유저 정보는 없다.
			if (finder == userList.end())
			{
				userList.insert(make_pair(userInfo, senderAddr));
			}
		}


		//	map 있는 모든 사람에게 전송
		for (auto i = userList.begin(); i != userList.end(); i++)
		{
			sendto(
				mySocket,
				buffer,
				strlen(buffer),
				0,						//	플래그
				(sockaddr*)&i->second,	//	받을 사람
				sizeof(i->second));		//	destAddr의 바이트
		}

		// UTF8 -> ANSI변환
		UTF8to_ANSI(buffer, sizeof(buffer));

		// 수신된 주소와 내용을 출력
		cout << "수신된 내용 : " << buffer << endl;

		//	quit 입력시 반복문 종료
		//	buffer == "ko : quit"
		if (strstr(buffer, "quit") != nullptr)
		{
			break;
		}
	}


	////
	closesocket(mySocket);
	WSACleanup();

	return 0;
}