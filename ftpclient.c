#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main()
{
    int sock, data_sock;
    char message[BUF_SIZE];
    int str_len, data_port;
    struct sockaddr_in serv_adr, data_adr;
    int h1, h2, h3, h4, p1, p2;

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

    // PASV 응답에서 데이터 연결 포트 추출
    sscanf(message, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &h1, &h2, &h3, &h4, &p1, &p2);
    data_port = p1 * 256 + p2;

    // Data Connection 생성
    data_sock=socket(PF_INET, SOCK_STREAM, 0);   
    if(data_sock==-1)
        printf("socket() error");

    memset(&data_adr, 0, sizeof(data_adr));
    data_adr.sin_family=AF_INET;
    data_adr.sin_addr.s_addr=inet_addr("223.194.7.95");
    data_adr.sin_port=htons(data_port);

    if(connect(data_sock, (struct sockaddr*)&data_adr, sizeof(data_adr))==-1)
        printf("connect() error!");
    else
        printf("Data Connected...\n");

    // RETR 명령 전송
    sprintf(message, "RETR %s\r\n", "welcome.txt");
    write(sock, message, strlen(message));
    read(sock, message, BUF_SIZE - 1);
    printf("Response: %s\n", message);

    // 데이터 연결로부터 파일 내용 수신
    while ((str_len = read(data_sock, message, BUF_SIZE - 1)) != 0) {
        message[str_len] = 0;  // 널 문자를 추가하여 올바른 문자열로 만듭니다.
        printf("%s", message);
    }

    close(sock);
    close(data_sock);
    return 0;
}
