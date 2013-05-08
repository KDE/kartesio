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
 *   the Free Software Foundation; either version 3 of the License, or     *
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

#include <QtGui/QMainWindow>
#include "QtGui/QTableWidget"
#include "QtGui/QLineEdit"
#include "QtCore/QString"
#include "QtCore/QStringList"
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptClass>
#include <QtCore/QVarLengthArray>
#include <QtGui/QComboBox>
#include <QtGui/QTableWidgetItem>
#include <KMessageBox>
#include <KFileDialog>

#include <KPlotWidget>
#include <KPlotAxis>
#include <KPlotObject>
#include <KPlotPoint>
#include <KDebug>
#include <KAction>
#include <KLocale>
#include <KActionCollection>
#include <KStandardAction>
#include <KApplication>
#include <KXmlGuiWindow>
#include <KLocalizedString>

#include <libzorbaneural/neuralnet.h>


using namespace std;

class Calculations
{

//private:
public:
    explicit Calculations();
    virtual ~Calculations();

    QString calculate(QTableWidget *table,  QLineEdit *func);
    QString trainNN(QTableWidget *table,  QComboBox *func, bool backprop, bool genalg);
    QString solvex(char *yvalue, QString dnum);
    QString replacevar(char *yvalue, QString dnum, QString var);
    double rmsError(QTableWidget *table,  QString func);

    QString m_greenPlot;
    QString m_bluePlot;
    QString m_greenPlotLatex;
    QString m_bluePlotLatex;
    QStringList m_oldValue;
    QString m_resultFunction;
    QString m_file;
    int m_tryNumber;
    double m_xmin;
    double m_xmax;
    double m_ymin;
    double m_ymax;
    double m_resolution;
    double m_rmserror;
    long m_maxIters;
    int m_width;
    QString m_myReport;
};

