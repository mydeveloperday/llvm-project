export CC=cl.exe
export CXX=cl.exe
cmake -G "CodeBlocks - Unix Makefiles" ../llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra"
make clang-format


