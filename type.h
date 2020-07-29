#ifndef TYPE_H
#define TYPE_H

#define FRAME_KEY_ARRAY_SIZE 9

typedef unsigned uint;
typedef struct _mp3Info
{
		char * src_fname;
		FILE * fptr_src;
		uint tag_size;
char *arg_key[7];
char data;
char idv[9];

char tag_3[3];
char tag_4[5];

// Frame Title
int f_size[FRAME_KEY_ARRAY_SIZE];
int f_pos[FRAME_KEY_ARRAY_SIZE];
char *f_data[FRAME_KEY_ARRAY_SIZE];
char *f_key[FRAME_KEY_ARRAY_SIZE];

// PIC Frame

// COM

} MP3Info;


typedef enum
{
		e_failure,
		e_success
}Status;

typedef enum
{		e_help,
		e_file,
		e_option,
e_ver,
		e_unsupport
}OperationType;

#endif
