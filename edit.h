#ifndef EDIT_H
#define EDIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "type.h"
#include "edit.h"
Status store_frame_size(FILE * fptr, int size);
Status find_tag_size_pos(char * Tag, int * pos, int * size,  MP3Info * mp3Info);
Status validate(char * argv);
Status validate_argument(int argc, char * argv[]);
Status do_option(int argc, char * argv[], MP3Info *mp3Info);

#endif
