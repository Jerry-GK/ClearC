# a script to run all tests on basic testbench
demoFilePath="test/basic"
demoFileName=""

cd build

make -j

cd ..

# try demoName for "test_prime", "test_matrix", "test_hanoi", "test_lcs"
for demoName in "test_prime" "test_matrix" "test_hanoi" "test_lcs"; do
    demoFileName=$demoName".cc"
    ./build/clearc ./${demoFilePath}/${demoFileName} ./test/bin/obj
    gcc ./test/bin/obj -o ./test/bin/${demoFileName}.out

    ./test/testbin/macos/arm64/$demoName ./test/bin/$demoName.out

    rm ./test/basic/*.html
    rm ./test/bin/obj
    rm ./test/bin/${demoFileName}.out
done