/*****************************************************
�ӽ����縸����֮�� ���������ܵ�ͨ��

���ڹܵ������ǰ�˫���ģ�����ֻ���ڵ�������������������
�����ӽ��̴�����ʱ�������̵Ĺܵ����˺��ӽ��̵Ĺܵ�д��
�������ȹر�
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
	{/*��������pipeʧ��*/
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
		close(pipefd[1]); /*�رղ���Ҫд��pipe*/
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