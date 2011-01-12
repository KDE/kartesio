#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTableWidget"
#include "QLineEdit"
#include "QString"
#include "QStringList"
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <QFileDialog>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptClass>
#include <QVarLengthArray>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "kplotwidget.h"
#include "kplotaxis.h"
#include "kplotobject.h"
#include "kplotpoint.h"
#include <kdebug.h>

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_xmin_valueChanged(double );
    void on_xmax_valueChanged(double );
    void on_ymin_valueChanged(double );
    void on_ymax_valueChanged(double );
    //void on_pushButton_2_clicked();
    void on_actionNew_triggered();

    void on_fitplot_stateChanged(int );

    void on_originalplot_stateChanged(int );

    void on_actionShow_example_triggered();

    void on_actionDraw_plot_triggered();

private:
    Ui::MainWindow *ui;
    QString calculate(QTableWidget *table,  QLineEdit *func);
    QString solvex(char *yvalue, QString dnum);
    QString replacevar(char *yvalue, QString dnum, QString var);
    //void plot(QTableWidget *table, QString function);
    void plot(QTableWidget *table, QString function, bool original, bool funz);
    QString redplot;
    QString greenplot;
    QString blueplot;
    QStringList oldcvalue;
    QString resfunz;
    int tryn;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    int width;
};

#endif // MAINWINDOW_H
