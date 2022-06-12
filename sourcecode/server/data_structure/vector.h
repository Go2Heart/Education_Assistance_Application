#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstddef>
using namespace std;

template<class T>
class Vector {
protected:
    int siz;
    int capacity;
    T* buf;
    void newCapacity(int capa) {
        capacity = capa;
        T* newbuf = new T[capacity];
        if(buf) {
            for(int i = 0; i < siz; i++) newbuf[i] = buf[i];
            //memcpy(newbuf, buf, siz * sizeof(T));
            delete [] buf;
            buf = NULL;
        }
        buf = newbuf;
    }
public:
    Vector() : siz(0), capacity(0), buf(NULL) {}
    ~Vector() {
        if(buf) {
            delete[] buf;
            buf = NULL;
        }
        siz = capacity = 0;
    }
    Vector(const Vector<T>& mv) {
        siz = mv.siz;
        capacity = mv.capacity;
        buf = new T[capacity];
        //memcpy(buf, mv.buf, siz * sizeof(T));
        for(int i = 0; i < siz; i++) buf[i] = mv.buf[i];
    }

    Vector<T> &operator = (const Vector<T> &mv) {
        if(&mv != this) {
            siz = mv.siz;
            capacity = mv.capacity;
            if(buf) delete [] buf;
            buf = new T[capacity];
            for(int i = 0; i < siz; i++) buf[i] = mv.buf[i];
            //memcpy(buf, mv.buf, siz * sizeof(T));
        }
        return *this;
    }
    T& operator[](int idx) const {
        assert(siz > 0 && idx >= 0 && idx < siz);
        return buf[idx];
    }
    //void getVal
    void push_back(const T& t) {
        if(siz == capacity) {
            int capa = capacity * 2 + 1;
            newCapacity(capa);
        }
        buf[siz++] = t;
    }
    void clear() {
        siz = capacity = 0;
        if(buf) {
            delete [] buf;
            buf = NULL;
        }
    }
    void del(int idx) {
        T* newbuf = new T[capacity - 1];
        for(int i = 0; i < idx; i++) newbuf[i] = buf[i];
        for(int i = idx + 1; i < siz; i++) newbuf[i - 1] = buf[i];
        if(buf) delete [] buf;
        buf = newbuf;
        capacity--;
        siz--;
    }
    int size() const { return siz; }
    //int capacity() { return capacity; }
};

#endif