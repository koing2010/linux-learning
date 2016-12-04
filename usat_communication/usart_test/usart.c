/*****************************************************************************
creat by Mr.kon @2016-11-13
*
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <limits.h>
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>

#define MAX_READ_SIZE 0xFF

int file_t;//serial port file
unsigned char rxbuf[MAX_READ_SIZE];
/*****************************************************************************
seriral_test
description:
open or close a usart port as a file.
edited by Mr.kon  @2016-11-14`
*****************************************************************************/
static int seriral_test(void)
{
	int op_status = 0;
	file_t = open("/dev/ttySP0",O_RDWR | O_NOCTTY | O_NDELAY );
	if( file_t < 0)
	{
		printf("open SPI device error\n");
		return -1;
	}
	op_status = write(file_t, "usat test project!\n", 18);
	if(op_status < 0)
	{
		printf("write file error\n");
	}
	else
	{
		printf("write file success\n");
	}
	return 0;
}
/*****************************************************************************
 **/

/*****************************************************************************

******************************************************************************/
int main(void)
{
	pid_t uart_tx_pid;
	int status;

	file_t = open("/dev/ttySP0",O_RDWR | O_NOCTTY | O_NDELAY );
	if(status < 0)
	{
		return 0;
	}
	/** open the usart port0 **/
//	if( seriral_test() < 0)
//	{
//     return 0;
//	}

	uart_tx_pid = fork();

	/* child process */
	if(uart_tx_pid == 0)
	{
		printf("here is chld prcess, child_pid = %d  , paaent_pid = %d \n",getpid(),getppid() );
		file_t =  open("/dev/ttySP0",O_RDWR | O_NOCTTY | O_NDELAY );
		for(unsigned char i = 0; i < 5; i ++)
		{
			sleep(0.5);
			 status = write(file_t, "usart tx test i = %d\n", 20);
            if(status < 0)
            {
            	exit(0);
            }
			/*exit child process */
		}
		exit(0);
	}
	/* parent process */
	else if(uart_tx_pid > 0)
	{
		printf("parent process, parent_pid = %d \n",getpid());
		unsigned char n = 0;
		while(n < 5)
		{
			/* read MAX_READ_SIZE bytes data value from serial port */
			 int read_len = read(file_t, rxbuf ,MAX_READ_SIZE);
			if(read_len < MAX_READ_SIZE)
			{

				for( unsigned char i = 0; i < read_len; i --)
				{

					printf("%s", rxbuf[i]);//print the rx message from child process

				}
				n ++;
			}
		}
	}
	else
	{
		printf("fork erro,creat process erro! \n");
	}
	close(file_t);

return 0;
}
