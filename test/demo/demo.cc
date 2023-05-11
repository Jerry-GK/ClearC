func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;

typedef Student struct {
    int x;
};

typedef Teacher struct {
    int x;
};


typedef varType int;


func printStr(const ptr<char> arr)->void {
    int cur = 0;
    for{
        if (arr[cur] == '\0')
        {
            break;
        }
        else {
            printf("%c", arr[cur]);
        }
        cur++;
    }
    return;
}

func main() -> int {
    array<char, 10> arr;
    arr[0] = 'h';
    arr[1] = 'e';
    arr[2] = 'l';
    arr[3] = 'l';
    arr[4] = 'o';
    arr[5] = '\0';

    printStr(arr);

    //printf("%s\n", arr);

    return 0;
}