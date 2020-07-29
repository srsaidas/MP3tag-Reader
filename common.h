#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "type.h"
Status get_info_in_text_tag(MP3Info * mp3Info);
Status print_tag_data(MP3Info *mp3Info);
Status get_tag(char * tag, int size, FILE * fptr);
Status load_key_tag(MP3Info * mp3Info);
Status mp3_tag_head_size(MP3Info * mp3Info);
Status fram_size(char * size, FILE* fptr);
Status check_for_text_frame(MP3Info *mp3Info);
Status find_meta(int argc, char * argv, MP3Info *mp3Info);
Status validate_argv(int argc, char *argv, MP3Info *mp3Info);
Status open_file(MP3Info * mp3Info);
Status id_type(MP3Info * mp3Info);
void print_help();
OperationType check_operation(char * argv[]);

#endif
