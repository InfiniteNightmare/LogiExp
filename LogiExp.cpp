#include "LogiExp.h"
#include <cmath>
#include <iostream>
#include <stack>
#include <string>

//Ĭ�Ϲ��캯��
LogiExp::LogiExp() : LogiExp("")
{
}

//���캯��
LogiExp::LogiExp(std::string exp)
{
    inffix_exp = exp;
    PCNF = "��M(";
    PDNF = "��m("; 
    // �������жϱ��ʽ�Ƿ�Ϸ�,�����Ϸ�����ճ�Ա���������ش���
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

//�趨���ʽ
void LogiExp::SetExp(std::string exp)
{
    inffix_exp = exp;
    PCNF = "��M(";
    PDNF = "��m(";
    suffix_exp.clear();
    // �������жϱ��ʽ�Ƿ�Ϸ�,�����Ϸ�����ճ�Ա���������ش���
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

//������׺���ʽ
std::string LogiExp::GetInffixExp()
{
    return inffix_exp;
}

//���غ�׺���ʽ
std::string LogiExp::GetSuffixExp()
{
    return suffix_exp;
}

// ��������ȡ��ʽ
std::string LogiExp::GetPCNF()
{
    return PCNF;
}

// ��������ȡ��ʽ
std::string LogiExp::GetPDNF()
{
    return PDNF;
}

//���ʽ�Ϸ����ж�
//����ǰ�ַ���һ����������������ǰ�Ƿ��������Ż��д��ĸ��������ʽ��Ч
//����ǰ�ַ��Ƿ�������������ţ���Ҫ�������ǰ�Ƿ��������Ż��д��ĸ��������ʽ��Ч
//����ǰ�ַ��Ǵ�д��ĸ����ĸǰ������ĸ����ʽ��Ч
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

//�Ա��ʽ����
//����ǰ׺���ʽת���ɺ�׺���ʽ
bool LogiExp::ParseExp()
{
    std::stack<char> stack; //������洢ջ
    char top;               // ջ��Ԫ��
    int alphabet[26] = {0}; // ��ϣ���¼���ʽ�е������Ԫ��
    int i, j;               // ѭ��������
    int len = inffix_exp.length();  // ��׺���ʽ����
    // ������������׺���ʽ
    for (int i = 0; i < len; ++i)
    {
        switch (char c = inffix_exp[i])
        {
        //������
        case '(':
            //���ʽ�Ϸ����ж�
            if (!IsSignValid(i))
                return false;
            stack.push(c);
            break;

        //������
        case ')':
            // ���ʽ�Ϸ����ж�
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && (top = stack.top()) != '(')
            {
                stack.pop();

                // �������
                if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                // ������
                else if (top == '$')
                {
                    suffix_exp.append("+!");
                    continue;
                }
                // ��������
                else if (top == '>')
                {
                    suffix_exp.append("!*");
                    continue;
                }
                // ����������
                else if (top == '@')
                {
                    suffix_exp.append("!*!");
                    continue;
                }
                // ����ͬ��
                else if (top == '=')
                {
                    ParseXOR_XNOR(stack, '=');
                    continue;
                }
                // �������
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

        // �񶨺�
        case '!':
            // ���ʽ�Ϸ����ж�
            if (!IsSignValid(i))
                return false;
            stack.push(c);
            break;

        // ��Ǻ�
        case '#':
        // ��ȡ��
        case '*':
            //���ʽ�Ϸ����ж�
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '#'))
            {
                stack.pop();
                // �������
                if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        // ��Ǻ�
        case '$':
        // ��ȡ��
        case '+':
            //���ʽ�Ϸ����ж�
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '+' || top == '$'))
            {
                stack.pop();
                // ������
                if (top == '$')
                {
                    suffix_exp.append("+!");
                    continue;
                }
                // �������
                else if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        // �����񶨺�
        case '@':
        // ������
        case '>':
            //���ʽ�Ϸ����ж�
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '#' || top == '+' || top == '$' || top == '>' || top == '@'))
            {
                stack.pop();
                // ������
                if (top == '$')
                {
                    suffix_exp.append("+!");
                    continue;
                }
                // �������
                else if (top == '#')
                {
                    suffix_exp.append("*!");
                    continue;
                }
                // ��������
                else if (top == '>')
                {
                    suffix_exp.append("!*!");
                    continue;
                }
                // ����������
                else if (top == '@')
                {
                    suffix_exp.append("!*");
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        //����
        case '^':
        //˫������ͬ�򣩺�
        case '=':
            //���ʽ�Ϸ����ж�
            if (!IsSignValid(i))
                return false;
            while (!stack.empty() && ((top = stack.top()) == '!' || top == '*' || top == '#' || top == '+' || top == '$' || top == '>' || top == '@' || top == '='))
            {
                top = stack.top();
                stack.pop();
                // ����ͬ��
                if (top == '=')
                {
                    ParseXOR_XNOR(stack, '=');
                    continue;
                }
                // �������
                else if (top == '^')
                {
                    ParseXOR_XNOR(stack, '^');
                    continue;
                }
                suffix_exp.push_back(top);
            }
            stack.push(c);
            break;

        //�հ��ַ�
        case '\t':
        case ' ':
            break;

        //�����Ԫ
        default:
            if (c >= 'A' && c <= 'Z')
            {
                //���ʽ�Ϸ����ж�
                if (!IsSignValid(i))
                    return false;
                alphabet[c - 'A']++;
                suffix_exp.push_back(c);
            }
            else
                return false;
        }
    }
    // ����ջ��Ԫ�ز�����
    while (!stack.empty())
    {
        top = stack.top();
        suffix_exp.push_back(top);
        stack.pop();
        switch (top)
        {
        //�ж�������������ʽ�Ƿ�
        case '(':
            return false;

        // ����ͬ���������������ת��Ϊ�����߼�����
        case '=':
            suffix_exp.pop_back();
            ParseXOR_XNOR(stack, '=');
            break;

        // ����ͬ���������������ת��Ϊ�����߼�����
        case '^':
            suffix_exp.pop_back();
            ParseXOR_XNOR(stack, '^');
            break;

        // ��������������������ת��Ϊ�����߼�����
        case '#':
            suffix_exp.pop_back();
            suffix_exp.append("*!");
            break;

        // ��������������������ת��Ϊ�����߼�����
        case '$':
            suffix_exp.pop_back();
            suffix_exp.append("+!");
            break;

        // ���������������������ת��Ϊ�����߼�����
        case '>':
            suffix_exp.pop_back();
            suffix_exp.append("!*!");
            break;

        // �����������������������ת��Ϊ�����߼�����
        case '@':
            suffix_exp.pop_back();
            suffix_exp.append("!*");
            break;
        }
    }
    // �洢�����Ԫ����¼����
    var_num = 0;
    for (i = 0; i < 26; ++i)
        if (alphabet[i] != 0)
            var_num++;
    num = pow(2, var_num);
    var = new char[var_num + 1]; //�����洢����ռ�Ŀռ�
    for (i = 0, j = 0; i < 26 && j < var_num; ++i, ++j)
        if (alphabet[i] != 0)
            var[j] = i + 'A';
    var[var_num] = 0;
    return true;
}

//����ͬ��/���
void LogiExp::ParseXOR_XNOR(std::stack<char> &stack, char op)
{
    int i = 0, j = 0;                               //ѭ��������
    std::string subexp1, subexp2;                   //�����ݴ�ͬ���������ʱ���˵ĺ�׺���ʽ
    int subexp1_sign_num = 0, subexp2_sign_num = 0; //�����ʽ�г���������������������������Ԫ����=num+1

    //�������ʽ2
    for (i = suffix_exp.length() - 1, j = 0; i >= 0 && j <= subexp2_sign_num; --i)
    {
        subexp2.push_back(suffix_exp[i]);
        if (suffix_exp[i] == '*' || suffix_exp[i] == '+')
            subexp2_sign_num++;
        if (suffix_exp[i] >= 'A' && suffix_exp[i] <= 'Z')
            ++j;
    }

    //�������ʽ1
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

    // ���ݴ�ı��ʽ��������׺���ʽ��
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

// ʮ����ת������
void DexToBin(int dex, bool *bin, int n)
{
    for (int i = n - 1; i >= 0; --i)
    {
        bin[i] = dex % 2;
        dex >>= 1;
    }
}

// �����׺���ʽ���õ���ֵ������ȡ��ʽ������ȡ��ʽ
void LogiExp::Caculate()
{
    int i, j;                          // ѭ��������
    bool opnd1, opnd2;                 // ������
    char strnum[var_num + 1];          // ���ֵ��ַ�����ʽ
    bool binary[var_num];              // ʮ���ƶ�Ӧ�Ķ�����(�������ʾ��λ��ȡֵ)
    TruthTable = new std::string[num]; // ��ʼ����ֵ��,����ռ�
    std::stack<bool> stack;            // �ݴ��������ջ
    // �����������׺���ʽ
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
    // ���ĩβ���ය�Ų�����������
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

//�����ֵ��
void LogiExp::GetTruthTable()
{
    // ��׺���ʽ�п�,����,��˵�����ʽ�Ƿ�,ֱ�ӷ���
    if (suffix_exp.empty())
        return;
    int i;
    // �����ͷ
    for (i = 0; i < var_num; ++i)
        std::cout << var[i] << "\t";
    std::cout << inffix_exp << std::endl;
    // �����������µ������Ԫֵ�ͼ�����
    for (i = 0; i < num; ++i)
    {
        for (char c : TruthTable[i])
            std::cout << c << "\t";
        std::cout << std::endl;
    }
}

// �Ƚ��߼����ʽ�Ƿ�ȼ�
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

// �����==���أ����ڱȽ��߼����ʽ�ǹ۷�ȼ�
bool LogiExp::operator==(LogiExp logiexp)
{
    if (IsEuqal(logiexp))
        return true;
    return false;
}

// �����=���أ����ڶ��߼����ʽ����ֵ
void LogiExp::operator=(const LogiExp logiexp)
{
    // ����Ա��ֵ
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