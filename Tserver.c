#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define PORT 3031
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
  int serv_sock, clnt_sock;
  struct sockaddr_in serv_addr, clint_addr;
  socklen_t clnt_addr_size;
  int str_len;
  char message[BUF_SIZE];

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  // int socket(int domain, int type, int protocol);     // 성공시 파일 디스크럽터, 실패시 -1 반환
  // domain : 소켓이 사용할 프로토콜 체계(Protocol Family) 정보 전달
  // type : 소켓의 데이터 전송방식에 대한 정보 전달
  // protocol : 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달

  if (serv_sock == -1)
    error_handling("Socket() error");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error");

  if (listen(serv_sock, 5) == -1)
    error_handling("listen() error");
  printf("listen on PORT %d \n", PORT);

  while (1)
  {

    clnt_addr_size = sizeof(clint_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clint_addr, &clnt_addr_size);

    if (clnt_sock == -1)
      error_handling("accept() error");
    else
      printf("Connected client \n");
    while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
    {
      write(clnt_sock, message, str_len);
    }

    close(clnt_sock);
  }
  close(serv_sock);

  /* code */
  // #include <unistd.h>
  // ssize_t write(int fd, const void * buf, size_t nbytes); // 성공시 전달한 바이트 수, 실패시 -1 반환
  // fd : 데이터 전송대상을 나타내는 파일 디스크립터 전달
  // buf : 전송할 데이터가 저장된 버퍼의 주소 값 전달
  // nbytes : 전송할 데이터의 바이트 수 전달
  return 0;
}

void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
