func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;


typedef Point struct {
    int x;
    int Y;
};

typedef MyPoint Point;

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

func MyPoint : PrintX() -> void {
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
    p.Y = 1;
    int Y = p.Y;
    printf("%d\n", x);
    printf("%d\n", Y);
    pp->PrintX();

    return 0;
}