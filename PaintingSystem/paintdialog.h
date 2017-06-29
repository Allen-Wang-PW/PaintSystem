#ifndef PAINTDIALOG_H
#define PAINTDIALOG_H

#include <QWizard>
#include "paintpane.h"
namespace Ui {
class PaintDialog;
}

class PaintDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaintDialog(QWidget *parent = 0);
    ~PaintDialog();

    int getWidth();
    int getHeight();
    QColor getBackColor();

private slots:
    void on_colorBtn_clicked();

private:
    Ui::PaintDialog *ui;
    QColor backColor;

};

#endif // PAINTDIALOG_H
