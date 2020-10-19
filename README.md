# �����߼�����ʽ��LogiExp�ĵ�

## ǰ��

��ѧϰ�������߼�����ɢ��ѧ������һ����ҵ��Ҫ���������ı���ʽ�����ֵ��������ȡ����ʽ������ȡ����ʽ���ھ������������ÿα���ѧ��֪ʶ����ѧ�����֪ʶ����д��һ�������߼�����ʽ��ר�Ŵ��������߼�����ʽ���ڻ���һ�������ں�����ɲ������˳���Ĵ��룬�ڲ��Ե����������ж�ʵ����Ԥ�ڵĹ��ܣ������Բ����ܼ��������������д����벻��ָ�̡�

## �߼�����ʽ

### ����

�������Ԫ��������д��ĸ��ʾ������������ʣ�������ʾ��������ɢ��ѧ��������ɵı���ʽ��

### �����

����'('��')'
��'!'
��ȡ'*'
��ȡ'+'
����'>'
˫����(ͬ��)'='
���'^'
�����񶨡�@��
���'#'
���'$'

### ��������ȼ�
1.((��ջǰ), )
2.!
3.=,^(��ջ��)
4.*,#
5.+,$
6.>,@
7.=,^(��ջǰ)
8.��(��ջ��)

## ��˵��

```c++
class LogiExp
{
private:
    std::string inffix_exp;                       //��׺����ʽ
    std::string suffix_exp;                       //��׺����ʽ
    int var_num;                                  //�����Ԫ����
    int num;                                      //���б�Ԫ��ֵ�������������ֵ����������
    char *var;                                    //�����Ԫ��
    std::string PCNF;                             //����ȡ��ʽ
    std::string PDNF;                             //����ȡ��ʽ
    std::string *TruthTable;                      //��ֵ��
    bool InffixToSuffix();                        //ǰ׺����ʽת���ɺ�׺����ʽ
    bool IsSignValid(int);                        //����ʽ�Ϸ����ж�
    bool ParseExp();                              //�Ա���ʽ���н���
    void ParseXOR_XNOR(std::stack<char> &, char); //����ͬ��/���
    void Caculate();                              //�����׺����ʽ���õ���ֵ��������ȡ��ʽ������ȡ��ʽ

public:
    LogiExp();                     //Ĭ�Ϲ��캯��
    LogiExp(std::string);          //���캯��
    void SetExp(std::string);      //�趨����ʽ
    std::string GetInffixExp();    //����ת�������׺����ʽ
    std::string GetSuffixExp();    //���غ�׺����ʽ
    void GetTruthTable();          //�����ֵ��
    std::string GetPCNF();         //��������ȡ��ʽ
    std::string GetPDNF();         //��������ȡ��ʽ
    bool IsEuqal(LogiExp);         //�Ƚ��߼�����ʽ�Ƿ�ȼ�
    bool operator==(LogiExp);      //�����==���أ����ڱȽ��߼�����ʽ�ǹ۷�ȼ�
    void operator=(const LogiExp); //�����=���أ����ڶ��߼�����ʽ����ֵ
};
```



## �㷨

### ����ͼ

![image-20201019211335782](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019211335782.png)

### ��Ҫ˼·

���õ��߼�����ʽΪ��׺����ʽ������׺����ʽ�������ȼ������Ҽ��������ֱ�����㣬����Ƚ�����׺����ʽ������׺����ʽת���ɺ�׺����ʽ�ٽ��м��㣬ת����׺����ʽʱ����ջ�ݴ�������������µ������Ԫʱ�洢��Ԫ�����֣����ں����ļ��㡣ͬʱc����ֻ�ṩ���������ֻ����߼����㣬����ڽ�����ת����׺����ʽʱ�������߼���������������µĻ������⹫ʽ����ת���������׺����ʽʱ����ջ�ݴ�������������ݼ���������ֵ��������ȡ���������ȡ����ʽ��

#### �������⹫ʽ

$A \to B = \neg (A \wedge \neg B) \\ A \Leftrightarrow B = (A \wedge B) \vee (\neg A \wedge \neg B) \\ A \overline{\vee} B = (A \wedge \neg B) \vee (\neg A \wedge B) \\ A \overset{c}{\to} B = A \wedge \neg B \\ A \uparrow B = \neg (A \wedge B) \\ A \downarrow B = \neg(A \vee B)$

### ��׺����ʽת��׺����ʽ
���������Ԫֱ�Ӵ��ں�׺����ʽ�У�����������ö�ջ�洢������ջ����������ȼ���ջ��������ߣ�ֱ����ջ������ջ����������ȼ���ջ��������ͣ����ε���ջ���������ֱ��ջ����������ȼ��ȸ�������ͣ������������ջ����������ջʱ�����������ȼ���ߣ���������ջ�����������ȼ���͡���������ջʱ�����������ȼ���ͣ����ε���ջ���������ֱ������������Ϊֹ�������ջ���������ţ���ñ���ʽ�Ƿ������������ʱ���������������ǻ���������������������ת����ʽת���ɽ������ǵĺ�׺����ʽ��

#### ����ʽ�Ϸ����ж�
����ǰ�ַ���һ����������������ǰ�Ƿ��������Ż��д��ĸ���������ʽ��Ч
����ǰ�ַ��Ƿ�������������ţ���Ҫ�������ǰ�Ƿ��������Ż��д��ĸ���������ʽ��Ч
����ǰ�ַ��Ǵ�д��ĸ����ĸǰ������ĸ�����ʽ��Ч

#### ����ͬ��/���������Ĵ���
##### �㷨һ
����$A \Leftrightarrow B = (A \wedge B) \vee (\neg A \wedge \neg B) \quad A \overline{\vee} B = (A \wedge \neg B) \vee (\neg A \wedge B)$��AB�������ڽ������ʽ�г��������Σ���ʹ�ö�ջֻ�ܴ���һ�Σ�������Ҫ���䱣�������ٴ���������ʽ�С�������"A=B"��ʽ������A��B�ĺ�׺����ʽ���������ʽ���"AB\*A!B!\*+"�������ں�׺����ʽ��ͬһ����ʽ������������ֵĺ��棬���ҳ��˷��������������������Ƕ�Ԫ����������Զ���ʶ��A������ͨ���Ӻ���ǰ������ջ����¼�����Ԫǰ��
��������ĸ������ٴӺ���ǰȡ����+1����ĸ�͹�����A�ĺ�׺����ʽ������ʶ��B�����Լ�����׺-��׺����ʽ��ת����ֱ��������ԭջ����������ȼ��ͻ��β�������²����ĺ�׺����ʽ���棬��ΪB�ĺ�׺����ʽ��

##### �㷨��
���㷨һ�д���B��Ϊ���ӣ���˿�������B���������������ĺ�׺����ʽ������ȡAB��
��ջ��ʱ��������������������ȼ�����ջ�����ȼ���ߵ��������ȼ�����������ĺ�׺����ʽ�а�����ͬ��/���������������������ջ���ٽ��о����������׺����ʽ���Կ��ɶ�������Ҷ�ӽ��Ϊ�����Ԫ���������Ϊ�����������ں�׺����ʽͬ��/��������ǰ�����������ֱ���A��B�������㷨���㷨һ�д���Aһ�����Ӻ���ǰ��������һ�ó��˷������������������������+1��������B���ڶ�����A��

### �����׺����ʽ

������ܴ���num=2^var_num��var_numΪ����ʽ�������Ԫ�������������������Ҫѭ��num�Σ�ÿ�θ���Ԫ��ֵΪ��ǰѭ������ת��Ϊ�����ƺ��Ӧλ���Ķ�����ֵ����ABC����Ԫ�ı���ʽ��5��ѭ��ʱ��ֵ�ֱ�Ϊ101������Ϊ5�Ķ�����ֵ����

�����������õĺ�׺����ʽ�����������Ԫ����д��ĸ�����丳ֵ������ջ�У�����������Ŵ�ջ��ȡ�����������㣬������������ջ�С�����������ִ�У�ֱ����׺����ʽ������������ʱջ��ʣ�µ�Ԫ�ؾ��Ǽ�����������Ԫ��ȡ������ֵ����ǰ����ȡֵ����ֵ�����ͬʱ�жϸ�Ԫ�ص�ֵ�����Ϊtrue�����ȡֵ���������С���������ȡ��ʽ�����Ϊfalse�����ȡֵ�������������������ȡ��ʽ��

## ʵ����ʾ

### ��ʾ1

#### ��ʾ����

```c++
    std::string exp;
    std::cin >> exp;                                
    LogiExp logiexp = LogiExp(exp);												   // ʵ����LogiExp����
    std::cout << logiexp.GetSuffixExp() << std::endl; 							   // ���ת����ĺ�׺����ʽ
    std::cout << logiexp.GetPCNF() << std::endl << logiexp.GetPDNF() << std::endl; // �������ȡ��ʽ������ȡ��ʽ
    logiexp.GetTruthTable();  													   // �����ֵ��
```

#### ����1

![image-20201019201742072](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019201742072.png)

#### ���1

![image-20201019201828034](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019201828034.png)

#### ����2

![image-20201019202136298](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019202136298.png)

#### ���2

![image-20201019202221190](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019202221190.png)

### ��ʾ2

#### ��ʾ����

```c++
    std::string exp1, exp2;
    std::cin >> exp1 >> exp2;
    // �����߼�����ʽ����
    LogiExp logiexp1 = LogiExp(exp1), logiexp2{exp2};
    // �߼�����ʽ���
    std::cout << "logiexp1:\n"
              << logiexp1.GetPCNF() << std::endl
              << logiexp1.GetPDNF() << std::endl;
    logiexp1.GetTruthTable();
    std::cout << "logiexp2:\n"
              << logiexp2.GetPCNF() << std::endl
              << logiexp2.GetPDNF() << std::endl;
    logiexp2.GetTruthTable();
    if (logiexp1 == logiexp2) // �߼�����ʽ�ж�
        std::cout << "logiexp1 == logiexp2" << std::endl;
    else
        std::cout << "logiexp1 != logiexp2" << std::endl;
    logiexp1 = logiexp2; // �߼�����ʽ��ֵ
    // �߼�����ʽ���
    std::cout << "After assignment, logiexp1:\n"
              << logiexp1.GetPCNF() << std::endl
              << logiexp1.GetPDNF() << std::endl;
    logiexp1.GetTruthTable();
```

#### ����1

![image-20201019203439225](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019203439225.png)

#### ���1

![image-20201019203511364](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019203511364.png)

#### ����2

![image-20201019204509858](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019204509858.png)

#### ���2

![image-20201019204543090](https://gitee.com/InfiniteNightmare/album/raw/master/img/image-20201019204543090.png)