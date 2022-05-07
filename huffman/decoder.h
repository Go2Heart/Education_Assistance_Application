struct Decoder
{
	long long FileSize;//存储明文长度 
	int N, Son[512][2], LastLen, Root;
	unsigned char Last;
	//Last为输出缓冲区,LastLen为当前缓冲区已存储字符长度,Root为哈夫曼树根节点
	//Son[x][0/1]用于存储树结构 Son[x][0]表示x的左儿子,Son[x][1]表示x的右儿子 
	bool InputType, OutputType;//存储输入输出方式,为0表示输出为二进制文件，为1表示输出为文本文件
    FILE *InFile, *OutFile;//输入，输出文件指针
    unsigned char Code[512];//Code用于存储哈夫曼树叶子节点的明文
    void Init();//初始化 
	bool Read(int &x);//缓冲区读入函数 
    void ForceRead();//强制清空缓冲区内剩余字符（补全到8位）
    bool BuildTree(int &x);//根据得到的存储文件构建哈夫曼树 
    bool InsertVal(int x);//为哈夫曼树的叶子节点存入明文解码权值 
    bool Decoding(int x);//从根节点出发，解密一个字符 
    bool Decode(FILE *fin, FILE *fout, bool InTy, bool OuTy);//解码主程序 
};
