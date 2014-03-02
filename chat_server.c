#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_USERS 100
#define USER_INFO 1024

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen, n_users = 0;
  char buffer[256];
  char users[MAX_USERS][USER_INFO];
  struct sockaddr_in serv_addr, cli_addr;
  int n, i;

  /* Command line arguments */
  if(argc < 2)
  {
    fprintf(stderr, "usage : %s port\n", argv[0]);
    exit(1);
  }

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
  while(1)
  {
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd < 0)
    {
      perror("Error on accept\n");
      exit(1);
    }

    /* if connection is established, communicate */
    bzero(buffer, 256);
    n = read( newsockfd, buffer, 255);
    if(n < 0)
    {
      perror("Error reading from socket\n");
      exit(1);
    }
    printf("New user : %s\n", buffer);

    /* Add new user to list */
    for(i=0; i<strlen(buffer); i++)
    {
      users[n_users][i] = buffer[i];
    }
    n_users++;
    
    /* write a response to the client */
    n = write(newsockfd, "Registered successfully\n\n", 25);
    if(n < 0)
    {
      perror("Error writing to socket\n");
      exit(1);
    }
    n = write(newsockfd, "Online users:\n", 14);
    if(n < 0)
    {
      perror("Error writing to socket\n");
      exit(1);
    }
    for(i=0; i<n_users; i++)
    {
      n = write(newsockfd, users[i], strlen(users[i]));
      if(n < 0)
      {
        perror("Error writing to socket\n");
        exit(1);
      }
    }
  }
  return 0;
}
