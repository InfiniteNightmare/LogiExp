#pragma once
#include <stack>
#include <string>

/*
*�߼����ʽ��
*��д��ĸ��ʾ�߼�����
*�����:����'('��')';��'!';��ȡ'*';��ȡ'+';����'>';˫����'=';���'^';�����񶨡�@��;���'#';���'$'
*���ȼ���1.()
********2.!
********3.*,#
********4.+,$
********5.>,@
********6.=
*/
class LogiExp
{
private:
    std::string inffix_exp;                       //��׺���ʽ
    std::string suffix_exp;                       //��׺���ʽ
    int var_num;                                  //�����Ԫ����
    int num;                                      //���б�Ԫ��ֵ�������������ֵ���������
    char *var;                                    //�����Ԫ��
    std::string PCNF;                             //����ȡ��ʽ
    std::string PDNF;                             //����ȡ��ʽ
    std::string *TruthTable;                      //��ֵ��
    bool InffixToSuffix();                        //ǰ׺���ʽת���ɺ�׺���ʽ
    bool IsSignValid(int);                        //���ʽ�Ϸ����ж�
    bool ParseExp();                              //�Ա��ʽ���н���
    void ParseXOR_XNOR(std::stack<char> &, char); //����ͬ��/���
    void Caculate();                              //�����׺���ʽ���õ���ֵ������ȡ��ʽ������ȡ��ʽ

public:
    LogiExp();                     //Ĭ�Ϲ��캯��
    LogiExp(std::string);          //���캯��
    void SetExp(std::string);      //�趨���ʽ
    std::string GetInffixExp();    //����ת�������׺���ʽ
    std::string GetSuffixExp();    //���غ�׺���ʽ
    void GetTruthTable();          //�����ֵ��
    std::string GetPCNF();         //��������ȡ��ʽ
    std::string GetPDNF();         //��������ȡ��ʽ
    bool IsEuqal(LogiExp);         //�Ƚ��߼����ʽ�Ƿ�ȼ�
    bool operator==(LogiExp);      //�����==���أ����ڱȽ��߼����ʽ�Ƿ�ȼ�
    void operator=(const LogiExp); //�����=���أ����ڶ��߼����ʽ����ֵ
};