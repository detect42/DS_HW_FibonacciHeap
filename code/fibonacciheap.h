#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

#include<iomanip>
#include<iostream>
#include<cstdlib>
#include<cmath>
using namespace std;

template <class T>
class Fibnode{
public:
    T key;
    int degree;
    Fibnode<T> *left;
    Fibnode<T> *right;
    Fibnode<T> *child;
    Fibnode<T> *parent;
    bool marked;
    Fibnode(T value): key(value),degree(0),marked(false),left(this),right(this),child(nullptr),parent(nullptr){

    }

};


template <class T>
class FibHeap{
private:
    int keyNum;
    int maxDegree;

    Fibnode<T> **cons;
public:
    FibHeap();Fibnode<T> *min;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ~FibHeap();
    void Insert(T key);
    void Removemin();
    void Combine(FibHeap<T> *other);
    bool Minimum(T *pkey);
    void Update(T oldkey,T newkey);
    void Remove(T key);
    bool Contains(T key);
    void Print();
    void Destroy();
    void Check(Fibnode<T> * now);
private:
    void removeNode(Fibnode<T> *node);

    void addNode(Fibnode<T> *node,Fibnode<T> *root);

    void catList(Fibnode<T> *a,Fibnode<T> *b);

    void insert(Fibnode<T> *node);

    Fibnode<T>* extractMin();

    void link(Fibnode<T> *node,Fibnode<T> * root);

    void makeCons();

    void consolidate();

    void renewDegree(Fibnode<T> *parent,int degree);

    void cut(Fibnode<T> *node,Fibnode<T> *parent);

    void cascadingCut(Fibnode<T> *node);

    void decrease(Fibnode<T> *node,T key);

    void increase(Fibnode<T> *node, T key);

    void update(Fibnode<T> *node,T key);

    Fibnode<T>* search(T key);

    Fibnode<T>* search(Fibnode<T> *root,T key);

    void remove(Fibnode<T> *node);

    void  destroyNode(Fibnode<T> * node);

    void print(Fibnode<T> *node,Fibnode<T> *prev,int direction);
};

#include "fibonacciheap.tpp"
#endif // FIBONACCIHEAP_H
