#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests/list.h"
#include "tests/map.h"
#include "tests/string.h"
#include "test.h"

#define TEST_COUNT 3

TestResult *setMessage(TestResult *result, char *message) {
    result->message = message;
    return result;
}

int main(int argc, char **argv) {
    int isVisibleLog = argc > 1 ? !strcmp(*argv, "--nologs") : 1;

    TestResult *(*tests[TEST_COUNT])() =
        {
            &testSysList,
            &testSysMap,
            &testSysString
        };

    enum {
        SUCCESS, ERROR
    } code = SUCCESS;

    for (int counter = 0; counter < TEST_COUNT; counter++) {
        TestResult *result = tests[counter]();

        int isSuccess = result->message == NULL;
        if (!isSuccess) code = ERROR;

        if (isVisibleLog) {
            printf("[%s] Test \"%s\" finished%s%s\n",
                isSuccess ? "SUCCESS" : " ERROR ",
                result->testName,
                isSuccess ? ";" : " with message:\n  ",
                isSuccess ? "" : result->message
            );
        }

        free(result);
    }

    return code;
}