#ifndef STRING_H
#define STRING_H

#include "vector.h"

class String : public Vector<char> {
public:
    String(const char *str = NULL) : Vector() {
        if(str) {
            siz = strlen(str) + 1;
            capacity = siz;
            buf = new char[capacity];
            memcpy(buf, str, siz * sizeof(char));
        } else {
            Vector::push_back('\0');
        }
    }
    String(const char x) : Vector() {
        Vector::push_back(x);
        Vector::push_back('\0');
    }
    String &operator = (const char* x) {
        siz = strlen(x) + 1;
        capacity = siz;
        buf = new char[capacity];
        memcpy(buf, x, siz * sizeof(char));
        return *this;
    }
    String operator + (const String &str) {
        if(str.siz > 0) {
            buf[siz - 1] = str.buf[0]; 
            for(int i = 1; i < str.siz; i++) Vector::push_back(str.buf[i]);
        }
        return *this;
    }
    String operator + (const char x) {
    	String str_temp(*this);
        str_temp.buf[siz - 1] = x;
        str_temp.Vector::push_back('\0');
        return str_temp;
    }
    String operator + (const char* x) {
    	String str_temp(*this);
        if(strlen(x) > 0) {
            str_temp.buf[siz - 1] = x[0];
            for(int i = 1; i < strlen(x); i++) str_temp.Vector::push_back(x[i]);
            str_temp.Vector::push_back('\0');
        }
        return str_temp;
    }
    friend String operator + (const char a, const String b) {
    	String tmp;
    	tmp.siz = tmp.capacity = b.siz + 1;
    	char* newbuf = new char[tmp.siz];
    	newbuf[0] = a;
    	memcpy(newbuf + 1, b.buf, b.siz * sizeof(char));
    	tmp.buf = newbuf;
    	return tmp;
	}
    friend String operator + (const char* a, const String b) {
    	String tmp;
    	tmp.siz = tmp.capacity = strlen(a) + b.siz;
    	char* newbuf = new char[tmp.siz];
    	memcpy(newbuf, a, strlen(a) * sizeof(char));
    	memcpy(newbuf + strlen(a), b.buf, b.siz * sizeof(char));
		tmp.buf = newbuf;
    	return tmp;
	}
    bool operator == (const String &str) const {
        if(siz != str.siz) return false;
        for(int i = 0; i < siz; i++) if(buf[i] != str.buf[i]) return false;
        return true;
    }
    void clear() {
    	Vector::clear();
        Vector::push_back('\0');
    }
    void push_back(const char& t) {
    	buf[siz - 1] = t;
    	Vector::push_back('\0');
    }
    int length() const { return siz - 1; }
    const char* c_str() const { return buf; }
};

extern String ToString(const int x);

#endif