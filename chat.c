#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



void serverMode(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  /*call to socket() */
  sockfd=socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0)
  {
    perror("Error opening socket\n");
    exit(1);
  }

  /*initialize socket structure*/
  bzero((char *) &serv_addr, sizeof(serv_addr)); /* set serv_addr to zeros*/
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* bind the host address */
  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("Error on binding\n");
    exit(1);
  }

  /* start listening for clients */
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  /* accept actual connection from client */
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if(newsockfd < 0)
  {
    perror("Error on accept\n");
    exit(1);
  }

  /* receive and send chat messages */
  while(1)
  {
    /* if connection is established, communicate */
    bzero(buffer, 256);
    n = read( newsockfd, buffer, 255);
    if(n < 0)
    {
      perror("Error reading from socket\n");
      exit(1);
    }
    printf("remote user > %s\n", buffer);

    /* Decode HTTP header */

    /* write a response to the client */
    printf("me >");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);
    n = write(newsockfd, buffer, strlen(buffer));
    if(n < 0)
    {
      perror("Error writing to socket\n");
      exit(1);
    }
  }
}


void clientMode(int argc, char *argv[])
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

  /* send and receive chat messages */
  while(1)
  {
    /* Message to be sent to the server */
    printf("me > ");
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
      printf("remote user > %s\n", buffer);
    }
    if(n < 0)
    {
      perror("Error reading from socket\n");
      exit(1);
    }
  }
}


int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    serverMode(argc, argv);
  }
  else if(argc == 3)
  {
    clientMode(argc, argv);
  }
  else
  {
    printf("usage :\n");
    printf("%s port\nor", argv[0]);
    printf("%s hostname port\n", argv[0]);
    exit(0);
  }
}
