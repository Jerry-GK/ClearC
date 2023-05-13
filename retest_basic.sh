# a script to run all tests on basic testbench
demoFilePath="test/basic"
demoFileName=""
bar="======================================"

cd build

make -j

cd ..

# try demoName for "test_prime", "test_matrix", "test_hanoi", "test_lcs", "test_arithmetic"
for demoName in "test_prime" "test_matrix" "test_hanoi" "test_lcs" "test_arithmetic"; do
    echo ${bar}
    echo "Runing Test for: " ${demoName}
    echo ${bar}
    demoFileName=$demoName".cc"
    ./build/clearc -O3 ./${demoFilePath}/${demoFileName} -o ./test/bin/obj
    gcc ./test/bin/obj -o ./test/bin/${demoName}.out

    ./test/testbin/macos/arm64/$demoName ./test/bin/${demoName}.out

    rm ./test/basic/*.html
    rm ./test/bin/obj
    rm ./test/bin/${demoName}.out
done

echo ${bar}
echo "Test Finished"
echo ${bar}
