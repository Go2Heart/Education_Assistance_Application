#include "string.h"

String ToString(const int x) {
	if(x == 0) return String("0");
	char st[15];
	int top = 0, y = x;
	while(y) st[top++] = '0' + y % 10, y /= 10;
	String tmp;
	for(int i = top - 1; i >= 0; i--) tmp.push_back(st[i]);
	return tmp; 
}