// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#include "ipc.h"


int create_socket(void)
{
	/* TODO: Implement create_socket(). */
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   	}
	
	return sockfd;
	
}
// 443

int connect_socket(int fd)
{	
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;


	int cs = connect(fd, &addr, sizeof(addr));
	if(cs < 0){
		perror("ERROR connecting socket");
      	exit(1);
	}
	/* TODO: Implement connect_socket(). */
	return cs;
}

ssize_t send_socket(int fd, const char *buf, size_t len)
{
	/* TODO: Implement send_socket(). */
	return -1;
}

ssize_t recv_socket(int fd, char *buf, size_t len)
{
	/* TODO: Implement recv_socket(). */
	return -1;
}

void close_socket(int fd)
{
	/* TODO: Implement close_socket(). */
}
