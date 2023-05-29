demoFilePath="test/demo"
demoFileName="demo.cc"

cd build

make -j

./clearc -O3 ../${demoFilePath}/${demoFileName} -o ../test/bin/obj

gcc ../test/bin/obj -o ../test/bin/${demoFileName}.out

cd ..

./test/bin/${demoFileName}.out

#rm ./test/basic/*.html
rm ./test/bin/obj
rm ./test/bin/${demoFileName}.out

rm ./test/demo/*.html
