struct HeapNode {
	int Val,Id;
	bool operator < (const HeapNode &x) {
        return Val < x.Val;
    }
    HeapNode &operator =(const HeapNode &x) {
        Val = x.Val; Id = x.Id;
        return *this;
    }
};

class Heap {
private:
	HeapNode Obj[256];//小根堆存储数组 
    int Sz = 0;//小根堆大小 
public:
    bool Push(int x, int y);//插入一个权值为x，ID为y的数据 
    bool Pop(HeapNode &x);//将堆根节点弹出 
    int Size();//返回小根堆大小 
};