typedef Point struct {
    int x;
    int y;
};

func main() -> void {
    array<Point, 10> parr;

    Point p;
    ptr<Point> pp = addr(p);
    p->x = 1;
    p->y = 2;

    parr[0] = dptr(p);
}