- 编译器采用的是llvm tutoriol中给ast的节点类加上codegen方法的模式，然后在root上调用codegen可以递归生成整个IR。虽然这样简单，但这种模式极大增加了模块耦合程度。

    理想的方法应该是ast模块只提供一棵语法树，由CodeGenerator模块全面负责语法树转化为IR的过程。\
    
- 符号表问题

  目前是function、type、variable的符号表采用的是一张（但可以确定类型），不允许任何重名。
  
  这样可以更clear，但更容易命名冲突。可以像C一样用多个符号表。
  
  另外目前的符号表是复制模式，消耗大，可以改为二叉树模式。
  
- 全局变量初始化问题 ✅

    全局变量只能用常量初始化。

    在初始化时，理论上不应该属于任何basic block。但为了检测是否纯在常量情况，解决方法是构建临时的bb，在bb里codegen初始化的表达式，如果发现bb的size大于0了，认为不是常量初始化，报错。

    这种方法不是很graceful。

- array问题 ✅

    如何看待数组，看成const ptr吗？传值、赋值等情况中，怎么理解？

    难以初始化！

    不让初始化吧。

- const问题 ✅

    如何实现const逻辑？llvm貌似没有提供。

    如何实现特殊语法ptr< const >逻辑？

    目前的解决方法：包装llvm::Type成ExprValue，额外存const信息。

    不是很graceful，但能解决问题。

- Typecast问题 ✅ 

    如何让typecast（底层隐式）时，能提示类型的字符串信息，比如结构体的名称？可能需要利用ExprValue。

    放弃实现，llvm::Type和自定义type之间有不少矛盾，难以统一管理。

- OOP问题

    如何实现struct的成员函数？
    
- 重构代码 ✅

    变量命名、输出统一。