#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

int main()
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    // Control Connection 생성
    sock=socket(PF_INET, SOCK_STREAM, 0);   
    if(sock==-1)
        printf("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr("223.194.7.95");
    serv_adr.sin_port=htons(21);

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        printf("connect() error!");
    else
        printf("Connected...\n");

    // USER 명령 전송
    sprintf(message, "USER %s\r\n", "elec");
    write(sock, message, strlen(message));
    read(sock, message, BUF_SIZE - 1);
    printf("Response: %s\n", message);

    // PASS 명령 전송
    sprintf(message, "PASS %s\r\n", "comm");
    write(sock, message, strlen(message));
    read(sock, message, BUF_SIZE - 1);
    printf("Response: %s\n", message);

    // PASV 명령 전송
    sprintf(message, "PASV\r\n");
    write(sock, message, strlen(message));
    read(sock, message, BUF_SIZE - 1);
    printf("Response: %s\n", message);

    // TODO: PASV 응답에서 데이터 연결 포트 추출
    // NOTE: 실제 프로그램에서는 이 부분을 구현해야 합니다.

    // RETR 명령 전송
    sprintf(message, "RETR %s\r\n", "welcome.txt");
    write(sock, message, strlen(message));
    read(sock, message, BUF_SIZE - 1);
    printf("Response: %s\n", message);

    // TODO: 데이터 연결로부터 파일 내용 수신
    // NOTE: 실제 프로그램에서는 이 부분을 구현해야 합니다.

    close(sock);
    return 0;
}
