#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
	// WSAStartup(MAKEWORD(2, 0));	//���ſ��� CPU�� �Ǽ� ����� ���ؼ� WSAStartup(2.0f) ������� �ۼ��Ͽ���.

	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (Result != 0)
	{

		printf("error");
		exit(-1);


	}

	//	socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);	//	������ ������ ����� �ִ°�.

	if (ServerSocket == SOCKET_ERROR)
	{

		printf("socket error %d", GetLastError());
		exit(-1);


	}

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				//	"127.0.0.1"�� ������ IP �ּ�.
	ServerSockAddr.sin_port = htons(15000);


	//=========================================���� �ܰ谡 IP�� Port Number Settings.==============================================================//


	Result = connect(ServerSocket, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result != 0)
	{

		printf("connect error %d", GetLastError());
		exit(-1);

	}

//============================================�̹��� ���� �б�======================================//

	FILE* OpenFile = fopen("C:/Cpp/L240820/Diablo.jpg", "wb");
	int IRecvBytes = 0;
	char IBuffer[1024] = { 0, };

	do
	{

		IRecvBytes = recv(ServerSocket, IBuffer, (int)sizeof(IBuffer), 0);

		if (IRecvBytes == 0)
		{
			break;
		}

		fwrite(IBuffer, sizeof(char), IRecvBytes, OpenFile);
		printf("Recv\n");

	}

	while (IRecvBytes != 0);

//===============================================================================================//

	char Buffer[1024] = { 0, };

//	blocking mode �����δ� ��� X 
	
	int RecvBytes = recv(ServerSocket, Buffer, 1024, 0);
	if (RecvBytes <= 0)
	{

		printf("recv error %d", GetLastError());
		exit(-1);

	}

	printf("client :%s\n", Buffer);

	char Message[1024] = { 0, };
	sprintf(Message, "%s from Client", Buffer);
	int SendBytes = send(ServerSocket, Message, (int)strlen(Message), 0);
	if (SendBytes <= 0)
	{

		printf("send error %d", GetLastError());
		exit(-1);

	}

	fclose(OpenFile);
	closesocket(ServerSocket);


	WSACleanup();

	return 0;

}