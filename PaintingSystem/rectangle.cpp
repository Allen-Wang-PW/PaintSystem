#include "rectangle.h"

int Rectangle::distance(QPoint cur) {
    if (Common::multiple(cur, points[0], points[1]) * Common::multiple(cur, points[3], points[2]) <= 0 &&
            Common::multiple(cur, points[3], points[0]) * Common::multiple(cur, points[2], points[1]) <= 0)
        return 0;
    else
        return 100;
}

QPoint Rectangle::getCenter() {
    return center;
}

void Rectangle::move(int dx, int dy) {
    points[0].setX(lastPoints[0].x() + dx);
    points[0].setY(lastPoints[0].y() + dy);
    points[1].setX(lastPoints[1].x() + dx);
    points[1].setY(lastPoints[1].y() + dy);
    points[2].setX(lastPoints[2].x() + dx);
    points[2].setY(lastPoints[2].y() + dy);
    points[3].setX(lastPoints[3].x() + dx);
    points[3].setY(lastPoints[3].y() + dy);
    seedPoint.setX(lastSeedPoint.x() + dx);
    seedPoint.setY(lastSeedPoint.y() + dy);
}

void Rectangle::rotate(double angle) {
    rotateAngle += angle;
    double cs = cos(rotateAngle);
    double sn = sin(rotateAngle);

    int x1 = lastPoints[0].x() - center.x();
    int y1 = lastPoints[0].y() - center.y();
    points[0].setX((int)(x1*cs-y1*sn+center.x()));
    points[0].setY((int)(x1*sn+y1*cs+center.y()));
    int x2 = lastPoints[1].x() - center.x();
    int y2 = lastPoints[1].y() - center.y();
    points[1].setX((int)(x2*cs-y2*sn+center.x()));
    points[1].setY((int)(x2*sn+y2*cs+center.y()));
    int x3 = lastPoints[2].x() - center.x();
    int y3 = lastPoints[2].y() - center.y();
    points[2].setX((int)(x3*cs-y3*sn+center.x()));
    points[2].setY((int)(x3*sn+y3*cs+center.y()));
    int x4 = lastPoints[3].x() - center.x();
    int y4 = lastPoints[3].y() - center.y();
    points[3].setX((int)(x4*cs-y4*sn+center.x()));
    points[3].setY((int)(x4*sn+y4*cs+center.y()));
}

void Rectangle::scale(QPoint choosen, QPoint newPoint) {
    int dis[4];
    dis[0] = (int)Common::compute2PointDis(choosen, lastPoints[0]);
    dis[1] = (int)Common::compute2PointDis(choosen, lastPoints[1]);
    dis[2] = (int)Common::compute2PointDis(choosen, lastPoints[2]);
    dis[3] = (int)Common::compute2PointDis(choosen, lastPoints[3]);

    int min_idx = 0;
    for (int i = 1; i < 4; i++) {
        if (dis[i] < dis[min_idx] && dis[i] < 5)
            min_idx = i;
    }
    switch(min_idx) {
    case 0: {
        //qDebug("1");
        /*
        if (dis[0] < 5) {
            Point1 = newPoint;
            Point2.setX(newPoint.x());
            Point4.setY(newPoint.y());
        }
        */
        double Sx = (float)(newPoint.x() - center.x()) / float(lastPoints[0].x() - center.x());
        double Sy = (float)(newPoint.y() - center.y()) / float(lastPoints[0].y() - center.y());
        points[0] = newPoint;
        points[2].setX(Common::round(float(lastPoints[2].x()) * Sx + center.x() * (1.0 - Sx)));
        points[2].setY(Common::round(float(lastPoints[2].y()) * Sy + center.y() * (1.0 - Sy)));
        points[1].setX(Common::round(float(lastPoints[1].x()) * Sx + center.x() * (1.0 - Sx)));
        points[1].setY(Common::round(float(lastPoints[1].y()) * Sy + center.y() * (1.0 - Sy)));
        points[3].setX(Common::round(float(lastPoints[3].x()) * Sx + center.x() * (1.0 - Sx)));
        points[3].setY(Common::round(float(lastPoints[3].y()) * Sy + center.y() * (1.0 - Sy)));
    }
        break;
    case 1: {
        //qDebug("2");
        /*
        Point2 = newPoint;
        Point1.setX(newPoint.x());
        Point3.setY(newPoint.y());
        */
        double Sx = (float)(newPoint.x() - center.x()) / float(lastPoints[1].x() - center.x());
        double Sy = (float)(newPoint.y() - center.y()) / float(lastPoints[1].y() - center.y());
        points[1] = newPoint;
        points[3].setX(Common::round(float(lastPoints[3].x()) * Sx + center.x() * (1.0 - Sx)));
        points[3].setY(Common::round(float(lastPoints[3].y()) * Sy + center.y() * (1.0 - Sy)));
        points[0].setX(Common::round(float(lastPoints[0].x()) * Sx + center.x() * (1.0 - Sx)));
        points[0].setY(Common::round(float(lastPoints[0].y()) * Sy + center.y() * (1.0 - Sy)));
        points[2].setX(Common::round(float(lastPoints[2].x()) * Sx + center.x() * (1.0 - Sx)));
        points[2].setY(Common::round(float(lastPoints[2].y()) * Sy + center.y() * (1.0 - Sy)));
    }
        break;
    case 2: {
        //qDebug("3");
        /*
        Point3 = newPoint;
        Point2.setY(newPoint.y());
        Point4.setX(newPoint.x());
        */
        double Sx = (float)(newPoint.x() - center.x()) / float(lastPoints[2].x() - center.x());
        double Sy = (float)(newPoint.y() - center.y()) / float(lastPoints[2].y() - center.y());
        points[2] = newPoint;
        points[0].setX(Common::round(float(lastPoints[0].x()) * Sx + center.x() * (1.0 - Sx)));
        points[0].setY(Common::round(float(lastPoints[0].y()) * Sy + center.y() * (1.0 - Sy)));
        points[1].setX(Common::round(float(lastPoints[1].x()) * Sx + center.x() * (1.0 - Sx)));
        points[1].setY(Common::round(float(lastPoints[1].y()) * Sy + center.y() * (1.0 - Sy)));
        points[3].setX(Common::round(float(lastPoints[3].x()) * Sx + center.x() * (1.0 - Sx)));
        points[3].setY(Common::round(float(lastPoints[3].y()) * Sy + center.y() * (1.0 - Sy)));
    }
        break;
    case 3: {
        //qDebug("4");
        /*
        Point4 = newPoint;
        Point1.setY(newPoint.y());
        Point3.setX(newPoint.x());
        */
        double Sx = (float)(newPoint.x() - center.x()) / float(lastPoints[3].x() - center.x());
        double Sy = (float)(newPoint.y() - center.y()) / float(lastPoints[3].y() - center.y());
        points[3] = newPoint;
        points[2].setX(Common::round(float(lastPoints[2].x()) * Sx + center.x() * (1.0 - Sx)));
        points[2].setY(Common::round(float(lastPoints[2].y()) * Sy + center.y() * (1.0 - Sy)));
        points[1].setX(Common::round(float(lastPoints[1].x()) * Sx + center.x() * (1.0 - Sx)));
        points[1].setY(Common::round(float(lastPoints[1].y()) * Sy + center.y() * (1.0 - Sy)));
        points[0].setX(Common::round(float(lastPoints[0].x()) * Sx + center.x() * (1.0 - Sx)));
        points[0].setY(Common::round(float(lastPoints[0].y()) * Sy + center.y() * (1.0 - Sy)));
    }
        break;
    }

}

void Rectangle::zoom(Shape::ZoomMode mode) {
    double S = (mode == Zin) ? S_in : S_out;
    points[0].setX(Common::round(float(lastPoints[0].x()) * S + center.x() * (1.0 - S)));
    points[0].setY(Common::round(float(lastPoints[0].y()) * S + center.y() * (1.0 - S)));
    points[2].setX(Common::round(float(lastPoints[2].x()) * S + center.x() * (1.0 - S)));
    points[2].setY(Common::round(float(lastPoints[2].y()) * S + center.y() * (1.0 - S)));
    points[1].setX(Common::round(float(lastPoints[1].x()) * S + center.x() * (1.0 - S)));
    points[1].setY(Common::round(float(lastPoints[1].y()) * S + center.y() * (1.0 - S)));
    points[3].setX(Common::round(float(lastPoints[3].x()) * S + center.x() * (1.0 - S)));
    points[3].setY(Common::round(float(lastPoints[3].y()) * S + center.y() * (1.0 - S)));
}

int Rectangle::isInside(QPoint p, Boundary b, QList<QPoint> borderPoints) {
    switch(b) {
    case Left:
        if (p.x() < borderPoints[0].x())
            return false;
        break;
    case Right:
        if (p.x() > borderPoints[1].x())
            return false;
        break;
    case Bottom:
        if (p.y() < borderPoints[0].y())
            return false;
        break;
    case Top:
        if (p.y() > borderPoints[1].y())
            return false;
        break;
    }
    return true;
}

int Rectangle::isCross(QPoint p1, QPoint p2, Boundary b, QList<QPoint> borderPoints) {
    if (isInside(p1, b, borderPoints) == isInside(p2, b, borderPoints))
        return false;
    return true;
}

QPoint Rectangle::intersect(QPoint p1, QPoint p2, Rectangle::Boundary b, QList<QPoint> borderPoints) {
    QPoint p;
    double m;
    if (p1.x() != p2.x())
        m = Common::computeSlope(p1, p2);
    switch(b) {
    case Left:
        p.setX(borderPoints[0].x());
        p.setY(p2.y() + abs(borderPoints[0].x() - p2.x()) * m);
        break;
    case Right:
        p.setX(borderPoints[1].x());
        p.setY(p2.y() - abs(borderPoints[1].x() - p2.x()) * m);
        break;
    case Bottom:
        p.setY(borderPoints[0].y());
        if (p1.x() != p2.x())
            p.setX(p2.x() + abs(borderPoints[0].y() - p2.y()) / m);
        else
            p.setX(p2.x());
        break;
    case Top:
        p.setY(borderPoints[1].y());
        if (p1.x() != p2.x())
            p.setX(p2.x() - abs(borderPoints[1].y() - p2.y()) / m);
        else
            p.setX(p2.x());
        break;
    }
    return p;
}

int Rectangle::edgeCliper(Boundary b, QList<QPoint> borderPoints, QPoint *pIn, int cnt, QPoint *pOut) {
    QPoint s;
    int i, Outcnt = 0;
    s = pIn[0];
    for (i = 1; i < cnt; i++) {
        if (!isInside(s, b, borderPoints) && isInside(pIn[i], b, borderPoints)) {
            //pOut.push_back(intersect(s, pIn[i], b, borderPoints));
            pOut[Outcnt] = intersect(s, pIn[i], b, borderPoints);
            Outcnt++;
            //pOut.push_back(pIn[i]);
            pOut[Outcnt] = pIn[i];
            Outcnt++;
        }
        else if (isInside(s, b, borderPoints) && isInside(pIn[i], b, borderPoints)) {
            //pOut.push_back(pIn[i]);
            pOut[Outcnt] = pIn[i];
            Outcnt++;
        }
        else if (isInside(s, b, borderPoints) && !isInside(pIn[i], b, borderPoints)) {
            //pOut.push_back(intersect(pIn[i], s, b, borderPoints));
            pOut[Outcnt] = intersect(pIn[i], s, b, borderPoints);
            Outcnt++;
        }
        s = pIn[i];
    }
    return Outcnt;
}

QList<Shape*> Rectangle::trim(QList<QPoint> borderPoints) {
    //qDebug("in");
    QList<Shape*> res;

    //QList<QPoint> pOut, pIn;
    QPoint pOut[100], pIn[100];
    int Outcnt, cnt = points.size();
    for (int i = 0; i < points.size(); i++)
        //pIn.push_back(points[i]);
        pIn[i] = points[i];
    for (int i = 0; i < 4; i++) {
        //pOut.clear();
        Outcnt = edgeCliper(Boundary(i), borderPoints, pIn, cnt, pOut);
        //pIn.clear();
        for (int i = 0; i < Outcnt; i++)
            //pIn.push_back(pOut[i]);
            pIn[i] = pOut[i];
        pIn[Outcnt] = pOut[0];
        //pIn.push_back(pOut[0]);
        cnt = Outcnt + 1;
    }
    QList<QPoint> newPoints;
    for (int i = 0; i < cnt; i++)
        newPoints.push_back(pOut[i]);
    qDebug("cnt:%d", cnt);
    Polygon *polygon = new Polygon(newPoints, true);
    //Polygon *polygon = new Polygon(pOut, true);
    Shape *shape = polygon;
    res.push_back(shape);
    return res;
}

QList<QPoint> Rectangle::getPoints() {
    QList<QPoint> retPoints;
    retPoints.append(Figure::BresenhamDrawLine(points[0], points[1]));
    retPoints.append(Figure::BresenhamDrawLine(points[1], points[2]));
    retPoints.append(Figure::BresenhamDrawLine(points[2], points[3]));
    retPoints.append(Figure::BresenhamDrawLine(points[3], points[0]));
    if (isChosen) {
        retPoints.append(Figure::emphasize(points[0]));
        retPoints.append(Figure::emphasize(points[1]));
        retPoints.append(Figure::emphasize(points[2]));
        retPoints.append(Figure::emphasize(points[3]));
    }
    return retPoints;
}

void Rectangle::reset() {
    lastPoints[0].setX(points[0].x());
    lastPoints[0].setY(points[0].y());
    lastPoints[1].setX(points[1].x());
    lastPoints[1].setY(points[1].y());
    lastPoints[2].setX(points[2].x());
    lastPoints[2].setY(points[2].y());
    lastPoints[3].setX(points[3].x());
    lastPoints[3].setY(points[3].y());
    center.setX((points[0].x()+points[2].x())/2);
    center.setY((points[0].y()+points[2].y())/2);
    rotateAngle = 0;
    //oldColor = newColor;
    lastSeedPoint = seedPoint;
}
