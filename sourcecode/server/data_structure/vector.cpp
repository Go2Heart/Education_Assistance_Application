#include "vector.h"

Vector<int> a;
int main(){
    a.push_back(3);
    printf("%d %d\n", a[0], a.size());
}