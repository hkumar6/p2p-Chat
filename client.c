#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct in_addr *pptr;
  struct hostent *server;

  char buffer[256];
  if (argc < 3)
  {
    fprintf(stderr, "usage : %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);

  /* Create a socket point */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
  {
    perror("Error opening socket\n");
    exit(1);
  }
  server = gethostbyname(argv[1]);
  if (server == NULL)
  {
    fprintf(stderr, "No such host\n");
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  pptr = (struct in_addr  *)server->h_addr;
  bcopy((char *)pptr, (char *)&serv_addr.sin_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  /* Connect to server */
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
  {
    perror("Error in connecting to server\n");
    exit(1);
  }

  /* Message to be sent to the server */
  printf("message to server : ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);

  /* Send message to server */
  n = write(sockfd, buffer, strlen(buffer));
  if(n < 0)
  {
    perror("Error writing to socket\n");
    exit(1);
  }

  /* Read server response */
  bzero(buffer, 256);
  if (n = read(sockfd, buffer, 255) > 0)
  {
    printf("%s", buffer);
  }
  if(n < 0)
  {
    perror("Error reading from socket\n");
    exit(1);
  }
  /* printf("Response: \n%s\n", buffer); */
  return 0;
}
