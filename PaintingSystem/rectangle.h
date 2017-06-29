#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "figure.h"
#include "polygon.h"
class Rectangle : public Shape {
public :
    Rectangle(QPoint s, QPoint e)
    {
        type = Figure::Rectangle;
        points.push_back(s);
        points.push_back(QPoint(s.x(), e.y()));
        points.push_back(e);
        points.push_back(QPoint(e.x(), s.y()));
        points.push_back(s);
        for (int i = 0; i < points.size(); i++)
            lastPoints.push_back(points[i]);
        rotateAngle = 0;
        center.setX((s.x()+e.x())/2);
        center.setY((s.y()+e.y())/2);
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

    typedef enum {
        Left,
        Right,
        Bottom,
        Top
    } Boundary;

    int isCross(QPoint p1, QPoint p2, Boundary b, QList<QPoint> borderPoints);
    QPoint intersect(QPoint p1, QPoint p2, Rectangle::Boundary b, QList<QPoint> borderPoints);
    int edgeCliper(Boundary b, QList<QPoint> borderPoints, QPoint *pIn, int cnt, QPoint *pOut);
    int isInside(QPoint p, Boundary b, QList<QPoint> borderPoints);
};

#endif // RECTANGLE_H
