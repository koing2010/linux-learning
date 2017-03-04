/*
 * socket.h
 *
 *  Created on: Feb 24, 2017
 *      Author: Mrkon
 */

#ifndef SOCKET_H_
#define SOCKET_H_

int Recv(int sockfd, char *recvbuf, int len, int flags);
int Send(int sockfd, char *sendbuf, int len, int flags);

#endif /* SOCKET_H_ */
