#include "string.h"

String ToString(const int x) {
	char st[15];
	int top = 0, y = x;
	while(y) st[top++] = '0' + y % 10, y /= 10;
	String tmp;
	for(int i = top - 1; i >= 0; i--) tmp.push_back(st[i]);
	return tmp; 
}

String ToString_Time(const int x) {
    /*
     * x < 60, x >= 0
     * */
    String tmp;
    tmp.push_back(x / 10 + '0');
    tmp.push_back(x % 10 + '0');
    return tmp;
}
