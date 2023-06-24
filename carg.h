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


/* carg special keys */
#define KEY_ARG -1
#define KEY_END -2


enum carg_status {
    STATUS_ERR = -1,
    STATUS_OK = 0,
    STATUS_OK_EXIT = 1,
};


enum carg_eatstatus {
    EAT_OK,
    EAT_OK_EXIT,
    EAT_FLAG,
    EAT_UNRECOGNIZED,
    EAT_VALUE_REQUIRED,
    EAT_BAD_VALUE,
    EAT_ARG_REQUIRED,
};


enum carg_optionflags {
    OPTIONAL_VALUE = 2,
};


struct carg_option {
    const char *longname;
    const int key;
    const char *arg;
    enum carg_optionflags flags;
    const char *help;
};


struct carg_state;
typedef enum carg_eatstatus (*carg_eater) (int key, const char *value,
        struct carg_state *state);


/* carg flags */
enum carg_flags{
    NO_HELP = 2,
    NO_USAGE = 4,
    NO_CLOG = 8,
};


struct carg {
    carg_eater eat;
    struct carg_option *options;
    const char *args;
    const char *header;
    const char *footer;
    const char *version;
    enum carg_flags flags;
};


struct carg_state {
    struct carg *carg;
    int fd;
    bool dashdash;

    int argc;
    char **argv;
    void *userptr;
    int index;
    const char *next;
    int arg_index;
    bool last;
};


void
carg_outfile_set(int fd);


void
carg_errfile_set(int fd);


enum carg_status
carg_parse(struct carg *c, int argc, char **argv, void *userptr);


#endif  // CARG_H_
