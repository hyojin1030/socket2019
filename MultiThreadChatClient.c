#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>

#define CHATDATA 1024
#define IPADDR "127.0.0.1"
#define PORT 9000
#define WHISPER "/w"
#define DELIMETER " "

void *do_send_chat(void *);	//메세지 보내기
void *do_receive_chat(void *); //메세지 받기
pthread_t thread_1, thread_2;

char	escape[ ] = "exit";
char	nickname[20];

int main(int argc, char *argv[ ])
{
	int c_socket;
	struct sockaddr_in c_addr;
	int len;
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int nfds;
	fd_set read_fds;
	int n;
	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	printf("Input nickname : ");
	scanf("%s", nickname);
	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1) {
		printf("Can not connect\n");
		return -1;
	}
	write(c_socket, nickname, strlen(nickname));	
	pthread_create(&thread_1, NULL, do_send_chat, (void *) &c_socket);
	pthread_create(&thread_2, NULL, do_receive_chat, (void *) &c_socket);
	//do_sent_chat과 do_receive_chat의 스레드를 생성
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	//스레드 종료를 기다림
	close(c_socket);
}
void * do_send_chat(void *arg)
{
	char chatData[CHATDATA];
	char buf[CHATDATA];
	int n;
	int c_socket = *((int *) arg);
	while(1) {
		memset(buf, 0, sizeof(buf));
		if((n = read(0, buf, sizeof(buf))) > 0 ) {	// 키보드로 문자열을 입력받아 buf에 저장
			char *token = NULL;
			char *toNickname = NULL;
			char *message = NULL;
			if(strncasecmp(buf, WHISPER, 2) == 0){
				token = strtok(buf, DELIMETER);
				if(token != NULL)
					toNickname = strtok(NULL, DELIMETER);
				if(toNickname != NULL)
					message = strtok(NULL, "\0");
				if(token == NULL || toNickname == NULL || message ==  NULL){ //귓속말을 보내기 위한 명령어 중 NULL이 있다면 출력
					printf("Your whisper message is wrong. Please input '/w nickname message'\n");
					continue;
				}
				sprintf(chatData, "%s %s [%s] %s", \
					token, toNickname, nickname, message);
			}else{ 
				sprintf(chatData, "[%s] %s", nickname, buf);
			}
			write(c_socket, chatData, strlen(chatData)); //서버소켓으로 메세지 전달
			if(!strncmp(buf, escape, strlen(escape))) { //exit을 입력하면 종료
				pthread_kill(thread_2, SIGINT);
				break;
			}
		}
	}
}
void *do_receive_chat(void *arg)
{
	char	chatData[CHATDATA];
	int	n;
	int	c_socket = *((int *)arg);	
	while(1) {
		memset(chatData, 0, sizeof(chatData));
		if((n = read(c_socket, chatData, sizeof(chatData))) > 0 ) { //메세지를 화면에 출력
			write(1, chatData, n);
		}
	}
}
