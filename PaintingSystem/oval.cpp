#include "oval.h"

int Oval::distance(QPoint cur) {
    if (abs((cur.x() - center.x())*(cur.x() - center.x())/(a*a)+(cur.y() - center.y())*(cur.y() - center.y())/(b*b) - 1) < 3)
        return 0;
    else
        return 100;
}

QPoint Oval::getCenter() {
    return center;
}

void Oval::move(int dx, int dy) {
    center.setX(lastC.x()+dx);
    center.setY(lastC.y()+dy);
    showPoints.clear();
    showPoints.push_back(QPoint(center.x() + a, center.y()));
    showPoints.push_back(QPoint(center.x() - a, center.y()));
    showPoints.push_back(QPoint(center.x(), center.y() + b));
    showPoints.push_back(QPoint(center.x(), center.y() - b));
    seedPoint.setX(lastSeedPoint.x() + dx);
    seedPoint.setY(lastSeedPoint.y() + dy);
}

void Oval::rotate(double angle) {
    angle--;

    mode = rotate_mode;
    rotateAngle += angle;
    double cs = cos(rotateAngle);
    double sn = sin(rotateAngle);
    for (int i=0; i<points.size(); i++) {
        int x1 = lastPoints[i].x() - center.x();
        int y1 = lastPoints[i].y() - center.y();
        int x2 = lastPoints[i].x() - center.x();
        int y2 = lastPoints[i].y() - center.y();

        points[i].setX((int)(x1*cs-y1*sn+center.x()));
        points[i].setY((int)(x1*sn+y1*cs+center.y()));
        points[i].setX((int)(x2*cs-y2*sn+center.x()));
        points[i].setY((int)(x2*sn+y2*cs+center.y()));
    }

}

void Oval::scale(QPoint choosen, QPoint newPoint) {
    if (abs(Common::compute2PointDis(choosen, lastC) - lastA) < 3) {
        a = abs(newPoint.x() - lastC.x());
    }
    if (abs(Common::compute2PointDis(choosen, lastC) - lastB) < 3) {
        b = abs(newPoint.y() - lastC.y());
    }
    showPoints.clear();
    showPoints.push_back(QPoint(center.x() + a, center.y()));
    showPoints.push_back(QPoint(center.x() - a, center.y()));
    showPoints.push_back(QPoint(center.x(), center.y() + b));
    showPoints.push_back(QPoint(center.x(), center.y() - b));
}

void Oval::zoom(Shape::ZoomMode mode) {
    double S = (mode == Zin) ? S_in : S_out;
    a = Common::round(a * S);
    b = Common::round(b * S);
    showPoints.clear();
    showPoints.push_back(QPoint(center.x() + a, center.y()));
    showPoints.push_back(QPoint(center.x() - a, center.y()));
    showPoints.push_back(QPoint(center.x(), center.y() + b));
    showPoints.push_back(QPoint(center.x(), center.y() - b));
}

QList<Shape*> Oval::trim(QList<QPoint> borderPoints) {
    QList<Shape*> res;
    return res;
}

QList<QPoint> Oval::getPoints() {
    //if (mode == rotate_mode)
    //    return points;
    QList<QPoint> retPoints;
    double sqa = a*a;
    double sqb = b*b;

    double p = sqb + sqa*(0.25 - b);
    int x = 0;
    int y = b;
    retPoints.push_back(QPoint(center.x()+x, center.y()+y));
    retPoints.push_back(QPoint(center.x()+x, center.y()-y));
    retPoints.push_back(QPoint(center.x()-x, center.y()-y));
    retPoints.push_back(QPoint(center.x()-x, center.y()+y));
    // 1
    while (sqb*(x + 1) < sqa*(y - 0.5)) {
        if (p < 0) {
            p += sqb*(2 * x + 3);
        }
        else {
            p += (sqb*(2 * x + 3) + sqa*((-2)*y + 2));
            y--;
        }
        x++;
        retPoints.push_back(QPoint(center.x()+x, center.y()+y));
        retPoints.push_back(QPoint(center.x()+x, center.y()-y));
        retPoints.push_back(QPoint(center.x()-x, center.y()-y));
        retPoints.push_back(QPoint(center.x()-x, center.y()+y));
    }
    p = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;
    // 2
    while (y > 0) {
        if (p < 0) {
            p += sqb * (2 * x + 2) + sqa * ((-2) * y + 3);
            x++;
        }
        else {
            p += sqa * ((-2) * y + 3);
        }
        y--;
        retPoints.push_back(QPoint(center.x()+x, center.y()+y));
        retPoints.push_back(QPoint(center.x()+x, center.y()-y));
        retPoints.push_back(QPoint(center.x()-x, center.y()-y));
        retPoints.push_back(QPoint(center.x()-x, center.y()+y));
    }
    if (isChosen) {
        for (int i = 0; i < showPoints.size(); i++)
            retPoints.append(Figure::emphasize(showPoints[i]));
    }
    return retPoints;
    //return lastPoints;
}

void Oval::reset() {
    lastC.setX(center.x());
    lastC.setY(center.y());
    lastA = a;
    lastB = b;
    rotateAngle = 0;

    lastPoints.clear();
    for (int i = 0; i < points.size(); i++) {
        lastPoints.push_back(points[i]);
    }
    lastSeedPoint.setX(seedPoint.x());
    lastSeedPoint.setY(seedPoint.y());

}
