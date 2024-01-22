#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <set>
using namespace std;
#pragma comment (lib, "Ws2_32.lib") // cpp

/*
*	�⺻ �ڷ���
	int, float, bool,
*/
/*
	��������� �ڷ���

	struct ����ü��
	{
		�ڷ��� ���������;
		�ڷ��� ���������2;
	};

	int num;

	����ü�� ������;

	������.��������� = ��;
*/
struct ClientInfo
{
	WSAOVERLAPPED	overlapped;

	SOCKET			socket;
	char			buffer[512];
	WSABUF			dataBuf;
	DWORD			recvByte;
	DWORD			flags;
	BOOL			isSend;
};

/*
	�ڷᱸ��

		������ ����.
		�������ٿ� �����ϴ�.
		���Ի����� �����ϴ�.
		
		�� �ؼ��ϸ鼭 �˻�(Ž��)�� ���ϴ�.
		map, set


		Ž��,  �˻�

*/

set<ClientInfo*>	g_clientList;


void CALLBACK RecvCallBackFunc(DWORD _error, DWORD _recvLen, LPWSAOVERLAPPED _overlapped, DWORD _inflags)
{
	ClientInfo* info = (ClientInfo*)_overlapped;

	if (_recvLen == 0)
	{
		cout << "Ŭ�� ���� ����" << endl;
		closesocket(info->socket);
		
		g_clientList.erase(info);
		
		delete info;

		return;
	}

	if (_error == 0)
	{
		if (info->isSend == false)
		{
			cout << "�޾Ҵ�" << endl;
			cout << "���� �޽��� : " << info->buffer << endl;

			for (auto iter = g_clientList.begin(); iter != g_clientList.end(); iter++)
			{
				ClientInfo* data = *iter;

				strcpy_s(data->dataBuf.buf, 512, info->dataBuf.buf);
				data->dataBuf.len = _recvLen;

				WSASend(
					data->socket,
					&data->dataBuf,
					1,
					nullptr,
					0,
					&data->overlapped,
					RecvCallBackFunc);

				data->isSend = true;
			}
		}
		else
		{
			info->dataBuf.len = 512;

			WSARecv(
				info->socket,
				&info->dataBuf,
				1,
				&info->recvByte,
				&info->flags,
				&info->overlapped,
				RecvCallBackFunc);

			cout << "���Ŵ�� ����" << endl;

			info->isSend = false;
		}
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
		cout << "WSAStartup ����" << endl;
		return -1;
	}

	//	���� ����
	SOCKET	mySocket = WSASocket(
		AF_INET,
		SOCK_STREAM,
		0,
		nullptr,
		0,
		WSA_FLAG_OVERLAPPED);

	if (mySocket == INVALID_SOCKET)
	{
		cout << "���� ���� ����" << endl;
		WSACleanup();
		return -1;
	}

	//	���ε�
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
		cout << "���ε� ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	//	����
	if (listen(mySocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "���� ����" << endl;
		closesocket(mySocket);
		WSACleanup();
	}

	SOCKET clientSocket;

	while (true)
	{
		cout << "���� ��� ����" << endl;

		//	���
		clientSocket = accept(mySocket, nullptr, nullptr);

		if (clientSocket == INVALID_SOCKET)
		{
			cout << "��� ����" << endl;
			closesocket(mySocket);
			WSACleanup();
			return -1;
		}

		cout << "Ŭ�� ���� ����" << endl;

		ClientInfo* info = new ClientInfo;
		info->socket		= clientSocket;
		info->dataBuf.buf	= info->buffer;
		info->dataBuf.len	= 512;
		info->flags			= 0;
		info->recvByte		= 0;
		info->isSend		= false;

		//	����Ʈ�� Ŭ�� ���� ����
		g_clientList.insert(info);

		//	���� ��Ʈ ����
		strcpy_s(info->dataBuf.buf, info->dataBuf.len, "system : welcome");
		info->dataBuf.len = strlen("system : welcome");

		//	�۽�
		WSASend(
			info->socket,
			&info->dataBuf,
			1,
			nullptr,
			0,
			&info->overlapped,
			RecvCallBackFunc);

		info->isSend = true;
	}

	//	������
	closesocket(clientSocket);
	closesocket(mySocket);
	WSACleanup();

	return 0;
}