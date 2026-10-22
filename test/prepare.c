#include <stdio.h>
#include "unity.h"
#include "statement.h"
#include "input-buffer.h"

static FILE *fp;
static Statement st;
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
	fputs("insert", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fp));
	TEST_ASSERT_EQUAL_INT(PREPARE_SYNTAX_ERROR, prepare_statement(buf, &st));
}

void case2(void)
{
	fputs("select", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fp));
	TEST_ASSERT_EQUAL_INT(PREPARE_SUCCESS, prepare_statement(buf, &st));
	TEST_ASSERT_EQUAL_INT(STATEMENT_SELECT, st.type);
}

void case3(void)
{
	fputs("insert 1 hello 12", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fp));
	TEST_ASSERT_EQUAL_INT(PREPARE_SUCCESS, prepare_statement(buf, &st));
	TEST_ASSERT_EQUAL_INT(STATEMENT_INSERT, st.type);
}

void case4(void)
{
	fputs("show", fp);
	rewind(fp);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fp));
	TEST_ASSERT_EQUAL_INT(PREPARE_UNRECOGNIZED, prepare_statement(buf, &st));
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
