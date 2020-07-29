#include <stdio.h>
#include <string.h>
#include "common.h"
#include "type.h"


Status find_meta(int argc, char * argv, MP3Info *mp3Info)
{
		if (validate_argv(argc, argv, mp3Info) == e_failure)
		{
				fprintf(stderr,"%s function failed\n","validate_argv");
				return e_failure;
		}

		if (open_file(mp3Info) == e_failure)
		{
				fprintf(stderr,"%s function failed\n","Open_file");
				return e_failure;
		}

		if(id_type(mp3Info) == e_failure)
		{
				fprintf(stderr,"%s function failed\n","id_type");
				return e_failure;
		}
		if (mp3_tag_head_size(mp3Info) == e_failure)
		{
				fprintf(stderr,"%s function failed\n","mp3_tag_head_size");
				return e_failure;
		}

		if(load_key_tag(mp3Info) == e_failure)
		{
				fprintf(stderr,"%s function failed\n","load_key_tag");
				return e_failure;
		}

		if(check_for_text_frame(mp3Info) == e_failure)
		{
				fprintf(stderr,"%s function failed\n","Check for text_frame");
				return e_failure;
		}

		return e_success;
}

Status print_tag_data(MP3Info *mp3Info)
{
		printf("\n\n");
		printf("=================================================================\n");
		printf("File\t: %s\n", mp3Info->src_fname);
		printf("Version\t: %s\n", mp3Info->idv);
		printf("=================================================================\n");
		printf("Title\t: %s\n",mp3Info->f_data[0]);
		printf("Artist\t: %s\n",mp3Info->f_data[2]);
		printf("Album\t: %s\n",mp3Info->f_data[3]);
		printf("Year\t: %s\n",mp3Info->f_data[4]);
		printf("Genere\t: %s\n", mp3Info->f_data[6]);
		printf("Comment\t: %s\n", mp3Info->f_data[5]);
		printf("Image\t: %s %s\n", mp3Info->f_data[8],mp3Info->f_data[8]+strlen(mp3Info->f_data[8])+2);
		printf("=================================================================\n\n");
		return e_success;
}

//==========================================================================================================================

Status check_for_text_frame(MP3Info *mp3Info)
{
		char ch;
		while(ftell(mp3Info->fptr_src) < mp3Info->tag_size+10)
		{
				fread(&ch,1,1,mp3Info->fptr_src);
				if ((ch == 'T') || (ch == 'C') || ch == 'A')
				{
						get_tag(mp3Info->tag_4,4, mp3Info->fptr_src);
						get_info_in_text_tag(mp3Info);
				}


		}
}

// Getting tag 
Status get_info_in_text_tag(MP3Info * mp3Info)
{
		int j;
		int i = 0;
		while(i < 9)
		{       
				if(!strcmp(mp3Info->tag_4, mp3Info->f_key[i]))
				{
						// Getting frame size
						fram_size((char *)&mp3Info->f_size[i],mp3Info->fptr_src);

						// Skipping flag (2B)and null(1B)
						fseek(mp3Info->fptr_src,3L,SEEK_CUR);

						//storing flag position
						mp3Info -> f_pos[i] = ftell(mp3Info->fptr_src);


						// array to store tag data
						char *array = (char *)malloc(mp3Info->f_size[i]-1);

						//Reading tag data
						fread(array, mp3Info->f_size[i]-1, 1, mp3Info->fptr_src);


						for (j = 0; j < mp3Info->f_size[i]-1 ; j++)
						{
								if (array[j]==0 && array[j] > 127) && strcmp(mp3Info->tag_4,"APIC"))
										array[j] = 127;

						}

						//Copying addres 
						mp3Info -> f_data[i] = array;
						return e_success;
				}
				i++;

		}

}

// Getttig tag
Status get_tag(char * tag,int size, FILE * fptr)
{
		fseek(fptr,-1L,SEEK_CUR);
		fread(tag, 1, size,fptr);
		return e_success;
}

// Loading key value to array
Status load_key_tag(MP3Info * mp3Info)
{
		mp3Info->f_key[0] = "TIT2";
		mp3Info->f_key[1] = "TRCK";
		mp3Info->f_key[2] = "TPE1";
		mp3Info->f_key[3] = "TALB";
		mp3Info->f_key[4] = "TYER";
		mp3Info->f_key[5] = "COMM";
		mp3Info->f_key[6] = "TCON";
		mp3Info->f_key[7] = "TIME";
		mp3Info->f_key[8] = "APIC";
		return e_success;
}

// Reading tag head sizzze
Status mp3_tag_head_size(MP3Info * mp3Info)
{
		fseek(mp3Info->fptr_src,6L, SEEK_SET);
		fram_size((char *)&mp3Info->tag_size,mp3Info->fptr_src);
		return e_success;
}

// Reading frame size
Status fram_size(char * size, FILE* fptr)
{
		int i;
		for (i = 3; i >= 0; i--)
		{
				fread((size+i),1,1,fptr);
		}

}


// Finding Id of mp3 file
Status id_type(MP3Info * mp3Info)
{
		char buff[3];
		fread(buff,3,1,mp3Info->fptr_src);

		char mv;
		fread(&mv,1,1,mp3Info->fptr_src);
		if (!strcmp(buff,"ID3"))
		{

				mv += 48;
				sprintf(mp3Info->idv,"%s.%c","ID3 v2", mv);
				return e_success;
		}

		return e_success;

}



// Open mp3 file
Status open_file(MP3Info * mp3Info)
{
		if ((mp3Info->fptr_src = fopen(mp3Info->src_fname,"rb+")) == NULL)
		{
				perror("fopen");
				return e_failure;
		}
		
}


// validate arguments
Status validate_argv(int argc, char *argv, MP3Info *mp3Info)
{
		if ((strrchr(argv,'.') != NULL) && (strcmp(strrchr(argv,'.'),".mp3" )))
		{
				fprintf(stderr,"Please input a mp3 file\n");
				return e_failure;
		}
		mp3Info->src_fname = argv;
		return e_success;

}


// Print help message
void print_help()
{
		printf("usage: 	./a.out -[tYaAycg] \"value\" file.mp3\n\t./a.out -v\n\n");
		printf("%3s\t%s","-t","Modifies a Title tag\n");
		printf("%3s\t%s","-T","Modifies a Track tag\n");
		printf("%3s\t%s","-a","Modifies a Artist tag\n");
		printf("%3s\t%s","-A","Modifies a Album tag\n");
		printf("%3s\t%s","-y","Modifies a Year tag\n");
		printf("%3s\t%s","-c","Modifies a Comment tag\n");
		printf("%3s\t%s","-g","Modifies a Genre tag\n");
		printf("%3s\t%s","-h","Displays this help info\n");
		printf("%3s\t%s","-v","Prints version info\n");
}


// Check Operation type
OperationType check_operation(char * argv[])
{
		if (!strcmp(argv[1], "--help") || (!strcmp(argv[1], "-h"))) 
				return e_help;
		
		if (!strcmp(argv[1], "-v")) 
				return e_ver;

		if (argv[1][0] != '-')
				return e_file;

		if (argv[1][0] == '-' && argv[1][1] != '-')
				return e_option;


		return e_unsupport;
}
