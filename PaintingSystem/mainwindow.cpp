#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QToolButton>
#include <QSpinBox>
#include <QPushButton>
#include <QDir>
#include "figure.h"
#include <QColorDialog>
#include <QCoreApplication>
#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(WindowWidth, WindowHeight);

    pane = new PaintPane;
    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(pane);
    scrollArea->widget()->setMinimumSize(AreaWidth, AreaHeight);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setCentralWidget(scrollArea);

    isSaved = false;
    curFile = tr("untitled.png");
    pane->setMode(PaintPane::Paint);
    pane->setAttribute(Qt::WA_X11DoNotAcceptFocus);
    this->setFocus();
    pane->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::querySave()
{
    if (pane->isModified())
    {
        QMessageBox::StandardButton box;
        box = QMessageBox::warning(this, tr("Save File"), tr("The picture has been modified. Do you want to save it?"),
                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (box == QMessageBox::Yes)
            return doSaveFile();
        else if (box == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(QString fileName)
{
    if (pane->saveImage(fileName, "png"))
    {
        isSaved = true;
        return true;
    }
    return false;
}

void MainWindow::doNewFile()
{
    if (querySave())
    {
        PaintDialog dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            qDebug()<<dlg.getBackColor();
            int width = dlg.getWidth();
            int height = dlg.getHeight();
            pane->setPaneSize(width, height);
            pane->clearFigures();
            scrollArea->widget()->resize(width, height);
            pane->setPaneColor(dlg.getBackColor());
            isSaved = false;
        }
    }
}

void MainWindow::doOpenFile()
{
    if (querySave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File..."), QDir::currentPath());
        if (!fileName.isEmpty())
        {
            pane->openImage(fileName);
            scrollArea->widget()->resize(pane->getPaneSize());
            isSaved = true;
            curFile = fileName;
        }
    }
}

bool MainWindow::doSaveFile()
{
    if (isSaved)
    {
        return saveFile(curFile);
    }
    else
        return doSaveAsFile();
}

bool MainWindow::doSaveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), curFile);
    if (fileName.isEmpty())
    {
        return false;
    }
    else
        return saveFile(fileName);
}

void MainWindow::on_actionClose_triggered()
{
    int ret = QMessageBox::question(this, tr("Exit"), tr("Do you want to save before exit?"), QMessageBox::Save, QMessageBox::Close, QMessageBox::Cancel);
    if (ret == QMessageBox::Close)
        this->close();
    else if (ret == QMessageBox::Cancel)
        qDebug()<<tr("Cancel!");
    else {
        qDebug()<<tr("Save!");
    }
}

void MainWindow::on_actionOpen_triggered()
{
    doOpenFile();
}

void MainWindow::on_actionNew_triggered()
{
    doNewFile();
}

void MainWindow::on_actionSave_triggered()
{
    doSaveFile();
}

void MainWindow::on_actionSave_as_triggered()
{
    doSaveAsFile();
}

void MainWindow::on_actionAbout_triggered()
{
    qDebug()<<tr("To be done");
}

void MainWindow::on_actionTechnical_Support_triggered()
{
    qDebug()<<tr("To be done");
}

void MainWindow::on_actionDraw_None_triggered()
{
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(true);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::None);
}

void MainWindow::on_actionDraw_Line_triggered() {
    QStringList items;
    items << tr("DDA") << tr("Bresenhem");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Choose Line Algorithm"),tr("Select or Choose an algorithm"),items,0,true,&ok);
    int i = 1;
    if (ok) {
        qDebug()<<"algorithm:"<<item;
        if (item == items[0])
            i = 0;
        else
            i = 1;
    }
    pane->setLineAlgorithm(i);
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(true);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::Line);
}

void MainWindow::on_actionDraw_Curve_triggered()
{
    QStringList items;
    items << tr("Bezier")<< tr("BSpline");
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Choose Curve Algorithm"),tr("Select or Choose an algorithm"),items,0,true,&ok);
    int i = 1;
    if (ok) {
        qDebug()<<"algorithm:"<<item;
        if (item == items[0])
            i = 1;
        else
            i = 0;
    }
    pane->setCurveAlgorithm(i);
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(true);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::Curve);
}

void MainWindow::on_actionDraw_Rectangle_triggered()
{
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(true);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::Rectangle);
}

void MainWindow::on_actionDraw_Polygon_triggered()
{
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(true);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::Polygon);
}

void MainWindow::on_actionDraw_Circle_triggered()
{
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(true);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::Circle);
}

void MainWindow::on_actionDraw_Oval_triggered()
{
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(true);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::Oval);
}

void MainWindow::on_actionDraw_Cube_triggered()
{
    pane->setMode(PaintPane::Paint);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(true);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setShape(Figure::Cube);
    qDebug("in");
    threeDWindow *threeD;
    threeD = new threeDWindow();
    threeD->show();
}

void MainWindow::on_actionTrim_triggered()
{
    pane->setMode(PaintPane::Operate);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(true);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setOperate(Figure::Trim);
}

void MainWindow::on_actionRotation_triggered()
{
    pane->setMode(PaintPane::Operate);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(true);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setOperate(Figure::Rotate);
}

void MainWindow::on_actionShift_triggered()
{
    pane->setMode(PaintPane::Operate);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(true);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setOperate(Figure::Shift);
}

void MainWindow::on_actionScale_triggered()
{
    pane->setMode(PaintPane::Operate);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(true);

    ui->actionFill->setChecked(false);

    pane->setOperate(Figure::Scale);
}

void MainWindow::on_actionZoom_triggered()
{
    ui->actionZoom->setChecked(true);
    ui->actionMinimize->setChecked(false);
    //ui->actionRestore->setChecked(false);

    pane->zoom();

    ui->actionZoom->setChecked(false);
}

void MainWindow::on_actionMinimize_triggered()
{
    ui->actionZoom->setChecked(false);
    ui->actionMinimize->setChecked(true);
    //ui->actionRestore->setChecked(false);

    pane->minimize();
    ui->actionMinimize->setChecked(false);
}
/*
void MainWindow::on_actionRestore_triggered()
{
    ui->actionZoom->setChecked(false);
    ui->actionMinimize->setChecked(false);
    ui->actionRestore->setChecked(true);

    pane->restore();
    ui->actionRestore->setChecked(false);
}
*/
/*
void MainWindow::on_actionStyle_triggered()
{
    if (ui->actionStyle->isChecked())
        pane->setPenStyle(Qt::DotLine);
    else
        pane->setPenStyle(Qt::SolidLine);
}
*/
void MainWindow::on_actionColor_triggered()
{
    if (ui->actionColor->isChecked()) {
        QColor color = QColorDialog::getColor(Qt::white, this, tr("Color Dialog"));
        pane->setPenColor(color);
        curPenColor = color;
    }
    ui->actionColor->setChecked(false);
}

void MainWindow::on_actionFill_triggered() {
    pane->setMode(PaintPane::Operate);
    if (ui->actionFill->isChecked()) {
        QColor color = QColorDialog::getColor(Qt::white, this, tr("Color Dialog"));
        pane->setBrushColor(color);
        curBrushColor = color;
    }
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setOperate(Figure::Fill);
}

void MainWindow::on_actionSelect_triggered()
{
    pane->setMode(PaintPane::Operate);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(true);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);

    pane->setOperate(Figure::Select);
}

void MainWindow::on_actionDelete_triggered() {
    pane->setMode(PaintPane::Operate);
    pane->deleteFigure();
    pane->setOperate(Figure::Delete);
    ui->actionDraw_None->setChecked(false);
    ui->actionDraw_Cube->setChecked(false);
    ui->actionDraw_Circle->setChecked(false);
    ui->actionDraw_Curve->setChecked(false);
    ui->actionDraw_Line->setChecked(false);
    ui->actionDraw_Oval->setChecked(false);
    ui->actionDraw_Polygon->setChecked(false);
    ui->actionDraw_Rectangle->setChecked(false);

    ui->actionSelect->setChecked(false);
    ui->actionTrim->setChecked(false);
    ui->actionRotation->setChecked(false);
    ui->actionShift->setChecked(false);
    ui->actionScale->setChecked(false);

    ui->actionFill->setChecked(false);
    ui->actionDelete->setChecked(false);
}
