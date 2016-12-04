/*****************************************************************************
creat by Mr.kon @2016-11-13
*
****************************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"

int file_t;//serial port file
/*****************************************************************************
seriral_test
description:
open or close a usart port as a file.
edited by Mr.kon  @2016-11-14
*****************************************************************************/
static int seriral_test(void)
{
	int op_status = 0;
	file_t = open("/dev/ttySP0",O_RDWR | O_NOCTTY | O_NDELAY );
	if( file_t < 0)
	{
		printf("open SPI device error\n");
		return 0;
	}
	op_status = write(file_t, "usat test project!\n", 18);
	if(op_status < 0)
	{
		printf("write file error\n");
	}
	close(file_t);
	return 0;
}


/*****************************************************************************

******************************************************************************/
int main(int)
{

 seriral_test();

return 0;
}
