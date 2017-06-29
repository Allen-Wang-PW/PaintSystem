#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintpane.h"
#include <QScrollArea>
#include "figure.h"
#include "threedwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void doNewFile();
    void doOpenFile();
    bool doSaveFile();
    bool doSaveAsFile();

private slots:
    void on_actionClose_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionAbout_triggered();

    void on_actionTechnical_Support_triggered();

    void on_actionDraw_Line_triggered();

    void on_actionDraw_Curve_triggered();

    void on_actionDraw_Polygon_triggered();

    void on_actionDraw_Circle_triggered();

    void on_actionDraw_Oval_triggered();

    void on_actionDraw_Cube_triggered();

    void on_actionTrim_triggered();

    void on_actionRotation_triggered();

    void on_actionShift_triggered();

    void on_actionZoom_triggered();

    void on_actionMinimize_triggered();

    //void on_actionRestore_triggered();

    //void on_actionStyle_triggered();

    void on_actionColor_triggered();

    void on_actionFill_triggered();

    void on_actionDraw_None_triggered();

    void on_actionDraw_Rectangle_triggered();

    void on_actionScale_triggered();

    void on_actionSelect_triggered();

    void on_actionDelete_triggered();

private:
    Ui::MainWindow *ui;
    int WindowHeight = 600;
    int WindowWidth = 900;

    int AreaWidth = 900;
    int AreaHeight = 535;

    bool isSaved;
    QString curFile;
    QColor curPenColor = Qt::black;
    QColor curBrushColor = Qt::black;

    PaintPane *pane;
    QScrollArea *scrollArea;

    bool querySave();
    bool saveFile(QString fileName);
};

#endif // MAINWINDOW_H
