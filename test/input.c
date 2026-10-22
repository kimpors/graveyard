#include <stdio.h>
#include <stdlib.h>

#include "unity.h"
#include "input-buffer.h"

static FILE *fp;
static InputBuffer *buf;

void setUp(void)
{
	buf = init_input_buffer();
	fp = tmpfile();
}

void tearDown(void)
{
	free_input_buffer(buf);
	fclose(fp);
}

void case1(void)
{
	fputs("hello there\n", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fp));
	TEST_ASSERT_EQUAL_STRING("hello there", buf->data);
	TEST_ASSERT_EQUAL_INT(11, buf->len);
}

void case2(void)
{
	fputs("", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_FAILURE, readline(buf, fp));
}

void case3(void)
{
	fputs("\n", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fp));
	TEST_ASSERT_EQUAL_STRING("", buf->data);
	TEST_ASSERT_EQUAL_INT(0, buf->len);
}

void case4(void)
{
	fputs("hello there", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fp));
	TEST_ASSERT_EQUAL_STRING("hello there", buf->data);
	TEST_ASSERT_EQUAL_INT(11, buf->len);
}

int main(void) 
{
	UNITY_BEGIN();
	RUN_TEST(case1);
	RUN_TEST(case2);
	RUN_TEST(case3);
	RUN_TEST(case4);
	return UNITY_END();
}
