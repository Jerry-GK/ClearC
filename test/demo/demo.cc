func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;

typedef Student struct {
    const int x;
};

typedef Teacher struct {
    int x;
};


typedef varType int;


func f(ptr<int> p)-> ptr<int> {
    if p[0] > 1{
        return p;
    }
    else {
        return null;
    }
}

typedef Point struct {
    int x;
    int y;
    const int cval;
};

func main() -> int {
    const Point p;
    //p.x = 1; //illegal
    ptr<const Point> pp = addr(p);
    //pp->y = 1; //illegal

    Point p1;
    p1.cval = 1; //illegal

    return 0;
}