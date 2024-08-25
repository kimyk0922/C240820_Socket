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


	}														// 소켓의 공간이 문제가 없는지 확인.

	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY;				//	아무 주소나 사용하겠다. (다만, 밖의 주소를 사용할 수 있어 사용하지 않는 것이 좋다.)
	ServerSockAddr.sin_port = htons(15000);						//	포트 번호를 적어 줄때는 htons(포트번호)를 넣어줘야한다.


	//=========================================위의 단계가 IP와 Port Number Settings.==============================================================//


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

	//	blocking mode, 서비스 X

	SOCKET ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSockAddr, &ClientSockAddrSize);

	if (ClientSocket == INVALID_SOCKET)
	{

		printf("accept error %d", GetLastError());
		exit(-1);

	}


//============================================이미지 파일 전송======================================//

	char IBuffer[1024] = { 0, };		 //	전송 속도 Buffer 뒤의 [숫자]는 파일이 초당 전송될 크기.

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
	int SendBytes = send(ClientSocket, Message, strlen(Message), 0);	//	메시지를 실제로 전송하는게 아닌 OS 요청만 한 뒤 크기만 받는다.
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