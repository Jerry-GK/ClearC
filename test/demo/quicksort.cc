func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;
func malloc(long)->ptr<void>;

// 交换两个元素的值
func swap(ptr<int>a, ptr<int>b) -> void {
    int temp = dptr(a);
    dptr(a) = dptr(b);
    dptr(b) = temp;
    return;
}

// 划分数组并返回划分点的下标
func partition(ptr<int> arr, int low, int high) -> int {
    int pivot = arr[high];  // 选择最后一个元素作为基准
    int i = (low - 1);  // i指向小于等于基准的元素
    int j;

    for j = low; j < high; j++ {
        // 如果当前元素小于等于基准，则将其交换到i指向的位置
        if (arr[j] <= pivot) {
            i++;
            swap(addr(arr[i]), addr(arr[j]));
        }
    }

    swap(addr(arr[i + 1]), addr(arr[high]));  // 将基准元素放到正确的位置上
    return (i + 1);
}

// 快速排序递归函数
func quickSort(ptr<int> arr, int low, int high) -> void {
    if low < high{
        // 对数组进行划分
        int partitionIndex = partition(arr, low, high);

    // 递归排序划分的两个子数组
    quickSort(arr, low, partitionIndex - 1);
    quickSort(arr, partitionIndex + 1, high);
    }
    return;
}

// 打印数组
func printArray(ptr<int> arr, int size) -> void {
    int i;
    for i = 0; i < size; i++ {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return;
}

func main() -> int {
    int size = 10;
    ptr<int> arr = typecast(malloc(sizeof(char) * size), ptr<char>);
    arr[0] = 10;
    arr[1] = 2;
    arr[2] = 4;
    arr[3] = 5;
    arr[4] = 8;
    arr[5] = 1;
    arr[6] = 9;
    arr[7] = 3;
    arr[8] = 7;
    arr[9] = 6;

    printf("original array: \n");
    printArray(arr, size);

    quickSort(arr, 0, size - 1);

    printf("array after quick sort: \n");
    printArray(arr, size);

    return 0;
}
