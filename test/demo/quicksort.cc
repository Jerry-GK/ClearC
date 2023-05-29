func printf(ptr<char>, ...) -> int;
func scanf(ptr<char>, ...) -> int;
func malloc(long)->ptr<void>;

// ��������Ԫ�ص�ֵ
func swap(ptr<int>a, ptr<int>b) -> void {
    int temp = dptr(a);
    dptr(a) = dptr(b);
    dptr(b) = temp;
    return;
}

// �������鲢���ػ��ֵ���±�
func partition(ptr<int> arr, int low, int high) -> int {
    int pivot = arr[high];  // ѡ�����һ��Ԫ����Ϊ��׼
    int i = (low - 1);  // iָ��С�ڵ��ڻ�׼��Ԫ��
    int j;

    for j = low; j < high; j++ {
        // �����ǰԪ��С�ڵ��ڻ�׼�����佻����iָ���λ��
        if (arr[j] <= pivot) {
            i++;
            swap(addr(arr[i]), addr(arr[j]));
        }
    }

    swap(addr(arr[i + 1]), addr(arr[high]));  // ����׼Ԫ�طŵ���ȷ��λ����
    return (i + 1);
}

// ��������ݹ麯��
func quickSort(ptr<int> arr, int low, int high) -> void {
    if low < high{
        // ��������л���
        int partitionIndex = partition(arr, low, high);

    // �ݹ����򻮷ֵ�����������
    quickSort(arr, low, partitionIndex - 1);
    quickSort(arr, partitionIndex + 1, high);
    }
    return;
}

// ��ӡ����
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
