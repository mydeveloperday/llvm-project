export CC=cl.exe
export CXX=cl.exe
cmake -G "CodeBlocks - Unix Makefiles" c:/projects/llvm-project/llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DPYTHON_EXECUTABLE:FILEPATH=c:\\Python27\\python.exe

make clang-format
echo `pwd`
cd /cygdrive/c/projects/llvm-project/build
echo `pwd`
cp ./bin/clang-format.exe ./bin/clang-format-experimental.exe
zip clang-format-experimental.zip ./bin/clang-format-experimental.exe
