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
		scanf("%d", &ty);//���봦�����ͣ�0��ʾ���ܣ�1��ʾ���ܣ�--1��ʾ�˳����� 
		printf("please announce type and name of the file: 0(binary)/1(txt) filename\n");
		printf("input file:");
		scanf("%d%s", &Ty1, s);//����һ��������һ���ַ�������ʾ�����ļ������ͺ��ļ���������Ϊ0��ʾΪ�������ļ�������Ϊ1��ʾΪ�ı��ļ� 
		printf("output file:");
		scanf("%d%s", &Ty2, t);//����һ��������һ���ַ�������ʾ����ļ������ͺ��ļ���������Ϊ0��ʾΪ�������ļ�������Ϊ1��ʾΪ�ı��ļ� 
		FILE *fin, *fout;
		if(ty < 0 || ty > 1 || Ty1 < 0 || Ty1 > 1 || Ty2 < 0 || Ty2 > 1)//��������Ƿ� 
		{
			printf("invalid input! please read the information above carefully!");
			break;
		}
		if(ty == 0)//���� 
		{	
			if(Ty1) fin = fopen(s, "r");
			else fin = fopen(s, "rb");
			if(Ty2) fout = fopen(t, "w");
			else fout = fopen(t,"wb");
			Encoder EncodeSolver;
			if(!EncodeSolver.Encode(fin, fout, Ty1, Ty2))//����ʧ�ܣ�������ִ��� 
			{
				fclose(fin);
				fclose(fout);
				fprintf(stderr, "ERROR!\n");
			}
		} else if(ty == 1)//���� 
		{
			if(Ty1) fin = fopen(s, "r");
			else fin = fopen(s, "rb");
			if(Ty2) fout = fopen(t, "w");
			else fout = fopen(t,"wb");
			Decoder DecodeSolver;
			if(!DecodeSolver.Decode(fin, fout, Ty1, Ty2))
			{//����ʧ�ܣ�Ϊ�Ƿ����� 
				fclose(fin);
				fclose(fout);
				fprintf(stderr, "INVALID INPUT!\n");
				if(Ty2) fout = fopen(t, "w");
				else fout = fopen(t,"wb");
				fprintf(fout, "INVALID");
				fclose(fin);
				fclose(fout);
			}
		} else if (ty == -1) break;//�˳�
	}
}
