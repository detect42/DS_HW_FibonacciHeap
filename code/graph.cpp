// graph.cpp

#include "graph.h"

// 实现线段类的构造函数和成员函数
Line::Line(double x1, double y1, double x2, double y2)
    : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2) {}

double Line::getX1() const {
    return m_x1;
}

double Line::getY1() const {
    return m_y1;
}

double Line::getX2() const {
    return m_x2;
}

double Line::getY2() const {
    return m_y2;
}

// 实现圆类的构造函数和成员函数
Circle::Circle(double centerX, double centerY, double radius,bool mark=false,int val_=0,bool ismin_=false)
    : m_centerX(centerX), m_centerY(centerY), m_radius(radius),marked(mark),val(val_),ismin(ismin_){}

double Circle::getCenterX() const {
    return m_centerX;
}

double Circle::getCenterY() const {
    return m_centerY;
}

double Circle::getRadius() const {
    return m_radius;
}
