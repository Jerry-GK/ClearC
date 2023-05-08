func printf(ptr<char>, ...) -> int;
// int scanf(char ptr, ...);
typedef Point struct {
    int x;
    int y;
};

func main() -> void {
    const int x = 1;
    array<int, 10> a1;
    array<int, 10> a2;

    const ptr<int> p = addr(x);
    p = null;

    x= 2;

    //ptr<const int> pc;

    p = a1;


    return;
}