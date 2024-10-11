#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<stdio.h>

#pragma comment (lib, "WSock32.lib")

void ChatServer();

void ChatClient();

int main() {

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//�������G���[
		printf("WinSock�̏������Ɏ��s���܂���\n");
		return 1;
	}

	printf("�T�[�o�[���Ȃ�0, �N���C�A���g���Ȃ�1����͂��Ă�������\n");

	int num = 0;
	scanf_s("%d", &num);

	if (num == 0) {
		ChatServer();
	}
	else if (num == 1) {
		ChatClient();
	}

	WSACleanup();

	return 0;
}

void ChatServer()
{
	//�|�[�g�ԍ�
	unsigned short port = 0;
	SOCKET sock;
	SOCKET sock2;

	//�|�[�g�ԍ��̓���
	printf("�|�[�g�ԍ�����͂��Ă�������\n");
	scanf_s("%hu", &port);

	//�\�P�b�g�쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);

	//�\�P�b�g�ɖ��O������
	SOCKADDR_IN saddr;
	SOCKADDR_IN from;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) != 0) {
		printf("�G���[�ł�\n");
		closesocket(sock);
		return;
	}

	printf("�\�P�b�g�ɖ��O������\n");

	if (listen(sock, 0) != 0) {
		printf("�G���[�ł�\n");
		closesocket(sock);
		return;
	}

	printf("�֐��̑ҋ@���\n");

	int fromlen = sizeof(from);

	sock2 = accept(sock, (SOCKADDR*)&from, &fromlen);

	printf("�ڑ����󂯓���\n");


	while (1) {
		int nRcv;
		char szBuf[1024];

		nRcv = recv(sock2, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';

		printf("��M�F%s\n", szBuf);
		printf("���M�F");

		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(sock2, szBuf, (int)strlen(szBuf), 0);

		if (strcmp(szBuf, "end") == 0) {
			printf("�I�����܂�");
			break;
		}

	}

	closesocket(sock2);
	WSACleanup();

}

void ChatClient()
{
	//�|�[�g�ԍ�
	unsigned short port = 0;

	char ip[64];
	struct hostent* hostName;

	SOCKADDR_IN saddr;
	memset(&saddr, 0, sizeof(SOCKADDR_IN));

	SOCKET sock;

	//�|�[�g�ԍ��̓���
	printf("�|�[�g�ԍ�����͂��Ă�������\n");
	scanf_s("%hu", &port);

	//�\�P�b�g�쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);

	printf("ip�A�h���X����͂��Ă�������\n");
	scanf_s("%63s", ip, (unsigned)_countof(ip));

	hostName = gethostbyname(ip);

	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = hostName->h_addrtype;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = *((u_long*)hostName->h_addr);

	if (connect(sock, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("�G���[�ł�\n");
		closesocket(sock);
		return;
	}

	while (1) {
		int nRcv;
		char szBuf[1024];

		printf("���M�F");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		send(sock, szBuf, (int)strlen(szBuf), 0);

		nRcv = recv(sock, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\0';
		printf("��M�F%s\n", szBuf);

		if (strcmp(szBuf, "end") == 0) {
			printf("�I�����܂�");
			break;
		}

	}

	closesocket(sock);
	WSACleanup();
}
