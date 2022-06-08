#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "encoder.h"
#include "decoder.h"
#include "heap.h"
using namespace std;
int main()
{
	while(1) 
	{
		int ty, Ty1, Ty2;char s[105], t[105];
		printf("please input mode: 0(encode) 1(decode) -1(exit):");
		scanf("%d", &ty);//读入处理类型，0表示加密，1表示解密，--1表示退出程序 
		printf("please announce type and name of the file: 0(binary)/1(txt) filename\n");
		printf("input file:");
		scanf("%d%s", &Ty1, s);//读入一个整数和一个字符串，表示读入文件的类型和文件名，类型为0表示为二进制文件，类型为1表示为文本文件 
		printf("output file:");
		scanf("%d%s", &Ty2, t);//读入一个整数和一个字符串，表示输出文件的类型和文件名，类型为0表示为二进制文件，类型为1表示为文本文件 
		FILE *fin, *fout;
		if(ty < 0 || ty > 1 || Ty1 < 0 || Ty1 > 1 || Ty2 < 0 || Ty2 > 1)//输入参数非法 
		{
			printf("invalid input! please read the information above carefully!");
			break;
		}
		if(ty == 0)//加密 
		{	
			if(Ty1) fin = fopen(s, "r");
			else fin = fopen(s, "rb");
			if(Ty2) fout = fopen(t, "w");
			else fout = fopen(t,"wb");
			Encoder EncodeSolver;
			if(!EncodeSolver.Encode(fin, fout, Ty1, Ty2))//加密失败，程序出现错误 
			{
				fclose(fin);
				fclose(fout);
				fprintf(stderr, "ERROR!\n");
			}
		} else if(ty == 1)//解密 
		{
			if(Ty1) fin = fopen(s, "r");
			else fin = fopen(s, "rb");
			if(Ty2) fout = fopen(t, "w");
			else fout = fopen(t,"wb");
			Decoder DecodeSolver;
			if(!DecodeSolver.Decode(fin, fout, Ty1, Ty2))
			{//解密失败，为非法输入 
				fclose(fin);
				fclose(fout);
				fprintf(stderr, "INVALID INPUT!\n");
				if(Ty2) fout = fopen(t, "w");
				else fout = fopen(t,"wb");
				fprintf(fout, "INVALID");
				fclose(fin);
				fclose(fout);
			}
		} else if (ty == -1) break;//退出
	}
}
