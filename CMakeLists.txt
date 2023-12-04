# CMake is a software tool for building source code into an executable.
# It helps define a cross-platform mechanism for invoking build utilities,
# creating project metadata, and much more.
#
# It's a complicated piece of software with a difficult-to-grok manual.
# When browsing the manual, take care to note the version of CMake used
# and the version of the manual.
# See: https://cmake.org/cmake/help/latest/
# The tutorial is a good place to start: https://cmake.org/cmake/help/latest/guide/tutorial/index.html

# Set the version of CMake required to build this project.
#
# Newer versions may introduce incompatible changes (e.g. removing keywords,
# changing how keywords work, or changing the defaults).)
cmake_minimum_required(VERSION 3.26)

# Set the name of the project.
# See: https://cmake.org/cmake/help/latest/command/project.html#command:project
project(AdventOfCode)

# Create a library called aoc_compiler_flags.
#
# This creates a "target", which is something CMake uses to describe
# "a thing that is built". The target is called "aoc_compiler_flags" and
# it is a library.
#
# It is an "Interface Library" which doesn't actually build any code.
# It's used by CMake to define common properties across multiple different builds.
#
# It can also be used by third-party consumers of your code, to build
# their sources in a compatible way.
#
# See: https://cmake.org/cmake/help/latest/command/add_library.html
add_library(aoc_compiler_flags INTERFACE)

# Define that the 2023 C Standard is required for this project.
#
# We specify this is an INTERFACE compile feature, which makes it "public"
# for third-party consumers.
#
# You would make dependencies "private" if it shouldn't be used by third-parties,
# maybe because your library may change in unexpected ways between releases,
# or because "hiding" certain options makes the code easier to approach from
# a third-party.
#
# In our case, we're marking it with INTERFACE so that we can re-use this
# configuration. It's also self-documenting, because if you know what a
# CMake Interface is, you know that this is being set mostly for re-use.
#
# The C Standard is the version of the C language that we're using.
# A body called ISO creates and publishes the language reference for C.
# Anything not present in the language reference is not guaranteed to be uniform
# across different compiler implementations.
#
# We set the standard to the currently unreleased 2023 standard.
# The standard gets set as a "compile feature", so the name is defined
# under the CMAKE_C_KNOWN_FEATURES document.
#
# See: https://cmake.org/cmake/help/latest/command/target_compile_features.html
# See: https://cmake.org/cmake/help/latest/prop_tgt/C_STANDARD.html
# See: https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_C_KNOWN_FEATURES.html#prop_gbl:CMAKE_C_KNOWN_FEATURES
target_compile_features(aoc_compiler_flags
  INTERFACE c_std_23 # You might commonly see cxx_std, which refers to C++.
  )

# Define a variable 'gcc_like_c' that is '1' if we're using a GCC-like compiler.
#
# set() lets us define a variable in CMake. We're defining the variable called
# 'gcc_like_c'.
#
# 'gcc', or the GNU Compiler Collection, is a suite of programs that let you
# compile source code. It used to stand for "GNU C Compiler". It's not just
# for C and C++, it actually works for languages like Fortran and Go.
#
# Unfortunately, 'gcc' is a little overloaded. It stands for multiple things:
# 1. GNU Compiler Collection (all the tools/languages we can compile)
# 2. GNU C Compiler (just compiling C code)
#
# In our case, we're only building C code. So you can think of GCC as
# the way we compile C code.
#
# See: https://gcc.gnu.org/onlinedocs/gcc/G_002b_002b-and-GCC.html
#
# We call it 'gcc_like_c' because we're taking all the different compiler
# programs that "behave like" GCC. Behavior as in, they accept the same command-line
# arguments.
#
# The gcc_like_c compilers here are:
#
# * ARM clang (clang variant for ARM, a computer architecture)
# * Apple clang (clang variant for Apple)
# * clang (a different compiler based on a different technology)
# * GCC (what we've just been describing)
#
# See: https://gcc.gnu.org/
# See: https://clang.llvm.org/
# See: https://en.wikipedia.org/wiki/ARM_architecture_family
#
# To actually determine if we're using a GCC-like compiler, we use a
# CMake generator expression. The expression being used is called:
# COMPILE_LANG_AND_ID
#
# This expression accepts two arguments: language, and compiler_ids. We've
# passed it the language 'C' and a list of different compiler IDs. You can
# find the CMake known compiler IDs at the below links.
#
# See: https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:COMPILE_LANG_AND_ID
# See: https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html
#
# We're basically having CMake figure out what compiler is being used,
# and then we'll set some options depending on it. This is likely better maintained
# than us going in and specifying all the different compilers that are "gcc_like".
#
# The way CMake is figuring out what compiler is being used is through a
# "generator expression" (which is the funky syntax starting with the "$<" and ending with ">".
#
# Generator expressions let CMake set configuration properties dynamically, based on
# properties like the compiler in use.
#
# See: https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html
set(gcc_like_c "$<COMPILE_LANG_AND_ID:C,ARMClang,AppleClang,Clang,GNU>")

# This is covered above. The only new piece is "msvc".
#
# msvc stands for Microsoft Visual C++, which is their version of the C compiler.
# It doesn't have a common command-line interface, so you specify arguments in a different
# way.
#
# See: https://learn.microsoft.com/en-us/cpp/build/reference/compiling-a-c-cpp-program?view=msvc-170
set(msvc_c "$<COMPILE_LANG_AND_ID:C,MSVC>")

# Set the command-line options for the compiler to increase warnings.
#
# When you compile code, you can pass in command-line arguments. Compilers have
# features to emit warnings if they detect "bad" code practices. It might be bad
# because the code is prone to subtle errors or relies on undefined behavior.
#
# Undefined behavior is generally something which isn't declared in the ISO C standard.
# For example, lets say INT_MAX is the biggest integer you can declare. We want to run this:
# ```c
# if (INT_MAX + 1 > 12) { printf("Duh!\n"; ) } else { printf("How?\n"); }
# ```
#
# When you "overflow" the value of an integer type, what happens is not defined by
# the C standard. So the same compiler could produce different behavior depending on
# the compiler options. It might "wrap around", going from INT_MAX to INT_MIN (the smallest
# integer). Or it might do something else entirely, like always returning true.
#
# A more common source of "undefined behavior" (or UB) would be a variable that is declared
# but not initialized.
#
# If you're ever written something like:
# ```c
# int p;
#
# printf("%d\n", p);
# ```
# You might notice that you get different results depending on how the program is compiled
# (e.g. "compile with optimizations" or "compile for debug"). Using an un-initialized variable
# is undefined. You'll probably grab some data that just happened to be on the stack in that
# location, rather than anything meaningful or correct.
#
# See: https://en.cppreference.com/w/c/language/behavior
#
# The rest of this is kind of CMake syntax related to generator expressions. We're using
# a conditional expression: if (gcc_like_c) { provide compile options }
#
# The BUILD_INTERFACE generator expression accepts some arbitrary contents. We're passing it
# a list (which is defined using semicolons in generator expressions). The list is a bunch of
# command-line arguments you'd pass to GCC. For example, gcc -Wall -Wextra -Wshadow ...
#
# BUILD_INTERFACE just prevents the these arguments from appearing in third-party code
# unless we explicitly export() the property. You wouldn't want someone to link to our code
# and suddenly have to deal with a bunch of compiler warnings.
#
# All the command-line arguments can be found by consulting the tool's manuals.
#
# See: https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Warning-Options.html
# See: https://learn.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170
# See: https://stackoverflow.com/a/8946900
target_compile_options(aoc_compiler_flags INTERFACE
  "$<${gcc_like_c}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-pedantic>>"
  "$<${msvc_c}:$<BUILD_INTERFACE:-W3>>"
)

# enable_testing() allows CMake to enable source code testing functions.
#
# Testing code lets us know the code actually does what we expect it to do.
# It's easy to have mistakes in code, which cause it to crash or just produce outputs
# that are just wrong.
#
# Rather than testing ad-hoc, it's helpful to integrate tests into your entire build
# and development process. That way, you save time (because you write the tests once)
# and your code is more reliable (because you have your previous tests).
#
# See: https://cmake.org/cmake/help/latest/command/enable_testing.html
# See: https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html
enable_testing()

# Loads a CMake module called ExternalData, so we can use test data.
#
# ExternalData lets us reference files that may not be stored inside our source control
# (like files from the web, or files from a remote network share, or files in a different folder on your PC).
#
# It has a lot of features that we aren't using. For us, we're using it to reference files that are in source control.
# Like a text file with our test case to run.
#
# This is a lot better for files which don't work that well in source control, like image files or other binary formats.
#
# See: https://cmake.org/cmake/help/latest/module/ExternalData.html
# See: https://cmake.org/cmake/help/latest/command/include.html
include(ExternalData)

# This function declares a simple test that runs the target with the arguments and compares its output against the regex result.
#
# This example is pulled from the CMake tutorial, with a few minor changes.
# 1. Use ExternalData_Add_Test so that DATA{} is supported inside the arguments.
# 2. We use DATA{} to reference a file in our source control.
#
# First, functions. Functions are re-usable snippets of code. I like to think of them like mathematical operations:
# You take in some input, you spit some output back. A function lets us do some common operations without needing to re-write
# or copy/paste code. You may also have heard these described as "methods".
#
# Here, we're declaring
# a function called "do_test" which takes three arguments: 'target', 'arg', 'result'.
#
# target is a CMake target name that we're testing.
# arg are arguments to pass to the command-line of the target.
# result is a regular expression to match the output of our target, to determine if it passes the test or not.
#
# A regular expression (regex) is a special syntax for writing out a pattern. There are different algorithms for
# implementing regular expressions, so the syntax and the feature set is not uniform across all applications.
# However, there are a few major standards: POSIX and Perl regex expressions.
#
# CMake uses POSIX regular expressions.
#
# See: https://en.wikipedia.org/wiki/Regular_expression
function(do_test target arg result)
    # Create an ExternalData target called ${arg}Data (where ${arg} is the passed-in "arg" parameter) and
    # create a test called "Comp${arg}" (Comp means Comparison).
    #
    # The test launches the executable created in ${target} and passes in the name of the file
    # specified in ${arg}.
    ExternalData_Add_Test(${arg}Data
        NAME Comp${arg}
        COMMAND ${target} DATA{${arg}}
        )
    # Set the regex match to determine success.
    set_tests_properties(Comp${arg}
        PROPERTIES PASS_REGULAR_EXPRESSION ${result}
        )
endfunction()

# Import the directory "2023" and handle its CMakeLists.txt files.
add_subdirectory(2023)
