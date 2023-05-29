// A list written in OOP style
// To vertify recursive structure

func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;
func malloc(long)->ptr<void>;

typedef Node struct {
    int Val;
    ptr<Node> Next;
};

typedef List struct {
    ptr<Node> head;
};

func List : Init() -> void {
    this->head = null;
    return;
}

func List : PushFront(int val) -> void {
    ptr<Node> p = typecast(malloc(sizeof(Node)), ptr<Node>);
    p->Val = val;
    p->Next = this->head;
    this->head = p;
    return;
}

func List : PrintList() -> void {
    ptr<Node> p = this->head;
    for p != null{
        printf("%d ", p->Val);
        p = p->Next;
    }
    printf("\n");
    return;
}

func main() -> int {
    List list;
    list.Init();
    for int i = 1; i <= 10; i++ {
        list.PushFront(i);
    }
    list.PrintList();
    return 0;
}