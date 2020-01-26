#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QColorDialog>
#include <QDebug>
#include <QVector>
#include <QTimer>
#include <QRectF>
#include <QPainter>
#include <QBrush>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void initailizeVec();

    void settingGrid();
    void randomColors();

    void startGradient(QColor start, QColor end);
    void addGradient(QColor next);

    void drawRectangles(QPainter &p);


private slots:
    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_spinBox_2_valueChanged(int arg1);
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    void paintEvent(QPaintEvent *e);

//    void on_radioButton_clicked();
//    void on_radioButton_2_clicked();
//    void on_radioButton_3_clicked();
//    void on_radioButton_4_clicked();

    void run();
    void run2();
    void run3();
    void run4();
    void run5();

    void on_comboBox_activated(int index);

    void on_pushButton_6_clicked();

    void on_horizontalSlider_valueChanged(int value);



    void on_horizontalSlider_2_valueChanged(int value);

    void on_checkBox_00_stateChanged(int arg1);
    void on_checkBox_01_stateChanged(int arg1);
    void on_checkBox_02_stateChanged(int arg1);
    void on_checkBox_03_stateChanged(int arg1);
    void on_checkBox_04_stateChanged(int arg1);
    void on_checkBox_05_stateChanged(int arg1);
    void on_checkBox_06_stateChanged(int arg1);

    void on_checkBox_10_stateChanged(int arg1);
    void on_checkBox_11_stateChanged(int arg1);
    void on_checkBox_12_stateChanged(int arg1);
    void on_checkBox_13_stateChanged(int arg1);
    void on_checkBox_14_stateChanged(int arg1);
    void on_checkBox_15_stateChanged(int arg1);
    void on_checkBox_16_stateChanged(int arg1);

    void on_checkBox_20_stateChanged(int arg1);
    void on_checkBox_21_stateChanged(int arg1);
    void on_checkBox_22_stateChanged(int arg1);
    void on_checkBox_23_stateChanged(int arg1);
    void on_checkBox_24_stateChanged(int arg1);
    void on_checkBox_25_stateChanged(int arg1);
    void on_checkBox_26_stateChanged(int arg1);

    void on_checkBox_30_stateChanged(int arg1);
    void on_checkBox_31_stateChanged(int arg1);
    void on_checkBox_32_stateChanged(int arg1);

    void on_checkBox_34_stateChanged(int arg1);
    void on_checkBox_35_stateChanged(int arg1);
    void on_checkBox_36_stateChanged(int arg1);

    void on_checkBox_40_stateChanged(int arg1);
    void on_checkBox_41_stateChanged(int arg1);
    void on_checkBox_42_stateChanged(int arg1);
    void on_checkBox_43_stateChanged(int arg1);
    void on_checkBox_44_stateChanged(int arg1);
    void on_checkBox_45_stateChanged(int arg1);
    void on_checkBox_46_stateChanged(int arg1);

    void on_checkBox_50_stateChanged(int arg1);
    void on_checkBox_51_stateChanged(int arg1);
    void on_checkBox_52_stateChanged(int arg1);
    void on_checkBox_53_stateChanged(int arg1);
    void on_checkBox_54_stateChanged(int arg1);
    void on_checkBox_55_stateChanged(int arg1);
    void on_checkBox_56_stateChanged(int arg1);

    void on_checkBox_60_stateChanged(int arg1);
    void on_checkBox_61_stateChanged(int arg1);
    void on_checkBox_62_stateChanged(int arg1);
    void on_checkBox_63_stateChanged(int arg1);
    void on_checkBox_64_stateChanged(int arg1);
    void on_checkBox_65_stateChanged(int arg1);
    void on_checkBox_66_stateChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_comboBox_2_currentIndexChanged(int index);

private:
    Ui::Widget *ui;

    QTimer *myTimer = new QTimer();

    int automataType = 0;
    int automataTypeTwo = 0;


    int frames = 10;
    int count = 0;

    int steps = 10;
    int stepsMore = 5;

    int height = 50;
    int width = 50;

    int rows = height;
    int cols = width;

    bool squares[7][7];

    float cellWidth = 500.0/width;
    float cellHeight = 500.0/height;

    int *grid = nullptr;
    int *nextGrid = nullptr;

    int threshold = 1;

    QVector<QColor> colorVec;


};

#endif // WIDGET_H
