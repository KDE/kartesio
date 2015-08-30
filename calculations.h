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

#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <vector>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QLineEdit>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptClass>
#include <QtCore/QVarLengthArray>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableWidgetItem>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QSize>
#include <QMenu>
#include <QFutureWatcher>
#include <QTextEdit>
#include <QTextDocument>

#include <QtConcurrent/QtConcurrent>
#include <QTimer>
#include <QTime>
#include <QFuture>
#include <QThread>
#include <QDebug>
#include <QAction>
#include <QLocale>
#include <QTranslator>
#include <QApplication>
#include <QCoreApplication>

#include "qcustomplot/qcustomplot.h"

#include "libzorbaneural/neuralnet.h"

using namespace std;

class Calculations
{

    //private:
public:
    explicit Calculations();
    virtual ~Calculations();
    /** calculate fitting curve with regression algorithm */
    QString calculate(QTableWidget *table,  QLineEdit *func);
    /** calculate fitting curve with neural network */
    QString trainNN(QTableWidget *table,  QComboBox *func, bool backprop, bool genalg);
    /** solve equation in a variable */
    QString solvex(QString yvalue, QString dnum);
    /** substitute a variable with another value */
    QString replacevar(QString yvalue, QString dnum, QString var);
    /** calculate rms error */
    double rmsError(QTableWidget *table,  QString func);
    /** test the string to see if it contains dangerous chars */
    bool check(QString func);
    /** maxima path*/
    QString maximap;

    /** svg green line */
    QString m_greenPlot;
    /** svg blue line */
    QString m_bluePlot;
    /** latex green line */
    QString m_greenPlotLatex;
    /** latex blue line */
    QString m_bluePlotLatex;
    /** temporary string list */
    QStringList m_oldValue;
    /** the calculated function */
    QString m_resultFunction;
    /** the file actually opened (or just saved) */
    QString m_file;
    /** counting how many times the routing have been run */
    int m_tryNumber;
    /** minimumx x value */
    double m_xmin;
    /** maximum x value */
    double m_xmax;
    /** minimum y value */
    double m_ymin;
    /** maximum y value */
    double m_ymax;
    /** plot resolution */
    double m_resolution;
    /** last rms error calculated */
    double m_rmserror;
    /** maximum number of iteration for neural network training */
    long m_maxIters;
    /** width of the plottable area */
    int m_width;
    /** last Maxima report */
    QString m_myReport;
    /** the place where I have save the network*/
    QString NNsavename;
    /** Would you like to view the weights in real time?*/
    QString weightssavename;
    bool viewweights;
    /** Would you like to load a pre-saved network?*/
    QString loadnetwork;

};
