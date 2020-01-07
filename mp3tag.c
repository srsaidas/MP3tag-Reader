#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include "common.h"
#include "type.h"
//===========================Main Start=================================
int main(int argc, char * argv[])
{
		MP3Info mp3Info;
		if(argc > 1)
		{
				switch(check_operation(argv))
				{
						case e_help:
								print_help();
								break;

						case e_file:
								find_meta(argc, argv, &mp3Info);
								break;

						case e_option:
								printf("e-option");
								break;
						default:
								print_help();
								break;
				}
		}
		else
		{
				printf("Not enough number of arguments!\n");
				print_help();
		}
}
//==========================Main END ============================


