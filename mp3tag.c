#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include <stdlib.h>
#include "common.h"
#include "type.h"
#include "edit.h"

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
								if(find_meta(argc, argv[1], &mp3Info) == e_success)
										print_tag_data(&mp3Info);
								break;

						case e_option:
								if(do_option(argc,argv, &mp3Info) == e_failure)
								{		print_help();}
								break;
						case e_ver:
								printf("Vesrion 1.1\n");
								break;

						default:
								printf("Invalid option ---'%s'", argv[1]);
								print_help();
								break;
				}

		}
		else
		{
				printf("Invalid option--\n");
				print_help();
		}
}
