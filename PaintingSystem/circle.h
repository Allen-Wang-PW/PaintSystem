#ifndef CIRCLE_H
#define CIRCLE_H

#include "figure.h"
class Circle : public Shape
{
public:
    int r;
    int lastR;
    Circle(QPoint c, int rr)
    {
        center = c;
        lastC = c;
        r = rr;
        lastR = rr;
        type = Figure::Circle;
    }
    int distance(QPoint cur) override;
    QPoint getCenter() override;
    void move(int dx, int dy) override;
    void rotate(double angle) override;
    void scale(QPoint choosen, QPoint newPoint) override;
    void zoom(ZoomMode mode) override;
    QList<Shape*> trim(QList<QPoint>) override;
    QList<QPoint> getPoints() override;
    void reset() override;
};

#endif // CIRCLE_H
