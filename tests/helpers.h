// Copyright 2023 Vahid Mardani
/*
 * This file is part of CArg.
 *  CArg is free software: you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  CArg is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with CArg. If not, see <https://www.gnu.org/licenses/>.
 *
 *  Author: Vahid Mardani <vahid.mardani@gmail.com>
 */
#ifndef TESTS_HELPERS_H_
#define TESTS_HELPERS_H_


#include "carg.h"


#define BUFFSIZE    2047
#define LOREM "Lorem merol ipsum dolor sit amet, consectetur adipiscing " \
    "elit, sed do eiusmod tempor incididunt ut labore et dolore magna " \
    "aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco " \
    "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor."


extern struct carg_option nooption[];
extern char out[];
extern char err[];


enum carg_status
carg_parse_string(struct carg *c, const char *line, void *userptr,
        void **handler);


#endif  // TESTS_HELPERS_H_
