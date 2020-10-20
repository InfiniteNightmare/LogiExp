# 命题逻辑表达式类LogiExp文档

## 前言

在学习完命题逻辑后，离散数学布置了一个作业，要求根据输入的表达式输出真值表和主合取表达式和主析取表达式，在经过分析后运用课本所学的知识和自学的相关知识，我写了一个命题逻辑表达式类专门处理命题逻辑表达式。在花了一个多星期后，我完成并调试了初版的代码，在测试的所有样例中都实现了预期的功能，但测试不可能兼顾所有情况，如有错误请不吝指教。

## 逻辑表达式

### 定义

由命题变元（单个大写字母表示）和命题联结词（如下所示）按照离散数学定义所组成的表达式。

### 联结词

括号'('和')'
否定'!'
合取'*'
析取'+'
条件'>'
双条件(同或)'='
异或'^'
条件否定‘@’
与非'#'
或非'$'

### 联结词优先级
1.((入栈前), )
2.!
3.=,^(入栈后)
4.*,#
5.+,$
6.>,@
7.=,^(入栈前)
8.（(入栈后)

## 类说明

```c++
class LogiExp
{
private:
    std::string inffix_exp;                       //中缀表达式
    std::string suffix_exp;                       //后缀表达式
    int var_num;                                  //命题变元数量
    int num;                                      //所有变元赋值情况的数量（真值表的行数）
    char *var;                                    //命题变元名
    std::string PCNF;                             //主合取范式
    std::string PDNF;                             //主析取范式
    std::string *TruthTable;                      //真值表
    bool InffixToSuffix();                        //前缀表达式转换成后缀表达式
    bool IsSignValid(int);                        //表达式合法性判断
    bool ParseExp();                              //对表达式进行解析
    void ParseXOR_XNOR(std::stack<char> &, char); //处理同或/异或
    void Caculate();                              //计算后缀表达式，得到真值表及主合取范式和主析取范式

public:
    LogiExp();                     //默认构造函数
    LogiExp(std::string);          //构造函数
    void SetExp(std::string);      //设定表达式
    std::string GetInffixExp();    //返回转换后的中缀表达式
    std::string GetSuffixExp();    //返回后缀表达式
    void GetTruthTable();          //输出真值表
    std::string GetPCNF();         //返回主合取范式
    std::string GetPDNF();         //返回主析取范式
    bool IsEuqal(LogiExp);         //比较逻辑表达式是否等价
    bool operator==(LogiExp);      //运算符==重载，用于比较逻辑表达式是观否等价
    void operator=(const LogiExp); //运算符=重载，用于对逻辑表达式对象赋值
};
```



## 算法

### 流程图

![image-20201019211335782](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019211335782.png)

### 主要思路

常用的逻辑表达式为中缀表达式，但中缀表达式存在优先级，而且计算机不易直接运算，因此先解析中缀表达式，将中缀表达式转换成后缀表达式再进行计算，转换中缀表达式时运用栈暂存运算符，遇到新的命题变元时存储变元的名字，便于后续的计算。同时c语言只提供了与或非三种基本逻辑运算，因此在解析并转换中缀表达式时对其他逻辑运算符号运用如下的基本命题公式进行转换。计算后缀表达式时运用栈暂存操作数，并根据计算结果填真值表和主析取表达和主合取表达式。

#### 基本命题公式

$A \to B = \neg (A \wedge \neg B) \\ A \Leftrightarrow B = (A \wedge B) \vee (\neg A \wedge \neg B) \\ A \overline{\vee} B = (A \wedge \neg B) \vee (\neg A \wedge B) \\ A \overset{c}{\to} B = A \wedge \neg B \\ A \uparrow B = \neg (A \wedge B) \\ A \downarrow B = \neg(A \vee B)$

### 中缀表达式转后缀表达式
遇到命题变元直接存在后缀表达式中，遇到运算符用堆栈存储。当入栈的运算符优先级比栈顶运算符高，直接入栈；当入栈的运算符优先级比栈顶运算符低，依次弹出栈顶运算符，直到栈顶运算符优先级比该运算符低，将该运算符入栈。左括号入栈时，左括号优先级最高；左括号入栈后，左括号优先级最低。右括号入栈时，右括号优先级最低，依次弹出栈顶运算符，直到遇到左括号为止，如果到栈内无左括号，则该表达式非法。弹出运算符时，如果该运算符不是基本的与或非运算符，则根据转换公式转换成仅含与或非的后缀表达式。

#### 表达式合法性判断
若当前字符是一般运算符，看运算符前是否是右括号或大写字母，否则表达式无效
若当前字符是否定运算符或左括号，还要看运算符前是否无右括号或大写字母，否则表达式无效
若当前字符是大写字母且字母前还是字母则表达式无效

#### 对于同或/异或运算符的处理
##### 算法一
由于$A \Leftrightarrow B = (A \wedge B) \vee (\neg A \wedge \neg B) \quad A \overline{\vee} B = (A \wedge \neg B) \vee (\neg A \wedge B)$，AB两命题在结果表达式中出现了两次，而使用堆栈只能处理一次，所以需要将其保存起来再存入结果表达式中。即遇到"A=B"形式，保存A和B的后缀表达式，结果表达式输出"AB\*A!B!\*+"。由于在后缀表达式中同一层子式中运算符在数字的后面，而且除了否定运算符，其他运算符都是二元运算符，所以对于识别A，可以通过从后往前遍历堆栈，记录命题变元前非
否定运算符的个数，再从后往前取个数+1的字母就构成了A的后缀表达式。对于识别B，可以继续中缀-后缀表达式的转换，直到遇到比原栈顶运算符优先级低或表尾，并将新产生的后缀表达式保存，即为B的后缀表达式。

##### 算法二
在算法一中处理B较为复杂，因此可以先让B正常处理，在最后的后缀表达式中再提取AB。
在栈中时看作除左括号外最低优先级，入栈后优先级提高到第三优先级，这样结果的后缀表达式中包含了同或/异或运算符，当该运算符出栈后再进行具体解析。后缀表达式可以看成二叉树，叶子结点为命题变元，其他结点为运算符，因此在后缀表达式同或/异或运算符前的两棵子树分别是A和B，解析算法如算法一中处理A一样，从后往前遍历将第一棵除了否定运算符外其他运算符及其个数+1的子树作B，第二棵作A。

### 计算后缀表达式

计算的总次数num=2^var_num（var_num为表达式中命题变元的种类数量），因此需要循环num次，每次各变元的值为当前循环次数转化为二进制后对应位数的二进制值（如ABC三变元的表达式第5次循环时的值分别为101，正好为5的二进制值）。

遍历解析所得的后缀表达式：遇到命题变元（大写字母）将其赋值并存入栈中；遇到运算符号从栈中取出数进行运算，将计算结果存入栈中。按上述步骤执行，直到后缀表达式遍历结束，此时栈内剩下的元素就是计算结果，将该元素取出，赋值给当前变量取值的真值结果。同时判断该元素的值，如果为true，则该取值情况属于最小项，存入主析取范式，如果为false，则该取值情况属于最大项，存入主合取范式。

## 实例演示

### 演示1

#### 演示代码

```c++
    std::string exp;
    std::cin >> exp;                                
    LogiExp logiexp = LogiExp(exp);												   // 实例化LogiExp对象
    std::cout << logiexp.GetSuffixExp() << std::endl; 							   // 输出转化后的后缀表达式
    std::cout << logiexp.GetPCNF() << std::endl << logiexp.GetPDNF() << std::endl; // 输出主合取范式和主析取范式
    logiexp.GetTruthTable();  													   // 输出真值表
```

#### 输入1

![image-20201019201742072](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019201742072.png)

#### 输出1

![image-20201019201828034](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019201828034.png)

#### 输入2

![image-20201019202136298](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019202136298.png)

#### 输出2

![image-20201019202221190](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019202221190.png)

### 演示2

#### 演示代码

```c++
    std::string exp1, exp2;
    std::cin >> exp1 >> exp2;
    // 创建逻辑表达式对象
    LogiExp logiexp1 = LogiExp(exp1), logiexp2{exp2};
    // 逻辑表达式输出
    std::cout << "logiexp1:\n"
              << logiexp1.GetPCNF() << std::endl
              << logiexp1.GetPDNF() << std::endl;
    logiexp1.GetTruthTable();
    std::cout << "logiexp2:\n"
              << logiexp2.GetPCNF() << std::endl
              << logiexp2.GetPDNF() << std::endl;
    logiexp2.GetTruthTable();
    if (logiexp1 == logiexp2) // 逻辑表达式判断
        std::cout << "logiexp1 == logiexp2" << std::endl;
    else
        std::cout << "logiexp1 != logiexp2" << std::endl;
    logiexp1 = logiexp2; // 逻辑表达式赋值
    // 逻辑表达式输出
    std::cout << "After assignment, logiexp1:\n"
              << logiexp1.GetPCNF() << std::endl
              << logiexp1.GetPDNF() << std::endl;
    logiexp1.GetTruthTable();
```

#### 输入1

![image-20201019203439225](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019203439225.png)

#### 输出1

![image-20201019203511364](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019203511364.png)

#### 输入2

![image-20201019204509858](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019204509858.png)

#### 输出2

![image-20201019204543090](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019204543090.png)

## 总结

本次作业运用和巩固了离散数学中的命题逻辑以及数据结构中的栈的知识，同时600多行的源代码以及文档的书写也提升了程序设计的能力——我掌握了多文件的编译、c++中面向对象的思想以及运算符重载的知识。在程序设计中运用了“自顶向下，逐步求精”的思想，将复杂的逻辑表达式运算的问题分解成许多子问题，对于每个子问题分别用不同的数据结构和算法进行处理。在写完最后一行代码后我深深感觉这并没有想象中那么难，只要有耐心、多思考，不半途而废，问题总能迎刃而解。