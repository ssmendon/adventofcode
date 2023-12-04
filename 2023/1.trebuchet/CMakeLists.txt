# This file describes how we build and test the "trbuchet" Advent of Code challenge.

# Create a new executable target called "trebuchet" from the source code in "main.c".
#
# Executable targets are programs that you can run. For our code, it's usually going to be
# source code that runs in your terminal.
add_executable(trebuchet main.c)

# Link "trebuchet" to the "aoc_compiler_flags" so that it inherits all the options
# we set in the root CMakeLists.txt file.
target_link_libraries(trebuchet PUBLIC aoc_compiler_flags)

# Declare a test where we pass in the file "basic01.txt" and expect to see
# output that contains the line 'Sum = 142'.
do_test(trebuchet basic01.txt "Sum = 142")
