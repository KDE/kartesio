#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

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
#include "kaction.h"
#include "klocale.h"
#include "kactioncollection.h"
#include "kstandardaction.h"
#include "kapplication.h"

#include "kxmlguiwindow.h"

using namespace std;

class MainWindow : public KXmlGuiWindow
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

    void on_actionNew_triggered();
    void on_fitplot_stateChanged(int );
    void on_originalplot_stateChanged(int );
    void on_actionShow_example_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionSvg_triggered();
    void on_actionTex_triggered();

private:
    //Ui::MainWindow *ui;
    //Ui::MainWindow ui;
    Ui::centralWidget uid;
    Ui::centralWidget *ui;
    QString calculate(QTableWidget *table,  QLineEdit *func);
    QString solvex(char *yvalue, QString dnum);
    QString replacevar(char *yvalue, QString dnum, QString var);
    void drawpl();
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
