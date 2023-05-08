cd src
./clean.sh
cd ..

rm -rf build

cmake -S . -B ./build
cd build

# LDFLAGS="-L/opt/homebrew/opt/llvm@14/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm@14/lib/c++"
# export LDFLAGS="-L/opt/homebrew/opt/llvm@14/lib"
# export CPPFLAGS="-I/opt/homebrew/opt/llvm@14/include"

make -j
