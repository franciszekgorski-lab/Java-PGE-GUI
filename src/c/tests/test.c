#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

static int test_count = 0;
static int test_pass = 0;

static int file_exists(const char *path)
{
    return access(path, F_OK) == 0;
}

static int file_lines(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    int lines = 0;
    char buf[256];
    while (fgets(buf, sizeof(buf), f))
        lines++;
    fclose(f);
    return lines;
}

static int run_program(const char *input, const char *output, const char *format, int algo)
{
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "../graph -n %s -a %d -o %s -f %s -s 1 > /dev/null 2>&1",
             input, algo, output, format);
    int ret = system(cmd);
    return WIFEXITED(ret) ? WEXITSTATUS(ret) : -1;
}

int main(void)
{
    int exit_code;

    printf("\n=== Running Tests ===\n\n");

    test_count++;
    printf("Test 1: Fruchterman (1) with TXT output ... ");
    exit_code = run_program("data/simple.txt", "out/fruchterman_out", "txt", 1);
    int lines = file_lines("out/fruchterman_out");
    if (exit_code == 0 && file_exists("out/fruchterman_out") && lines == 3)
    {
        printf("OK\n");
        test_pass++;
    }
    else
        printf("FAIL\n");

    test_count++;
    printf("Test 2: Tutte (2) with TXT output ... ");
    exit_code = run_program("data/simple.txt", "out/tutte_out", "txt", 2);
    lines = file_lines("out/tutte_out");
    if (exit_code == 0 && file_exists("out/tutte_out") && lines == 3)
    {
        printf("OK\n");
        test_pass++;
    }
    else
        printf("FAIL\n");

    test_count++;
    printf("Test 3: Fruchterman with BIN output ... ");
    exit_code = run_program("data/simple.txt", "out/simple_bin", "bin", 1);
    if (exit_code == 0 && file_exists("out/simple_bin"))
    {
        printf("OK\n");
        test_pass++;
    }
    else
        printf("FAIL\n");

    test_count++;
    printf("Test 4: Complex graph with Tutte ... ");
    exit_code = run_program("data/complex.txt", "out/complex_out", "txt", 2);
    if (exit_code == 0 && file_exists("out/complex_out"))
    {
        printf("OK\n");
        test_pass++;
    }
    else
        printf("FAIL\n");

    test_count++;
    printf("Test 5: Missing input file ... ");
    exit_code = run_program("data/nonexistent.txt", "out/dummy", "txt", 1);
    if (exit_code == 1)
    {
        printf("OK\n");
        test_pass++;
    }
    else
        printf("FAIL (got code %d)\n", exit_code);

    test_count++;
    printf("Test 6: Bad format ... ");
    exit_code = run_program("data/bad_format.txt", "out/dummy", "txt", 1);
    if (exit_code == 2)
    {
        printf("OK\n");
        test_pass++;
    }
    else
        printf("FAIL (got code %d)\n", exit_code);

    test_count++;
    printf("Test 7: Invalid algorithm ... ");
    char cmd[512];
    snprintf(cmd, sizeof(cmd),
             "../graph -n data/simple.txt -a 99 -o out/dummy -f txt -s 42 > /dev/null 2>&1");
    int ret = system(cmd);
    exit_code = WIFEXITED(ret) ? WEXITSTATUS(ret) : -1;
    if (exit_code == 4)
    {
        printf("OK\n");
        test_pass++;
    }
    else
        printf("FAIL (got code %d)\n", exit_code);

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n\n", test_pass, test_count);

    return test_pass == test_count ? 0 : 1;
}
