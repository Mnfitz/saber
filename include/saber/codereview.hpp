#ifndef SABER_CODEREVIEW_HPP
#define SABER_CODEREVIEW_HPP

#include <iostream>
#include <string.h>
#include <vector>
#include <stdlib.h>

using namespace std;

int globalCounter = 0;

#define MAX 1024
#define min(a,b) a<b?a:b

class UndefinedType;

class Widget {
public:
	virtual void PrintWidget(Widget widget) = 0;

public:
    char* m_name;
    int m_size;
    bool m_flag;

    Widget(const char* n, int s) {
        m_name = (char*)malloc(strlen(n));
        strcpy(m_name, n);

        m_size = s;
        m_flag = rand() % 2;
        globalCounter++;
    }

    Widget(const Widget& other) {
        m_name = other.m_name;
        m_size = other.m_size;
        m_flag = other.m_flag;
    }

    Widget& operator=(const Widget& other) {
        m_name = other.m_name;
        m_size = other.m_size;
        m_flag = other.m_flag;
        return *this;
    }

    ~Widget() {
        delete m_name;
	}

    void print() {
        cout << "Widget: " << m_name << " size=" << m_size << endl;
    }

    char* getName() {
        return m_name;
    }

    int compute(int x) {
        return x * rand();
    }
}; // class Widget

int DoStuff(Widget* w) {
    return w->compute(w->m_size) / MAX;
}

#endif // SABER_CODEREVIEW_HPP
