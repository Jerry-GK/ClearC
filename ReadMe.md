# ClearC

<img src="assets/image-20230430132636411.png" alt="image-20230430132636411" style="zoom:20%;" />

**A Compiler for ClearC, a Simplified but More Clear C Language with some Go Features.**

It's initial framework and inspiration come from [C-Compiler](https://github.com/YJJfish/C-Compiler).

To be finished.

## Language Features

1. Macro: **No macros. (No code pretreat, only single source code file available)**

2. Pointer: In C, ` *` has three meanings: multiply, declare a pointer, pointer dereference, which will make the compling procedure complicated and confusing.
   In ClearC, we use the critical word `ptr` to declare a pointer, and use `dptr` to dereference.

   We also use `addr()` to get the address instead of using `&`.

   `[]` is still available to dereference based on bias.

   example.

   ```c
   int a = 1;
   ptr<int> pa = addr(a);
   dptr(pa) = dptr(pa) + 1; //or pa[0] = pa[0] + 1;
   ```

3. Array:

   array<`type`, `size`> name; is the legal declaration for an array. Array is regarded as a type as well, which is const ptr<`type`>.

   ```c
   array<int, 100> arr;
   array<ptr<int>, 5> arrp;
   arrp[0][0] = 2;
   ```

4. Function

   We want the function declaration more clear.

   func `funcname` (`args list`) -> `return type` { `funcbody` }

   ```c
   func sum(int a, int b) -> int {
     return a + b;
   }
   
   func mul(int a, int b) -> int res {
   	res = a * b;
     return; //return res;
   }
   ```

5. struct, class, union, enum

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

6. typedef

   ClearC use `typedef name type` instead of  `typedef type name` in C.

   ```c
   typedef MYINT int;
   
   //this is just like class Dog{};
   typedef Dog struct{
     array<char, 10> name;
   }
   ```

7. const and static

   Usually, const means a variable cannot change its value, so it will remain the initial value forever.

   ptr<`const varType`> is a specially grammar (inner-const), which means nobody can modify the variable pointed by this pointer **through this pointer**, and there are some rules (just like in C).

   - const variable can only pointed by ptr<`const`>
   - Variable pointed by ptr<`const`> may change its value(if it's not const), but cannot change through the ptr<`const`>.
   - const ptr must point to the fixed address, but the value in this address may change.
   - Can Not assign ptr<`const`> to ptr which is not inner-const.

   ```c
   const varType v = val; //this var cannot be modified
   ptr<varType> pv = addr(v); //illegal: v may be modified through pv
   
   ptr<const varType> pcv = addr(v); //legal
   dptr(pcv）= val_; //illgeal: cannot modify v through pcv
   pv = pcv; //illegal: ptr<const varType> cannot assign ptr<varType> (or const ptr<varType>)
   
   const ptr<varType> cpv= null; //cpv cannot be modified
   cpv = pv //illegal: const ptr cannot be assigned
   
   ```

   **`static` is abandoned in ClearC.**

8. Control Grammar

   In `if, for, switch`, condition does NOT need to be in `()`, but the body code must be in `{}`

   Declaration is allowed in for's initial statement (like c++).

   We use `for condition {}` to replace `while condition {}`.  `for{}` is unconditional loop.

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

9. naming

   For an identifier, it may be the name for a function, type or a variable, or may not be defined yet.

   In ClearC, to make naming more clear and avoid ambiguity, we require that in the same namespace(block with the same symbol table), identifiers must be distinct. Functions, types and variables can NOT have the same name with each other, otherwise will raise **naming conflict error**.

   ```c
   //naming conflict for identifier "Student"
   int Student; //variable
   typedef Student struct{}; //type
   func Student() -> void; //function
   
   func f() -> void{
     int Student; //this will NOT cause naming conflict
     return;
   }
   ```

   

10. OOP (to do)

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

11. String (to do)

    Should we make **string** a primitive data type? Maybe not.

12. Function Declaration (To do)

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





