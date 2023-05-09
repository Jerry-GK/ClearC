func printf(ptr<char>, ...) -> int;
// int scanf(char ptr, ...);


func plus(int a) -> int {
    return a + 1;
}

int y = 3;

typedef Student struct {
    array<char, 10> name;
};


int x = 2;

typedef Teacher struct {
    array<char, 10> name;
};
int a = 1;
//int b = a;

func main() -> void {
    Student s;
    Teacher t;

    printf("x=%d", x);
    printf("y=%d", y);
    printf("a=%d", a);
    return;
}