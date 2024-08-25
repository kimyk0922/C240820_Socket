#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

int main()
{
	// WSAStartup(MAKEWORD(2, 0));	//과거에는 CPU가 실수 계산을 못해서 WSAStartup(2.0f) 방식으로 작성하였다.

	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (Result != 0)
	{

		printf("error");
		exit(-1);


	}

	//	socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);	//	소켓의 공간을 만들어 주는것.

	if (ServerSocket == SOCKET_ERROR)
	{

		printf("socket error %d", GetLastError());
		exit(-1);


	}

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				//	"127.0.0.1"은 본인의 IP 주소.
	ServerSockAddr.sin_port = htons(15000);


	//=========================================위의 단계가 IP와 Port Number Settings.==============================================================//


	Result = connect(ServerSocket, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Result != 0)
	{

		printf("connect error %d", GetLastError());
		exit(-1);

	}

//============================================이미지 파일 읽기======================================//

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

//	blocking mode 실제로는 사용 X 
	
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