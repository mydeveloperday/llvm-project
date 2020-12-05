# Experimental LLVM Compiler Infrastructure

[![Build status](https://ci.appveyor.com/api/projects/status/9dd77w8dvbvi9ia1?svg=true)](https://ci.appveyor.com/project/mydeveloperday/llvm-project)

This fork of the LLVM project is to maintain experimental changes for clang-format (maybe and other tools in the future)

As there is often delay to land some revisions into clang-format upstream, this fork aims to promote those revisions by the production of a clang-format-experimental.exe binary.

This will allow the effectiveness of unlanded revisions to be evaluated without users having to work out how to merge a non committed change and go through the process of building clang-format.

This repo will contain current tip of master + revisions which appear to be wanted/desired by the wider community (revisions that have a large number of subscribers, Phabricator like tokens or are blocked from acceptance by lack of review or lack of consensous).

It may be revisions are blocked for legitimate reasons, But they may seem blocked for no real technical reason, except the view that either clang-format is "done" or that clang-format should not support too many options, or it that makes it hard for people to discover which options to use. But a search of github shows a worrying amount of usage of clang-format off/on in code which shows that using clang-format can be cumbersome if it doesn't quite match your style.  As a community I believe developers are often more than happy to search for and tweak a numerous set of options to get their code looking just right.

Code complexity aside, If all new options come with unit test  and those unit tests don't break or change existing tests then it 
should be safe to land such changes safely.

The purpose here is to give those unlanded revisions some velocity, pulling  those uncommitted revisions into a repo and using 
CI system (appveyor) to build a clang format binary for the  wider community to use, This may aid to get some capability landed upstream.

Inclusion into this repository, requires the same revisions submitted to phabricator and
those revisions have NOT yet been upstreamed. (we will get upstream revisions for free, as we sync with upstream master)

As the production of a release binary is based on committing to this repo, this may also help to reduce the time between new binaries being built on (https://llvm.org/builds/)

Unlanded Revisions will me merged by pulling those revision down, and building a commit in this fork using the following.

arc export --revision D12345 --git > revD12345.patch
patch -p1 < revD12345.patch

Unlanded revisions need to remain rebased to the HEAD of master to avoid excessive merge conflicts, and updates should go via the original revision (until landed)

I would like to follow the LLVM process of code review over in http://reviews.llvm.org.

If you see a revision over in reviews.llvm.org that you think should be included because its useful then raise an 
issue here citing the revision number D1234

# Revisions Included

The following is a list of revisions included in clang-format-experimental, for documentation you currently need to visit the revision in Phabricator

* [D54881](https://reviews.llvm.org/D54881) - [clang-format] Prevent Clang-Format from editing leading whitespace on lines outside of the format range
* [D68554](https://reviews.llvm.org/D68554) - [clang-format] Proposal for clang-format to give compiler style warnings
* [D68568](https://reviews.llvm.org/D68568) - [clang-format] Make '.clang-format' variants finding a loop
* [D68569](https://reviews.llvm.org/D68569) - [clang-format] Also look for .{ext}.clang-format file


# Revisions Removed (as now in trunk)

* [D67660](https://reviews.llvm.org/D67660) - [clang-format] [PR43338] C# clang format has space issues betweern C# only keywords
* [D67629](https://reviews.llvm.org/D67629) - [clang-format] [PR43333] Fix C# breaking before function name when using Attributes
* [D28462](https://reviews.llvm.org/D28462) - [clang-format] Add new style option AlignConsecutiveMacros
* [D52150](https://reviews.llvm.org/D52159) - [clang-format] BeforeHash added to IndentPPDirectives
* [D59292](https://reviews.llvm.org/D59292) - [clang-format] messes up indentation when using JavaScript private fields and methods
* [D59309](https://reviews.llvm.org/D59309) - [clang-format] BreakAfterReturnType ignored on functions with numeric template parameters
* [D58404](https://reviews.llvm.org/D58404) - [clang-format] Add basic support for formatting C# files
* [D40988](https://reviews.llvm.org/D40988) - [clang-format] Add finer-grained options for putting all arguments on one line
* [D55170](https://reviews.llvm.org/D55170) - [clang-format]: Add NonEmptyParentheses spacing option



This directory and its sub-directories contain source code for LLVM,
a toolkit for the construction of highly optimized compilers,
optimizers, and run-time environments.

The README briefly describes how to get started with building LLVM.
For more information on how to contribute to the LLVM project, please
take a look at the
[Contributing to LLVM](https://llvm.org/docs/Contributing.html) guide.

## Getting Started with the LLVM System

Taken from https://llvm.org/docs/GettingStarted.html.

### Overview

Welcome to the LLVM project!

The LLVM project has multiple components. The core of the project is
itself called "LLVM". This contains all of the tools, libraries, and header
files needed to process intermediate representations and converts it into
object files.  Tools include an assembler, disassembler, bitcode analyzer, and
bitcode optimizer.  It also contains basic regression tests.

C-like languages use the [Clang](http://clang.llvm.org/) front end.  This
component compiles C, C++, Objective-C, and Objective-C++ code into LLVM bitcode
-- and from there into object files, using LLVM.

Other components include:
the [libc++ C++ standard library](https://libcxx.llvm.org),
the [LLD linker](https://lld.llvm.org), and more.

### Getting the Source Code and Building LLVM

The LLVM Getting Started documentation may be out of date.  The [Clang
Getting Started](http://clang.llvm.org/get_started.html) page might have more
accurate information.

This is an example work-flow and configuration to get and build the LLVM source:

1. Checkout LLVM (including related sub-projects like Clang):

     * ``git clone https://github.com/llvm/llvm-project.git``

     * Or, on windows, ``git clone --config core.autocrlf=false
    https://github.com/llvm/llvm-project.git``

2. Configure and build LLVM and Clang:

     * ``cd llvm-project``

     * ``mkdir build``

     * ``cd build``

     * ``cmake -G <generator> [options] ../llvm``

        Some common build system generators are:

        * ``Ninja`` --- for generating [Ninja](https://ninja-build.org)
          build files. Most llvm developers use Ninja.
        * ``Unix Makefiles`` --- for generating make-compatible parallel makefiles.
        * ``Visual Studio`` --- for generating Visual Studio projects and
          solutions.
        * ``Xcode`` --- for generating Xcode projects.

        Some Common options:

        * ``-DLLVM_ENABLE_PROJECTS='...'`` --- semicolon-separated list of the LLVM
          sub-projects you'd like to additionally build. Can include any of: clang,
          clang-tools-extra, libcxx, libcxxabi, libunwind, lldb, compiler-rt, lld,
          polly, or debuginfo-tests.

          For example, to build LLVM, Clang, libcxx, and libcxxabi, use
          ``-DLLVM_ENABLE_PROJECTS="clang;libcxx;libcxxabi"``.

        * ``-DCMAKE_INSTALL_PREFIX=directory`` --- Specify for *directory* the full
          path name of where you want the LLVM tools and libraries to be installed
          (default ``/usr/local``).

        * ``-DCMAKE_BUILD_TYPE=type`` --- Valid options for *type* are Debug,
          Release, RelWithDebInfo, and MinSizeRel. Default is Debug.

        * ``-DLLVM_ENABLE_ASSERTIONS=On`` --- Compile with assertion checks enabled
          (default is Yes for Debug builds, No for all other build types).

      * ``cmake --build . [-- [options] <target>]`` or your build system specified above
        directly.

        * The default target (i.e. ``ninja`` or ``make``) will build all of LLVM.

        * The ``check-all`` target (i.e. ``ninja check-all``) will run the
          regression tests to ensure everything is in working order.

        * CMake will generate targets for each tool and library, and most
          LLVM sub-projects generate their own ``check-<project>`` target.

        * Running a serial build will be **slow**.  To improve speed, try running a
          parallel build.  That's done by default in Ninja; for ``make``, use the option
          ``-j NNN``, where ``NNN`` is the number of parallel jobs, e.g. the number of
          CPUs you have.

      * For more information see [CMake](https://llvm.org/docs/CMake.html)

Consult the
[Getting Started with LLVM](https://llvm.org/docs/GettingStarted.html#getting-started-with-llvm)
page for detailed information on configuring and compiling LLVM. You can visit
[Directory Layout](https://llvm.org/docs/GettingStarted.html#directory-layout)
to learn about the layout of the source code tree.
