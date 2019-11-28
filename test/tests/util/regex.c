#include <assert.h>
#include <stdlib.h>

#include "../../../src/util/regex.h"

#define FREE _regex_free_error(error); _regex_free_match(match); match = NULL;

#define ASSERT_OK assert(result); assert(match != NULL); \
                  assert(error == NULL); FREE

#define ASSERT_NOT_OK assert(!result); assert(match == NULL); \
                      assert(error == NULL); FREE

int main(int argc, char **argv) {

    _RegError *error;
    _RegMatch *match;
    bool result;

    result = _regex_match(&error, &match, "dr[o-zA-V\\d]+r*i", "drEpr2i");
    ASSERT_OK

    result = _regex_match(&error, &match, "dr[o-zA-V\\d]+r*i", "drEpXr2i");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "[\\]]+\\[]\\.+", "]][].....");
    ASSERT_OK

    result = _regex_match(&error, &match, "[\\]]+\\[]\\.+", "]][]1");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "[^a-v]+ef", "yzwef");
    ASSERT_OK

    result = _regex_match(&error, &match, "[^a-v]+ef", "bef");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "abc", "aabcaaapaaabc");
    ASSERT_OK

    result = _regex_match(&error, &match, "abc", "aabacaacbbaca");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "aaa*aa", "aaaa");
    ASSERT_OK

    result = _regex_match(&error, &match, "aaa*aa", "aaa");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "[a-c]*ab", "ab");
    ASSERT_OK

    result = _regex_match(&error, &match, "[a-c]*ab", "b");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "[a-c]+ab", "aab");
    ASSERT_OK

    result = _regex_match(&error, &match, "[a-c]+ab", "ab");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "[a-c]?ab", "ab");
    ASSERT_OK

    result = _regex_match(&error, &match, "[a-c]?ab", "aaab");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "^$", "");
    ASSERT_OK

    result = _regex_match(&error, &match, "^$", ".");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "^abc$", "abc");
    ASSERT_OK

    result = _regex_match(&error, &match, "^abc$", "abcd");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "^abc", "abcdef");
    ASSERT_OK

    result = _regex_match(&error, &match, "^abc", "aabcdef");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "abc$", "0123abc");
    ASSERT_OK

    result = _regex_match(&error, &match, "abc$", "0123abcd");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "(abc)+$", "abcabc");
    ASSERT_OK

    result = _regex_match(&error, &match, "(abc)+$", "abcd");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "(abc[x-z]*)+$", 
        "abc8abcabcyabczzzxy");
    ASSERT_OK

    result = _regex_match(&error, &match, "(abc[x-z]*)+$", "abczab");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "(abc|bca)+", "bcabc");
    ASSERT_OK

    result = _regex_match(&error, &match, "(abc|bca)+", "abaca");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "[\\S\\W\\D]+", "& 2w");
    ASSERT_OK

    result = _regex_match(&error, &match, "[\\S\\W\\D]+", " 2w");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "a{0,3}[a-z]{4,}", "aaaz");
    ASSERT_OK

    result = _regex_match(&error, &match, "a{0,3}[a-z]{4,}", "aaa");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "a{2,3}", "aaa");
    ASSERT_OK

    result = _regex_match(&error, &match, "a{2,3}", "a");
    ASSERT_NOT_OK

    return 0;
}
