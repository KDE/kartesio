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
  
  QString calculate(QTableWidget *table,  QLineEdit *func); ///calculate fitting curve with regression algorithm
  QString trainNN(QTableWidget *table,  QComboBox *func, bool backprop, bool genalg); ///calculate fitting curve with neural network
  QString solvex(QString yvalue, QString dnum); ///solve equation in a variable
  QString replacevar(QString yvalue, QString dnum, QString var); ///substitute a variable with another value
  double rmsError(QTableWidget *table,  QString func); ///calculate rms error
  bool check(QString func); ///test the string to see if it contains dangerous chars
  
  QString m_greenPlot;  ///svg green line
  QString m_bluePlot;   ///svg blue line
  QString m_greenPlotLatex; ///latex green line
  QString m_bluePlotLatex; ///latex blue line
  QStringList m_oldValue;  ///temporary string list.
  QString m_resultFunction; ///the calculated function
  QString m_file; ///the file actually opened (or just saved)
  int m_tryNumber; ///counting how many times the routing have been run
  double m_xmin; ///minimumx x value
  double m_xmax; ///maximum x value
  double m_ymin; ///minimum y value
  double m_ymax;  ///maximum y value
  double m_resolution; ///resolution of the plot
  double m_rmserror;  ///last rms error calculated
  long m_maxIters; ///maximum number of iteration for neural network training
  int m_width; ///width of the plottable area
  QString m_myReport; ///last maxima report
};

