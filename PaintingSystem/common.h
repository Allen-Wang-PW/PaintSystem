#ifndef COMMON_H
#define COMMON_H

#include <QPoint>
#include <QtCore/qmath.h>
#include <QDebug>
class Common
{
public:
    Common();
    static double compute2PointDis(QPoint a, QPoint b);
    static void printPoint(QPoint a);
    static double computeRotateDeg(QPoint a, QPoint b, QPoint center);
    static double multiple(QPoint a, QPoint b, QPoint c);
    static double computeSlope(QPoint a, QPoint b);
    static bool isVertical(QPoint p1, QPoint p2, QPoint p3);
    static int round(double x);
    enum lineAlgorithm {
        DDA,
        Bresenham
    };
    enum curveAlgorithm {
        BSpline,
        Bezier
    };
};

#endif // COMMON_H
