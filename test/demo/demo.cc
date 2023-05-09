func printf(ptr<char>, ...) -> int;
// int scanf(char ptr, ...);

typedef Student struct {
    int x;
};

typedef Teacher struct {
    int x;
};


func main() -> int {

    const int a = 1;

    ptr<const int> p = null;
    p = addr(a);
}