#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include "encoder.h"
#include "../data_structure/heap.h"
void Encoder::Init()//初始化数组 
{
	memset(Cnt, 0, sizeof(Cnt));
	memset(Son, 0, sizeof(Son));
	Last = LastLen = N = FileSize = 0;
}
void Encoder::Print(int x, int y)//缓冲区输出函数 Print(x,y) 表示在当前缓冲区存入一个值为x，长度为y位的二进制数（若x不足y位则高位补0） 
{
	if(y + LastLen < 8)//如果缓冲区内数长度仍未达到8 
	{
		Last = (Last << y) + x;//缓冲区内插入x 
		LastLen +=y;//缓冲区长度更新 
	}
	else 
	{//长度达到8 
		int TmpLen = y - (8 - LastLen);
		Last = (Last << 8 - LastLen) + (x >> TmpLen);//将x的前(y-8+LastLen)位与缓冲区拼在一起输出 
		if(OutputType == 0)fwrite(&Last, 1, 1, OutFile);
		else 
		{
			int Temp = Last;
			for(int i = 7 ; i >= 0; i--)fprintf(OutFile, "%d", (Temp >> i) % 2);
		}
		Last = x % (1 << TmpLen);//将x的剩余部分存入缓冲区 
		LastLen = TmpLen;//缓冲区长度更新 
	}
}
void Encoder::ForcePrint()//强制输出缓冲区内剩余字符（补全到8位）
{
	if(LastLen == 0) return;//缓冲区内没有字符，返回 
	Last <<= 8 - LastLen;//强制末尾补0并输出 
	if(OutputType == 0)fwrite(&Last, 1, 1, OutFile);
	else
	{
		int Temp = Last;
		for(int i = 7 ; i >= 0; i--)fprintf(OutFile, "%d", (Temp >> i) % 2);
	}
	Last = LastLen = 0;//清空缓冲区 
}
void Encoder::GetCode(int x)//构造每个字符对应的密文，并输出哈夫曼树结构 
{
	int HasSon = Son[x][0] != 0;
	Print(HasSon, 1);//输出当前节点是否有儿子
	if(HasSon)//如果有儿子，递归构造儿子的编码 
	{
		CodeLen[Son[x][0]] = CodeLen[x] + 1;
		Code[Son[x][0]] = Code[x] << 1;
		GetCode(Son[x][0]);
		CodeLen[Son[x][1]] = CodeLen[x] + 1;
		Code[Son[x][1]] = (Code[x] << 1) + 1;
		GetCode(Son[x][1]);
	}
}
void Encoder::PrintVal(int x)//按先序遍历输出每个叶子节点对应的明文
{
	int HasSon = Son[x][0] != 0;
	if(HasSon)//先序遍历 
	{
		PrintVal(Son[x][0]);
		PrintVal(Son[x][1]);
	} else
	{
		Print(x, 8);//若当前节点为叶子节点，输出该节点的明文(8 bit)
	}
}
bool Encoder::Encode(FILE *fin,FILE *fout, bool InTy, bool OuTy){//加密主程序
	OutFile = fout;
	OutputType = OuTy;
	Init();
	//以上均为初始化 
	while(!feof(fin))//读入加密文件 
	{
		unsigned char Cx;
		if(fread(&Cx, 1, 1, fin) > 0)
		{
			Cnt[Cx]++;
			FileSize++;
		} else break;
	}
	//输出明文长度
	if(OutputType == 0)//若输出文件类型为二进制，fwrite输出 
		fwrite(&FileSize, 8, 1, OutFile); 
	else//若输出文件类型为文本，fprintf输出 
	{
		long long Temp = FileSize;
		for(int i = 0; i < 8; i++)
		{
			for(int j = 7;j >= 0; j--)fprintf(OutFile, "%d", (Temp >> j) % 2);
			Temp >>= 8;
		}
	}		
	Heap<HuffmanNode> Hp;//小根堆 
	int NodeCount = 0;//统计叶子节点个数 
	for(int i = 0; i < 256; i++)
		if(Cnt[i] != 0)
		{
			Hp.Push(HuffmanNode(Cnt[i], i));//小根堆插入明文节点 
			NodeCount ++;
		}
	N = 256;
	while(Hp.Size() > 1)//小根堆根节点个数大于1个
	{
		HuffmanNode Tmp1, Tmp2;
		Tmp1 = Hp.Top(); Hp.Pop();
		Tmp2 = Hp.Top(); Hp.Pop();
		//从小根堆中取出两个出现次数权值和最小的两个节点
		Son[N][0] = Tmp1.id, Son[N][1] = Tmp2.id;//新建节点作为这两个节点的新根  
		Hp.Push(HuffmanNode(Tmp1.x + Tmp2.x, N));//将新根插入小根堆中 
		++N;
	}
	Code[N - 1] = CodeLen[N - 1] = 0;//根节点的编码为空 
	
	int TreeSize = (2 * NodeCount - 1) / 8 + 1;//统计哈夫曼树结构需要多少字节存储 
	if(OutputType == 0)//输出哈夫曼树结构所需字节 
		fwrite(&TreeSize, 1, 1, OutFile);
	else 
		for(int i = 7;i >= 0; i--)fprintf(OutFile, "%d", (TreeSize >> i) % 2);
	GetCode(N - 1);//输出哈夫曼树结构，得到每个字符对应密文 
	ForcePrint();//如果缓冲区内还有字符，强制补0输出 
	PrintVal(N - 1);//输出哈夫曼树叶子节点明文对应值（0~255） 
	fseek(fin, 0, SEEK_SET);//将文件读入指针重新指向文件开头 
	for(int i = 0; i < FileSize; i++)
	{
		unsigned char Cx;
		int Temp;
		if(InTy == 0)
		{
			fread(&Cx, 1, 1, fin);
			Temp = Cx;
		}
		else
		{
			fread(&Cx, 1, 1, fin);
			Temp = Cx;
		}
		if(CodeLen[Temp] > 8)//如果密文长度大于一个字节，分开进行输出 
		{
			Print(Code[Temp] >> 8, CodeLen[Temp] - 8);
			Print(Code[Temp] % (1 << 8), 8);
		} else Print(Code[Temp], CodeLen[Temp]);
	}
	ForcePrint();//强制补全末尾0 
	fclose(fin);
	fclose(fout);
	return true;
}
