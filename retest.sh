demoFilePath="test/demo"
demoFileName="demo.cc"

cd build

make -j

./clearc ../${demoFilePath}/${demoFileName} ../test/bin/obj
gcc ../test/bin/obj -o ../test/bin/${demoFileName}.out

cd ..

./test/bin/${demoFileName}.out

rm ./test/bin/obj
rm ./test/bin/${demoFileName}.out
