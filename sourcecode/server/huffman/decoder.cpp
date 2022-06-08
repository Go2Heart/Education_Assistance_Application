#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include "decoder.h"
#include "../data_structure/heap.h"
void Decoder::Init()//初始化 
{
	memset(Son, 0, sizeof(Son));
	LastLen = Last = N = 0;
}
bool Decoder::Read(int &x)//缓冲区读入函数 
{
	if(LastLen == 0)//若缓冲区内没有比特，则读入一个字节的数据 
	{
		if(InputType == 1)
		{
			unsigned char Temp; Last = 0;
			for(int i = 0; i < 8; i++)
			{
				if(feof(InFile)) return false;//若读入到EOF，输出文件不合法，返回false 
				if(fscanf(InFile, "%c", &Temp) <= 0)return false;
				Last = (Last << 1) + Temp - '0';
			}
		}
		else
		{
			if(feof(InFile)) return false;
			if(fread(&Last, 1, 1, InFile) <= 0)return false;
		}
		LastLen = 7;
	}else LastLen--;
	x = (Last >> LastLen) % 2;//取出缓冲区最后一个字节的数据 
	return true;
}
void Decoder::ForceRead()//强制清空缓冲区 
{
	Last = LastLen = 0;
}
bool Decoder::BuildTree(int &x)//根据得到的存储文件构建哈夫曼树 
{
	x = N; ++N; 
	int HasSon;
	if(!Read(HasSon))//如果读入失败返回false 
		return false;
	if(HasSon)//如果当前节点右儿子递归构造左子树 
	{
		if(!BuildTree(Son[x][0]))
			return false;
		if(!BuildTree(Son[x][1]))
			return false;
	}
	return true;
}
bool Decoder::InsertVal(int x)//为哈夫曼树的叶子节点存入明文解码权值
{
	int HasSon = Son[x][0] != 0;
	if(HasSon)
	{
		if(!InsertVal(Son[x][0]))
			return false;
		if(!InsertVal(Son[x][1]))
			return false;
	} else
	{//如果当前节点为叶子节点，则读入一个字节，表示这个节点的明文 
		unsigned char Temp; 
		if(InputType == 1)
		{
			unsigned char Tmp;
			for(int i = 0; i < 8; i++)
			{
				if(feof(InFile)) return false;
				fscanf(InFile, "%c", &Tmp);
				Temp = (Temp << 1) + Tmp - '0';
			}
			if(feof(InFile)) return false;
		}
		else
		{
			if(feof(InFile)) return false;
			if(fread(&Temp, 1, 1, InFile));
		}
		Code[x] = Temp;
	}
	return true;
}
bool Decoder::Decoding(int x)//从根节点出发，解密一个字符
{
	if(Son[x][0] == 0)//如果当前节点为叶子节点，解码成功，输出该叶子节点表示的明文 
	{
		if(OutputType == 0)
			fwrite(&Code[x], 1, 1, OutFile);
		else
			fprintf(OutFile, "%c", Code[x]);
		return true;
	}
	int Temp;
	if(!Read(Temp))//根据密文为0/1决定往左儿子还是往右儿子走 
		return false;
	bool Flag = true;
	if(Temp == 0) Flag &= Decoding(Son[x][0]);
	else Flag &= Decoding(Son[x][1]);
	return Flag;
}
bool Decoder::Decode(FILE *fin,FILE *fout, bool InTy, bool OuTy){//解码主程序 
	InFile = fin; OutFile = fout;
	InputType = InTy; OutputType = OuTy;
	Init();
	//以上均为初始化 
	if(InputType == 1)//若输入文件为文本文件 
	{
		FileSize = 0;
		long long bit = 1;
		unsigned char Temp;
		for(int i = 0; i < 8; i++)//读入8字节大小的明文长度FileSize 
		{
			int Nownum = 0;
			for(int j = 0; j < 8; j++)
			{
				if(feof(InFile)) return false;
				fscanf(fin, "%c", &Temp);
				Nownum = (Nownum << 1) + Temp - '0';
			}
			if(feof(InFile)) return false;
			FileSize += bit * Nownum;
			bit = bit * 256;
		}
		for(int i = 0; i < 8; i++)
		{
			if(feof(InFile)) return false;
			fscanf(fin, "%c", &Temp);
		}
		if(feof(InFile)) return false;
	} else //若输入文件为二进制文件 
	{
		if(feof(InFile)) return false;
		fread(&FileSize, 8, 1, fin);
		unsigned char Temp;
		if(feof(InFile)) return false;
		fread(&Temp, 1, 1,fin);
	}
	if(!BuildTree(Root))//构建哈夫曼树 
		return false;
	ForceRead();
	if(!InsertVal(Root))//为哈夫曼树叶子结点插入权值 
		return false;
	for(int i = 0; i < FileSize; i++)//解码FileSize个字符 
	{
		if(!Decoding(Root)) 
			return false;
	}
	for(int i = 0; i < LastLen; i++)//若剩余字符中仍有1，即不为补0部分，输入非法 
		if((Last >> i) % 2)
			return false;
	unsigned char test;
	if(InputType == 1)
		fscanf(InFile, "%c", &test);
	else fread(&test, 1, 1, fin);
	if(test != 0) return false;//若仍有可读入字节，输入非法 
	fclose(fin);
	fclose(fout);
	return true;
}
