# adventofcode

This repository holds solutions for the Advent of Code. It's written in a pedagogical style, to help beginner programmers get some insight into places where you can grow.

> [!WARNING]
> For the record, **I am not an expert** on these technologies. I don't want you to feel misled that this is the definitive best source on information for CMake, for C, for design. I'm not even 100% sure the solutions will be accurate!
>
> This is something you can use to get information on how to approach problems. Links to documentation or letting you know about concepts in Computer Science that aren't usually in beginner lessons. It's a launching point for your own self-study.

That being said, if you see any errors or have any suggestions, please feel free to open a PR.

## Who Is This For

Someone that has knowledge of C-syntax and basic programming concepts (e.g. you know what a function is, or you know how to look it up). I try not to assume any experience beyond that.

## Getting Started

You can read the source code directly to get some links to useful resources.

I've placed comments in the files to try and explain concepts. I'd recommend visiting this stuff top-bottom, so start with the [CMakeLists.txt](./CMakeLists.txt) file in the root directory. Then work your way through each day in order.

## Tools Used

* [CMake](https://cmake.org/), for cross-platform builds
* [Visual Studio Code](https://code.visualstudio.com/), for text editing
* [git](https://git-scm.com/), for version control

### Dependencies

You'll need to install the following dependencies to build the source code.

1. A C/C++ compiler
    * Windows: Download the [MSVC Build Tools](https://visualstudio.microsoft.com/vs/features/cplusplus/) and launch VS Code from a [MSVC Developer shell](https://learn.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2022).
    * Windows: Alternatively, download the [MSYS2](https://code.visualstudio.com/) environment and install the UCRT64 GCC tools.
    * macOS: Download the [XCode build tools](https://developer.apple.com/xcode/resources/).
    * Linux: Depending on your distribution, [gcc](https://gcc.gnu.org/) should be sufficient.
2. CMake

### Repository Layout

```
CMakeLists.txt      // CMakeLists for the entire project
2023                // 2023 advent of code challenges
    CMakeLists.txt  // CMake files for the 2023 folder
    1.trebuchet     // solution for day 1
        main.c
        CMakeLists.txt  // build files for day 1
    ...
```

Solutions for each day are stored under [2023](./2023).


## Inspiration

I mostly wrote this to help motivate and explain programming concepts for a friend of mine. I decided to add additional comments and make it public, in case others find this useful.
