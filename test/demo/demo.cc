func printf(ptr<char>, ...) -> int;
// int scanf(char ptr, ...);

typedef Student struct {
    int x;
};

typedef Teacher struct {
    int x;
};

func main() -> int {
    Student s;
    Teacher t;
    t = s;
    return 0;
}