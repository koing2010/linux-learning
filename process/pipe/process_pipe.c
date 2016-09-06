/*****************************************************
子进程如父进程之间 利用匿名管道通信

由于管道传输是半双工的，数据只能在单个方向上流动，父进
程往子进程传数据时，父进程的管道读端和子进程的管道写端
都可以先关闭
*/
#include "sys/wait.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stding.h"

int main(int argc, char *argc)
{
	int pipefd;
	pid_t child_pid;
	char buf;
	if( argc != 2)
	{
		fprintf(stderr,"Usage:%s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if(pipe(pipefd) == -1)
	{/*创建匿名pipe失败*/
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	child_pid = fork();
	if(child_pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if( child_pid == 0)
	{
		close(pipefd[1]); /*关闭不需要写的pipe*/
		while( read(pipefd, &buf, 1) > 0)
		{
			write( STDOUT_FILENO, &buf, 1);
		}
		write( STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
		_exit( EXIT_SUCCESS );
	}
	
	close(pipefd[0]);
	write(pipefd[1],argv[1], strlen(argv[1]));
	close(pipefd[1]);
	wait(NULL);
	
	return 0;
}