#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {

  //リクエストデータを受け取る
  char buffer[BUFFER_SIZE];
  read(client_socket, buffer, sizeof(buffer) - 1);
  printf("Request:\n%s\n", buffer);

  //簡単なレスポンスを送信
  const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";
  write(client_socket, response, strlen(response));

  close(client_socket);
}


int main (){
  // ソケットの作成と基本的な接続
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket == -1){
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  //ソケットのバインド
  struct sockaddr_in server_address = {0};
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1){
    perror("Binding failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  //接続待機状態にする
  if (listen(server_socket, 5) == -1) {
    perror("Listening failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }
  printf("Server is listening on port %d\n", PORT);

  //ループで複数リクエストを処理
  while(1){
    int client_socket = accept(server_socket, NULL, NULL);
    if(client_socket == -1) {
      perror("Connection acceptance failed");
      continue;
    }
    handle_client(client_socket);
  }

  close(server_socket);
  return 0;
}