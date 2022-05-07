#include "heap.h"

template<class T>
void Heap<T>::Init(int N) {
	if(Obj) {
		delete [] Obj;
		Obj = NULL;
	}
	Obj = new T[N];
}

template<class T>
bool Heap<T>::Push(T x) { // 插入数据元素
	Obj[Sz] = x;
	int Cur = Sz;
	++Sz;
	while(Cur > 0) {
		int Parent = (Cur - 1)/2;
		if(Obj[Cur] < Obj[Parent]) {//若该节点权值小于父亲，将其与父亲交换，否则退出循环 
			T Temp = Obj[Parent];
			Obj[Parent] = Obj[Cur];
			Obj[Cur] = Temp;
			Cur = Parent;
		} else break;
	}
	return true;
}

template<class T>
bool Heap<T>::Pop(T &x) {//弹出堆根节点
	if(Sz < 0) return false;
	x = Obj[0];
	--Sz;
	Obj[0] = Obj[Sz];//将最后一个元素与堆根节点交换 
	int Cur = 0;
	while(Cur < Sz) {
		int Left = 2 * Cur + 1, Right = 2 * Cur + 2;
		if(Left >= Sz) break;
		int Min = Left;
		if(Right < Sz && Obj[Right] < Obj[Left]) Min = Right;
		if(Obj[Min] < Obj[Cur]) {//若左儿子和右儿子的较小值比当前值小，则交换，否则退出循环 
			T Temp = Obj[Min];
			Obj[Min] = Obj[Cur];
			Obj[Cur] = Temp;
			Cur = Min;
		} else break;
	}
	return true;
}

template<class T>
int Heap<T>::Size() { return Sz; }//返回小根堆元素个数 