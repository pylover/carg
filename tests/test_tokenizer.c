// Copyright 2023 Vahid Mardani
/*
 * This file is part of yacap.
 *  yacap is free software: you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  yacap is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with yacap. If not, see <https://www.gnu.org/licenses/>.
 *
 *  Author: Vahid Mardani <vahid.mardani@gmail.com>
 */
#include <stdlib.h>

#include <cutest.h>

#include "yacap.c"


#define ARGVSIZE(a) (sizeof(a) / sizeof(char*))
// TODO: quotes "", ''


/*
 * foo: -f -c CAR
 * bar: -v -g GAR
 *
 * foo -fvc bar bar -g GAR
 * foo
 */

void
test_tokenizer() {
    struct token tok;
    struct optiondb optdb;
    struct yacap_option options1[] = {
        {"foo", 'f', NULL, 0, "Foo flag"},
        {"bar", 'b', "BAR", 0, "Bar option with value"},
        {NULL}
    };
    struct yacap_option options2[] = {
        {"baz", 'z', "[BAZ]", 0, "baz option"},
        {NULL}
    };


    const char *argv[] = {
        "foo",
        "-fthud",
        "-fbbar",
        "bar",
        "-qux",
        "",
        "--foo=bar",
        "--foo=bar baz",
        "--foo=",
        "--foo",
        "--baz=_baz_",
        "-zzoo",
        "--",
        "--foo",
    };

    optiondb_init(&optdb);
    optiondb_insertvector(&optdb, options1, NULL);
    optiondb_insertvector(&optdb, options2, NULL);
    struct tokenizer *t = tokenizer_new(ARGVSIZE(argv), argv, &optdb);
    isnotnull(t);

    /* foo */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_POSITIONAL, tokenizer_next(t, &tok));
    eqstr("foo", tok.text);
    eqint(3, tok.len);
    isnull(tok.optioninfo);

    /* f */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnull(tok.text);
    eqint(0, tok.len);
    isnotnull(tok.optioninfo);
    eqchr('f', tok.optioninfo->option->key);

    /* thud */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_UNKNOWN, tokenizer_next(t, &tok));
    eqstr("thud", tok.text);
    eqint(1, tok.len);
    isnull(tok.optioninfo);

    /* f */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnull(tok.text);
    eqint(0, tok.len);
    isnotnull(tok.optioninfo);
    eqchr('f', tok.optioninfo->option->key);

    /* b */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnotnull(tok.optioninfo);
    isnotnull(tok.text);
    eqchr('b', tok.optioninfo->option->key);
    eqstr("bar", tok.text);
    eqint(3, tok.len);

    /* bar */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_POSITIONAL, tokenizer_next(t, &tok));
    isnull(tok.optioninfo);
    eqstr("bar", tok.text);
    eqint(3, tok.len);

    /* -qux */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_UNKNOWN, tokenizer_next(t, &tok));
    eqnstr("q", tok.text, tok.len);
    eqint(1, tok.len);
    isnull(tok.optioninfo);

    /* --foo=bar (option) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnotnull(tok.optioninfo);
    eqchr('f', tok.optioninfo->option->key);
    eqnstr("bar", tok.text, tok.len);

    /* --foo=bar baz (option) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnotnull(tok.optioninfo);
    eqchr('f', tok.optioninfo->option->key);
    eqnstr("bar baz", tok.text, tok.len);
    eqint(7, tok.len);

    /* --foo= (option) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnotnull(tok.optioninfo);
    eqchr('f', tok.optioninfo->option->key);
    eqstr("", tok.text);
    eqint(0, tok.len);

    /* --foo (option) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnotnull(tok.optioninfo);
    isnull(tok.text);
    eqint(0, tok.len);
    eqchr('f', tok.optioninfo->option->key);

    /* --baz=baz (option) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnotnull(tok.optioninfo);
    eqchr('z', tok.optioninfo->option->key);
    eqnstr("_baz_", tok.text, tok.len);
    eqint(5, tok.len);

    /* -zzoo (baz option) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_OPTION, tokenizer_next(t, &tok));
    isnotnull(tok.optioninfo);
    eqchr('z', tok.optioninfo->option->key);
    eqstr("zoo", tok.text);
    eqint(3, tok.len);

    /* --foo (positional) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_POSITIONAL, tokenizer_next(t, &tok));
    eqnstr("--foo", tok.text, tok.len);
    eqint(5, tok.len);
    isnull(tok.optioninfo);

    /* Termination */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_END, tokenizer_next(t, &tok));
    isnull(tok.text);
    eqint(0, tok.len);
    isnull(tok.optioninfo);

    tokenizer_dispose(t);
    optiondb_dispose(&optdb);
}


void
test_tokenizer_error() {
    struct token tok;
    struct optiondb optdb;
    struct yacap_option options1[] = {
        {"foo", 'f', NULL, 0, "Foo flag"},
        {"bar", 'b', "BAR", 0, "Bar option with value"},
        {NULL}
    };
    struct yacap_option options2[] = {
        {"baz", 'z', "[BAZ]", 0, "baz option"},
        {NULL}
    };

    const char *argv[3] = {
        "foo",
        NULL,
        "foo",
    };

    optiondb_init(&optdb);
    optiondb_insertvector(&optdb, options1, NULL);
    optiondb_insertvector(&optdb, options2, NULL);
    struct tokenizer *t = tokenizer_new(3, argv, &optdb);
    isnotnull(t);

    /* foo */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_POSITIONAL, tokenizer_next(t, &tok));
    eqstr("foo", tok.text);
    isnull(tok.optioninfo);

    /* NULL */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_ERROR, tokenizer_next(t, &tok));
    isnull(tok.text);
    isnull(tok.optioninfo);

    /* NULL (Again) */
    memset(&tok, 0, sizeof(tok));
    eqint(YACAP_TOK_ERROR, tokenizer_next(t, &tok));
    isnull(tok.text);
    isnull(tok.optioninfo);

    tokenizer_dispose(t);
    optiondb_dispose(&optdb);
}


int
main() {
    test_tokenizer();
    test_tokenizer_error();
    return EXIT_SUCCESS;
}
