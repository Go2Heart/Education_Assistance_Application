#ifndef TIMEHEAP_H
#define TIMEHEAP_H

#include "basicClass.h"
#include <cstring>

class ClockTimer {
public:
    int originId;
    Timer t;
    ClockTimer() {}
    ClockTimer(Timer t, int originId) : t(t), originId(originId) {}
    bool operator < (const ClockTimer &tt) {
        return ( t.week < tt.t.week ||
            (t.week == tt.t.week && t.day < tt.t.day) ||
            (t.week == tt.t.week && t.day == tt.t.day && t.hour < tt.t.hour) ||
            (t.week == tt.t.week && t.day == tt.t.day && t.hour == tt.t.hour && t.minute < tt.t.minute));
    }
};

class TimeHeap {
private:
	ClockTimer* Obj = NULL;//小根堆存储数组
    int Pos[200005];
    int Sz = 0;//小根堆大小 
public:
    explicit TimeHeap() {}
    explicit TimeHeap(int N) {
        Obj = new ClockTimer[N];
        memset(Pos, -1, sizeof(Pos));
    }
    ~TimeHeap() {
        if(Obj) {
            delete [] Obj;
            Obj = NULL;
        }
    }
    void Init(int N) {
        if(Obj) {
            delete [] Obj;
            Obj = NULL;
        }
        Obj = new ClockTimer[N];
        memset(Pos, -1, sizeof(Pos));
    }
    void Push(ClockTimer x) {
        Obj[Sz] = x;
        Pos[x.originId] = Sz;
        int Cur = Sz;
        ++Sz;
        while(Cur > 0) {
            int Parent = (Cur - 1) / 2;
            if(Obj[Cur] < Obj[Parent]) {//若该节点权值小于父亲，将其与父亲交换，否则退出循环 
                ClockTimer Temp = Obj[Parent];
                Obj[Parent] = Obj[Cur];
                Obj[Cur] = Temp;
                Pos[Obj[Cur].originId] = Cur;
                Pos[Obj[Parent].originId] = Parent;
                Cur = Parent;
            } else break;
        }
    }
    bool Pop() {
        if(Sz < 0) return false;
        --Sz;
        Obj[0] = Obj[Sz];//将最后一个元素与堆根节点交换
        Pos[Obj[0].originId] = 0;
        int Cur = 0;
        while(Cur < Sz) {
            int Left = 2 * Cur + 1, Right = 2 * Cur + 2;
            if(Left >= Sz) break;
            int Min = Left;
            if(Right < Sz && Obj[Right] < Obj[Left]) Min = Right;
            if(Obj[Min] < Obj[Cur]) {//若左儿子和右儿子的较小值比当前值小，则交换，否则退出循环 
                ClockTimer Temp = Obj[Min];
                Obj[Min] = Obj[Cur];
                Obj[Cur] = Temp;
                Pos[Obj[Cur].originId] = Cur;
                Pos[Obj[Min].originId] = Min;
                Cur = Min;
            } else break;
        }
        return true;
    }
    bool Delete(int id) {
        if(Pos[id] == -1) return false;
        int Cur = Pos[id];
        Pos[id] = -1;
        Sz--;
        if(Obj[Cur] < Obj[Sz]) {
            Obj[Cur] = Obj[Sz];
            Pos[Obj[Cur].originId] = Cur;
            while(Cur < Sz) {
                int Left = 2 * Cur + 1, Right = 2 * Cur + 2;
                if(Left >= Sz) break;
                int Min = Left;
                if(Right < Sz && Obj[Right] < Obj[Left]) Min = Right;
                if(Obj[Min] < Obj[Cur]) {//若左儿子和右儿子的较小值比当前值小，则交换，否则退出循环 
                    ClockTimer Temp = Obj[Min];
                    Obj[Min] = Obj[Cur];
                    Obj[Cur] = Temp;
                    Pos[Obj[Cur].originId] = Cur;
                    Pos[Obj[Min].originId] = Min;
                    Cur = Min;
                } else break;
            }
        } else {
            Obj[Cur] = Obj[Sz];
            Pos[Obj[Cur].originId] = Cur;
            while(Cur > 0) {
                int Parent = (Cur - 1) / 2;
                if(Obj[Cur] < Obj[Parent]) {//若该节点权值小于父亲，将其与父亲交换，否则退出循环 
                    ClockTimer Temp = Obj[Parent];
                    Obj[Parent] = Obj[Cur];
                    Obj[Cur] = Temp;
                    Pos[Obj[Cur].originId] = Cur;
                    Pos[Obj[Parent].originId] = Parent;
                    Cur = Parent;
                } else break;
            }
        }
    }
    ClockTimer Top() { assert(Sz >= 0); return Obj[0]; }
    int Size() { return Sz; }
    void clear() {
        Sz = 0;
        memset(Pos, -1, sizeof(Pos));
    }
};

#endif