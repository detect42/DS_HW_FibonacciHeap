#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include "graph.h"
#include "fibonacciheap.h"

class MyWidget : public QWidget {
    Q_OBJECT
public:
    MyWidget(QWidget *parent = nullptr);

    // 添加新的线段和圆
    void addLine(const Line& line);
    void addCircle(Fibnode<int> *node,double x,double y);

    // 清空线段和圆
    void clearGraphs();

    // 在 MyWidget 类中声明插入整数的函数
    void insertInteger();

    struct info{
        double x,y,length;
        info(int _x=0,int _y=0,int _length=0){
            x=_x;y=_y;length=_length;
        }
    };

    //插入节点
    void Insert(int val);
    void RandomInsert(int n,int lowbound=1,int upperbound=100);
    info dfs(Fibnode<int> * now,int beginx,int depth);
    //将Fibheap转换成graph展示
    void Transform();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void printGraphs();
    void insertValue();
    void deleteMin();
    void randominsert();
    void deletevalue();

private:

    std::vector<Line> m_lines;
    std::vector<Circle> m_circles;
    QPushButton *m_printButton;
    QPushButton *m_clearButton;
    QPushButton *m_insertButton;
    QPushButton *m_deleteminButton;
    QPushButton *m_randominsertButton;
    QPushButton *m_deletevalueButton;
public:
    FibHeap<int>*Fibheap;
};

#endif // MYWIDGET_H
