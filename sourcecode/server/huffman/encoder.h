#ifndef ENCODER_H
#define ENCODER_H
struct HuffmanNode {
    int x, id;
    HuffmanNode() {}
    HuffmanNode(int x,int id) : x(x), id(id) {}
    friend bool operator < (HuffmanNode x, HuffmanNode y) {
        return x.x < y.x;
    }
};
struct Encoder {
	int N, Cnt[256], Son[512][2], Last, LastLen;
	//Last为输出缓冲区,LastLen为当前缓冲区已存储字符长度 
	//Cnt为每个字符出现次数,Son[x][0/1]用于存储树结构 Son[x][0]表示x的左儿子,Son[x][1]表示x的右儿子 
	long long FileSize;//存储明文长度 
	bool OutputType;//存储输出方式,OutputType为0表示输出为二进制文件，为1表示输出为文本文件 
    FILE *OutFile;//输出文件指针 
    int Code[512], CodeLen[512];//Code用于存储哈夫曼树每个节点的密文（将01串压缩进int内） CodeLen用于存储哈夫曼树每个节点的密文长度 
    void Init();//初始化数组 
    void Print(int x, int y);//缓冲区输出函数 
    void ForcePrint();//强制输出缓冲区内剩余字符（补全到8位） 
    void GetCode(int x);//构造每个字符对应的密文，并输出哈夫曼树结构 
    void PrintVal(int x);//按先序遍历输出每个叶子节点对应的明文 
    bool Encode(FILE *fin, FILE *fout, bool InTy, bool OuTy);//加密主程序 
};

#endif