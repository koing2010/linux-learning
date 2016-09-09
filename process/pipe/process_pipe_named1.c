/******************************************************
命名管道通信的第二个进程

*/
#include "uninstd.h"
#include "stdlid.h"
#include "fcntl.h"
#include "limits.h"
#include "sys/tyes.h"
#include "sys/stat.h"
#include "stdio.h"
#include "string.h"
#define BUFSIZE 512

int main(int argc, char *argv[])
{
	const char *fifoname = "/tmp/fifo";
	int pipefd, datafd;
	int bytes, ret;
	char buffer[BUFSIZE];
	
	if( argc != 2 )
	{
		fprintf(stderr, "Usage: %s <>\n", argv[0]);
		exit(EXIT_FALURE);
	}
	pipefd = open(fifoname,O_RDONLY);   /* 打开管道文件 */
	datafd = open(argv[],O_WRONLY|O_CREAT,0644); /* 打开目标文件 */
	
	if((pipefd > 0) && (datafd > 0))/* 成功打开管道后将管道文件读出并写入目标文件 */
	{
		bytes = read(pipefd, buffer, BUFSIZE);
		while(bytes > 0)
		{
			ret = write(datafd, buffer, bytes);//写入
			if(ret < 0)
			{
				perro("write erro\n");
			}
			bytes = read(pipefd, buffer, BUFSIZE);
		}
	}
	close(pipefd);
	close(datafd);
	
	return 0;
}