#ifndef __TEST_H__
#define __TEST_H__

typedef struct _test_result {
    char *message,
         *testName;
} TestResult;

TestResult *setMessage(TestResult *, char *);

#endif /* __TEST_H__ */