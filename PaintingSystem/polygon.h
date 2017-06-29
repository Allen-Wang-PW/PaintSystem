#ifndef POLYGON_H
#define POLYGON_H

#include "figure.h"
class Polygon : public Shape {
public :
    Polygon(QList<QPoint> p, bool isDone) {
        int size = p.size();
        if (isDone)
            p[size-1] = p[0];
        int sum_x = 0;
        int sum_y = 0;
        for (int i = 0; i < size; i++) {
            points.push_back(p[i]);
            lastPoints.push_back(p[i]);
            sum_x += p[i].x();
            sum_y += p[i].y();
        }
        center.setX(sum_x/size);
        center.setY(sum_y/size);
        type = Figure::Polygon;
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

    int isInside(QPoint p, Boundary b, QList<QPoint> borderPoints);
    int isCross(QPoint p1, QPoint p2, Boundary b, QList<QPoint> borderPoints);
    QPoint intersect(QPoint p1, QPoint p2, Boundary b, QList<QPoint> borderPoints);
    //int edgeCliper(Boundary b, QList<QPoint> borderPoints, QList<QPoint> &pIn, int cnt, QList<QPoint> &pOut);
    int edgeCliper(Boundary b, QList<QPoint> borderPoints, QPoint *pIn, int cnt, QPoint *pOut);

};

#endif // POLYGON_H
