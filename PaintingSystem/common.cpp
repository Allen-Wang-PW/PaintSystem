#include "common.h"

Common::Common()
{

}

double Common::compute2PointDis(QPoint a, QPoint b) {
    double ans = qSqrt((double)((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y())));
    return ans;
}

void Common::printPoint(QPoint a)
{
    qDebug("x: %d, y: %d", a.x(), a.y());
}

double Common::computeRotateDeg(QPoint a, QPoint b, QPoint center) {
    double dis1 = compute2PointDis(a, center);
    double dis2 = compute2PointDis(b, center);
    double dis3 = compute2PointDis(a, b);
    double temp = (dis1*dis1+dis2*dis2-dis3*dis3)/(2*dis1*dis2);
    double degree = acos(temp);

    int a1 = a.x() - center.x();
    int b1 = a.y() - center.y();
    int a2 = b.x() - center.x();
    int b2 = b.y() - center.y();
    if (a1*b2-a2*b1 > 0)
        return degree;
    else
        return -degree;
}

double Common::multiple(QPoint a, QPoint b, QPoint c) {
    return ((a.x() - c.x()) * (b.y() - a.y()) - (b.x() - a.x()) * (a.y() - c.y()));
}

double Common::computeSlope(QPoint a, QPoint b) {
    if (a.x() == b.x())
        return INFINITY;
    else {
        return -((float)a.y() - (float)b.y())/((float)a.x() - (float)b.x());
    }
}

bool Common::isVertical(QPoint p1, QPoint p2, QPoint p3) {
    if ((p1.x() - p2.x()) * (p2.x() - p3.x()) + (p1.y() - p2.y()) * (p2.y() - p3.y()) == 0)
        return true;
    return false;
}

int Common::round(double x) {
    if (x - int(x) >= 0.5)
        return int(x) + 1;
    return int(x);
}
