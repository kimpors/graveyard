#include <stdio.h>
#include <unistd.h>

#include "unity.h"
#include "statement.h"
#include "input-buffer.h"

static Table *tb;
static FILE *fpin;
static FILE *fpout;
static Statement st;
static InputBuffer *buf;

void setUp(void)
{
	buf = init_input_buffer();
	tb = db_open("testdb");
	fpin = tmpfile();
	fpout = tmpfile();
}

void tearDown(void)
{
	free_input_buffer(buf);
	db_close(tb);
	fclose(fpout);
	fclose(fpin);
}

// ExecuteResult execute_select(Statement *st, Table *tb);

void redirect_stdout(ExecuteResult (*func)(Statement *, Table *))
{
    fflush(stdout);
    int saved = dup(STDOUT_FILENO);

    dup2(fileno(fpout), STDOUT_FILENO);

	func(&st, tb);

    fflush(stdout);
    dup2(saved, STDOUT_FILENO);
    close(saved);
}

void case1(void)
{
	fputs("insert 1 hello 12", fpin);
	rewind(fpin);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fpin));
	TEST_ASSERT_EQUAL_INT(PREPARE_SUCCESS, prepare_statement(buf, &st));
	TEST_ASSERT_EQUAL_INT(EXECUTE_SUCCESS, execute_statement(&st, tb));

	redirect_stdout(execute_select);
	rewind(fpout);

	readline(buf, fpout);
	TEST_ASSERT_EQUAL_STRING("(1, hello, 12)", buf->data);
}

void case2(void)
{
	fputs("insert 2 hello hello@example.com\ninsert 3 bye bye@example.com", fpin);
	rewind(fpin);

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fpin));
	TEST_ASSERT_EQUAL_INT(PREPARE_SUCCESS, prepare_statement(buf, &st));
	TEST_ASSERT_EQUAL_INT(EXECUTE_SUCCESS, execute_statement(&st, tb));

	TEST_ASSERT_EQUAL_INT(EXIT_SUCCESS, readline(buf, fpin));
	TEST_ASSERT_EQUAL_INT(PREPARE_SUCCESS, prepare_statement(buf, &st));
	TEST_ASSERT_EQUAL_INT(EXECUTE_SUCCESS, execute_statement(&st, tb));

	redirect_stdout(execute_select);
	rewind(fpout);

	readline(buf, fpout);
	TEST_ASSERT_EQUAL_STRING("(1, hello, 12)", buf->data);

	readline(buf, fpout);
	TEST_ASSERT_EQUAL_STRING("(2, hello, hello@example.com)", buf->data);

	readline(buf, fpout);
	TEST_ASSERT_EQUAL_STRING("(3, bye, bye@example.com)", buf->data);
}

int main(void) 
{
	UNITY_BEGIN();
	RUN_TEST(case1);
	RUN_TEST(case2);
	return UNITY_END();
}
