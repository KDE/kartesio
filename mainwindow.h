/***************************************************************************
 *   Kartesio is a program for calculating best fit curves with            *
 *   experimental points using regression algorithms or neural networks.   *
 *                                                                         *
 *                   Kartesio has been created by                          *
 *                Luca Tringali, TRINGALINVENT@libero.it                   *
 *                                                                         *
 *                    Copyright 2011-2013 Luca Tringali                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "calculations.h"

using namespace std;
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    //explicit MainWindow(QWidget *parent = 0);
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Openarg(QString filename); ///open file
    void opencsv(QString csvfile);
    QTableWidget* tmptable;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    //void on_sort_clicked();
    void on_xmin_valueChanged(double );
    void on_xmax_valueChanged(double );
    void on_ymin_valueChanged(double );
    void on_ymax_valueChanged(double );
    void on_resolution_valueChanged(double );
    void on_maxiters_valueChanged(double );

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
    void on_actionQuit_triggered();
    void drawpl();  ///draw plot

    void on_fittoorig_clicked();

    void on_totalrows_valueChanged(int arg1);

    void on_sortxaxis_clicked();

    void on_resolution_valueChanged(int arg1);
    void on_actionFind_triggered();
    void on_actionReplace_triggered();

    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
    void on_actionAboutZorbaNN_triggered();
    void on_autorecognition_triggered();
    void on_loadcsv_clicked();
    void on_savenet_triggered();
    void on_loadnet_triggered();

    void on_exportcsv_clicked();
    void on_xrangecng(QCPRange newRange);
    void on_yrangecng(QCPRange newRange);
    int autorecofinished();

private:
    Ui::centralWidget uid;
    QToolBar* toolbar;
    QActionGroup* actionCollection;
    QJsonObject configuration;
    QString netcheckalert;
    QString genalglimitalert;
    int genalglimit;
    double myerr;

    void delay( int millisecondsToWait );
    void plot(QTableWidget* table, QString function, bool original, bool funz, bool fittoorig = false); ///draw plot
    void Openfile();  ///open file (without argument)
    void savecsv(QString filen);
    Calculations mycalcs ; //object from class Calculations


    int do_autorecognition(QString att);

    QFutureWatcher<int> watcher;

};

class TableNumberItem : public QTableWidgetItem
{
public:
    TableNumberItem(const QString txt = QString("0"))
        :QTableWidgetItem(txt)
    {
    }
    bool operator <(const QTableWidgetItem &other) const
    {
        QString str1 = text();
        QString str2 = other.text();

        if (str1[0] == '$' || str1[0] == '€') {
            str1.remove(0, 1);
            str2.remove(0, 1); // we assume both items have the same format
        }

        if (str1[str1.length() - 1] == '%') {
            str1.chop(1);
            str2.chop(1); // this works for "N%" and for "N %" formatted strings
        }

        double f1 = str1.toDouble();
        double f2 = str2.toDouble();

        return str1.toDouble() < str2.toDouble();
    }
};

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(QString filen);
    ~Worker();
    QString weightsfile;

public slots:
    void process();

signals:
    void finished();
    void error(QString err);

private:
    // add your variables here
    void delay( int millisecondsToWait );
    QMainWindow* myQMainWindow;
    QVBoxLayout *layout;
    QLabel* lineed;
};
#endif // MAINWINDOW_H
