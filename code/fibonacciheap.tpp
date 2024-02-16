#include "fibonacciheap.h"

using namespace std;


/* 构造函数 */

template <class T>
FibHeap<T>::FibHeap(){
    keyNum = 0;
    maxDegree = 0;
    min = nullptr;
    cons = nullptr;
}

/* 析构函数 */
template <class T>
FibHeap<T>::~FibHeap(){

}

/* 将node从双链表中移除 */
template <class T>
void FibHeap<T>::removeNode(Fibnode<T> *node){
    node->left->right = node->right;
    node->right->left = node->left;
}

/* 将node堆结点加入root结点之前(循环链表中) */
template <class T>
void FibHeap<T>::addNode(Fibnode<T> *node,Fibnode<T> *root){
    node->left = root->left;
    root->left->right = node;
    node->right = root;
    root->left = node;
}

/* 将节点node插入到斐波那契堆中 */
template <class T>
void FibHeap<T>::insert(Fibnode<T> *node){
    if(keyNum == 0){
        min = node;
        node->left = node;
        node->right = node;
    }
    else{
        addNode(node,min);
        if(node->key < min->key){
            min = node;
        }
    }
    keyNum++;
}

/*新建价值key节点，并插入堆中 */
template <class T>
void FibHeap<T>::Insert(T key){
    Fibnode<T> *node;
    node = new Fibnode<T>(key);
    if(node == nullptr){
        return;
    }
    insert(node);
}

/* 双链表b连接到双链表a的后面 ???????*/
template <class T>
void FibHeap<T>::catList(Fibnode<T> *a,Fibnode<T> *b){
    Fibnode<T> *tmp;

    tmp = a->right;
    a->right = b->right;
    b->right->left = a;
    b->right = tmp;
    tmp->left = b;
}

/* 将other合并到当前堆中 */
template <class T>
void FibHeap<T>::Combine(FibHeap<T> *other){
    if(other == nullptr)
        return;

    if(other->maxDegree > this -> maxDegree){
        swap(*this,*other);
    }

    if((this->min) == nullptr){
        this->min = other->min;
        this->keyNum = other->keyNum;
        free(other->cons);
        delete other;
    }
    else if((other->min)==nullptr){
        free(other->cons);
        delete other;
    }
    else{
        catList(this->min,other->min);
        if(this->min->key > other->min->key){
            this->min = other->min;
        }
        this->keyNum += other->keyNum;
        free(other->cons);
        delete other;
    }
}

/* 将根的最小节点从根链表中移除 */
template <class T>
Fibnode<T>* FibHeap<T>::extractMin(){
    Fibnode<T> *p =min;
    if(p==p->right)
        min =nullptr;
    else{
        removeNode(p);
        min = p->right;
    }
    p->left = p->right = p;
    return p;
}

/* 将node链接到root根结点 */
template <class T>
void FibHeap<T>::link(Fibnode<T> *node,Fibnode<T> *root){
    removeNode(node);
    if(root->child==nullptr)
        root->child = node;
    else
        addNode(node,root->child);
    node->parent = root;
    root->degree++;
    node->marked = false;
}

/*创建consolidate所需空间*/
template <class T>
void FibHeap<T>::makeCons() {
    int old = maxDegree;
    maxDegree=(log(keyNum)/log(2))+1;
    if(old>=maxDegree) return;
    cons = (Fibnode<T> **)realloc(cons,sizeof(FibHeap<T> *)*(maxDegree+1));
}

/*合并堆链表中左右度数相同的树*/
template<class T>
void FibHeap<T>::consolidate() {
    int i,d,D;
    Fibnode<T> *x,*y,*tmp;

    makeCons();
    D=maxDegree+1;

    for(i=0;i<D;i++)
        cons[i]=nullptr;

    while(min!=nullptr){
        x=extractMin();
        d=x->degree;
        while(cons[d]!=nullptr){
            y=cons[d];
            if(x->key>y->key)
                swap(x,y);
            link(y,x);
            cons[d]=nullptr;
            d++;
        }
        cons[d]=x;
    }
    min = nullptr;
    for(i=0;i<D;i++){
        if(cons[i]!=nullptr){
            if(min==nullptr)
                min=cons[i];
            else{
                addNode(cons[i],min);
                if(cons[i]->key<min->key)
                    min=cons[i];
            }
        }
    }
}

/*移除最小节点*/
template<class T>
void FibHeap<T>::Removemin(){
    if(min==nullptr) return;
    Fibnode<T> *child = nullptr;
    Fibnode<T> *m = min;
    while(m->child!=nullptr){
        child = m->child;
        removeNode(child);
        if(child->right==child)
            m->child=nullptr;
        else
            m->child=child->right;
        addNode(child,min);
        child->parent=nullptr;
        removeNode(m);
    }
    removeNode(m);
    if(m->right==m)
        min=nullptr;
    else{
        min=m->right;
        consolidate();
    }
    keyNum--;
    delete m;
}

/*返回最小节点的键值*/
template<class T>
bool FibHeap<T>::Minimum(T *pkey){
    if(min==nullptr) return false;
    *pkey=min->key;
    return true;
}


/*修改度数*/
//?????????????????????????????????/
template<class T>
void FibHeap<T>::renewDegree(Fibnode<T> *parent, int degree) {
    cout<<"!!!!"<<parent->key<<" -- "<<parent->degree<<" - "<<degree<<endl;
    parent->degree -= degree;//??????/
    if(parent->parent!=nullptr)
        renewDegree(parent->parent,degree);
}

/* 把node从parent中剥离出来，放到根链表上去*/
template<class T>
void FibHeap<T>::cut(Fibnode<T> *node,Fibnode<T> *parent) {
    removeNode(node);
    //renewDegree(parent,node->degree);
    parent->degree--;
    if(node==node->right)
        parent->child=nullptr;
    else
        parent->child=node->right;
    node->parent=nullptr;
    node->left=node->right=node;
    node->marked=false;
    addNode(node,min);
}

/*对节点node进行"级联剪切"*/
template<class T>
void FibHeap<T>::cascadingCut(Fibnode<T> *node) {
    Fibnode<T> *parent=node->parent;
    if(parent==nullptr) return;
    if(node->marked==false)
        node->marked=true;
    else{
        cut(node,parent);
        cascadingCut(parent);
    }
}

/*将键值减少到key*/
template<class T>
void FibHeap<T>::decrease(Fibnode<T> *node, T key){
    Fibnode<T> *parent;
    if(min==nullptr||node==nullptr) return;
    if(key>node->key){
        throw std::runtime_error("decrease key is error");
        return;
    }
    node->key =key;
    parent=node->parent;
    if(parent!=nullptr&&node->key<parent->key){
        cut(node,parent);
        cascadingCut(parent);
    }
    if(node->key<min->key)
        min=node;
}

/*将节点node的值增加到key*/
template<class T>
void FibHeap<T>::increase(Fibnode<T> *node, T key) {
    Fibnode<T> *child,*parent,*right;
    if(min==nullptr||node==nullptr) return;
    if(key<node->key){
        throw std::runtime_error("increase key is error");
        return;
    }
    while(node->child!=nullptr){
        child=node->child;
        removeNode(child);
        if(child->right==child)
            node->child=nullptr;
        else
            node->child=child->right;
        addNode(child,min);
        child->parent=nullptr;
    }
    node->degree=0;
    node->key=key;

    parent=node->parent;
    if(parent!=nullptr){
        cut(node,parent);
        cascadingCut(parent);
    }
    else if(min==node){
        right=node->right;
        while(right!=node){
            if(node->key>right->key&&right->key<min->key) //!!!!!!!!!!!!!!!!!!!
                min=right;
            right=right->right;
        }
    }
}

/*更新节点node的键值为key*/
template<class T>
void FibHeap<T>::update(Fibnode<T> *node, T key) {
    if(node->key<key)
        increase(node,key);
    else if(node->key>key)
        decrease(node,key);
}

template<class T>
void FibHeap<T>::Update(T oldkey,T newkey){
    Fibnode<T> *node;
    node=search(oldkey);
    if(node!=nullptr) update(node,newkey);
}

/*在最小堆中查找键值为key的节点*/
template<class T>
Fibnode<T>* FibHeap<T>::search(Fibnode<T>* root,T key) {
    Fibnode<T> *t=root;
    Fibnode<T> *p=nullptr;

    if(root==nullptr) return root;
    do {
        if (t->key == key) {
            p = t;
            break;
        } else {
            if (t->child != nullptr)
                p = search(t->child, key);
            if (p != nullptr) break;
        }
        t = t->right;
    }while(t!=root);

    return p;
}

template<class T>
Fibnode<T>* FibHeap<T>::search(T key) {
    if(min==nullptr) return nullptr;
    return search(min,key);
}

/*查找是否存在键值为key的节点*/
template<class T>
bool FibHeap<T>::Contains(T key) {
    return search(key)!=nullptr;
}

/*删除节点*/
template<class T>
void FibHeap<T>::remove(Fibnode<T> *node) {
    T m;
    if(node==nullptr) return;
    m=min->key-1;
    decrease(node,m);
    Removemin();
}

template<class T>
void FibHeap<T>::Remove(T key) {
    Fibnode<T> *node;
    if(min==nullptr) return;
    node=search(key);
    if(node!=nullptr) remove(node);
}

/*销毁斐波那契堆*/
template<class T>
void FibHeap<T>::destroyNode(Fibnode<T> *node) {
    Fibnode<T> *start=node;
    Fibnode<T> *p=nullptr;
    if(node==nullptr) return;
    do{
        destroyNode(node->child);
        p=node;
        node=node->right;
        delete p;
    }while(node!=start);
}

template<class T>
void FibHeap<T>::Destroy() {
    destroyNode(min);
    free(cons);
}


template <class T>
void FibHeap<T>::print(Fibnode<T> *node, Fibnode<T> *prev, int direction)
{
    Fibnode<T> *start=node;

    if (node==nullptr)
        return ;
    do
    {
        if (direction == 1)
            cout << setw(8) << node->key << "(" << node->degree << ") is "<< setw(2) << prev->key << "'s child" << endl;
        else
            cout << setw(8) << node->key << "(" << node->degree << ") is "<< setw(2) << prev->key << "'s next" << endl;

        if (node->child != nullptr)
            print(node->child, node, 1);

        // 兄弟节点
        prev = node;
        node = node->right;
        direction = 2;
    } while(node != start);
}

template <class T>
void FibHeap<T>::Check(Fibnode<T> * now){
    auto child = now ->child;
    if(child==nullptr){
        return;
    }
    Fibnode<int>* start=child;

    do{
        if(child->key<now->key)
            throw std::runtime_error("Check error");
        Check(child);
        child = child ->right;
        if(child!=child->left->right||child!=child->right->left) throw std::runtime_error("Check error");
    }while(child!=start);
}

template <class T>
void FibHeap<T>::Print()
{
    int i=0;
    Fibnode<T> *p;

    if (min==nullptr)
        return ;

    cout << "== 斐波那契堆的详细信息: ==" << endl;
    p = min;cout<<p->key<<endl;
    do {
        i++;
        cout << setw(2) << i << ". " << setw(4) << p->key << "(" << p->degree << ") is root" << endl;

        print(p->child, p, 1);
        p = p->right;
    } while (p != min);
    cout << endl;
}
