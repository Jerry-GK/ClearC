# ClearC

<img src="assets/image-20230430132636411.png" alt="image-20230430132636411" style="zoom:20%;" />

**A Compiler for ClearC, a Simplified but More Clear C Language with some Go Features.**

It's initial framework and inspiration come from [C-Compiler](https://github.com/YJJfish/C-Compiler).

To be finished.

## Language Features
1. Macro: **No macros. (No code pretreat)**

2. Pointer: In C, ' * ' has three meanings: multiply, declare a pointer, pointer dereference, which will make the compling procedure complicated and confusing.
   In ClearC, we use the critical word 'ptr' to declare a pointer, and use 'dptr' to dereference.

   We also use add() to get the address instead of using &.

   example.

   ```c
   int a = 1;
   int ptr pa = addr(a);
   dptr(pa) = dptr(pa) + 1; //or pa[0] = pa[0] + 1;
   ```

3. Array:

   array<type, size> name; is the legal declaration for an array. Array is regarded as a type as well, which is const ptr< type >.

   ```c
   array<int, 100> arr;
   array<ptr<int>, 5> arrp;
   arrp[0][0] = 2;
   ```

4. Function

   ```c
   func sum(int a, int b) -> int {
     return a + b;
   }
   
   func mul(int a, int b) -> int res {
   	res = a * b;
     return; //return res;
   }
   ```

5. typedef

    ClearC use `typedef name type` instead of  `typedef type name` in C.

    ```c
    typedef MYINT int;
    
    //this is just like class Dog{};
    typedef Dog struct{
      array<char, 10> name;
    }
    ```

6. OOP (to do)

   We want to make the OOP style like golang.

   - Should the struct pointer be anonymous?
   - Should we use `class A{}` to represent `typedef struct{} A`? 

   ```c
   typedef Student struct {
     int id;
     array<char, 10> name;
   };
   
   func Student::SetName(array<char, 10> name) -> void {
   	Student.name = name; 
   }
   //func Student::SetName(ptr<Student> anonymous, array<char, 10> name) -> void {
   	//anonymous->name = name;
   //}
   
   func Student::GetId() -> int {
   	return Student.id;
   }
   //func Student::GetId(ptr<Student> anonymous) -> int {
   	//return anonymous->id;
   //}
   
    Student s;
    s.SetName("Duck");
   
    ptr<Student> ps = addr(s);
    int sid = ps->GetId();
   ```

7. String (to do)

    Should we make **string** a primitive data type?

8. Control Grammar

    In `if, for, switch`, condition does NOT need to be in `()`, but the body code must be in `{}`

    Declaration is allowed in for's initial statement (like c++).

    We use `for condition {}` to replace `while condition {}`.  for{} is unconditional loop.

    **`do{} while`  is abandoned in ClearC.**

    ```c
    //legal
    for int i = 0; i < n; i++ {
    	sum += i;
    }
    
    //illegal
    for int i = 0; i < n; i++
    	sum += i;
    
    //illegal
    for (int i = 0; i < n; i++) {
    	sum += i;
    }
    
    //legal
    for i < 10 {
      sum += i;
      i++;
    }
    
    //legal
    for {
      if i > 5{
        break;
      }
      i++;
    }
    ```

9. struct, class, union, enum

    - `struct {Fileds;};` is a data type defined by programmers, which is a anonymous and temporary.

        You should use `typedef A struct {Fileds.};` to name this type "A".

        Fileds equality does not mean that structs are equal.

        ```c
        struct {int x; int y;} a;
        struct {int x; int y;} b;
        a = b; //illegal
        
        typedef Point struct {int x; int y;};
        Point r;
        Point s;
        r = s; //legal
        ```

        class is not a critical word, which is replaced by  `typedef A struct {Fileds.};`

    - **`union` and `enum` are temporarily abandoned in ClearC.**

10. Function Declaration (To do)

    We think function declarations is redundant and unnecessary, so we want to elimate them.



## Compile Procedure

source code -> tokens -> ast -> IR -> object file -> executable file



## Build & Run

```shell
cmake -S . -B ./build 
cd build
make
./clearc <source code path> <target file name>
./<target file name>
```



## Reference
- The lexer, parser, ast and visualization module of this project have given me great help [C-Compiler](https://github.com/YJJfish/C-Compiler).

- The [LLVM Tutorial](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html).
