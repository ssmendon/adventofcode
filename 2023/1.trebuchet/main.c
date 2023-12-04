// This file contains the solution to the Day 1 Advent of Code challenge.
//
// It's an executable program, so it contains a main() function.

/*
    Two pages for C library information:
        https://en.cppreference.com/w/c (not super accessible)
        https://cplusplus.com/reference/clibrary/ (not as detailed)
*/

// assert.h used for the assert() function call
#include <assert.h>
// ctype.h used for handling character types
#include <ctype.h>
// limits.h used for upper/lower bounds on types
#include <limits.h>
// stdio.h used for input/output and file handling
#include <stdio.h>
// stdlib.h used for converting a string to an integer
#include <stdlib.h>

/*
    This is the name of our program.
    It is set in main(), and used mostly for
    error messages.

    Credit to sqlite3.c <https://github.com/sqlite/sqlite/blob/master/src/shell.c.in#L500-L504>
*/
static char *Argv0;

/*
    Seen describes how many numbers we've seen this line,
    ranging from 0 until 2.

    Enums are, by default, an integer which starts at 0 and goes up by 1 each time.
    So we can treat this like an integer.

    Enumerations in C are data types that let us assign human-readable names
    to special constant values.

    The 'typedef' says that 'seen_t' is a type alias for 'enum SEEN'.
    Without the typedef, we'd have to use 'enum SEEN x = SeenZero;' to
    declare and initialize an 'enum SEEN' type. Because of the typedef, we
    can write 'seen_t x = SeenZero;'.

    See: https://en.cppreference.com/w/cpp/language/enum
    See: https://en.cppreference.com/w/c/language/typedef


    Wikipedia or Geeks for Geeks are other online resources.
*/
typedef enum SEEN
{
    SeenZero,
    SeenOne,
    SeenTwo
} seen_t;

/*
    Prints the program's usage message, then terminates with failure.

    [[noreturn]] is an attribute declaring that this function will not return.
    It was added in C23 (the 2023 C standard).

    Before C23, the attribute noreturn would be provided without the [[]].
    The noreturn convenience macro is declared in stdnoreturn.h, but in C23 that is deprecated.

    See: https://en.cppreference.com/w/c/language/_Noreturn

    static is a function attribute. We use it to constrain this function only to this file.
    Basically, no other source file will be able to use this method. Only main.c.
    This isn't a problem for such a small project, since there's only one file. In larger projects,
    it can help prevent "name collisions" where two functions have the same name.

    See: https://en.cppreference.com/w/c/language/storage_duration
*/
[[noreturn]] static void usage(void)
{
    // For details about the format of a usage string, check out: https://en.wikipedia.org/wiki/Usage_message

    (void)fprintf(stderr, "Usage: %s [filename]\n", Argv0); // fprintf returns a status code, which we silently ignore.
    exit(EXIT_FAILURE);
}

// Execute like so:
//
// cat basic01.txt | ./trebuchet.exe
//
// OR
//
// ./trebuchet.exe basic01.txt
int main(int argc, char **argv)
{
    // Handling command-line arguments.
    if (argv[0] == NULL) // It could be NULL, but it's not common.
        Argv0 = "main";  // Default name for error messages is 'main'.
    else
        Argv0 = argv[0]; // Default name for error messages is the program's name.
    if (argc > 2)        // If you passed in more arguments than we expect, exit.
        usage();

    // Open the file used for reading.
    FILE *f = NULL; // declare f to be NULL in case neither conditions are true, somehow.
    if (argc < 2)   // If you passed in no arguments, use stdin for input.
    {
        f = stdin;
        printf("Reading from stdin... (press ^C to exit).");
    }
    else if (!(f = fopen(argv[1], "r"))) // Open the file passed in as an argument for reading.
    {
        (void)fprintf(stderr, "Unable to open file: %s", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Note on assert(): it only happens when the program is built in debug mode.
    // So this cannot be relied on for run-time error checking.
    // It's just to help catch unexpected circumstances during debugging.
    assert(f != NULL); // This shouldn't occur during runtime at all, 'f' must be non-NULL.

    char calibration[3] = {0};    // String containing leftmost and rightmost digits for this line.
                                  // We initialize it to all 0s, so that way there's a null-terminator at the end.
                                  // Otherwise, atoi() later on would possibly read past the end of the array.
    seen_t digitsSeen = SeenZero; // Number of digits seen this line.
    int sum = 0,                  // Running sum of the values.
        tmp,                      // Storage for temporary integers.
        c;                        // Current character being processed.
    while ((c = fgetc(f)))        // Loop through each character in the file.
    {
        // Neither assert() is executed in "Release" builds.
        assert(digitsSeen >= SeenZero || digitsSeen <= SeenTwo);
        assert(sum >= 0);

        if (c == '\n' || c == EOF) // If we're at the end of a line, sum the values up.
        {
            if (digitsSeen == SeenOne) // If we've seen only one digit, then the left and right digits are the same.
            {
                calibration[1] = calibration[0];
                digitsSeen = SeenTwo; // We've now "seen" two digits, which is checked in the next if-statement.
            }
            if (digitsSeen == SeenTwo) // If we've seen two digits, then convert to an integer and sum.
            {
                // Figure out if adding sum+tmp would overflow the maximum value of an integer.
                tmp = atoi(calibration);
                assert(tmp >= 0); // Should not occur at runtime.
                if (tmp > INT_MAX - sum)
                {
                    (void)fprintf(stderr, "INTEGER OVERFLOW: %d + %d > %d", sum, tmp, INT_MAX);
                    exit(EXIT_FAILURE);
                }
                else
                    sum += atoi(calibration);
            }
            digitsSeen = SeenZero; // Reset number of digits seen.
        }
        if (c == EOF) // EOF means there's no more file to read.
            break;
        if (!isdigit(c)) // If the current character isn't a digit, then skip this loop iteration.
            continue;
        if (digitsSeen == SeenZero) // If we haven't seen anything, this is the first character this line.
        {
            digitsSeen = SeenOne;
            calibration[0] = c;
        }
        // If we've seen one character, this is the second character.
        // If we've seen two characters, this is currently the rightmost character.
        else
        {
            digitsSeen = SeenTwo; // Since this is inside an "else" block, we explicitly set to SeenTwo from SeenOne.
            calibration[1] = c;
        }
    }
    (void)fclose(f); // not really needed, OS will clean up the file when we exit
    (void)printf("Sum = %d\n", sum);

    return EXIT_SUCCESS; // Success status code.
}

/* Misc info: Encoding

We ignore non-ASCII encodings with how we're processing strings here.

ASCII is a way of assigning a number to each character. Internally, computers store
the number and "know" that the number corresponds to a letter. For example, the number "104" is the letter 'h' in ASCII.

See: https://www.asciitable.com/

However, you can't assume that the encoding is ASCII. On Windows, it's typically using Unicode (wide character types). On Linux
it tends to be UTF-8.

This is a problem, because in non-ASCII encodings, characters may be represented by more than one byte (the fundamental size of a character).
For example, input[0] and input[1] might together make up one grapheme.

Plus, multiple characters may not be understood. Non-English languages especially have different ways of writing letters and words
depending on where it's used in the entire input.

Making this code portable for all kinds of text formats would require more thought.

Here are some helpful resources about string encoding:

    https://en.wikipedia.org/wiki/Character_encoding
    https://home.unicode.org/
    https://doc.rust-lang.org/stable/book/ch08-02-strings.html
    https://docs.python.org/3/howto/unicode.html
*/

/* Misc info: Testing

The test cases that are included with the CMakeTests.txt are not really a good indication that this program works properly.
It has poor code coverage, which is a measure of "what gets tested" by your test suite. It's not the only measure, mind you,
but it's a very common one to evaluate.

See: https://en.wikipedia.org/wiki/Code_coverage
See: https://csrc.nist.gov/glossary/term/statement_coverage
See: https://csrc.nist.gov/glossary/term/Condition_coverage
See: https://csrc.nist.gov/glossary/term/decision_or_branch_coverage
See: https://csrc.nist.gov/glossary/term/modified_condition_decision_coverage

Adding additional test cases to increase its coverage would be a good step. There are C testing frameworks like Criterion
that let you produce a standard file format indicating your program's coverage.

See: https://criterion.readthedocs.io/en/master/debug.html

You could even use a fuzzer to conduct randomized testing
of the input surfaces.

See: https://en.wikipedia.org/wiki/Fuzzing

Separate from coverage, this code isn't really written for testing. It does most of its work in a single function, and all the logic
is amalgamated into a single loop.

Breaking this into separate functions would improve its testability. For example, a function `bool would_overflow(int a, int b)`
would let us test the implementation of overflow detection independently from the rest of the code. We could directly pass it input
from a test suite, rather than writing a script or creating a massive input file with 21,691,754 lines where each line is just '9'.

There's even a paradigm called Test Driven Development, where you take your requirements and write the test-cases *first*. Then you
implement code to pass those test cases. This is hugely useful for code in a big company that has a ton of legacy components. You
can write the test code to mirror how it currently works, then write your new code so that the behavior is the same.

See: https://en.wikipedia.org/wiki/Test-driven_development
*/
