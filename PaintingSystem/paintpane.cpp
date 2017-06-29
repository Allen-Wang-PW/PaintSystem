#include "paintpane.h"
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QStack>
#include <QMessageBox>
PaintPane::PaintPane(QWidget *parent) : QWidget(parent) {
    image = QImage(Width, Height, QImage::Format_RGB32);
    backColor = qRgb(255, 255, 255);
    image.fill(backColor);

    modified = false;
    isDone = false;

    scale = 1;
    angle = 0;
    shear = 0;

    penStyle = Qt::SolidLine;
    penColor = Qt::black;
    penWidth = 1;
    brushColor = Qt::black;
    curShape = Figure::None;
    curMode = PaintPane::Paint;
    moveObject = -1;
    //setCursor(Qt::OpenHandCursor);
}

QSize PaintPane::getPaneSize() {
    return image.size();
}

void PaintPane::setPaneColor(QColor color) {
    backColor = color.rgb();
    image.fill(backColor);
    update();
}

void PaintPane::setPaneSize(int width, int height) {
    QImage newImage(width, height, QImage::Format_RGB32);
    image = newImage;
    update();
}

bool PaintPane::saveImage(const QString &fileName, const char *fileFormat) {
    QImage newImage = image;
    if (newImage.save(fileName, fileFormat))
    {
        modified = false;
        return true;
    }
    else
        return false;
}

bool PaintPane::openImage(const QString &fileName) {
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;
    QSize newSize = loadedImage.size();
    setPaneSize(newSize.width(), newSize.height());
    image = loadedImage;
    modified = false;
    update();
    return true;
}

void PaintPane::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.scale(scale, scale);
    if (isDrawing) {

        painter.drawImage(0, 0, tempImage);
/*
        for (int i=0; i<tempFigures.size(); i++) {
            QList<QPoint> temp = tempFigures[i]->getPoints();
            for (int j=0; j<temp.size(); j++) {
                painter.drawPoint(temp[j].x(), temp[j].y);
            }
        }
*/
    }
    else {
        if(angle)
        {
            QPointF center(image.width()/2.0,image.height()/2.0);
            painter.translate(center);
            painter.rotate(angle);
            painter.translate(-center);
        }
        if(shear)
        {
            painter.shear(shear, shear);
        }
        painter.drawImage(0, 0, image);
    }
}

void PaintPane::paint(QImage &theImage, QList<Shape*> list) {
    theImage = QImage(Width, Height, QImage::Format_RGB32);
    //backColor = qRgb(255, 255, 255);
    theImage.fill(backColor);

    QPainter painter(&theImage);
    QPen pen = QPen();
    pen.setColor(penColor);
    pen.setStyle(penStyle);
    pen.setWidth(penWidth);
    QBrush brush = QBrush(brushColor);
    painter.setPen(pen);
    painter.setBrush(brush);

    for (int i = 0; i < nonePoints.size(); i++) {
        painter.drawPoint(nonePoints[i].x(), nonePoints[i].y());
    }
    qDebug("%d", list.size());
    for (int i = 0; i < list.size(); i++) {
        QList<QPoint> tmp;
        tmp.append(list[i]->getPoints());
        pen.setColor(list[i]->penColor);
        painter.setPen(pen);
        for (int j = 0; j < tmp.size(); j++)
            painter.drawPoint(tmp[j].x(), tmp[j].y());
    }
    //qDebug() << isTrim;
    if (isTrim) {
        //qDebug("draw");
        QList<QPoint> tmp;
        tmp.append(trim->getPoints());
        pen.setColor(Qt::red);
        painter.setPen(pen);
        for (int j = 0; j < tmp.size(); j++)
            painter.drawPoint(tmp[j].x(), tmp[j].y());
    }
    update();

    for (int i = 0; i < figures.size(); i++) {
        //qDebug("%d", i);
        if (figures[i]->isFill) {
            floodSeedFill(theImage, figures[i]->seedPoint, figures[i]->oldColor, figures[i]->newColor);
            update();
        }
    }

    modified = true;
}

void PaintPane::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (curMode == PaintPane::Paint) {
            moveObject = -1;
            isDrawing = true;
            for (int i = 0; i < figures.size(); i++)
                figures[i]->isChosen = false;
            startPoint = event->pos();
            if (curShape == Figure::Polygon) {
                if (polygonPoints.isEmpty()) {
                    isDone = false;
                    polygonPoints.push_back(startPoint);
                    tmpPolygonPoints.push_back(startPoint);
                }
                //qDebug("point : (%d, %d)", startPoint.x(), startPoint.y());
            }
            else if (curShape == Figure::Curve) {
                isDone = false;
            }
        }
        else {
            startPoint = event->pos();
            lastPoint = event->pos();
            if (curOperate == Figure::Select) {
                moveObject = -1;
                for (int i = 0; i < figures.size(); i++)
                    figures[i]->isChosen = false;
                for (int i=0; i<figures.size(); i++) {
                    if (figures[i]->distance(startPoint) < threshold) {
                        moveObject = i;
                        figures[i]->isChosen = true;
                        break;
                    }
                }
            }
            qDebug("choose %d", moveObject);
            if (curOperate == Figure::Trim) {
                isTrim = true;
                //qDebug("trim");
            }
        }
    }
}

void PaintPane::mouseMoveEvent(QMouseEvent *event) {
    QString pos = QString(" %1, %2").arg(event->pos().x()).arg(event->pos().y());
    QToolTip::showText(event->globalPos(), pos, this);
    if (curMode == PaintPane::Paint) {
        endPoint = event->pos();

        if (curShape == Figure::None) {
            isDrawing = false;
            nonePoints.push_back(endPoint);
            //writeBuffer(buffer, cbuffer, figures);
            paint(image, figures);
        }
        else {
            tempImage = image;
            //tempBuffer = buffer;
            tempFigures = figures;
            tmpPolygonPoints = polygonPoints;
            tmpCurvePoints = curvePoints;
            switch (curShape) {
            case Figure::None: {
                break;
            }
            case Figure::Line: {
                Line *line = new Line(startPoint, endPoint, line_algorithm);
                Shape *shape = line;
                tempFigures.push_back(shape);
                //Common::printPoint(startPoint);
                //Common::printPoint(endPoint);
                break;
            }
            case Figure::Curve: {
                qDebug()<<tr("Curve");
                Curve *curve = new Curve(tmpCurvePoints, curve_algorithm);
                Shape *shape = curve;
                tempFigures.push_back(shape);
                break;
            }
            case Figure::Circle: {
                int r = (int)Common::compute2PointDis(startPoint, endPoint);
                Circle *circle = new Circle(startPoint, r);
                Shape *shape = circle;
                tempFigures.push_back(shape);
                break;
            }
            case Figure::Oval: {
                int a = abs(startPoint.x() - endPoint.x());
                int b = abs(startPoint.y() - endPoint.y());
                Oval *oval = new Oval(startPoint, a, b);
                Shape *shape = oval;
                tempFigures.push_back(shape);
                break;
            }
            case Figure::Rectangle: {
                qDebug()<<tr("Rectangle");
                Rectangle *rectangle = new Rectangle(startPoint, endPoint);
                Shape *shape = rectangle;
                tempFigures.push_back(shape);
                break;
            }
            case Figure::Polygon: {
                qDebug()<<tr("Polygon");
                tmpPolygonPoints.push_back(endPoint);
                Polygon *polygon = new Polygon(tmpPolygonPoints, isDone);
                Shape *shape = polygon;
                tempFigures.push_back(shape);
                break;
            }
            case Figure::Cube: {
                qDebug()<<tr("Cube");
                break;
            }
            default: {
                qDebug()<<tr("unkonwn");
                break;
            }
            }
            tempFigures[tempFigures.size() - 1]->isChosen = true;
            tempFigures[tempFigures.size() - 1]->penColor = penColor;
            tempFigures[tempFigures.size() - 1]->brushColor = brushColor;
            //writeBuffer(tempBuffer, ctempBuffer, tempFigures);
            paint(tempImage, tempFigures);
        }
    }
    else {
        endPoint = event->pos();
        if (moveObject != -1) {
            switch(curOperate) {
            case Figure::Shift: {
                qDebug()<<tr("Shift");
                int dx = endPoint.x() - startPoint.x();
                int dy = endPoint.y() - startPoint.y();
//                qDebug("dx : %d, dy : %d", dx, dy);
                figures[moveObject]->move(dx, dy);
                break;
            }
            case Figure::Rotate: {
                //qDebug()<<tr("Rotate");
                double angle = Common::computeRotateDeg(lastPoint, endPoint, figures[moveObject]->getCenter());
                //qDebug("center : (%d, %d), degree : %lf", figures[moveObject]->getCenter().x(), figures[moveObject]->getCenter().y(), angle);
                figures[moveObject]->rotate(angle);
                lastPoint = endPoint;
                break;
            }
            case Figure::Trim: {
                //qDebug("triming");
                Rectangle *rectangle = new Rectangle(startPoint, endPoint);
                trim = rectangle;
                break;
            }
            case Figure::Scale: {
                qDebug()<<tr("Scale");
                figures[moveObject]->scale(lastPoint, endPoint);
                break;
            }
            default: {
                qDebug()<<tr("Unknown operation!");
                break;
            }
            }
            //writeBuffer(buffer, cbuffer, figures);
            paint(image, figures);
        }
        else if (curOperate == Figure::Trim) {
            //qDebug("triming");
            Rectangle *rectangle = new Rectangle(startPoint, endPoint);
            trim = rectangle;
            paint(image, figures);
        }
    }
}

void PaintPane::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (curMode == PaintPane::Paint) {
            isDrawing = false;
            endPoint = event->pos();
            switch (curShape) {
            case Figure::None: {
                break;
            }
            case Figure::Line: {
                Line *line = new Line(startPoint, endPoint, line_algorithm);
                qDebug("The two end-point is (%d,%d) and (%d,%d).\nThe slope of line : %lf", startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y(), Common::computeSlope(startPoint, endPoint));
                Shape *shape = line;
                figures.push_back(shape);
                break;
            }
            case Figure::Curve: {
                qDebug()<<tr("Curve");
                if (curvePoints.size() > 1)
                    figures.pop_back();
                curvePoints.push_back(endPoint);
                //qDebug("size:%d", curvePoints.size());
                if (curvePoints.size() > 1 && Common::compute2PointDis(endPoint, curvePoints[curvePoints.size() - 2]) < 3)
                    isDone = true;
                if (isDone)
                    curvePoints.pop_back();
                Curve *curve = new Curve(curvePoints, curve_algorithm);
                Shape *shape = curve;
                figures.push_back(shape);
                if (isDone) {
                    curvePoints.clear();
                    tmpCurvePoints.clear();
                    isDone = false;
                }
                else
                    figures[figures.size() - 1]->isChosen = true;
                break;
            }
            case Figure::Circle: {
                int r = (int)Common::compute2PointDis(startPoint, endPoint);
                Circle *circle = new Circle(startPoint, r);
                Shape *shape = circle;
                figures.push_back(shape);
                break;
            }
            case Figure::Oval: {
                int a = abs(startPoint.x() - endPoint.x());
                int b = abs(startPoint.y() - endPoint.y());
                Oval *oval = new Oval(startPoint, a, b);
                Shape *shape = oval;
                figures.push_back(shape);
                break;
            }
            case Figure::Rectangle: {
                Rectangle *rectangle = new Rectangle(startPoint, endPoint);
                Shape *shape = rectangle;
                figures.push_back(shape);
                break;
            }
            case Figure::Polygon: {
                qDebug()<<tr("Polygon");
                if (polygonPoints.size() > 1)
                    figures.pop_back();
                polygonPoints.push_back(endPoint);
                if (Common::compute2PointDis(endPoint, polygonPoints[0]) < 10)
                    isDone = true;
                Polygon *polygon = new Polygon(polygonPoints, isDone);
                Shape *shape = polygon;
                figures.push_back(shape);

                if (isDone) {
                    polygonPoints.clear();
                    tmpPolygonPoints.clear();
                    isDone = false;
                }
                else
                    figures[figures.size() - 1]->isChosen = true;
                break;
            }
            case Figure::Cube: {
                qDebug()<<tr("Cube");
                break;
            }
            default: {
                qDebug()<<tr("unkonwn");
                break;
            }
            }
            if (curShape != Figure::None) {
                figures[figures.size() - 1]->penColor = penColor;
                figures[figures.size() - 1]->brushColor = brushColor;
            }
            //writeBuffer(buffer, cbuffer, figures);
        }
        else {
            if (moveObject != -1) {
                //writeBuffer(buffer, cbuffer, figures);
                figures[moveObject]->reset();
                paint(image, figures);
            }
            if (curOperate == Figure::Fill) {
                qDebug("%d", moveObject);
                if (moveObject != -1) {
                    if (!figures[moveObject]->isFill) {
                        figures[moveObject]->isFill = true;
                        figures[moveObject]->lastSeedPoint = startPoint;
                        figures[moveObject]->seedPoint = startPoint;
                        figures[moveObject]->oldColor = image.pixelColor(startPoint);
                        figures[moveObject]->newColor = brushColor;
                    }
                    else {
                        figures[moveObject]->seedPoint = startPoint;
                        figures[moveObject]->oldColor = image.pixelColor(startPoint);
                        figures[moveObject]->newColor = brushColor;
                    }
                }
            }
            if (curOperate == Figure::Trim) {
                //qDebug("leave");
                isTrim = false;
                QList<QPoint> borderPoints;
                borderPoints.push_back(startPoint);
                borderPoints.push_back(endPoint);
                QList<Shape*> trimShape;
                //QList<int> removeIdx;
                if (moveObject != -1) {
                    if (figures[moveObject]->type == Figure::Line || figures[moveObject]->type == Figure::Polygon || figures[moveObject]->type == Figure::Rectangle) {
                        trimShape.append(figures[moveObject]->trim(borderPoints));
                        if (trimShape[0]->getPoints().size() > 1) {

                            qDebug("size: %d, %d", trimShape.size(), trimShape[0]->getPoints().size());
                            QColor tmppencolor = figures[moveObject]->penColor;
                            QColor tmpbrushcolor = figures[moveObject]->brushColor;
                            QPoint tmpseedPoint = figures[moveObject]->seedPoint;
                            QPoint tmplastseed = figures[moveObject]->lastSeedPoint;
                            QColor tmpoldcolor = figures[moveObject]->oldColor;
                            QColor tmpnewcolor = figures[moveObject]->newColor;
                            bool isfill = figures[moveObject]->isFill;
                            figures.removeAt(moveObject);
                            figures.append(trimShape);
                            moveObject = figures.size() - 1;
                            figures[figures.size() - 1]->isChosen = true;
                            figures[figures.size() - 1]->penColor = tmppencolor;
                            figures[figures.size() - 1]->brushColor = tmpbrushcolor;
                            figures[figures.size() - 1]->seedPoint = figures[figures.size() - 1]->center;
                            figures[figures.size() - 1]->lastSeedPoint = figures[figures.size() - 1]->center;
                            figures[figures.size() - 1]->oldColor = tmpoldcolor;
                            figures[figures.size() - 1]->newColor = tmpnewcolor;
                            figures[figures.size() - 1]->isFill = isfill;
                        }
                        else {
                            figures.removeAt(moveObject);
                        }
                    }
                    else {
                        int ret3 = QMessageBox::warning(this, tr("Warning"),
                                                                tr("Triming object must be line, polygon or rectangle!"), QMessageBox::Abort);
                        if(ret3 == QMessageBox::Abort) qDebug() << tr("Warning");
                    }
                }
                else {
                    int ret3 = QMessageBox::warning(this, tr("Warning"),
                                                            tr("Please choose one object!"), QMessageBox::Abort);
                    if(ret3 == QMessageBox::Abort) qDebug() << tr("Warning");
                }
                /*
                for (int i = 0; i < figures.size(); i++) {
                    if (figures[i]->type == Figure::Line || figures[i]->type == Figure::Polygon || figures[i]->type == Figure::Rectangle) {
                        trimShape.append(figures[i]->trim(borderPoints));
                        removeIdx.push_back(i);
                    }
                }
                for (int i = removeIdx.size() - 1; i >= 0; i--)
                    figures.removeAt(removeIdx[i]);
                figures.append(trimShape);
                */
            }
        }
        paint(image, figures);
    }
}

void PaintPane::zoom() {
    //printf("zoom\n");
    //scale *= 1.2;
    //update();
    int i;
    for (i = 0; i < figures.size(); i++)
        if (figures[i]->isChosen)
            break;
    if (i < figures.size()) {
        figures[i]->zoom(Shape::Zin);
        //writeBuffer(buffer, cbuffer, figures);
        paint(image, figures);
        figures[i]->reset();
    }
}

void PaintPane::minimize() {
    //scale /= 1.2;
    //update();
    int i;
    for (i = 0; i < figures.size(); i++)
        if (figures[i]->isChosen)
            break;
    if (i < figures.size()) {
        figures[i]->zoom(Shape::Zout);
        //writeBuffer(buffer, cbuffer, figures);
        paint(image, figures);
        figures[i]->reset();
    }
}

/*
void PaintPane::restore() {
    //scale = 1;
    //update();
    int i;
    for (i = 0; i < figures.size(); i++)
        if (figures[i]->isChosen)
            break;
    figures[i]->zoom(Shape::Zrestore);
    writeBuffer(buffer, figures);
    paint(image, buffer);
    figures[i]->reset();
}
*/

void PaintPane::doRotate() {
    angle += 90;
    update();
}

void PaintPane::doShear() {
    shear = 0.2;
    update();
}

void PaintPane::doClear() {
    image.fill(backColor);
    update();
}

void PaintPane::setShape(Figure::ShapeType shape) {
    curShape = shape;
}

void PaintPane::setOperate(Figure::OperateType operate) {
    curOperate = operate;
    switch(operate) {
    case Figure::Shift:
        setCursor(Qt::OpenHandCursor);
        update();
        break;
    /*
    case Figure::Rotate: {
        QCursor cursor;
        QPixmap pixmap(":/images/images/rotation.png");
        QSize picSize(24, 24);
        //缩放图片，按比例
        QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
        //设置样式
        cursor = QCursor(scaledPixmap, -1, -1);
        setCursor(cursor);
        update();
        break;
    }
    */
    case Figure::Fill: {
        QCursor cursor;
        QPixmap pixmap(":/images/images/fill.png");
        QSize picSize(24, 24);
        //缩放图片，按比例
        QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
        //设置样式
        cursor = QCursor(scaledPixmap, -1, -1);
        setCursor(cursor);
        update();
        break;
    }
    /*
    case Figure::Trim: {
        QCursor cursor;
        QPixmap pixmap(":/images/images/trim.png");
        QSize picSize(24, 24);
        //缩放图片，按比例
        QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
        //设置样式
        cursor = QCursor(scaledPixmap, -1, -1);
        setCursor(cursor);
        update();
        break;
    }
    */
    case Figure::Select:
        setCursor(Qt::ArrowCursor);
        update();
        break;
    /*
    case Figure::Scale: {
        QCursor cursor;
        QPixmap pixmap(":/images/images/zoom_in_out.png");
        QSize picSize(24, 24);
        //缩放图片，按比例
        QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
        //设置样式
        cursor = QCursor(scaledPixmap, -1, -1);
        setCursor(cursor);
        update();
    }
    */
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void PaintPane::setMode(PaintPane::Mode mode) {
    curMode = mode;
    if (mode == PaintPane::Paint) {
        setCursor(Qt::ArrowCursor);
        update();
    }
}

void PaintPane::clearFigures() {
    nonePoints.clear();
    figures.clear();
}

void PaintPane::deleteFigure() {
    if (moveObject >= 0 && moveObject < figures.size()) {
        figures.removeAt(moveObject);
        paint(image, figures);
        moveObject = -1;
    }
}

void PaintPane::setLineAlgorithm(int i) {
    if (i == 0)
        line_algorithm = Common::DDA;
    else
        line_algorithm = Common::Bresenham;
}

void PaintPane::setCurveAlgorithm(int i) {
    if (i == 0)
        curve_algorithm = Common::BSpline;
    else
        curve_algorithm = Common::Bezier;
}

void PaintPane::floodSeedFill(QImage &image, QPoint seedPoint, QColor oldColor, QColor newColor) {
    int height = image.height();
    int width = image.width();
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
    QStack<QPoint> stack;
    stack.push(seedPoint);
    while (!stack.empty()) {
        QPoint p = stack.pop();
        if (p.x() < 0 || p.x() > width || p.y() < 0 || p.y() > height)
            continue;
        if (image.pixelColor(p) == oldColor)
            image.setPixelColor(p, newColor);
        for (int i = 0; i < 4; i++) {
            QPoint q = QPoint(p.x() + dir[i][0], p.y() + dir[i][1]);
            if (q.x() < 0 || q.x() > width || q.y() < 0 || q.y() > height || image.pixelColor(q) != oldColor)
                continue;
            stack.push(q);
        }
    }
    update();
}

/*
void PaintPane::writeBuffer(QList<QPoint> &buf, QList<QColor> &cbuf, QList<Shape*> list) {
    buf.clear();
    for (int i=0; i<list.size();i++) {
        int s = buf.size();
        buf.append(list[i]->getPoints());
        int e = buf.size();
        for (int j = s; j < e; j++)
            cbuf.push_back(list[i]->penColor);
    }
    buf.append(nonePoints);
}
*/

void PaintPane::setPenStyle(Qt::PenStyle style) {
    penStyle = style;
}

void PaintPane::setPenWidth(int width) {
    penWidth = width;
}

void PaintPane::setPenColor(QColor color) {
    penColor = color;
}

void PaintPane::setBrushColor(QColor color) {
    brushColor = color;
}
