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
#include <cutest.h>

#include "carg.h"
#include "helpers.h"



// void
// thud_main

void
test_command() {
    struct carg_option thud_options[] = {
        {"baz", 'z', NULL, 0, "Baz flag"},
        {NULL}
    };

    const struct carg_subcommand thud_cmd = {
        .name = "thud",
        .args = "qux",
        .options = thud_options,
    };

    struct carg_option root_options[] = {
        {"foo", 'f', NULL, 0, "Foo flag"},
        {"bar", 'b', "BAR", 0, "Bar option with value"},
        {NULL}
    };

    struct carg carg = {
        .args = NULL,
        .header = NULL,
        .eat = NULL,
        .options = root_options,
        .footer = NULL,
        .version = NULL,
        .flags = 0,
        .commands = (const struct carg_subcommand*[]) {
            &thud_cmd,
            NULL
        },
    };


    eqint(CARG_OK, carg_parse_string(&carg, "foo thud"));
}


int
main() {
    test_command();
    return EXIT_SUCCESS;
}
