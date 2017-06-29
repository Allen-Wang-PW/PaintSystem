#ifndef CURVE_H
#define CURVE_H

#include "figure.h"
class Curve : public Shape {
public:
    Common::curveAlgorithm curvealgo;
    Curve(QList<QPoint> p, Common::curveAlgorithm curvealg) {
        int sum_x = 0;
        int sum_y = 0;
        int size = p.size();
        for (int i = 0; i < size; i++) {
            points.append(p[i]);
            lastPoints.append(p[i]);
            sum_x += p[i].x();
            sum_y += p[i].y();
        }
        center.setX(sum_x/size);
        center.setY(sum_y/size);
        type = Figure::Curve;
        curvealgo = curvealg;
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

#endif // CURVE_H
