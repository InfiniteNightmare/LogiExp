#include "LogiExp.h"
#include <cmath>
#include <iostream>
#include <stack>
#include <string>

//默认构造函数
LogiExp::LogiExp() : LogiExp("")
{
}

//构造函数
LogiExp::LogiExp(std::string exp)
{
    inffix_exp = exp;
    PCNF = "∏M(";
    PDNF = "∑m("; 
    // 解析并判断表达式是否合法,若不合法则清空成员变量并返回错误
    if (!ParseExp())
    {
        suffix_exp.clear();
        PCNF.clear();
        PDNF.clear();
        std::cout << "Invalid expression!" << std::endl;
        return;
    }
    Caculate();
}

//设定表达式
void LogiExp::SetExp(std::string exp)
{
    inffix_exp = exp;
    PCNF = "∏M(";
    PDNF = "∑m(";
    suffix_exp.clear();
    // 解析并判断表达式是否合法,若不合法则清空成员变量并返回错误
    if (!ParseExp())
    {
        PCNF.clear();
        PDNF.clear();
        std::cout << "Invalid expression!" << std::endl;
        delete[] TruthTable;
        return;
    }
    Caculate();
}

//返回中缀表达式
std::string LogiExp::GetInffixExp()
{
    return inffix_exp;
}

//返回后缀表达式
std::string LogiExp::GetSuffixExp()
{
    return suffix_exp;
}

// 返回主合取范式
std::string LogiExp::GetPCNF()
{
    return PCNF;
}

// 返回主析取范式
std::string LogiExp::GetPDNF()
{
    return PDNF;
}

//表达式合法性判断
//若当前字符是一般运算符，看运算符前是否是右括号或大写字母，否则表达式无效
//若当前字符是否定运算符或左括号，还要看运算符前是否无右括号或大写字母，否则表达式无效
//若当前字符是大写字母且字母前还是字母则表达式无效
bool LogiExp::IsSignValid(int i)
{
    if (i == 0)
    {
        char firstchar = inffix_exp[0];
        if (firstchar == '(' || firstchar == '!' || (firstchar >= 'A' && firstchar <= 'Z'))
            return true;
        else
            return false;
    }
    char cur = inffix_exp[i], prior = inffix_exp[i - 1];
    if (cur >= 'A' && cur <= 'Z')
    {
        if (prior >= 'A' && prior <= 'Z')
            return false;
        else
            return true;
    }
    else
    {
        if ((cur == '(' || cur == '!'))
        {
            if (prior != ')' && (prior < 'A' || prior > 'Z'))
                return true;
            else
                return false;
        }
        else if (prior == ')' || (prior >= 'A' && prior <= 'Z'))
            return true;
        else
            return false;
    }
}

//对表达式解析
//并将前缀表达式转换成后缀表达式
bool LogiExp::ParseExp()
{
    std::stack<char> stack; //运算符存储栈
    char top;               // 栈顶元素
    int alphabet[26] = {0}; // 哈希表记录表达式中的命题变元名
    int i, j;               // 循环计数器
    int len = inffix_exp.length();  // 中缀表达式长度
    // 遍历并解析中缀表达式
    for (int i = 0; i < len; ++i)
    {
        switch (char c = inffix_exp[i])
        {
        //左括号
        case '(':
            //表达式合法性判断
            if (!IsSignValid(i))
                return false;
            stack.push(c);
            break;

        //右括号
        case ')':
            // 表达式合法性判断
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && (top = stack.top()) != '(')
            {
                stack.pop();

                // 处理与非
                if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                // 处理或非
                else if (top == '$')
                {
                    suffix_exp.append("+!");
                    continue;
                }
                // 处理条件
                else if (top == '>')
                {
                    suffix_exp.append("!*");
                    continue;
                }
                // 处理条件否定
                else if (top == '@')
                {
                    suffix_exp.append("!*!");
                    continue;
                }
                // 处理同或
                else if (top == '=')
                {
                    ParseXOR_XNOR(stack, '=');
                    continue;
                }
                // 处理异或
                else if (top == '^')
                {
                    ParseXOR_XNOR(stack, '^');
                    continue;
                }
                suffix_exp.push_back(top);
            }
            if (stack.empty())
                return false;
            stack.pop();
            break;

        // 否定号
        case '!':
            // 表达式合法性判断
            if (!IsSignValid(i))
                return false;
            stack.push(c);
            break;

        // 与非号
        case '#':
        // 合取号
        case '*':
            //表达式合法性判断
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '#'))
            {
                stack.pop();
                // 处理与非
                if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        // 或非号
        case '$':
        // 析取号
        case '+':
            //表达式合法性判断
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '+' || top == '$'))
            {
                stack.pop();
                // 处理或非
                if (top == '$')
                {
                    suffix_exp.append("+!");
                    continue;
                }
                // 处理与非
                else if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        // 条件否定号
        case '@':
        // 条件号
        case '>':
            //表达式合法性判断
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '#' || top == '+' || top == '$' || top == '>' || top == '@'))
            {
                stack.pop();
                // 处理或非
                if (top == '$')
                {
                    suffix_exp.append("+!");
                    continue;
                }
                // 处理与非
                else if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                // 处理条件
                else if (top == '>')
                {
                    suffix_exp.append("!*!");
                    continue;
                }
                // 处理条件否定
                else if (top == '@')
                {
                    suffix_exp.append("!*");
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        //异或号
        case '^':
        //双条件（同或）号
        case '=':
            //表达式合法性判断
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '#' || top == '+' || top == '$' || top == '>' || top == '@' || top == '='))
            {
                top = stack.top();
                stack.pop();
                // 处理同或
                if (top == '=')
                {
                    ParseXOR_XNOR(stack, '=');
                    continue;
                }
                // 处理异或
                else if (top == '^')
                {
                    ParseXOR_XNOR(stack, '^');
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        //空白字符
        case '\t':
        case ' ':
            break;

        //命题变元
        default:
            if (c >= 'A' && c <= 'Z')
            {
                //表达式合法性判断
                if (!IsSignValid(i))
                    return false;
                alphabet[c - 'A']++;
                suffix_exp.push_back(c);
            }
            else
                return false;
        }
    }
    // 弹出栈中元素并解析
    while (!stack.empty())
    {
        top = stack.top();
        suffix_exp.push_back(top);
        stack.pop();
        switch (top)
        {
        //有多余左括号则表达式非法
        case '(':
            return false;

        // 解析同或运算符，并将其转换为基本逻辑操作
        case '=':
            suffix_exp.pop_back();
            ParseXOR_XNOR(stack, '=');
            break;

        // 解析同或运算符，并将其转换为基本逻辑操作
        case '^':
            suffix_exp.pop_back();
            ParseXOR_XNOR(stack, '^');
            break;

        // 解析与非运算符，并将其转换为基本逻辑操作
        case '#':
            suffix_exp.pop_back();
            suffix_exp.append("*!");
            break;

        // 解析或非运算符，并将其转换为基本逻辑操作
        case '$':
            suffix_exp.pop_back();
            suffix_exp.append("+!");
            break;

        // 解析条件运算符，并将其转换为基本逻辑操作
        case '>':
            suffix_exp.pop_back();
            suffix_exp.append("!*!");
            break;

        // 解析条件否定运算符，并将其转换为基本逻辑操作
        case '@':
            suffix_exp.pop_back();
            suffix_exp.append("!*");
            break;
        }
    }
    // 存储命题变元并记录数量
    var_num = 0;
    for (i = 0; i < 26; ++i)
        if (alphabet[i] != 0)
            var_num++;
    num = pow(2, var_num);
    var = new char[var_num + 1]; //创建存储命题空间的空间
    for (i = 0, j = 0; i < 26 && j < var_num; ++i, ++j)
        if (alphabet[i] != 0)
            var[j] = i + 'A';
    var[var_num] = 0;
    return true;
}

//处理同或/异或
void LogiExp::ParseXOR_XNOR(std::stack<char> &stack, char op)
{
    int i = 0, j = 0;                               //循环计数器
    std::string subexp1, subexp2;                   //用于暂存同或异或运算时两端的后缀表达式
    int subexp1_sign_num = 0, subexp2_sign_num = 0; //两表达式中除否定运算符外其他运算符个数，变元个数=num+1

    //构建表达式2
    for (i = suffix_exp.length() - 1, j = 0; i >= 0 && j <= subexp2_sign_num; --i)
    {
        subexp2.push_back(suffix_exp[i]);
        if (suffix_exp[i] == '*' || suffix_exp[i] == '+')
            subexp2_sign_num++;
        if (suffix_exp[i] >= 'A' && suffix_exp[i] <= 'Z')
            ++j;
    }

    //构建表达式1
    for (j = 0; i >= 0 && j <= subexp1_sign_num; --i)
    {
        subexp1.push_back(suffix_exp[i]);
        if (suffix_exp[i] == '*' || suffix_exp[i] == '+')
            subexp1_sign_num++;
        if (suffix_exp[i] >= 'A' && suffix_exp[i] <= 'Z')
            ++j;
    }
    if (op == '^')
        suffix_exp.push_back('!');
    suffix_exp.push_back('*');

    // 将暂存的表达式弹出到后缀表达式中
    int len1 = subexp1.length(), len2 = subexp2.length();
    for (i = len1 - 1; i >= 0; --i)
        suffix_exp.push_back(subexp1[i]);
    suffix_exp.push_back('!');
    for (i = len2 - 1; i >= 0; --i)
        suffix_exp.push_back(subexp2[i]);
    if (op == '=')
        suffix_exp.push_back('!');
    suffix_exp.push_back('*');
    suffix_exp.push_back('+');
}

// 十进制转二进制
void DexToBin(int dex, bool *bin, int n)
{
    for (int i = n - 1; i >= 0; --i)
    {
        bin[i] = dex % 2;
        dex >>= 1;
    }
}

// 计算后缀表达式，得到真值表及主合取范式和主析取范式
void LogiExp::Caculate()
{
    int i, j;                          // 循环计数器
    bool opnd1, opnd2;                 // 操作数
    char strnum[var_num + 1];          // 数字的字符串形式
    bool binary[var_num];              // 十进制对应的二进制(用数组表示各位的取值)
    TruthTable = new std::string[num]; // 初始化真值表,分配空间
    std::stack<bool> stack;            // 暂存计算结果的栈
    // 遍历并计算后缀表达式
    for (i = 0; i < num; ++i)
    {
        DexToBin(i, binary, var_num);
        for (j = 0; j < var_num; ++j)
            TruthTable[i].push_back(binary[j] + '0');
        for (char c : suffix_exp)
        {
            switch (c)
            {
            case '*':
                opnd2 = stack.top();
                stack.pop();
                opnd1 = stack.top();
                stack.pop();
                opnd1 &= opnd2;
                stack.push(opnd1);
                break;
            case '+':
                opnd2 = stack.top();
                stack.pop();
                opnd1 = stack.top();
                stack.pop();
                opnd1 |= opnd2;
                stack.push(opnd1);
                break;
            case '!':
                opnd1 = stack.top();
                stack.pop();
                opnd1 = !opnd1;
                stack.push(opnd1);
                break;
            default:
                for (j = 0; j < var_num; ++j)
                {
                    if (c == var[j])
                    {
                        if (binary[j] == true)
                            stack.push(true);
                        else
                            stack.push(false);
                        break;
                    }
                }
            }
        }
        TruthTable[i].push_back(stack.top() + '0');
        itoa(i, &strnum[0], 10);
        if (stack.top())
        {
            PCNF.append(strnum);
            PCNF.push_back(',');
        }
        else
        {
            PDNF.append(strnum);
            PDNF.push_back(',');
        }

        stack.pop();
    }
    // 清除末尾多余逗号并补上右括号
    if (!PCNF.empty())
    {
        PCNF.pop_back();
        PCNF.push_back(')');
    }
    else
        PCNF.clear();
    if (!PDNF.empty())
    {
        PDNF.pop_back();
        PDNF.push_back(')');
    }
    else
        PDNF.clear();
}

//输出真值表
void LogiExp::GetTruthTable()
{
    // 后缀表达式判空,若空,则说明表达式非法,直接返回
    if (suffix_exp.empty())
        return;
    int i;
    // 输出表头
    for (i = 0; i < var_num; ++i)
        std::cout << var[i] << "\t";
    std::cout << inffix_exp << std::endl;
    // 输出所有情况下的命题变元值和计算结果
    for (i = 0; i < num; ++i)
    {
        for (char c : TruthTable[i])
            std::cout << c << "\t";
        std::cout << std::endl;
    }
}

// 比较逻辑表达式是否等价
bool LogiExp::IsEuqal(LogiExp logiexp)
{
    int i, num = pow(2, var_num);
    if (var_num != logiexp.var_num)
        return false;
    for (i = 0; i < num; ++i)
        if (TruthTable[i][var_num] != logiexp.TruthTable[i][var_num])
            return false;
    return true;
}

// 运算符==重载，用于比较逻辑表达式是观否等价
bool LogiExp::operator==(LogiExp logiexp)
{
    if (IsEuqal(logiexp))
        return true;
    return false;
}

// 运算符=重载，用于对逻辑表达式对象赋值
void LogiExp::operator=(const LogiExp logiexp)
{
    // 各成员赋值
    inffix_exp = logiexp.inffix_exp;
    suffix_exp = logiexp.suffix_exp;
    var_num = logiexp.var_num;
    // deep copy
    delete[] var;
    var = new char[var_num];
    memcpy(var, logiexp.var, var_num * sizeof(char));
    PCNF = logiexp.PCNF;
    PDNF = logiexp.PDNF;
    delete[] TruthTable;
    num = logiexp.num;
    // deep copy
    TruthTable = new std::string[num];
    for (int i = 0; i < num; ++i)
        TruthTable[i] = logiexp.TruthTable[i];
}