#include "line.h"
#include "figure.h"

int Line::distance(QPoint cur) {
    int dis = (int)Common::compute2PointDis(points[0], points[1]);
    int dis1 = (int)(Common::compute2PointDis(points[0], cur) + Common::compute2PointDis(points[1], cur));
    //qDebug("%d  %d", dis, dis1);
    return abs(dis-dis1);
}

QPoint Line::getCenter() {
    return center;
}

void Line::move(int dx, int dy) {
    points[0].setX(lastPoints[0].x() + dx);
    points[0].setY(lastPoints[0].y() + dy);
    points[1].setX(lastPoints[1].x() + dx);
    points[1].setY(lastPoints[1].y() + dy);
}

void Line::rotate(double angle) {
    rotateAngle += angle;
    double cs = cos(rotateAngle);
    double sn = sin(rotateAngle);

    int x1 = lastPoints[0].x() - center.x();
    int y1 = lastPoints[0].y() - center.y();
    int x2 = lastPoints[1].x() - center.x();
    int y2 = lastPoints[1].y() - center.y();

    points[0].setX((int)(x1*cs-y1*sn+center.x()));
    points[0].setY((int)(x1*sn+y1*cs+center.y()));
    points[1].setX((int)(x2*cs-y2*sn+center.x()));
    points[1].setY((int)(x2*sn+y2*cs+center.y()));
}

void Line::scale(QPoint choosen, QPoint newPoint) {
    int dis1 = (int)Common::compute2PointDis(choosen, lastPoints[0]);
    int dis2 = (int)Common::compute2PointDis(choosen, lastPoints[1]);
    if (dis1 < dis2 && dis1 < 5) {
        points[0] = newPoint;
    }
    else if (dis2 < dis1 && dis2 < 5) {
        points[1] = newPoint;
    }
}

void Line::zoom(Shape::ZoomMode mode) {
    double S = (mode == Zin) ? S_in : S_out;
    //Common::printPoint(points[0]);
    //Common::printPoint(points[1]);
    points[0].setX(Common::round(center.x() + (points[0].x() - center.x()) * S));
    points[0].setY(Common::round(center.y() + (points[0].y() - center.y()) * S));
    points[1].setX(Common::round(center.x() + (points[1].x() - center.x()) * S));
    points[1].setY(Common::round(center.y() + (points[1].y() - center.y()) * S));
    //Common::printPoint(points[0]);
    //Common::printPoint(points[1]);
}

QList<Shape*> Line::trim(QList<QPoint> borderPoints) {
    QList<Shape*> res;
    int code1, code2, code;
    int x, y;
    int x1 = points[0].x(), y1 = points[0].y();
    int x2 = points[1].x(), y2 = points[1].y();
    code1 = encode(points[0], borderPoints);
    code2 = encode(points[1], borderPoints);
    int XL = borderPoints[0].x(), XR = borderPoints[1].x();
    int YB = borderPoints[0].y(), YT = borderPoints[1].y();
    while (code1 != 0 || code2 != 0) {
        if (code1 & code2)
            break;
        if (code1 != 0)
            code = code1;
        else
            code = code2;
        if (LEFT & code) {
            x = XL;
            y = y1 + (y2 - y1) * (XL - x1) / (x2 - x1);
        }
        else if (RIGHT & code) {
            x = XR;
            y = y1 + (y2 - y1) * (XR - x1) / (x2 - x1);
        }
        else if (BOTTOM & code) {
            y = YB;
            x = x1 + (x2 - x1) * (YB - y1) / (y2 - y1);
        }
        else if (TOP & code) {
            y = YT;
            x = x1 + (x2 - x1) * (YT - y1) / (y2 - y1);
        }

        if (code == code1) {
            x1 = x;
            y1 = y;
            code1 = encode(QPoint(x1, y1), borderPoints);
        }
        else {
            x2 = x;
            y2 = y;
            code2 = encode(QPoint(x2, y2), borderPoints);
        }
    }
    Line *line = new Line(QPoint(x1, y1), QPoint(x2, y2), linealgo);
    Shape *shape = line;
    res.push_back(shape);
    return res;
}

QList<QPoint> Line::getPoints() {
    QList<QPoint> retPoints;
    if (linealgo == Common::Bresenham)
        retPoints = Figure::BresenhamDrawLine(points[0], points[1]);
    else
        retPoints = Figure::DDADrawLine(points[0], points[1]);
    if (isChosen) {
        retPoints.append(Figure::emphasize(points[0]));
        retPoints.append(Figure::emphasize(points[1]));
    }
    return retPoints;
}

void Line::reset(){
    rotateAngle = 0;
    lastPoints[0].setX(points[0].x());
    lastPoints[0].setY(points[0].y());
    lastPoints[1].setX(points[1].x());
    lastPoints[1].setY(points[1].y());
    center.setX((points[0].x()+points[1].x())/2);
    center.setY((points[0].y()+points[1].y())/2);
}

int Line::encode(QPoint p, QList<QPoint>borderPoints) {
    int x = p.x();
    int y = p.y();
    int c = 0;
    int XL = borderPoints[0].x(), XR = borderPoints[1].x();
    int YB = borderPoints[0].y(), YT = borderPoints[1].y();
    if (x < XL)
        c |= LEFT;
    if (x > XR)
        c |= RIGHT;
    if (y < YB)
        c |= BOTTOM;
    if (y > YT)
        c |= TOP;
    return c;
}

