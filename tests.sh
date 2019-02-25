echo `pwd`
cd /cygdrive/c/projects/llvm-project/build
make FormatTests
echo `pwd`
/cygdrive/c/projects/llvm-project/build/tools/clang/unittests/Format/FormatTests.exe
echo `pwd`
make LexTests
/cygdrive/c/projects/llvm-project/build/tools/clang/unittests/Lex/LexTests.exe
echo `pwd`
make BasicTests
/cygdrive/c/projects/llvm-project/build/tools/clang/unittests/Basic/BasicTests.exe

