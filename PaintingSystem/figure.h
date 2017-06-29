#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>
#include <QPoint>
#include <QtCore/qmath.h>
#include "common.h"
#include <QDebug>
class Figure : public QWidget
{
    Q_OBJECT
public:
    explicit Figure(QWidget *parent = 0);

    enum ShapeType {
        None,
        Line,
        Curve,
        Circle,
        Oval,
        Rectangle,
        Polygon,
        Cube
    };

    enum OperateType {
        No,
        Select,
        Shift,
        Rotate,
        Scale,
        Trim,
        ZoomInOut,
        Fill,
        Delete
    };

    void virtual paint(QPainter &painter) = 0;

    static QList<QPoint> BresenhamDrawLine(QPoint start, QPoint end);
    static QList<QPoint> DDADrawLine(QPoint start, QPoint end);
    static QList<QPoint> MiddleDrawCircle(QPoint center, QPoint edge);
    static QList<QPoint> MiddleDrawOval(QPoint center, QPoint edge);
    static QList<QPoint> BezierDrawCurve(QList<QPoint> points);
    static QList<QPoint> emphasize(QPoint point);
    static double C(int nn, int k);

    static double N(int k, int i, double u);
    static double N1(int i, double u);
    static double N2(int i, double u);
    static double N3(int i, double u);   // 三次B样条的基函数
    static QList<QPoint> BSplineDrawCurve(QList<QPoint> points);

protected:

signals:

public slots:
};

class Shape
{
public:
    enum Mode {
        rotate_mode,
        shift_mode,
        trim_mode
    };
    enum ZoomMode {
        Zin,
        Zout
    };

    bool isChosen;
    double rotateAngle;
    QColor penColor = Qt::black;
    QColor brushColor = Qt::black;
    double S_in = 1.2;
    double S_out = 1 / 1.2;
    bool isFill;
    QPoint seedPoint;
    QPoint lastSeedPoint;
    QColor oldColor;
    QColor newColor;
    Figure::ShapeType type;
    QList<QPoint> points;
    QList<QPoint> lastPoints;
    QPoint center;
    QPoint lastC;
    virtual int distance(QPoint cur) {return cur.x();} // Compute the distance of mouse and shape to judge whether be chosen
    virtual QPoint getCenter() {QPoint p; return p;} // Get the center point
    virtual void move(int dx, int dy) { int a=0; a += dx + dy;} // Shift
    virtual void rotate(double angle) { double a=0; a += angle;} // Rotate
    virtual void scale(QPoint start, QPoint end) {int a=0; a += start.x() + end.x();} // Modify
    virtual void zoom(ZoomMode mode) { int a; a = mode;} // Zoom in and Zoom out
    virtual QList<Shape*> trim(QList<QPoint>) { QList<Shape*> res; return res; } // Trim
    virtual QList<QPoint> getPoints() { QList<QPoint> l; return l;} // Return all points of this shape
    virtual void reset() {} // Reset some parameters
};
#endif // FIGURE_H
