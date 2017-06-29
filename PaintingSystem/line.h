#ifndef LINE_H
#define LINE_H
#include "figure.h"
#include "paintpane.h"

class Line : public Shape
{
public:
    Common::lineAlgorithm linealgo;
    Line(QPoint s, QPoint e, Common::lineAlgorithm lineal)
    {
        type = Figure::Line;
        points.push_back(s);
        points.push_back(e);
        lastPoints.push_back(s);
        lastPoints.push_back(e);
        center.setX((s.x()+e.x())/2);
        center.setY((s.y()+e.y())/2);
        rotateAngle = 0;
        linealgo = lineal;
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

    int LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;
    int encode(QPoint p, QList<QPoint>borderPoints);
};

#endif // LINE_H
