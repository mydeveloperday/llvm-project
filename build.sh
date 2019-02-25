export CC=cl.exe
export CXX=cl.exe
cmake -G "CodeBlocks - Unix Makefiles" c:/projects/llvm-project/llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DPYTHON_EXECUTABLE:FILEPATH=c:\\Python27\\python.exe

export PROJ_ROOT=/cygdrive/c/projects/llvm-project/build
make clang-format
echo `pwd`
cd $PROJ_ROOT
echo `pwd`
cp $PROJ_ROOT/bin/clang-format.exe $PROJ_ROOT/bin/clang-format-experimental.exe
zip $PROJ_ROOT/clang-format-experimental.zip $PROJ_ROOT/bin/clang-format-experimental.exe
