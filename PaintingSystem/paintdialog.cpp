#include "paintdialog.h"
#include "ui_paintdialog.h"
#include <QDebug>
#include <QColorDialog>

PaintDialog::PaintDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaintDialog)
{
    ui->setupUi(this);

    backColor = Qt::white;
}

PaintDialog::~PaintDialog()
{
    delete ui;
}

int PaintDialog::getWidth()
{
    return ui->widthSpinBox->text().toInt();
}

int PaintDialog::getHeight()
{
    return ui->heightSpinBox->text().toInt();
}

QColor PaintDialog::getBackColor()
{
    return backColor;
}

void PaintDialog::on_colorBtn_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, tr("Color Dialog"));
    if (color.isValid())
    {
        backColor = color;
        qDebug()<<"backColor:"<<backColor;
        QPalette palette = ui->colortextBrowser->palette();
        palette.setColor(QPalette::Base, backColor);
        ui->colortextBrowser->setPalette(palette);
        PaintPane *p = new PaintPane();
        p->setPaneColor(backColor);
        update();
    }

}
