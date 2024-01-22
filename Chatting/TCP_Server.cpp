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
		cout << "WSAStartup ����" << endl;
		return -1;
	}

	//===============================================
	//	���� ����
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);

	if (mySocket == INVALID_SOCKET)
	{
		cerr << "socket ���� ����" << endl;
		WSACleanup();

		return -1;
	}

	//===============================================
	//	���ε�
	sockaddr_in     receiverAddr = {};
	receiverAddr.sin_family = AF_INET;					//	���� ��������
	receiverAddr.sin_port = htons(12345);				//	�������� ��Ʈ ��ȣ
	receiverAddr.sin_addr.s_addr = INADDR_ANY;			//	������ IP

	//	bind
	result = bind(mySocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	if (result != 0)
	{
		cerr << "binding ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	//===============================================
	//	����
	// mySocket ���Ͽ� ���� ���� ��� ť�� �����ϰ� Ŭ���̾�Ʈ ���� ��û�� �޾Ƶ��̴� ���·� ����
	if (listen(mySocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cerr << "listen ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	cout << "���� ��� ����" << endl;

	//===============================================
	//	���
	// accept �Լ��� Ŭ���̾�Ʈ�� ���� ������ ���޵� ������ ������·� ���(Hold)�Ѵ�.
	auto clientSocket = accept(mySocket, nullptr, nullptr);

	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "accept ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	cout << "Ŭ�� ���� ����" << endl;


	//	recv send
	while (true)
	{
		// ���� ���� �� ������ �Է��� ���� ���� ����
		char buffer[512] = "";
		// clientSocket���� ���� �����͸� �����ϸ� ����� ��ȯ
		int recvBytes = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0)
		{
			cout << "Ŭ�� ���� ����" << endl;
			break;
		}

		// ���� �ֿܼ� Ŭ���̾�Ʈ���� ���� �����͸� ���
		cout << "�������� ������ �޽���  " << buffer << endl;
		// 
		send(clientSocket, buffer, strlen(buffer), 0);
	}


	closesocket(clientSocket);
	closesocket(mySocket);
	WSACleanup();
	return 1;
}