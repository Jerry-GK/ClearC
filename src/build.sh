# This script is disabled now

echo "Start building..."
echo "Building Lexer"
flex lexer.l
echo "Lexer built"
echo "Building Parser"
bison -d parser.y
echo "Parser built"

g++ *.cpp -std=c++17 -o clearc
