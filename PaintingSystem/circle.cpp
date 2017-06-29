#include "circle.h"

int Circle::distance(QPoint cur) {
    int dis = (int)Common::compute2PointDis(cur, center);
    if (dis < r)
        return 0;
    return 100;
}

QPoint Circle::getCenter() {
    return center;
}

void Circle::move(int dx, int dy) {
    center.setX(lastC.x()+dx);
    center.setY(lastC.y()+dy);
    seedPoint.setX(lastSeedPoint.x() + dx);
    seedPoint.setY(lastSeedPoint.y() + dy);
}

void Circle::rotate(double angle) {
    angle--;
}

void Circle::scale(QPoint choosen, QPoint newPoint) {
    if (std::abs(Common::compute2PointDis(lastC, choosen) - lastR) < 5) {
        r = (int)Common::compute2PointDis(lastC, newPoint);
    }
}

void Circle::zoom(Shape::ZoomMode mode) {
    double S = (mode == Zin) ? S_in : S_out;
    r = Common::round(r * S);
}

QList<Shape*> Circle::trim(QList<QPoint> borderPoints) {
    QList<Shape*> res;
    return res;
}

QList<QPoint> Circle::getPoints() {
    QList<QPoint> retPoints;
    int x = 0;
    int y = r;
    //qDebug("r : %d", r);

    int p = 1 - r;

    while (x <= y) {
        retPoints.push_back(QPoint(center.x()+x, center.y()+y));
        retPoints.push_back(QPoint(center.x()+y, center.y()+x));
        retPoints.push_back(QPoint(center.x()+y, center.y()-x));
        retPoints.push_back(QPoint(center.x()+x, center.y()-y));
        retPoints.push_back(QPoint(center.x()-x, center.y()-y));
        retPoints.push_back(QPoint(center.x()-y, center.y()-x));
        retPoints.push_back(QPoint(center.x()-y, center.y()+x));
        retPoints.push_back(QPoint(center.x()-x, center.y()+y));
        if (p < 0) {
            x++;
            p += 2*x+1;
        }
        else {
            x++;
            y--;
            p += 2*(x-y)+1;
        }
    }
    if (isChosen)
        retPoints.append(Figure::emphasize(center));
    return retPoints;
}

void Circle::reset() {
    lastC.setX(center.x());
    lastC.setY(center.y());
    lastR = r;
    lastSeedPoint = seedPoint;
}
