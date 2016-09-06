/*********************************************************
两个进程间用 命名管道通信
*/
#include "uninstd.h"
#include "stdlid.h"
#include "fcntl.h"
#include "limits.h"
#include "sys/tyes.h"
#include "sys/stat.h"
#include "stdio.h"
#include "string.h"

#define BUFSIZE 512     //写一次的数据个数
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
	
	if(access( fifoname, F_OK) < 0) /*判断文件是否存在*/
	{
		ret = mkfifo(fifoname, 0777);/* 创建这个fifo文件*/
		if(ret < 0)
		{
			perror("mkfifo error");
		}
	}
	pipefd = open(fifoname, O_WRONLY);
	datafd = open(argv[1],O_RDONLY );
	return 0;
}