#include "polygon.h"

int Polygon::distance(QPoint cur) {
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

QPoint Polygon::getCenter() {
    return center;
}

void Polygon::move(int dx, int dy) {
    int size = points.size();
    for (int i = 0; i < size; i++) {
        points[i].setX(lastPoints[i].x() + dx);
        points[i].setY(lastPoints[i].y() + dy);
    }
    seedPoint.setX(lastSeedPoint.x() + dx);
    seedPoint.setY(lastSeedPoint.y() + dy);
}

void Polygon::rotate(double angle) {
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
    seedPoint.setX((int)((lastSeedPoint.x() - center.x())*cs-(lastSeedPoint.y() - center.y())*sn+center.x()));
    seedPoint.setY((int)((lastSeedPoint.x() - center.x())*sn+(lastSeedPoint.y() - center.y())*cs+center.y()));
}

void Polygon::scale(QPoint choosen, QPoint newPoint) {
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
    //qDebug("min_idx : %d", min_idx);
}

void Polygon::zoom(Shape::ZoomMode mode) {
    double S = (mode == Zin) ? S_in : S_out;
    for (int i = 0; i < points.size(); i++) {
        points[i].setX(Common::round(center.x() + (lastPoints[i].x() - center.x()) * S));
        points[i].setY(Common::round(center.y() + (lastPoints[i].y() - center.y()) * S));
    }
}

int Polygon::isInside(QPoint p, Boundary b, QList<QPoint> borderPoints) {
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

int Polygon::isCross(QPoint p1, QPoint p2, Boundary b, QList<QPoint> borderPoints) {
    if (isInside(p1, b, borderPoints) == isInside(p2, b, borderPoints))
        return false;
    return true;
}

QPoint Polygon::intersect(QPoint p1, QPoint p2, Polygon::Boundary b, QList<QPoint> borderPoints) {
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

//int Polygon::edgeCliper(Polygon::Boundary b, QList<QPoint> borderPoints, QList<QPoint> &pIn, int cnt, QList<QPoint> &pOut) {
int Polygon::edgeCliper(Boundary b, QList<QPoint> borderPoints, QPoint *pIn, int cnt, QPoint *pOut) {
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


QList<Shape*> Polygon::trim(QList<QPoint> borderPoints) {
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

QList<QPoint> Polygon::getPoints() {
    QList<QPoint> retPoints;
    int size = points.size();


    for (int i = 0; i < size - 1; i++) {
        retPoints.append(Figure::BresenhamDrawLine(points[i], points[i+1]));
    }

    if (isChosen) {
        for (int i = 0; i < points.size(); i++)
            retPoints.append(Figure::emphasize(points[i]));
    }
    return retPoints;
}

void Polygon::reset() {
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
    //oldColor = newColor;
    lastSeedPoint.setX(seedPoint.x());
    lastSeedPoint.setY(seedPoint.y());
}


