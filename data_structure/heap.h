#ifndef HEAP_H
#define HEAP_H

#include <cstdio>
#include <cstddef>

template<class T>
class Heap {
private:
	T* Obj = NULL;//小根堆存储数组 
    int Sz = 0;//小根堆大小 
public:
    explicit Heap() {}
    explicit Heap(int N) {
        Obj = new T[N];
    }
    ~Heap() {
        if(Obj) {
            delete [] Obj;
            Obj = NULL;
        }
    }
    void Init(int N);
    bool Push(T x);//插入一个权值为x，ID为y的数据 
    bool Pop(T &x);//将堆根节点弹出 
    int Size();//返回小根堆大小 
};
#endif