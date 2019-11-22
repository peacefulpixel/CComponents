#include <assert.h>
#include <stdlib.h>

#include "../../../src/util/regex.h"

int main(int argc, char **argv) {

    _RegError *error;
    _RegMatch *match;

    // TODO: Replace hardcoded expressions and values to generated
    char *expr = "dr[o-zA-V\\d]+r*i";
    bool result = _regex_match(&error, &match, expr, "drEpr2i");

    assert(result);
    assert(match != NULL);
    assert(error == NULL);

    _regex_free_error(error);
    _regex_free_match(match);

    result = _regex_match(&error, &match, expr, "drEpXr2i");

    assert(!result);
    assert(match == NULL);
    assert(error == NULL);

    _regex_free_error(error);
    _regex_free_match(match);

    return 0;
}