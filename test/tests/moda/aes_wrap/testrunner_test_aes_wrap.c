/* AUTOGENERATED FILE. DO NOT EDIT. */

//=======Test Runner Used To Run Each Test Below=====
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT() && !TEST_IS_IGNORED) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

//=======Automagically Detected Files To Include=====
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>
#include "moda.h"
#include "aes.h"
#include "aes_wrap.h"

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_MODA_AES_WRAP_Encrypt(void);
extern void test_MODA_AES_WRAP_Encrypt_no_iv(void);
extern void test_MODA_AES_WRAP_Decrypt(void);
extern void test_MODA_AES_WRAP_Decrypt_no_iv(void);


//=======Test Reset Option=====
void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}


//=======MAIN=====
int main(void)
{
  UnityBegin("tests/moda/aes_wrap/test_aes_wrap.c");
  RUN_TEST(test_MODA_AES_WRAP_Encrypt, 102);
  RUN_TEST(test_MODA_AES_WRAP_Encrypt_no_iv, 117);
  RUN_TEST(test_MODA_AES_WRAP_Decrypt, 141);
  RUN_TEST(test_MODA_AES_WRAP_Decrypt_no_iv, 158);

  return (UnityEnd());
}