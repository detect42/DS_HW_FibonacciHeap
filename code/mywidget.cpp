#include "mywidget.h"
#include <QPainter>
#include <QInputDialog>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent) {

    resize(1200,800);

    // 创建按钮
    m_printButton = new QPushButton("Print Graphs", this);
    m_printButton->setGeometry(10, 10, 120, 30);

    m_clearButton = new QPushButton("Clear Graphs", this);
    m_clearButton->setGeometry(140, 10, 120, 30);

    m_insertButton = new QPushButton("Insert Integer", this);
    m_insertButton->setGeometry(270, 10, 120, 30);

    m_deleteminButton = new QPushButton("Delete min", this);
    m_deleteminButton->setGeometry(400, 10, 120, 30);

    m_randominsertButton = new QPushButton("Random insert", this);
    m_randominsertButton->setGeometry(530, 10, 120, 30);

    m_deletevalueButton = new QPushButton("Delete value", this);
    m_deletevalueButton->setGeometry(660, 10, 120, 30);

   /* m_lines.push_back(Line(50, 50, 150, 150));
    m_lines.push_back(Line(200, 50, 300, 150));
    m_circles.push_back(Circle(100, 100, 50));
    m_circles.push_back(Circle(250, 100, 50));*/

    Fibheap = new FibHeap<int>();

    // 连接按钮的 clicked() 信号到槽函数
    connect(m_printButton, &QPushButton::clicked, this, &MyWidget::printGraphs);
    connect(m_clearButton, &QPushButton::clicked, this, &MyWidget::clearGraphs);
    connect(m_insertButton, &QPushButton::clicked, this, &MyWidget::insertValue);
    connect(m_deleteminButton, &QPushButton::clicked, this, &MyWidget::deleteMin);
    connect(m_randominsertButton, &QPushButton::clicked, this, &MyWidget::randominsert);
    connect(m_deletevalueButton, &QPushButton::clicked, this, &MyWidget::deletevalue);
}

void MyWidget::addLine(const Line& line) {
    double sum  = sqrt((line.getX1()-line.getX2())*(line.getX1()-line.getX2()) + (line.getY1()-line.getY2())*(line.getY1()-line.getY2()));
    double k = 22/sum;
    double dx = (line.getX1()-line.getX2())*k,dy=(line.getY1()-line.getY2())*k;
    Line newline=Line(line.getX1()-dx,line.getY1()-dy,line.getX2()+dx,line.getY2()+dy);
    m_lines.push_back(newline);
}

Circle trans(Fibnode<int> *node,int x,int y,bool ismin_){
    return Circle(x,y,20,node->marked,node->key,ismin_);
}

void MyWidget::addCircle(Fibnode<int> *node,double x,double y) {
    if(!node->key) return ;
    m_circles.push_back(trans(node,x,y,node==Fibheap->min));
}
void MyWidget::insertInteger() {
    bool ok;
    int value = QInputDialog::getInt(this, tr("Insert Integer"), tr("Enter an integer:"), 0, -1000, 1000, 1, &ok);
    if (ok) {
        Insert(value);
    }
}
void MyWidget::randominsert() {
    bool ok;
    int n = QInputDialog::getInt(this, tr("Insert Integer"), tr("Enter an integer:"), 0, -1000, 1000, 1, &ok);
    if (ok) {
        RandomInsert(n);
    }
    Transform();
    update();
}
void MyWidget::insertValue() {
    insertInteger();
    Transform();
    update();
}


void MyWidget::deleteMin(){
    Fibheap->Removemin();
    Transform();
    update();
}
void MyWidget::deletevalue() {
    bool ok;
    int val = QInputDialog::getInt(this, tr("Delete Integer"), tr("Enter an integer key:"), 0, -1000, 1000, 1, &ok);
    if (ok) {
        Fibheap->Remove(val);
    }
    Transform();
    update();
}



void MyWidget::clearGraphs() {
    m_lines.clear();
    m_circles.clear();
    Fibheap->Destroy();
    Fibheap = new FibHeap<int>;
    update();
}

void MyWidget::Insert(int val){
    Fibheap->Insert(val);
}
void MyWidget::RandomInsert(int n,int lowerbound,int upperbound){
    for(int i=1;i<=n;i++){
        int val=rand()%(upperbound-lowerbound+1)+lowerbound;
        Insert(val);
    }
}

MyWidget::info MyWidget::dfs(Fibnode<int>* now,int beginx,int depth){
    auto child = now ->child;
    int sum=beginx,interval=80;
    if(child==nullptr){
        addCircle(now,beginx,depth);return info(beginx,depth,0);
    }
    Fibnode<int>* start=child;
    vector<pair<double,double> > Linerecorder;
    do{
        Fibnode<int> * copy = child;
        info ans=dfs(copy,sum,depth+50);
        sum += ans.length;
        Linerecorder.push_back(make_pair(ans.x,ans.y));
        sum += interval;
        child = child ->right;


    }while(child!=start);

    double nowx=(double(sum+beginx-interval)/2),nowy=depth;

    for(auto it:Linerecorder){
        Line line=Line(nowx,nowy,it.first,it.second);
        if(nowy!=100) addLine(line);
    }
    Linerecorder.clear();

    addCircle(now,nowx,nowy);
    
    return info(nowx,nowy,sum-beginx-interval);

}

void MyWidget::Transform(){
    Fibnode<int>* root=new Fibnode<int>(0);
    root->child=Fibheap->min;root->right=root->left=root;
    try {
        Fibheap->Check(root);
    } catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        throw(std::runtime_error("Check error"));
    }
    m_lines.clear();
    m_circles.clear();
    root=new Fibnode<int>(0);
    root->child=Fibheap->min;root->right=root;
    dfs(root,100,100);
}

void MyWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    // 绘制线段
    painter.setPen(Qt::black);
    for (const auto& line : m_lines) {
        painter.drawLine(line.getX1(), line.getY1(), line.getX2(), line.getY2());
    }

    // 绘制圆
    painter.setPen(Qt::red);
    for (const auto& circle : m_circles) {
        if(circle.marked == false) painter.setPen(Qt::black);
        else  painter.setPen(Qt::red);
        if(circle.ismin) painter.setPen(Qt::blue);
        painter.drawEllipse(circle.getCenterX() - circle.getRadius(),
                            circle.getCenterY() - circle.getRadius(),
                            circle.getRadius() * 2, circle.getRadius() * 2);


        // 设置数字的颜色和字体
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 12));

        QString number = QString::number(circle.val);

        int textX = circle.getCenterX() - 12; // 数字左上角 x 坐标
        int textY = circle.getCenterY() - 6; // 数字左上角 y 坐标

        // 绘制数字
        painter.drawText(textX, textY, 20, 12, Qt::AlignCenter, number);
    }
}


void MyWidget::printGraphs() {

}
