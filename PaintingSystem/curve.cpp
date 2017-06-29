#include "curve.h"

int Curve::distance(QPoint cur) {
    int size = points.size();
    int nCross = 0;
    for (int i = 0; i < size; i++) {
        QPoint p1 = points[i];
        QPoint p2 = points[(i+1)%size];
        if (p1.y() == p2.y())
            continue;
        if (cur.y() < fmin(p1.y(), p2.y()))
            continue;
        if (cur.y() >= fmax(p1.y(), p2.y()))
            continue;
        double x = (double)(cur.y() - p1.y()) * (double)(p2.x() - p1.x()) / (double)(p2.y() - p1.y()) + p1.x();
        if (x > cur.x())
            nCross++;
    }
    if (nCross % 2 == 1)
        return 0;
    for (int i = 0; i < size; i++)
        if (Common::compute2PointDis(cur, points[i]) < 5)
            return 0;
    return 100;
}

QPoint Curve::getCenter() {
    return center;
}

void Curve::move(int dx, int dy) {
    for (int i = 0; i < points.size(); i++) {
        points[i].setX(lastPoints[i].x() + dx);
        points[i].setY(lastPoints[i].y() + dy);
    }
}

void Curve::rotate(double angle) {
    rotateAngle += angle;
    double cs = cos(rotateAngle);
    double sn = sin(rotateAngle);

    int size = points.size();
    for (int i = 0; i < size; i++) {
        int x = lastPoints[i].x() - center.x();
        int y = lastPoints[i].y() - center.y();
        points[i].setX((int)(x*cs-y*sn+center.x()));
        points[i].setY((int)(x*sn+y*cs+center.y()));
    }
}

void Curve::scale(QPoint choosen, QPoint newPoint) {
    int size = points.size();
    int dis[size];
    for (int i = 0; i < size - 1; i++)
        dis[i] = (int)Common::compute2PointDis(lastPoints[i], choosen);
    int min_idx = 0;
    for (int i = 1; i < size - 1; i++)
        if (dis[i] < dis[min_idx] && dis[i] < 5)
            min_idx = i;
    if (min_idx == 0) {
        if (dis[0] < 5) {
            points[0] = newPoint;
            points[size-1] = newPoint;
        }
    }
    else
        points[min_idx] = newPoint;
}

void Curve::zoom(Shape::ZoomMode mode) {
    double S = (mode == Zin) ? S_in : S_out;
    for (int i = 0; i < points.size(); i++) {
        points[i].setX(Common::round(center.x() + (lastPoints[i].x() - center.x()) * S));
        points[i].setY(Common::round(center.y() + (lastPoints[i].y() - center.y()) * S));
    }
}

QList<Shape*> Curve::trim(QList<QPoint> borderPoints) {
    QList<Shape *> res;
    return res;
}

QList<QPoint> Curve::getPoints() {
    QList<QPoint> retPoints;
    if (curvealgo == Common::Bezier)
        retPoints = Figure::BezierDrawCurve(points);
    else
        retPoints = Figure::BSplineDrawCurve(points);
    if (isChosen) {
        for (int i = 0; i < points.size(); i++)
            retPoints.append(Figure::emphasize(points[i]));
    }
    return retPoints;
}

void Curve::reset() {
    int size = points.size();
    int sum_x = 0;
    int sum_y = 0;
    for (int i = 0; i < size; i++) {
        sum_x += points[i].x();
        sum_y += points[i].y();
        lastPoints[i].setX(points[i].x());
        lastPoints[i].setY(points[i].y());
    }
    center.setX(sum_x/size);
    center.setY(sum_y/size);
    rotateAngle = 0;
}

