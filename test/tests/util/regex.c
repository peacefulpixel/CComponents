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

    return 0;
}
