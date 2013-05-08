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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : KXmlGuiWindow(parent)
{

    setWindowIcon( KIcon( "kartesio" ) ); 
    KAction* clearAction = new KAction(this);
    clearAction->setText(i18n("&New"));
    clearAction->setIcon(KIcon("document-new"));
    clearAction->setShortcut(Qt::CTRL + Qt::Key_N);
    actionCollection()->addAction("clear", clearAction);

    KAction* openAction = new KAction(this);
    openAction->setText(i18n("&Open"));
    openAction->setIcon(KIcon("document-open"));
    openAction->setShortcut(Qt::CTRL + Qt::Key_O);
    actionCollection()->addAction("open", openAction);

    KAction* saveAction = new KAction(this);
    saveAction->setText(i18n("&Save"));
    saveAction->setIcon(KIcon("document-save"));
    saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
    actionCollection()->addAction("save", saveAction);

    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

    KAction* saveasAction = new KAction(this);
    saveasAction->setText(i18n("&Save as"));
    saveasAction->setIcon(KIcon("document-save"));
    actionCollection()->addAction("saveas", saveasAction);

    KAction* plotAction = new KAction(this);
    plotAction->setText(i18n("Draw &Plot"));
    plotAction->setIcon(KIcon("kmplot"));
    plotAction->setShortcut(Qt::CTRL + Qt::Key_P);
    actionCollection()->addAction("plot", plotAction);

    KAction* exampleAction = new KAction(this);
    exampleAction->setText(i18n("Show &Example"));
    exampleAction->setShortcut(Qt::CTRL + Qt::Key_E);
    actionCollection()->addAction("example", exampleAction);

    KAction*expsvgAction = new KAction(this);
    expsvgAction->setText(i18n("Export as SVG"));
    expsvgAction->setIcon(KIcon("image-svg+xml"));
    actionCollection()->addAction("expsvg", expsvgAction);

    KAction*exptexAction = new KAction(this);
    exptexAction->setText(i18n("Export as Latex"));
    exptexAction->setIcon(KIcon("text-x-tex"));
    actionCollection()->addAction("exptex", exptexAction);

    KAction*reportAction = new KAction(this);
    reportAction->setText(i18n("Read Maxima Report"));
    reportAction->setIcon(KIcon("kate"));
    actionCollection()->addAction("report", reportAction);


    connect(clearAction,SIGNAL(triggered(bool)),this,SLOT(on_actionNew_triggered()));
    connect(plotAction,SIGNAL(triggered(bool)),this,SLOT(drawpl()));
    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(on_actionOpen_triggered()));
    connect(saveAction,SIGNAL(triggered(bool)),this,SLOT(on_actionSave_triggered()));
    connect(saveasAction,SIGNAL(triggered(bool)),this,SLOT(on_actionSaveAs_triggered()));
    connect(expsvgAction,SIGNAL(triggered(bool)),this,SLOT(on_actionSvg_triggered()));
    connect(exptexAction,SIGNAL(triggered(bool)),this,SLOT(on_actionTex_triggered()));
    connect(exampleAction,SIGNAL(triggered(bool)),this,SLOT(on_actionShow_example_triggered()));
    connect(reportAction,SIGNAL(triggered(bool)),this,SLOT(on_actionReport_triggered()));



    setupGUI(Default, "kartesioui.rc");
    QWidget *widget = new QWidget( this );

    // create the user interface, the parent widget is "widget"
    uid.setupUi(widget);
    setCentralWidget(widget);


    mycalcs.m_xmin = 0;
    mycalcs.m_xmax = 50;
    mycalcs.m_ymin = 0;
    mycalcs.m_ymax = 50;
    mycalcs.m_resolution = 1.0;
    mycalcs.m_maxIters = 2000000;
    mycalcs.m_rmserror = 0.0;
    mycalcs.m_width = int(mycalcs.m_xmax - mycalcs.m_xmin);
    mycalcs.m_file = "";

    plot(uid.tableWidget, "",  uid.originalplot->isChecked(),uid.fitplot->isChecked());


    connect( uid.pushButton,SIGNAL(clicked()),this, SLOT(on_pushButton_clicked()));
    connect( uid.pushButton_2,SIGNAL(clicked()),this, SLOT(on_pushButton_2_clicked()));
    connect( uid.xmin,SIGNAL(valueChanged(double)),this, SLOT(on_xmin_valueChanged(double)));
    connect( uid.xmax,SIGNAL(valueChanged(double)),this, SLOT(on_xmax_valueChanged(double)));
    connect( uid.ymin,SIGNAL(valueChanged(double)),this, SLOT(on_ymin_valueChanged(double)));
    connect( uid.ymax,SIGNAL(valueChanged(double)),this, SLOT(on_ymax_valueChanged(double)));
    connect( uid.resolution,SIGNAL(valueChanged(double)),this, SLOT(on_resolution_valueChanged(double)));
    connect( uid.maxIters,SIGNAL(valueChanged(double)),this, SLOT(on_maxiters_valueChanged(double)));
    connect( uid.fitplot,SIGNAL(stateChanged(int)),this, SLOT(on_fitplot_stateChanged(int)));
    connect( uid.originalplot,SIGNAL(stateChanged(int)),this, SLOT(on_originalplot_stateChanged(int)));

}

MainWindow::~MainWindow()
{
    //delete ui;
    //delete mycalcs;
}

void MainWindow::on_sort_clicked()
{
  for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
    if (!uid.tableWidget->item(i,0) || uid.tableWidget->item(i,0)->text().isEmpty()) {
      uid.tableWidget->removeRow(i);
    }
  }
  uid.tableWidget->sortItems(1, Qt::AscendingOrder);
}

void MainWindow::on_pushButton_clicked()
{
    QString value = mycalcs.calculate(uid.tableWidget,  uid.function);
    if  (value==QString("died")) KMessageBox::error(this,"Error","Seems that Maxima process died calculating the result.") ;
    if  (value!=QString("died")) {
        uid.result->setText(value);
        drawpl();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString value = mycalcs.trainNN(uid.tableWidget,  uid.comboBox, uid.backprop->isChecked(), uid.genalg->isChecked());
        uid.result->setText(value);
        drawpl();
}

void MainWindow::drawpl() {
    QString tempstr = "";
    if (!(uid.result->text().isEmpty())) tempstr = uid.result->text().split('=').at(1);
    plot(uid.tableWidget, tempstr, uid.originalplot->isChecked(),uid.fitplot->isChecked());
}

void MainWindow::on_actionReport_triggered() {
    KMessageBox::information(this,"Maxima Report",mycalcs.m_myReport);
}

void MainWindow::plot(QTableWidget *table, QString function, bool original, bool funz) {

    //this function plots the original points and the best fit curve
    mycalcs.m_width = int(mycalcs.m_xmax - mycalcs.m_xmin);
    //now I'm preparing the kplot widget
    uid.kplotwidget->removeAllPlotObjects();
    uid.kplotwidget->setLimits( mycalcs.m_xmin, mycalcs.m_xmax, mycalcs.m_ymin, mycalcs.m_ymax ); //now I need to set the limits of the plot
    
    KPlotObject *kpog = new KPlotObject( Qt::green, KPlotObject::Lines );
    KPlotObject *kpob = new KPlotObject( Qt::blue, KPlotObject::Lines );

    mycalcs.m_greenPlot = "<polyline points=\"";
    mycalcs.m_bluePlot = "<polyline points=\"";
    mycalcs.m_greenPlotLatex = "\\psline[linecolor=green, showpoints=false]";
    mycalcs.m_bluePlotLatex = "\\psline[linecolor=blue, showpoints=true]";

    if (!table->item(0,0) || table->item(0,0)->text().isEmpty())
    {
        //go on
    } else {
      
      if (uid.showerror->isChecked() && !(function.isEmpty())) {
	KPlotObject *kpol = new KPlotObject( Qt::white, KPlotObject::Points );
	kpol->setLabelPen(QPen( Qt::red, 3.0, Qt::SolidLine ) );
	QString error = "RMS error: ";
	//if the error is too little, it is quite zero
	double myerr = mycalcs.rmsError(table, function);
	if (myerr<pow(10,-10)) myerr = 0.0;
	error = error +  QString::number(myerr);
	//KMessageBox::information(this,"RMS error",error) ;
	double h = uid.ymax->value() - uid.ymin->value();
	kpol->addPoint(mycalcs.m_xmin+(mycalcs.m_width/50), mycalcs.m_ymax-(h/50), error, 0);
	uid.kplotwidget->addPlotObject(kpol);
      }
        //now we can plot the values
        QVarLengthArray<double, 64> px(table->rowCount());
        QVarLengthArray<double, 64> py(table->rowCount());
        int totaldata=0;
        for (int i=0; i<table->rowCount() ; i++) {
            if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                break;
            } else {
                totaldata++;
                if (original==true) kpob->addPoint(table->item(i,0)->data(Qt::DisplayRole).toDouble(), table->item(i,1)->data(Qt::DisplayRole).toDouble());
                mycalcs.m_bluePlot = mycalcs.m_bluePlot + ' ' + QString::number((table->item(i,0)->data(Qt::DisplayRole).toDouble()*10)+5).replace(QString(","), QString(".")) + ',' + QString::number((mycalcs.m_ymax-table->item(i,1)->data(Qt::DisplayRole).toDouble())*10).replace(QString(","), QString("."));
                mycalcs.m_bluePlotLatex = mycalcs.m_bluePlotLatex + '(' + QString::number((table->item(i,0)->data(Qt::DisplayRole).toDouble())).replace(QString(","), QString(".")) + ',' + QString::number((table->item(i,1)->data(Qt::DisplayRole).toDouble())).replace(QString(","), QString("."))+')';
            }
        }
    } 
 
 if ((funz==true) && (!(function.isEmpty()))) {
    //THIS IS THE PLOT OF BEST FIT CURVE
    double definition = mycalcs.m_resolution;
    double id = (mycalcs.m_xmin);
            int totalx = (int(mycalcs.m_xmax-mycalcs.m_xmin)*definition)+1;
	  //cout << totalx << "|" << endl;
	for (int i=0; i<totalx; i++) {
            QString mreporto = function;
            QScriptEngine myEngine;
            QByteArray ban = mreporto.toLatin1();
            char *tmreporto = ban.data();

            QString istr;
            istr.append(QString("%1").arg((id)));
            //now i'm using QScript language to solve the expression
            //in a future we can consider to change it supporting some backends, but it's really complex
            QString myscript = mycalcs.solvex(tmreporto,istr); //myscript is the equation converted in QScript language and with the value of x axis (istr) instead of "x" variable
	    QScriptValue three = myEngine.evaluate(myscript);

            double tvalue = three.toNumber();
	    if ((tvalue==tvalue) && (tvalue!=log(0)) && (tvalue!=-log(0))) kpog->addPoint((id), (tvalue));  //remember: tvalue == tvalue is needed to check if tvalue is NaN or not, while log(0) is the infinity
            mycalcs.m_greenPlot = mycalcs.m_greenPlot + ' ' + QString::number((id*10)+5).replace(QString(","), QString(".")) + ',' + QString::number((mycalcs.m_ymax-tvalue)*10).replace(QString(","), QString("."));
            mycalcs.m_greenPlotLatex = mycalcs.m_greenPlotLatex + '(' + QString::number(id).replace(QString(","), QString(".")) + ',' + QString::number((tvalue)).replace(QString(","), QString("."))+')';
	    id = id + (1/definition);
        }
uid.kplotwidget->addPlotObject(kpog);
}
    
 uid.kplotwidget->addPlotObject(kpob);   

    mycalcs.m_bluePlot = mycalcs.m_bluePlot + "\" style=\"stroke:blue;fill:none\"/> ";
    mycalcs.m_greenPlot = mycalcs.m_greenPlot + "\" style=\"stroke:green;fill:none\"/> ";

}

void MainWindow::on_xmin_valueChanged(double val)
{
    mycalcs.m_xmin = val;
    drawpl();
}

void MainWindow::on_xmax_valueChanged(double val)
{
    mycalcs.m_xmax = val;
    drawpl();
}

void MainWindow::on_ymin_valueChanged(double val)
{
    mycalcs.m_ymin = val;
    drawpl();
}

void MainWindow::on_ymax_valueChanged(double val)
{
    mycalcs.m_ymax = val;
    drawpl();
}

void MainWindow::on_resolution_valueChanged(double val)
{
    mycalcs.m_resolution = val;
    drawpl();
}

void MainWindow::on_maxiters_valueChanged(double val)
{
    mycalcs.m_maxIters = long(val);
}


void MainWindow::on_actionNew_triggered()
{
    //set all the table cells as empty ("")
    uid.tableWidget->setRowCount(50); 
    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        QTableWidgetItem *titem = new QTableWidgetItem ;
        titem->setText("");
        uid.tableWidget->setItem(i,0,titem);
        QTableWidgetItem *titemo = new QTableWidgetItem ;
        titemo->setText("");
        uid.tableWidget->setItem(i,1,titemo);
    }
    //uid.tableWidget->clearContents();
    uid.function->setText("") ;
    uid.result->setText("") ;
    uid.xmin->setValue(0.00);
    uid.xmax->setValue(50.00);
    uid.ymin->setValue(0.00);
    uid.ymax->setValue(50.00);
    uid.resolution->setValue(1.00);
    plot(uid.tableWidget, "", uid.originalplot->isChecked(),uid.fitplot->isChecked());
    setCaption("");
}

void MainWindow::on_fitplot_stateChanged(int )
{
    drawpl();
}

void MainWindow::on_originalplot_stateChanged(int )
{
    drawpl();
}

void MainWindow::on_actionShow_example_triggered()
{
    on_actionNew_triggered();
    QTableWidgetItem *titemo = uid.tableWidget->item(0,0);
    titemo = uid.tableWidget->item(0,0);
    titemo->setText("0");
    titemo = uid.tableWidget->item(0,1);
    titemo->setText("0");
    titemo = uid.tableWidget->item(1,0);
    titemo->setText("1.7");
    titemo = uid.tableWidget->item(1,1);
    titemo->setText("4");
    titemo = uid.tableWidget->item(2,0);
    titemo->setText("3.14");
    titemo = uid.tableWidget->item(2,1);
    titemo->setText("1");

    //uid.function->setText("y=(a*(x^3))+(b*(x^2))+(c*x)+d");
    uid.function->setText("y=a*sin(x)+b");
    uid.xmin->setValue(0.00);
    uid.xmax->setValue(4.00);
    uid.ymin->setValue(0.00);
    uid.ymax->setValue(5.00);
}

void MainWindow::on_actionOpen_triggered() {
    mycalcs.m_file = KFileDialog::getOpenFileName(this,"Open work","","Kartesio File (*.kartesio)");
    Openfile();
}

void MainWindow::Openarg(QString filename) {
    //remove the first 7 characters
    mycalcs.m_file = filename.remove(0,7);
    Openfile();
}

void MainWindow::Openfile() {
    //loads all the cells text from a file prevoiusly saved
    if (!(mycalcs.m_file.isEmpty())) {
        setCaption(mycalcs.m_file); //for some reason I don't get, this method doesn't work
        QByteArray bac = mycalcs.m_file.toLatin1();
        char *filec = bac.data();
        ifstream texto(filec);
        if (!texto) KMessageBox::error(this,"Error","Unable to open "+mycalcs.m_file) ;
        if (texto) {
            on_actionNew_triggered();
            QString tempyval;
            char tmpchr;
            int i = 0;
            QString tempya;
            int tablea = 0;
            int xax = 0;
	    int res = 0;
            do {
                texto >> tmpchr;
                if (tmpchr!='|') tempyval = tempyval + tmpchr;
                if (tmpchr=='|') {
                    if ((tablea==1) and (tempyval != QString("table1"))  and (tempyval != QString("function")) and (tempyval != QString("result")) ) {
                        if ((i%2)!=0) {
                            QTableWidgetItem *titemo = uid.tableWidget->item((i-1)/2,1) ;
                            if (titemo) titemo->setText(tempyval) ;
                        } else {
                            QTableWidgetItem *titem = uid.tableWidget->item((i/2),0) ;
                            if (titem) titem->setText(tempyval) ;
                        }
                        i++;
                    }


                    if ((xax==1) and (tempyval != QString("table1"))  and (tempyval != QString("function")) and (tempyval != QString("result")) ) {
                        uid.function->setText(tempyval);
                        xax = 0;
                    }
                    
                    if ((res==1) and (tempyval != QString("table1"))  and (tempyval != QString("function")) and (tempyval != QString("result"))  ) {
                        uid.result->setText(tempyval);
                        res = 0;
                    }

                    if (tempyval == QString("table1")) {
                        i=0;
                        tablea=1;
                        xax = 0;
			res = 0;
                    }
                    if (tempyval == QString("function"))  {
                        tablea=0;
                        xax = 1;
			res = 0;
                    }
                    if (tempyval == QString("result"))  {
                        tablea=0;
                        xax = 0;
			res = 1;
                    }

                    tempyval = "";
                }
            } while (!texto.eof());
            texto.close();
        }
    }

}

void MainWindow::on_actionSave_triggered() {
    if (mycalcs.m_file == "") {
        on_actionSaveAs_triggered();
        exit;
    }
    //save all the cells values

    QString tempyval;
    tempyval = "table1|";
    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
        QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
        if (!titem || titem->text().isEmpty()) {
            break;
        } else {
            QString yvaluerq = titemo->data(Qt::DisplayRole).toString() ;
            QString valuerq = titem->data(Qt::DisplayRole).toString() ;
            tempyval = tempyval + QString("\n") + valuerq + QString("|\n") + yvaluerq + QString("|");
        }
    }
    tempyval =  tempyval + QString("\nfunction|");
    tempyval = tempyval + QString("\n") + uid.function->text() + QString("|");
    tempyval =  tempyval + QString("\nresult|");
    tempyval = tempyval + QString("\n") + uid.result->text() + QString("|");

    tempyval =  tempyval + QString("\nthe end|\n");

    if (!(mycalcs.m_file.isEmpty())) {
        QByteArray ba = tempyval.toLatin1();
        char *strsave = ba.data();
        QByteArray bac = mycalcs.m_file.toLatin1();
        char *filec = bac.data();

        ofstream out(filec);
        if (!out) KMessageBox::error(this,"Error","Unable to create "+mycalcs.m_file) ;
        out << strsave;
        out.close();
    }
}
void MainWindow::on_actionSaveAs_triggered() {
    //save as
    mycalcs.m_file = KFileDialog::getSaveFileName(this,"Save work","","Kartesio File (*.kartesio)");
    setCaption(mycalcs.m_file);
    on_actionSave_triggered();
}
void MainWindow::on_actionSvg_triggered() {
    //This function saves the plot into a SVG file
    QString svgheader = "<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"no\"?> <!DOCTYPE svg PUBLIC \"-//W3C//Dtd SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/Dtd/svg11.dtd\"> <svg width=\""+ QString::number((mycalcs.m_xmax*10)+5)+ "\" height=\""+ QString::number((mycalcs.m_ymax*10)+5)+ "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\"><polyline points=\"5," + QString::number(mycalcs.m_ymax*10) + " " + QString::number((mycalcs.m_xmax*10)-5) + "," + QString::number(mycalcs.m_ymax*10) + " " + QString::number((mycalcs.m_xmax*10)-5) + "," + QString::number((mycalcs.m_ymax*10)-5) + " " + QString::number(mycalcs.m_xmax*10) + "," + QString::number(mycalcs.m_ymax*10) + " " + QString::number((mycalcs.m_xmax*10)-5) + "," + QString::number((mycalcs.m_ymax*10)+5) + " " + QString::number((mycalcs.m_xmax*10)-5) + "," + QString::number(mycalcs.m_ymax*10) + "\" style=\"stroke:black;fill:none\"/> <polyline points=\"5," + QString::number(mycalcs.m_ymax*10);
    svgheader += " 5,5 10,5 5,0 0,5 5,5\" style=\"stroke:black;fill:none\"/> ";
    QString svgcomplete = svgheader ;
    if (uid.fitplot->isChecked()) svgcomplete = svgcomplete + mycalcs.m_greenPlot ;
    if (uid.originalplot->isChecked()) svgcomplete = svgcomplete + mycalcs.m_bluePlot;
    svgcomplete = svgcomplete + "</svg> ";

    QString files = KFileDialog::getSaveFileName(this,"Save plot","","Svg image (*.svg)");
    if (!(files.isEmpty())) {
        QByteArray svgt = svgcomplete.toLatin1();
        char *strsave = svgt.data();
        QByteArray ban = files.toLatin1();
        char *filec = ban.data();

        ofstream out(filec);
        if (!out) KMessageBox::error(this,"Error","Unable to create "+files) ;
        out << strsave;
        out.close();
    }
}
void MainWindow::on_actionTex_triggered() {
    //save as latex

    QString texheader = "\\documentclass[a4paper,12pt]{article}\n \\usepackage{pst-plot}\n \\begin{document} \n \\psset{xunit=1mm,yunit=1mm} %you can make the plot bigger changing xunit and yunit value";
    QString texcomplete = texheader ;
    if (uid.fitplot->isChecked()) texcomplete = texcomplete + '\n' + mycalcs.m_greenPlotLatex ;
    if (uid.originalplot->isChecked()) texcomplete = texcomplete + '\n' + mycalcs.m_bluePlotLatex;
    texcomplete = texcomplete + QString(" \n \\[ ") + uid.result->text() + QString(" \\] \n \\end{document} ");

    QString files = KFileDialog::getSaveFileName(this,"Save plot","","Latex document (*.tex)");
    if (!(files.isEmpty())) {
        QByteArray tex = texcomplete.toLatin1();
        char *strsave = tex.data();
        QByteArray ban = files.toLatin1();
        char *filec = ban.data();

        ofstream out(filec);
        if (!out) KMessageBox::error(this,"Error","Unable to create "+files) ;
        out << strsave;
        out.close();
        if (out) KMessageBox::information(this,"Well done","Please take note that you can't use pdflatex to convert this file directly into a pdf file. You can convert it only into dvi,and then will be possible to create a pdf.") ;
    }

}
