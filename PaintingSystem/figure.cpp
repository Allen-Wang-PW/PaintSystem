#include "figure.h"
#include "QtCore/qmath.h"
#include <QDebug>
Figure::Figure(QWidget *parent) : QWidget(parent)
{

}

QList<QPoint> Figure::BresenhamDrawLine(QPoint start, QPoint end)
{
    QList<QPoint> points;
    int x = start.x();
    int y = start.y();
    int dx = abs(end.x() - start.x());
    int dy = abs(end.y() - start.y());
    int s1 = end.x() > start.x() ? 1 : -1;
    int s2 = end.y() > start.y() ? 1 : -1;

    bool interchange = false;
    if (dy > dx)
    {
        int temp = dx;
        dx = dy;
        dy = temp;
        interchange = true;
    }

    int e = 2 * dy - dx;
    for(int i = 0; i <= dx; i++)
    {
        points.push_back(QPoint(x, y));
        if (e >= 0)
        {
            if (!interchange)
                y += s2;
            else
                x += s1;
            e -= 2 * dx;
        }
        if (!interchange)
            x += s1;
        else
            y += s2;
        e += 2 * dy;
    }
    return points;
}

QList<QPoint> Figure::DDADrawLine(QPoint start, QPoint end) {
    int dx = end.x() - start.x();
    int dy = end.y() - start.y();
    int steps;
    float xInc, yInc, x = start.x(), y = start.y();
    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);
    xInc = float(dx) / float(steps);
    yInc = float(dy) / float(steps);
    QList<QPoint> retPoints;
    retPoints.push_back(start);
    retPoints.push_back(end);
    for (int k = 0; k <= steps; k++) {
        x += xInc;
        y += yInc;
        retPoints.push_back(QPoint(x, y));
    }
    return retPoints;
}

QList<QPoint> Figure::MiddleDrawCircle(QPoint center, QPoint edge)
{
    QList<QPoint> points;
    int x = 0;
    int r = (edge.x() - center.x())*(edge.x() - center.x()) + (edge.y() - center.y())*(edge.y() - center.y());
    r = (int)qSqrt((double)r);
    int y = r;

    int p = 1 - r;

    while (x <= y) {
        points.push_back(QPoint(center.x()+x, center.y()+y));
        points.push_back(QPoint(center.x()+y, center.y()+x));
        points.push_back(QPoint(center.x()+y, center.y()-x));
        points.push_back(QPoint(center.x()+x, center.y()-y));
        points.push_back(QPoint(center.x()-x, center.y()-y));
        points.push_back(QPoint(center.x()-y, center.y()-x));
        points.push_back(QPoint(center.x()-y, center.y()+x));
        points.push_back(QPoint(center.x()-x, center.y()+y));
        if (p < 0)
        {
            x++;
            p += 2*x+1;
        }
        else
        {
            x++;
            y--;
            p += 2*(x-y)+1;
        }
    }
    return points;
}

QList<QPoint> Figure::MiddleDrawOval(QPoint center, QPoint edge)
{
    QList<QPoint> points;
    int a = abs(edge.x() - center.x());
    int b = abs(edge.y() - center.y());
    double sqa = a*a;
    double sqb = b*b;

    double p = sqb + sqa*(0.25 - b);
    int x = 0;
    int y = b;
    points.push_back(QPoint(center.x()+x, center.y()+y));
    points.push_back(QPoint(center.x()+x, center.y()-y));
    points.push_back(QPoint(center.x()-x, center.y()-y));
    points.push_back(QPoint(center.x()-x, center.y()+y));
    // 1
    while (sqb*(x + 1) < sqa*(y - 0.5))
    {
        if (p < 0)
        {
            p += sqb*(2 * x + 3);
        }
        else
        {
            p += (sqb*(2 * x + 3) + sqa*((-2)*y + 2));
            y--;
        }
        x++;
        points.push_back(QPoint(center.x()+x, center.y()+y));
        points.push_back(QPoint(center.x()+x, center.y()-y));
        points.push_back(QPoint(center.x()-x, center.y()-y));
        points.push_back(QPoint(center.x()-x, center.y()+y));
    }
    p = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;
    // 2
    while (y > 0)
    {
        if (p < 0)
        {
            p += sqb * (2 * x + 2) + sqa * ((-2) * y + 3);
            x++;
        }
        else
        {
            p += sqa * ((-2) * y + 3);
        }
        y--;
        points.push_back(QPoint(center.x()+x, center.y()+y));
        points.push_back(QPoint(center.x()+x, center.y()-y));
        points.push_back(QPoint(center.x()-x, center.y()-y));
        points.push_back(QPoint(center.x()-x, center.y()+y));
    }
    return points;
}

QList<QPoint> Figure::BezierDrawCurve(QList<QPoint> points) {
    QList<QPoint> retPoints;
    int n = points.size();
    double m = 0;
    for (int i = 0; i < n - 1; i++)
        m += Common::compute2PointDis(points[i], points[i + 1]);
    for (int i = 0; i < (int)m; i++) {
        double t = i / m;
        double x = 0, y = 0, Ber;
        for (int k = 0; k < n; k++) {
            Ber = Figure::C(n - 1, k) * pow(t, k) * pow(1 - t, n - 1 - k);
            x += points[k].x() * Ber;
            y += points[k].y() * Ber;
        }
        retPoints.push_back(QPoint((int)x, (int)y));
    }
    return retPoints;
}

QList<QPoint> Figure::emphasize(QPoint point) {
    QPoint tmp;
    tmp.setX(point.x());
    tmp.setY(point.y() + 3);
    return Figure::MiddleDrawCircle(point, tmp);
}

double Figure::C(int nn, int k) {
    int i;
    double sum = 1;
    for (i = 1; i <= nn; i++)
        sum *= i;
    for (i = 1; i <= k; i++)
        sum /= i;
    for (i = 1; i <= nn - k; i++)
        sum /= i;
    return sum;
}

double Figure::N(int k, int i, double u) {
    switch(k)
    {
    case 1:
        return N1(i,u);
    case 2:
        return N2(i,u);
    case 3:
        return N3(i,u);
    }
}

double Figure::N1(int i, double u) {
    double t = u-i;

    if(0<=t && t<1)
        return t;
    if(1<=t && t<2)
        return 2-t;
    return 0;
}

double Figure::N2(int i, double u) {
    double t = u-i;

    if(0<=t && t<1)
        return 0.5*t*t;
    if(1<=t && t<2)
        return 3*t-t*t-1.5;
    if(2<=t && t<3)
        return 0.5*pow(3-t,2);
    return 0;
}

double Figure::N3(int i, double u) {
    double t = u-i;
    double a = 1.0/6.0;

    if(0<=t && t<1)
        return a*t*t*t;
    if(1<=t && t<2)
        return a*(-3*pow(t-1,3) + 3*pow(t-1,2) + 3*(t-1) +1);
    if(2<=t && t<3)
        return a*(3*pow(t-2,3) - 6*pow(t-2,2) +4);
    if(3<=t && t<4)
        return a*pow(4-t,3);
    return 0;
}

QList<QPoint> Figure::BSplineDrawCurve(QList<QPoint> points) {
    QList<QPoint> curvePoints;
    QList<QPoint> retPoints;
    for(double u=3; u<points.size(); u+=0.01)
    {
        QPoint tmp(0,0);
        for(int i=0; i<points.size();i++)
        {
            QPoint t = points[i];

            t*=N(3,i,u);

            tmp+=t;
        }
        curvePoints.push_back(tmp);
    }
    for (int i = 0; i < curvePoints.size() - 1; i++)
        retPoints.append(Figure::BresenhamDrawLine(curvePoints[i], curvePoints[i+1]));
    return retPoints;
}
