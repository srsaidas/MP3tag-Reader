#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "type.h"
#include "edit.h"

// Store frame size
Status store_frame_size(FILE * fptr, int size)
{		int i;
		char *value =(char*)&size;
		for (i = 3; i >= 0; i--)
		{
				fwrite((value+i),1,1,fptr);
		}
}


// Find tag_size
Status find_tag_size_pos(char * Tag, int * pos, int * size,  MP3Info * mp3Info)
{
		int i;
		for (i = 0; i < 9; i++)
		{
				if(!strcmp(mp3Info->f_key[i],Tag))
				{
						*size = mp3Info->f_size[i]-1;
						*pos = mp3Info-> f_pos[i];
						return e_success;
				}

		}
		return e_failure;
}

// check arguments are valid
Status validate(char * argv)
{
		char opt[7][3] = {"-t","-T","-a","-A","-y","-c","-g"};

		int i;
		for (i = 0; i < 7; i++)
		{
				if (!strcmp(argv,opt[i]))
				{
						return e_success;
				}
		}

		return e_failure;
}

// DO operation
Status do_option(int argc, char * argv[], MP3Info *mp3Info)
{
		int i;
		char * string;
		uint length, f_pos;

		// If arguments are less than 3 it retuen failure
		if (argc <= 2)
		{
				return e_failure;
		}

		if (validate(argv[1]) == e_failure)
		{
				return e_failure;
		}

		if (find_meta(argc, argv[argc-1], mp3Info) == e_failure)
		{
				return e_failure;
		}


		FILE * fptr_dest = fopen("sample_1.mp3","wb+");
		fseek(mp3Info->fptr_src, 0L, SEEK_SET);

		char opt[7][3] = {"-t","-T","-a","-A","-y","-c","-g"};

		for (i = 0 ; i < 7 ; i ++)
		{
				if (!strcmp(argv[1], opt[i]))
				{
						string = argv[2];
						if(find_tag_size_pos(mp3Info->f_key[i], &f_pos, &length, mp3Info) == e_failure)
								return e_failure;
						break;

				}
		}

		// Copy  Frame tag ID
		for (i = 0;i < f_pos - 7; i++)
		{

				fread(&mp3Info->data, 1, 1, mp3Info -> fptr_src);
				fwrite(&mp3Info->data, 1, 1, fptr_dest);

		}
		int a = 0;
		
		//If COMM additional 4 byte need to be moved  
		if (!strcmp(argv[1], "-c"))
				a = 4;

		//Copy  Frame size
		store_frame_size(fptr_dest, a + strlen(string)+1);	
		fseek(mp3Info->fptr_src, 4, SEEK_CUR);

		//Copy 3 byte space after size
		for (i = 0; i < 3+a; i++)
		{
				fread(&mp3Info->data, 1, 1, mp3Info -> fptr_src);
				fwrite(&mp3Info->data, 1, 1, fptr_dest);
		}



		// Copy New string 
		for (i = 0; i < strlen(string); i++)
		{
				fwrite(&string[i], 1, 1, fptr_dest);
		}
		fseek(mp3Info->fptr_src, length-a, SEEK_CUR);

		//Copy Remaining data
		while( fread(&mp3Info->data, 1, 1, mp3Info -> fptr_src)>0 )
		{
				fwrite(&mp3Info->data, 1, 1, fptr_dest);
		}

		
		
		//Copy to orginal
		rewind(mp3Info -> fptr_src);
		rewind(fptr_dest);
		while( fread(&mp3Info->data, 1, 1, fptr_dest)>0 )
		{
				fwrite(&mp3Info->data, 1, 1,  mp3Info -> fptr_src);
		}
		return e_success;
}

