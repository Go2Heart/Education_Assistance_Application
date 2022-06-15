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
    /*
    String(const String& str) {
        siz = str.siz;
        capacity = siz;
        buf = new char[capacity];
        memcpy(buf, str.buf, siz * sizeof(char));
    }*/
    String &operator = (const char* x) {
        siz = strlen(x) + 1;
        capacity = siz;
        buf = new char[capacity];
        memcpy(buf, x, siz * sizeof(char));
        return *this;
    }
    String operator + (const String &str) {
        String str_temp(*this);
        for(int i = 0; i < str.length(); i++) str_temp.push_back(str.buf[i]);
        return str_temp;
    }
    String operator + (const char x) {
    	String str_temp(*this);
        str_temp.push_back(x);
        return str_temp;
    }
    String operator + (const char* x) {
    	String str_temp(*this);
        for(int i = 0; i < strlen(x); i++) str_temp.push_back(x[i]);
        return str_temp;
    }
    friend bool operator < (String x, String y) {
        if(x.length() == 0) return true;
        if(y.length() == 0) return false;
        for(int i = 0; i < min(x.length(), y.length()); i++) {
            if(x[i] < y[i]) return true;
            if(x[i] > y[i]) return false;
        }
        return x.length() <= y.length();
    }
    friend String operator + (const char a, const String b) {
    	String tmp;
    	tmp.push_back(a);
        for(int i = 0; i < b.length(); i++) tmp.push_back(b[i]);
    	return tmp;
	}
    friend String operator + (const char* a, const String b) {
    	String tmp;
        for(int i = 0; i < strlen(a); i++) tmp.push_back(a[i]);
        for(int i = 0; i < b.length(); i++) tmp.push_back(b[i]);
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
    bool find(String str) const {
        const char* toFind = str.c_str();
        for(int i = 0; i < siz; i++) if(buf[i] == toFind[0]) {
            bool flag = true;
            for(int j = 0; j < str.siz; j++) if(buf[i + j] != toFind[j]) flag = false;
            if(flag) return true;
        }
        return false;
    }
};

extern String ToString(const int x);
extern String ToString_Time(const int x);

#endif