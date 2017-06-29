#ifndef OVAL_H
#define OVAL_H

#include "figure.h"
class Oval : public Shape
{
public:
    QList<QPoint> showPoints;
    int a, lastA;
    int b, lastB;
    Mode mode;
    Oval(QPoint c, int aa, int bb)
    {
        center = c;
        lastC = c;
        a = aa;
        lastA = aa;
        b = bb;
        lastB = bb;
        showPoints.push_back(QPoint(c.x() + aa, c.y()));
        showPoints.push_back(QPoint(c.x() - aa, c.y()));
        showPoints.push_back(QPoint(c.x(), c.y() + bb));
        showPoints.push_back(QPoint(c.x(), c.y() - bb));
//        rotateAngle = 0;
        type = Figure::Oval;
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

#endif // OVAL_H
