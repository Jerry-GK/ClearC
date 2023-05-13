# ClearC

<img src="assets/image-20230430132636411.png" alt="image-20230430132636411" style="zoom:20%;" />

## Introduction

**A Compiler for ClearC, a Simplified but More Clear C Language with some Go Features.**

ClearC has a **clearer grammar**(especially on pointers), relatively complete **const** constraints, and  functions in **OOP** style.

**Current Version: 1.0**



## Language Features

1. **Macro and File**: 

    No macros. (`#define`, `#include` not available, no code pretreat)

    Multi-file coding managing tools like a package manager is not provided yet, only single source code file available.

2. **Pointer**: In C, ` *` has three meanings: multiply, declare a pointer, pointer dereference, which will make the compling procedure complicated and confusing.
   In ClearC, we use the critical word `ptr` to declare a pointer, and use `dptr` to dereference.

   We also use `addr()` to get the address instead of using `&`.

   `[]` is still available to dereference based on bias.

   **Pointers can NOT do self-increment/decrement in ClearC**. Pointer calculation is also not recommended.

   example.
   
   ```c
   int a = 1;
   ptr<int> pa = addr(a);
   dptr(pa) = dptr(pa) + 1; //or pa[0] = pa[0] + 1;
   pa++; //illegal
   ```
   
3. **Array**:

   array in ClearC has very limited functionality.

   array<`type`, `size`> name; is the legal declaration for an array. Array is regarded as a type as well, which is const ptr<`type`>.

   ```c
   array<int, 100> arr;
   array<ptr<int>, 5> arrp;
   arrp[0][0] = 2;
   ```

   Array is viewed as const ptr<>, with fixed head address assigned during initialization. 

   **We do NOT provide initialiazation method for array!** You have to assign it one by one.

   ```c
   array<char, 10> str = "hello"; //illegal
   array<int, 13> ints = {1, 2, 3}; //illegal
   ```

   Array can assign to pointers, but cannot be assigned, since it's "const".

   Array base type cannot be void or const;

   ```c
   array<char, 10> str =;
   array<char, 10> str1;
   ptr<char> ps = str; //legal
   str = ps; //illegal
   str = str1; //illegal
   array<char, 10> str2 = ps; //illegal
   array<const int, 10> arr1; //illgeal
   array<void, 10> arr2; //illgeal
   ```

   Array type can not be the function return type, since we don't want any array to accept the return value.

   Array type can neither be passed as function arguments, since it cannot be initialized.

   ```c
   func getArr() -> array<int, 10>; //illegal
   func printArr(array<char, 10>) -> void; //illegal
   ```

   If you want to use array more  flexibly, we recommend to use pointers.

   

4. **Function**

   We want the function declaration more clear.

   func `funcname` (`args list`) -> `return type` { `funcbody` }

   ```c
   func sum(int a, int b) -> int {
     return a + b;
   }
   ```
   Function in ClearC must have a return statement, even if it has void return type.

   Look at OOP for more function formats.

5. **struct**, class, union, enum

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

   

6. **const** and static

   Usually, const means a variable cannot change its value, so it will remain the initial value forever.

   ptr<`const varType`> is a specially grammar (inner-const), which means nobody can modify the variable pointed by this pointer **through this pointer**, and there are some rules (just like in C).

   - Two const:
       - Outer-const: The variable itself is const, it's value can only be initializaed, but not be assigned.
       - Inner-const: A feature of pointer. Cannot modify the content through inner-const pointer.

   - Rules:

       - const variable can only pointed by ptr<`const`> (Outer const variable can only be pointed by inner-const pointer)

       - Variable pointed by ptr<`const`> may change its value(if it's not const), but cannot change through the ptr<`const`>.

       - const ptr must point to the fixed address, but the value in this address may change.

       - Cannot assign ptr<`const`> to ptr which is not inner-const. Function with ptr<`const`> argument can neither receive non-inner-const pointer.
       - Cannot type cast a ptr<`const`> to a non-inner-const pointer.
       - Array type variable wil be regarded as outer-const pointer.

   ```c
   const varType v = val_0; //this var cannot be modified
   v = val_1; //illegal
   ptr<varType> pv = addr(v); //illegal: v may be modified through pv. addr(v) is type ptr<const varType>
   
   ptr<const varType> pcv = addr(v); //legal
   dptr(pcv)= val_2; //illgeal: cannot modify v through pcv
   pv = pcv; //illegal: ptr<const varType> cannot assign ptr<varType> (or const ptr<varType>)
   pv = typecast(pcv, ptr<varType>); //illegal: annot type cast inner-const to non-inner-const
   
   const ptr<varType> cpv= null; //cpv cannot be modified
   cpv = pv //illegal: const ptr cannot be assigned
   
   ```

   For a const struct variable, all member variables are also const.

   Const members is always const.

   ```c
   typedef Point struct{
   	int x;
   	int y;
     const int cval;
   };
   const Point p;
   p.x = 1; //illegal
   ptr<const Point> pp = addr(p);
   pp->y = 1; //illegal
   
   Point p1;
   p1.cval = 1; //illegal
   ```

   Result of arithmetic operation on inner-const pointer will still be inner-const.

   **`static` is abandoned in ClearC.**

   

7. **typedef**

   ClearC use `typedef name type` instead of  `typedef type name` in C.

   ```c
   typedef MYINT int;
   
   //this is just like class Dog{};
   typedef Dog struct{
     array<char, 10> name;
   }
   ```

8. **typecast**

   In ClearC, explicit typecast is in a more obvious way;

   ```c
   int a = 1;
   float f = typecast(a, float); //like float f = (float)a in C;
   ```

   Supported type cast:

   - Int -> Int, Float

   - Float -> Int, Float

   - Pointer -> Int, Pointer 

   Implicit typecast may also happen.

   **Cannot type cast an inner-const pointer to a non-inner-const pointer.**

   Pointer can type cast to a pointer with different base type, we don't figure out how to forbid this. However, it's mot recommended to do so.

   **Again, pointer cast with different base type is very dangerous and not recommended!**

   

9. **Control Grammar**

   In `if, for, switch`, **condition does NOT need to be in `()`, but the body code must be in `{}`**

   Declaration is allowed in for's initial statement (like c++).

   We use `for condition {}` to replace `while condition {}`.  `for{}` is unconditional loop.

   So `while` is not a critical word in ClearC.

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

10. naming

       For an identifier, it may be the name for a function, type or a variable, or may not be defined yet.

       In ClearC, to make naming more clear and avoid ambiguity, we require that in the same namespace(block with the same symbol table), identifiers must be distinct. **Functions, types and variables can NOT have the same name with each other**, otherwise will raise **naming conflict error**.

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

      

11. OOP (Encapsulation)

       We want to make the OOP style like golang, but a little different.

      `func BaseType : funcname(args list) -> return type {funcbody}`

     

      typedef Student struct {
        int id;
      	float score;
      };

      func Student : SetScore(float score) -> void {
      	this->score = score; 
      }
      //func SetName(const ptr<Student> this, array<char, 10> name) -> void {
      	//this->name = name;
      //}

      func Student : GetId() -> int {
      	return this->id;
      }
      //func GetId(const ptr<Student> this) -> int {
      	//return this->id;
      //}

       Student s;
       s.SetScore(95);

       ptr<Student> ps = addr(s);
       int sid = ps->GetId();
         ```c
         typedef Student struct {
         	int id;
         	float score;
         };
         
         func Student : SetScore(float score) -> void {
         	this->score = score; 
         }
         //func SetName(const ptr<Student> this, array<char, 10> name) -> void {
         	//this->name = name;
         //}
         
         func Student : GetId() -> int {
         	return this->id;
         }
         //func GetId(const ptr<Student> this) -> int {
         	//return this->id;
         //}
         
         Student s;
         s.SetScore(95);
         
         ptr<Student> ps = addr(s);
         int sid = ps->GetId();
         ```

      - The basetype of function is in fact a `const ptr<basetype>`  at the first position of the args list. It will represented by critical word `this` in funcbody.

      - The basetype must be a struct type defined by programmers.

      - `variable.Func()`, `varptr->Func()` can call memeber function, just like C.

      - Call a member function of other types(structs) will raise semantic error.

      - ClearC does NOT provide inheritance and polymorphism, it's OOP style is more like Go.

      - Member functions of different base types can NOT have the same name, otherwise will raise naming conflict error.

          

11. String (to do)

    Should we make **string** a primitive data type? Maybe not.

    Array<char, len> cannot be initialize by literal string such as "hello", this is a huge problem.

12. Function Declaration (To do)

    We think function declarations is redundant and unnecessary, so we want to elimate them.

    But we do not pre-scan the AST, so it's difficult.

    

## Language Limitations

1. Compiler internally does NOT manage TYPES clearly, it's kind of confused.

2. **No direct initialization method for structs.**

3. **No direct initialization method for arrays.** Arrays cannot be passed as function arguments.

    Statement like `array<char, 10> str = "hello"`  is not supported.

4. **No restriction on pointer assignment between different base types** (compiler cannot detect that).

    This can be very dangerous if abused.

5. No package manager and only support single source code file.

6. No built-in allocator.

7. **Member functions of different base types can NOT have the same name.** This is bad for encapsulation. 

    **There are also NO private/public constraint for members variables or functions for structs.**



## Compile Procedure

source code -> tokens -> ast -> IR -> object file -> executable file

## Environment

1. Flex and Bison

2. LLVM

    LLVM-14 is the recommended version.

3. CMake

## Build & Run

```shell
cmake -S . -B ./build 
cd build
make
./clearc <source code path> <target file name>
./<target file name>
```

## Test

There are some example source code files (name ended by `.cc`) in folders of `./test/`

You can compile and run them for test.

The root also has some test script for linux / macos.

`./rebuild.sh` is to regenerate lexer.cpp and parser.cpp, and compile, get executable file ./build/clearc

`./redemo.sh` is to recompile project and  use clearc to compile, link and run ./test/demo/demo.cc

`./retest_basic.sh` is recompile project, test all cases in ./test/basic.

## References

- The lexer, parser, ast and visualization module of this project have given me great help [C-Compiler](https://github.com/YJJfish/C-Compiler).

    It's where the initial framework and inspiration come from, some implementations of that project have been referenced. Many thanks to the author.

- The [LLVM Tutorial](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html). (official documents)





