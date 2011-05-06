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

#include <QtGui/QMainWindow>
#include "QtGui/QTableWidget"
#include "QtGui/QLineEdit"
#include "QtCore/QString"
#include "QtCore/QStringList"
#include <QtGui/QFileDialog>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptClass>
#include <QtCore/QVarLengthArray>
#include <QtGui/QMessageBox>
#include <QtGui/QTableWidgetItem>

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

class Calculations
{

//private:
public:
    explicit Calculations();
    virtual ~Calculations();

    QString calculate(QTableWidget *table,  QLineEdit *func);
    QString solvex(char *yvalue, QString dnum);
    QString replacevar(char *yvalue, QString dnum, QString var);

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
    int m_width;
    QString m_myReport;
};

