func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;


typedef Point struct {
    int x;
};

typedef Student struct {
    int x;
};

func Point : SetX(int x) -> void {
    this->x = x;
    return;
}

// func SetX(const ptr<Point> this, int x) -> void {
//     this->x = x;
//     return;
// }

func Point : GetX() -> int {
    return this->x;
}

func Point : PrintX() -> void {
    printf("Point's x = %d\n", this->x);
    return;
}

func main() -> int {
    Point p;
    ptr<Point> pp = addr(p);


    Student s;
    ptr<Student> sp = addr(s);

    p.SetX(37);
    int x = p.GetX();
    printf("%d\n", x);
    pp->PrintX();

    return 0;
}