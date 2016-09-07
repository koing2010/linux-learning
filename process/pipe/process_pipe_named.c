/*********************************************************
�������̼��� �����ܵ�ͨ��
*/
#include "uninstd.h"
#include "stdlid.h"
#include "fcntl.h"
#include "limits.h"
#include "sys/tyes.h"
#include "sys/stat.h"
#include "stdio.h"
#include "string.h"

#define BUFSIZE 512     //дһ�ε����ݸ���
int main(argc, char *argv[])
{
	const char *fifoname = "/tmp/fifo";
	int pipefd,datafd;
	int bytes,ret;
	char buffer[BUFSIZE];
	if( argc != 2 )
	{
		fprintf(srderr,"Usage:%s <filename>\n");
		exit(EXIT_FAILURE);
	}
	
	if(access( fifoname, F_OK) < 0) /*�ж��ļ��Ƿ����*/
	{
		ret = mkfifo(fifoname, 0777);/* �������fifo�ļ�*/
		if(ret < 0)
		{
			perror("mkfifo error");
		}
	}
	pipefd = open(fifoname, O_WRONLY);
	datafd = open(argv[1],O_RDONLY );
	if((pipefd > 0) && (datafd > 0))
	{
		bytes = read(datafd, buffer, BUFSIZE);
		while(bytes > 0) /*�ļ���ȡ�ɹ� */
		{
			ret = write(pipefd,buffer,BUFSIZE);
			if( ret < 0)
			{
				perror("write error");
				exit(EXIT_FAILURE);
			}
			bytes = read(datafd, buffer, BUFSIZE);
		}
		close(pipefd);
		close(datafd);
	}
	return 0;
}