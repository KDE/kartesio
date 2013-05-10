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

#include "src/ui_mainwindow.h"
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
    void on_pushButton_2_clicked();
    void on_sort_clicked();
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
    void drawpl();

private:
    Ui::centralWidget uid;
    Ui::centralWidget *ui;

    void plot(QTableWidget* table, QString function, bool original, bool funz);
    void Openfile();
    Calculations mycalcs ;


};

#endif // MAINWINDOW_H
