#ifndef GRAPH_H
#define GRAPH_H

// 线段类
class Line {
public:
    Line(double x1, double y1, double x2, double y2);
    double getX1() const;
    double getY1() const;
    double getX2() const;
    double getY2() const;

private:
    double m_x1, m_y1, m_x2, m_y2;
};

// 圆类
class Circle {
public:
    Circle(double centerX, double centerY, double radius,bool mark,int val,bool ismin_);
    double getCenterX() const;
    double getCenterY() const;
    double getRadius() const;
    bool marked,ismin;
    int val;
private:
    double m_centerX, m_centerY, m_radius;
};

#endif // GRAPH_H
