#pragma once
#include <stack>
#include <string>

/*
*逻辑表达式类
*大写字母表示逻辑变量
*联结词:括号'('和')';否定'!';合取'*';析取'+';条件'>';双条件'=';异或'^';条件否定‘@’;与非'#';或非'$'
*优先级：1.()
********2.!
********3.*,#
********4.+,$
********5.>,@
********6.=
*/
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
    bool operator==(LogiExp);      //运算符==重载，用于比较逻辑表达式是否等价
    void operator=(const LogiExp); //运算符=重载，用于对逻辑表达式对象赋值
};