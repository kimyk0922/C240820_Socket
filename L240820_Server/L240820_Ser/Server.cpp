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


	}														// ������ ������ ������ ������ Ȯ��.

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY;				//	�ƹ� �ּҳ� ����ϰڴ�. (�ٸ�, ���� �ּҸ� ����� �� �־� ������� �ʴ� ���� ����.)
	ServerSockAddr.sin_port = htons(15000);						//	��Ʈ ��ȣ�� ���� �ٶ��� htons(��Ʈ��ȣ)�� �־�����Ѵ�.


	//=========================================���� �ܰ谡 IP�� Port Number Settings.==============================================================//


	Result = bind(ServerSocket, (sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result != 0)
	{

		printf("bind error %d", GetLastError());
		exit(-1);

	}

	Result = listen(ServerSocket, 0);
	if (Result != 0)
	{

		printf("listen error %d", GetLastError());
		exit(-1);

	}

	struct sockaddr_in ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrSize = sizeof(ClientSockAddr);

	//	blocking mode, ���� X

	SOCKET ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSockAddr, &ClientSockAddrSize);

	if (ClientSocket == INVALID_SOCKET)
	{

		printf("accept error %d", GetLastError());
		exit(-1);

	}


//============================================�̹��� ���� ����======================================//

	char IBuffer[1024] = { 0, };		 //	���� �ӵ� Buffer ���� [����]�� ������ �ʴ� ���۵� ũ��.

	size_t ReadCount = 0;
	FILE* ImageFile = fopen("C:/Cpp/L240820/L240820_Server/Diablo.jpg", "rb");

	do
	{

		ReadCount = fread(IBuffer, sizeof(char), sizeof(IBuffer), ImageFile);
		int SendBytes = send(ClientSocket, IBuffer, (int)ReadCount, 0);
		printf("Send\n");

	} 
	while (ReadCount != 0);

//===============================================================================================//

	char Message[1024] = "Hello World";
	int SendBytes = send(ClientSocket, Message, strlen(Message), 0);	//	�޽����� ������ �����ϴ°� �ƴ� OS ��û�� �� �� ũ�⸸ �޴´�.
	if (SendBytes <= 0)
	{

		printf("send error %d", GetLastError());
		exit(-1);

	}

	char Buffer[1024] = { 0, };
	int RecvBytes = recv(ClientSocket, Buffer, 1024, 0);
	if (RecvBytes <= 0)
	{

		printf("recv error %d", GetLastError());
		exit(-1);

	}

	printf("server : % s", Buffer);

	fclose(ImageFile);
	closesocket(ClientSocket);
	closesocket(ServerSocket);


	WSACleanup();

	return 0;

}