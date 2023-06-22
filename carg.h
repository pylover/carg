// Copyright 2023 Vahid Mardani
/*
 * This file is part of Carrow.
 *  Carrow is free software: you can redistribute it and/or modify it under 
 *  the terms of the GNU General Public License as published by the Free 
 *  Software Foundation, either version 3 of the License, or (at your option) 
 *  any later version.
 *  
 *  Carrow is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
 *  details.
 *  
 *  You should have received a copy of the GNU General Public License along 
 *  with Carrow. If not, see <https://www.gnu.org/licenses/>. 
 *  
 *  Author: Vahid Mardani <vahid.mardani@gmail.com>
 */
#ifndef CARG_H_
#define CARG_H_


#include <stdbool.h>


enum carg_status {
    CARG_ERR = -1,
    CARG_OK = 0,
    CARG_OK_EXIT = 1,
};


enum carg_eatresult {
    CARG_EATEN,
    CARG_EATEN_EXIT,
    CARG_UNRECOGNIZED,
    CARG_VALUE_REQUIRED,
};


enum carg_optionflags {
    COF_NONE,
};


struct carg_option {
    const char *longname;
    const int key;
    const char *arg;
    enum carg_optionflags flags;
    const char *help;
};


struct carg_state;
typedef enum carg_eatresult (*carg_eater) (int key, const char *value,
        struct carg_state *state);


struct carg {
    const char *doc;
    carg_eater eat;
    const char *args;
    struct carg_option *options;
    const char *footer;
    const char *version;
};


struct carg_state {
    struct carg *carg;
    int argc;
    char **argv;
    int fd;
    void *userptr;
    int current;
    int next;
    bool dashdash;
};


void
carg_outfile_set(int fd);


void
carg_errfile_set(int fd);


enum carg_status
carg_parse(struct carg *c, int argc, char **argv, void *userptr);


#endif  // CARG_H_
