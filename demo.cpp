#include "LogiExp.h"
#include <iostream>
#include <string>

int main()
{
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
    return 0;
}