#include <assert.h>
#include <stdlib.h>

#include "../../../src/util/regex.h"
#include "../../../src/ccomponents.h"

#define FREE _regex_free_error(error); _regex_free_match(match); match = NULL;

#define ASSERT_OK assert(result); assert(match != NULL); \
                  assert(error == NULL);

#define ASSERT_NOT_OK assert(!result); assert(match == NULL); \
                      assert(error == NULL);

#define ASSERT_MATCH(__MATCH_INDEX__, __MATCH_FROM__, __MATCH_TO__) \
    { \
        /* This brackets are required for multiple cursors declaring
           by declaring in a new local context */ \
        _RegMatch *cursor = match; \
        for (unsigned int index = 0; index < __MATCH_INDEX__; index++) \
            cursor = cursor->next; \
        assert(cursor->from == __MATCH_FROM__); \
        assert(cursor->to   == __MATCH_TO__); \
    }

int main(int argc, char **argv) {

    _RegError *error;
    _RegMatch *match;
    bool result;

    result = _regex_match(&error, &match, "", "");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 0);
    FREE

    result = _regex_match(&error, &match, "dr[o-zA-V\\d]+r*i", "drEpr2i");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 7);
    FREE

    result = _regex_match(&error, &match, "dr[o-zA-V\\d]+r*i", "drEpXr2i");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "[\\]]+\\[]\\.+", "]][].....");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 9);
    FREE

    result = _regex_match(&error, &match, "[\\]]+\\[]\\.+", "]][]1");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "[^a-v]+ef", "yzwef");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 5);
    FREE

    result = _regex_match(&error, &match, "[^a-v]+ef", "bef");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "abc", "aabcaaapaaabc");
    ASSERT_OK
    ASSERT_MATCH(0, 1, 4);
    ASSERT_MATCH(1, 10, 13);
    FREE

    result = _regex_match(&error, &match, "abc", "aabacaacbbaca");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "aaa*aa", "aaaa");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 4);
    FREE

    result = _regex_match(&error, &match, "aaa*aa", "aaa");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "[a-c]*ab", "ab");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 2);
    FREE

    result = _regex_match(&error, &match, "[a-c]*ab", "b");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "[a-c]+ab", "aab");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    FREE

    result = _regex_match(&error, &match, "[a-c]+ab", "ab");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "[a-c]?ab", "ab");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 2);
    FREE

    result = _regex_match(&error, &match, "[a-c]?ab", "b");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, ".*", "abc");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    ASSERT_MATCH(1, 3, 3);
    FREE

    result = _regex_match(&error, &match, "[a-b]*.*", "bublik");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 6);
    ASSERT_MATCH(1, 6, 6);
    FREE

    result = _regex_match(&error, &match, ".*", "");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 0);
    FREE

    result = _regex_match(&error, &match, ".?", "abc");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 1);
    ASSERT_MATCH(1, 1, 2);
    ASSERT_MATCH(2, 2, 3);
    ASSERT_MATCH(3, 3, 3);
    FREE

    result = _regex_match(&error, &match, "^", "");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 0);
    FREE

    result = _regex_match(&error, &match, "^abc", "abc");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    FREE

    result = _regex_match(&error, &match, "^ab^c", "abc");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "^$", "");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 0);
    FREE

    result = _regex_match(&error, &match, "^$", ".");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "^abc$", "abc");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    FREE

    result = _regex_match(&error, &match, "^abc$", "abcd");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "^abc", "abcdef");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    FREE

    result = _regex_match(&error, &match, "^abc", "aabcdef");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "abc$", "0abcabc");
    ASSERT_OK
    ASSERT_MATCH(0, 4, 7);
    FREE

    result = _regex_match(&error, &match, "ab$c", "0abcabc");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "abc$", "0123abcd");
    ASSERT_NOT_OK
    FREE

#if CCOMP_V_MA > 0 || CCOMP_V_MI > 2
    result = _regex_match(&error, &match, "(abc)+$", "abcabc");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 6);
    FREE

    result = _regex_match(&error, &match, "(abc)+$", "abcd");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "(abc[x-z]*)+$", 
        "abc8abcabcyabczzzxy");
    ASSERT_OK
    ASSERT_MATCH(0, 4, 19);
    FREE

    result = _regex_match(&error, &match, "(abc[x-z]*)+$", "abczab");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "(abc|bca)+", "bcabc");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    FREE

    result = _regex_match(&error, &match, "(abc|bca)+", "abaca");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "(abc|def|ghj)", "abcdeffffghj");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    ASSERT_MATCH(1, 3, 6);
    ASSERT_MATCH(2, 9, 12);
    FREE

    result = _regex_match(&error, &match, "(abc|def|ghj)", "abdegh");
    ASSERT_NOT_OK

    result = _regex_match(&error, &match, "[\\S\\W\\D]+", " 2w");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 4);
    FREE

    result = _regex_match(&error, &match, "[\\S]+", " ");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "[\\W]+", "d");
    ASSERT_NOT_OK
    FREE
#endif

    result = _regex_match(&error, &match, "a{0,3}[a-z]{4,}", "aaaz");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 4);
    FREE

    result = _regex_match(&error, &match, "a{0,3}[a-z]{4,}", "aaa");
    ASSERT_NOT_OK
    FREE

    result = _regex_match(&error, &match, "a{2,3}", "aaa");
    ASSERT_OK
    ASSERT_MATCH(0, 0, 3);
    FREE

    result = _regex_match(&error, &match, "a{2,3}", "a");
    ASSERT_NOT_OK
    FREE

    return 0;
}
