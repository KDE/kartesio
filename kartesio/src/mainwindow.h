#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "calculations.h"

using namespace std;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Openarg(QString filename);

private slots:
    void on_pushButton_clicked();
    void on_xmin_valueChanged(double );
    void on_xmax_valueChanged(double );
    void on_ymin_valueChanged(double );
    void on_ymax_valueChanged(double );

    void on_actionNew_triggered();
    void on_fitplot_stateChanged(int );
    void on_originalplot_stateChanged(int );
    void on_actionShow_example_triggered();
    void on_actionOpen_triggered();
    void on_actionReport_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionSvg_triggered();
    void on_actionTex_triggered();

private:
    Ui::centralWidget uid;
    Ui::centralWidget *ui;

    void drawpl();
    void plot(QTableWidget *table, QString function, bool original, bool funz);
    void Openfile();
    //Calculations *mycalcs ;
    Calculations mycalcs ;


};

#endif // MAINWINDOW_H
