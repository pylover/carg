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
#include <clog.h>
#include <cutest.h>

#include "carg.h"
#include "helpers.h"


static struct {
    int foo;
    int bar;
    int baz;
    int qux;
} args = {0, 0, 0, 0};


static enum carg_eatstatus
_eater(const struct carg_option *opt, const char *value, void *) {
    if (opt == NULL) {
        /* Positional */
        ERROR("Positional detected: %s", value);
        return CARG_EAT_UNRECOGNIZED;
    }
    switch (opt->key) {
        case 'f':
            args.foo += atoi(value);
            break;
        case 'b':
            args.bar += atoi(value);
            break;
        case 'z':
            args.baz = 1;
            break;
        case 'q':
            args.qux += 1;
            break;
        default:
            return CARG_EAT_UNRECOGNIZED;
    }

    return CARG_EAT_OK;
}


static void
test_options_multiple() {
    struct carg_option options[] = {
        {"foo", 'f', "FOO", CARG_OPTION_MULTIPLE, "Foo option"},
        {"bar", 'b', "BAR", 0, "Bar option with value"},
        {"baz", 'z', NULL, 0, NULL},
        {"qux", 'q', NULL, CARG_OPTION_MULTIPLE, NULL},
        {NULL}
    };
    struct carg c = {
        .eat = NULL,
        .options = options,
        .eat = _eater,
        .args = NULL,
        .header = NULL,
        .footer = NULL,
        .version = NULL,
        .flags = 0,
        .commands = NULL,
    };

    clog_verbosity = CLOG_INFO;

    memset(&args, 0, sizeof(args));
    eqint(CARG_OK, carg_parse_string(&c, "foo -f1 -f2", NULL));
    eqint(3, args.foo);
    eqstr("", out);
    eqstr("", err);

    memset(&args, 0, sizeof(args));
    eqint(CARG_OK, carg_parse_string(&c, "foo -q -q", NULL));
    eqint(2, args.qux);
    eqstr("", out);
    eqstr("", err);

    memset(&args, 0, sizeof(args));
    eqint(CARG_OK, carg_parse_string(&c, "foo -qq", NULL));
    eqint(2, args.qux);
    eqstr("", out);
    eqstr("", err);

    memset(&args, 0, sizeof(args));
    eqint(CARG_OK, carg_parse_string(&c, "foo -qqqqqqqqqqqqqqqqqqqqq", NULL));
    eqint(21, args.qux);
    eqstr("", out);
    eqstr("", err);

    memset(&args, 0, sizeof(args));
    eqint(CARG_USERERROR, carg_parse_string(&c, "foo -b1 -b2", NULL));
    eqint(1, args.bar);
    eqstr("", out);
    eqstr("foo: redundant option -- '-b/--bar'\n"
        "Try `foo --help' or `foo --usage' for more information.\n", err);

    memset(&args, 0, sizeof(args));
    eqint(CARG_USERERROR, carg_parse_string(&c, "foo -z -z", NULL));
    eqint(1, args.baz);
    eqstr("", out);
    eqstr("foo: redundant option -- '-z/--baz'\n"
        "Try `foo --help' or `foo --usage' for more information.\n", err);

    memset(&args, 0, sizeof(args));
    eqint(CARG_USERERROR, carg_parse_string(&c, "foo -zz", NULL));
    eqint(1, args.baz);
    eqstr("", out);
    eqstr("foo: redundant option -- '-z/--baz'\n"
        "Try `foo --help' or `foo --usage' for more information.\n", err);
}


int
main() {
    test_options_multiple();
    return EXIT_SUCCESS;
}