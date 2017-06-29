#ifndef PAINTPANE_H
#define PAINTPANE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include "figure.h"
#include "common.h"
#include "line.h"
#include "curve.h"
#include "circle.h"
#include "oval.h"
#include "rectangle.h"
#include "polygon.h"
class PaintPane : public QWidget
{
    Q_OBJECT
public:
    enum Mode {
        Paint,
        Operate
    };

    explicit PaintPane(QWidget *parent = 0);

    void setPaneSize(int width, int height);    // Set the size of Pane
    void setPaneColor(QColor color );            // Set the color of Pane
    QSize getPaneSize();                        // Get the size of Pane

    bool isModified() const {return modified;}
    bool saveImage(const QString &fileName, const char* fileFormat);
    bool openImage(const QString &fileName);

    void zoom();
    void minimize();
    //void restore();
    void doRotate();
    void doShear();
    void doClear();

    void setPenStyle(Qt::PenStyle style);
    void setPenWidth(int width);
    void setPenColor(QColor color);
    void setBrushColor(QColor color);
    void setShape(Figure::ShapeType shape);
    void setOperate(Figure::OperateType operate);
    void setMode(Mode mode);
    void clearFigures();
    void deleteFigure();
    void setLineAlgorithm(int i);
    void setCurveAlgorithm(int i);
    //void writeBuffer(QList<QPoint> &buf, QList<QColor> &cbuf, QList<Shape*> list);

protected:
    void floodSeedFill(QImage &image, QPoint seedPoint, QColor oldColor, QColor newColor);
    void paint(QImage &theImage, QList<Shape*> list);
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    int Width = 900;
    int Height = 600;

    int threshold = 5;

    QImage image;
    QImage tempImage;

    QRgb backColor;

    QPoint startPoint, endPoint, lastPoint;
    QList<QPoint> polygonPoints;
    QList<QPoint> tmpPolygonPoints;
    QList<QPoint> curvePoints;
    QList<QPoint> tmpCurvePoints;
    QList<QPoint> nonePoints;
    bool isDone;

    bool modified;
    bool isDrawing;


    PaintPane::Mode curMode;
    Common::lineAlgorithm line_algorithm;
    Common::curveAlgorithm curve_algorithm;

    qreal scale;        // scale
    int angle;          // rotation angle
    qreal shear;        // stretch

    QColor penColor;
    QColor brushColor;
    int penWidth;
    Qt::PenStyle penStyle;
    Figure::ShapeType curShape;
    Figure::OperateType curOperate;

    QList<Shape*> figures;
    QList<Shape*> tempFigures;

    int moveObject;

    bool isTrim;
    Shape *trim;

signals:

public slots:
};

#endif // PAINTPANE_H
