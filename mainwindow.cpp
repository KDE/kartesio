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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    uid()
{

    setWindowTitle("Kartesio");
    setWindowIcon( QIcon( ":/icons/hi256-app-kartesio.png" ) );


    actionCollection = new QActionGroup(this);
    actionCollection->setExclusive(false);

    QAction* clearAction = new QAction(this);
    clearAction->setText(tr("&New"));
    clearAction->setIcon(QIcon(":/icons/document-new.png"));
    clearAction->setShortcut(Qt::CTRL | Qt::Key_N);
    actionCollection->addAction(clearAction);
    
    QAction* openAction = new QAction(this);
    openAction->setText(tr("&Open"));
    openAction->setIcon(QIcon(":/icons/document-open.png"));
    openAction->setShortcut(Qt::CTRL | Qt::Key_O);
    actionCollection->addAction(openAction);

    QAction* saveAction = new QAction(this);
    saveAction->setText(tr("&Save"));
    saveAction->setIcon(QIcon(":/icons/document-save.png"));
    saveAction->setShortcut(Qt::CTRL | Qt::Key_S);
    actionCollection->addAction(saveAction);

    QAction* quitAction = new QAction(this);
    quitAction->setText(tr("&Quit"));
    quitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
    quitAction->setIcon(QIcon(":/icons/application-exit.png"));
    //actionCollection->addAction(quitAction);
    
    QAction* saveasAction = new QAction(this);
    saveasAction->setText(tr("&Save as"));
    saveasAction->setIcon(QIcon(":/icons/document-save-as.png"));
    //actionCollection->addAction(saveasAction);

    QAction* plotAction = new QAction(this);
    plotAction->setText(tr("Draw &Plot"));
    plotAction->setIcon(QIcon(":/icons/kmplot.png"));
    plotAction->setShortcut(Qt::CTRL | Qt::Key_P);
    actionCollection->addAction(plotAction);

    QAction*expsvgAction = new QAction(this);
    expsvgAction->setText(tr("Export as SVG"));
    expsvgAction->setIcon(QIcon(":/icons/image-svg+xml.png"));
    actionCollection->addAction(expsvgAction);

    QAction*exptexAction = new QAction(this);
    exptexAction->setText(tr("Export as Latex"));
    exptexAction->setIcon(QIcon(":/icons/text-x-tex.png"));
    actionCollection->addAction(exptexAction);

    QAction*reportAction = new QAction(this);
    reportAction->setText(tr("Read Maxima Report"));
    reportAction->setIcon(QIcon(":/icons/kate.png"));
    //actionCollection->addAction(reportAction);

    QAction*autorecognitionAction = new QAction(this);
    autorecognitionAction->setText(tr("Automatic recognize shape"));
    //autorecognitionAction->setIcon(QIcon(":/icons/kate.png"));

    QAction*savenetAction = new QAction(this);
    savenetAction->setText(tr("Save current network in file"));

    QAction*loadnetAction = new QAction(this);
    loadnetAction->setText(tr("Load network form file and run"));
    //autorecognitionAction->setIcon(QIcon(":/icons/kate.png"));

    QAction* exampleAction = new QAction(this);
    exampleAction->setText(tr("Show &Example"));
    exampleAction->setShortcut(Qt::CTRL | Qt::Key_E);
    //actionCollection->addAction(exampleAction);

    QAction* findAction = new QAction(this);
    findAction->setText(tr("&Find"));
    findAction->setShortcut(Qt::CTRL | Qt::Key_F);

    QAction* replaceAction = new QAction(this);
    replaceAction->setText(tr("&Replace All"));
    replaceAction->setShortcut(Qt::CTRL | Qt::Key_R);

    QAction* opencsvAction = new QAction(this);
    opencsvAction->setText(tr("Load CSV"));

    QAction* savecsvAction = new QAction(this);
    savecsvAction->setText(tr("Export CSV"));


    QAction* aboutAction = new QAction(this);
    aboutAction->setText(tr("About Kartesio"));

    QAction* aboutQtAction = new QAction(this);
    aboutQtAction->setText(tr("About Qt"));

    QAction* aboutZorbaNNAction = new QAction(this);
    aboutZorbaNNAction->setText(tr("About ZorbaNN"));

    QMenu* fileMenu = new QMenu("File", this);
    QMenu* dataMenu = new QMenu("Data", this);
    QMenu* NNMenu = new QMenu("Neural Network", this);
    QMenu* plotMenu = new QMenu("Plot", this);
    QMenu* helpMenu = new QMenu("Help", this);
    fileMenu->addAction(clearAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveasAction);
    fileMenu->addAction(quitAction);
    dataMenu->addAction(findAction);
    dataMenu->addAction(replaceAction);
    dataMenu->addAction(opencsvAction);
    dataMenu->addAction(savecsvAction);
    NNMenu->addAction(autorecognitionAction);
    NNMenu->addAction(savenetAction);
    NNMenu->addAction(loadnetAction);
    plotMenu->addAction(plotAction);
    plotMenu->addAction(expsvgAction);
    plotMenu->addAction(exptexAction);
    plotMenu->addAction(reportAction);
    helpMenu->addAction(exampleAction);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutZorbaNNAction);
    helpMenu->addAction(aboutQtAction);
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(dataMenu);
    menuBar()->addMenu(NNMenu);
    menuBar()->addMenu(plotMenu);
    menuBar()->addMenu(helpMenu);

    toolbar = new QToolBar("MainToolBar", this);
    toolbar->addActions(actionCollection->actions());
    toolbar->setIconSize(QSize(48,48));
    addToolBar(Qt::TopToolBarArea, toolbar);

    connect(clearAction,SIGNAL(triggered(bool)),this,SLOT(on_actionNew_triggered()));
    connect(plotAction,SIGNAL(triggered(bool)),this,SLOT(drawpl()));
    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(on_actionOpen_triggered()));
    connect(saveAction,SIGNAL(triggered(bool)),this,SLOT(on_actionSave_triggered()));
    connect(saveasAction,SIGNAL(triggered(bool)),this,SLOT(on_actionSaveAs_triggered()));
    connect(expsvgAction,SIGNAL(triggered(bool)),this,SLOT(on_actionSvg_triggered()));
    connect(exptexAction,SIGNAL(triggered(bool)),this,SLOT(on_actionTex_triggered()));
    connect(exampleAction,SIGNAL(triggered(bool)),this,SLOT(on_actionShow_example_triggered()));
    connect(reportAction,SIGNAL(triggered(bool)),this,SLOT(on_actionReport_triggered()));
    connect(quitAction,SIGNAL(triggered(bool)),this,SLOT(on_actionQuit_triggered()));
    connect(findAction,SIGNAL(triggered(bool)),this,SLOT(on_actionFind_triggered()));
    connect(replaceAction,SIGNAL(triggered(bool)),this,SLOT(on_actionReplace_triggered()));
    connect(opencsvAction,SIGNAL(triggered(bool)),this,SLOT(on_loadcsv_clicked()));
    connect(savecsvAction,SIGNAL(triggered(bool)),this,SLOT(on_exportcsv_clicked()));
    connect(aboutAction,SIGNAL(triggered(bool)),this,SLOT(on_actionAbout_triggered()));
    connect(aboutQtAction,SIGNAL(triggered(bool)),this,SLOT(on_actionAboutQt_triggered()));
    connect(aboutZorbaNNAction,SIGNAL(triggered(bool)),this,SLOT(on_actionAboutZorbaNN_triggered()));
    connect(autorecognitionAction,SIGNAL(triggered(bool)),this,SLOT(on_autorecognition_triggered()));
    connect(savenetAction,SIGNAL(triggered(bool)),this,SLOT(on_savenet_triggered()));
    connect(loadnetAction,SIGNAL(triggered(bool)),this,SLOT(on_loadnet_triggered()));

    QWidget *widget = new QWidget( this );
    uid.setupUi(widget);
    setCentralWidget(widget);


    mycalcs.m_xmin = 0;
    mycalcs.m_xmax = 50;
    mycalcs.m_ymin = 0;
    mycalcs.m_ymax = 50;
    mycalcs.m_resolution = 10;
    mycalcs.m_maxIters = 2000000;
    mycalcs.m_rmserror = 0.0;
    mycalcs.m_width = int(mycalcs.m_xmax - mycalcs.m_xmin);
    mycalcs.m_file = "";
    genalglimit = 2000;

    mycalcs.NNsavename = "";
    mycalcs.loadnetwork = "";
    QTemporaryFile file;
    if (file.open()) {
        mycalcs.NNsavename = file.fileName();
    }
    mycalcs.NNsavename = mycalcs.NNsavename +"znn";

    mycalcs.viewweights = false;
    QTemporaryFile filen;
    if (filen.open()) {
        mycalcs.weightssavename = filen.fileName();
    }
    mycalcs.weightssavename = mycalcs.weightssavename +".zwf";
    qDebug() << "Eventually saving weights in " << mycalcs.weightssavename;
    qDebug() << "You can watch weights on a Unix system with this command: " << QString("watch -n 1 cat ")+mycalcs.weightssavename;

    uid.totalrows->setValue(50);
    uid.resolution->setValue(10);
    
    plot(uid.tableWidget, "",  uid.originalplot->isChecked(),uid.fitplot->isChecked());
    
    
    connect( uid.pushButton,SIGNAL(clicked()),this, SLOT(on_pushButton_clicked()));
    connect( uid.pushButton_2,SIGNAL(clicked()),this, SLOT(on_pushButton_2_clicked()));
    connect( uid.xmin,SIGNAL(valueChanged(double)),this, SLOT(on_xmin_valueChanged(double)));
    connect( uid.xmax,SIGNAL(valueChanged(double)),this, SLOT(on_xmax_valueChanged(double)));
    connect( uid.ymin,SIGNAL(valueChanged(double)),this, SLOT(on_ymin_valueChanged(double)));
    connect( uid.ymax,SIGNAL(valueChanged(double)),this, SLOT(on_ymax_valueChanged(double)));
    connect( uid.maxIters,SIGNAL(valueChanged(double)),this, SLOT(on_maxiters_valueChanged(double)));
    connect( uid.fitplot,SIGNAL(stateChanged(int)),this, SLOT(on_fitplot_stateChanged(int)));
    connect( uid.originalplot,SIGNAL(stateChanged(int)),this, SLOT(on_originalplot_stateChanged(int)));
    connect( uid.fittoorig,SIGNAL(clicked()),this, SLOT(on_fittoorig_clicked()));
    connect( uid.totalrows,SIGNAL(valueChanged(int)),this, SLOT(on_totalrows_valueChanged(int)));
    connect( uid.sortxaxis,SIGNAL(clicked()),this, SLOT(on_sortxaxis_clicked()));
    connect( uid.resolution,SIGNAL(valueChanged(int)),this, SLOT(on_resolution_valueChanged(int)));
    connect( uid.loadcsv,SIGNAL(clicked()),this, SLOT(on_loadcsv_clicked()));
    connect( uid.exportcsv,SIGNAL(clicked()),this, SLOT(on_exportcsv_clicked()));

    connect( uid.qcplotwidget->xAxis,SIGNAL(rangeChanged(QCPRange)),this, SLOT(on_xrangecng(QCPRange)));
    connect( uid.qcplotwidget->yAxis,SIGNAL(rangeChanged(QCPRange)),this, SLOT(on_yrangecng(QCPRange)));

    connect(&watcher, SIGNAL(finished()), this, SLOT(autorecofinished()));

    on_actionNew_triggered();

    mycalcs.maximap = "";
    QString configfile = QDir::home().absolutePath() + "/kartesioconfig";
    qDebug()<< "Using config file " << configfile;
    if (QFile(configfile).exists()){
        QString val;
        QFile file;
        file.setFileName(configfile);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        QJsonDocument configd = QJsonDocument::fromJson(val.toUtf8());
        configuration = configd.object();
        QJsonValue subobj = configuration["maximapath"];
        mycalcs.maximap = subobj.toString();
    }

    if ((mycalcs.maximap.isEmpty()) || (!QFile(mycalcs.maximap).exists())) {
        mycalcs.maximap = "/usr/bin/maxima";
        if (!QFile(mycalcs.maximap).exists()) {
            mycalcs.maximap = QDir::currentPath()+ "/Maxima/bin/maxima.bat";
            if (!QFile(mycalcs.maximap).exists()) {
                mycalcs.maximap = QInputDialog::getText(this, tr("Maxima not found"), tr("I couldn't find the program 'maxima'. If you know its position, write it here:"), QLineEdit::Normal);

                QString jsonconf = "{ \n \"maximapath\": \"" + mycalcs.maximap + "\"\n }";
                QFile fileco(configfile);
                if (fileco.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&fileco);
                    out << jsonconf;
                    fileco.close();
                }

            }
        }
    }

}

MainWindow::~MainWindow()
{
    //delete ui;
    //delete mycalcs;
}


void MainWindow::on_pushButton_clicked()
{
    if (uid.function->text().contains(QString("**"))) {
        QMessageBox::critical(this,tr("Wrong spell"),tr("Please, do not use '**' operator. If you need elevation to power, use '^'."));
        return;
    }
    if (mycalcs.check(uid.function->text())==false) return;
    if (!(uid.function->text().at(0)=='y' and uid.function->text().at(1)=='=')) return; //the function must start with "y="
    if (uid.function->text().lastIndexOf("=")!=1) return;  //we need only one "=" and it must be the second char
    
    QString value = mycalcs.calculate(uid.tableWidget,  uid.function);
    if  (value==QString("died")) QMessageBox::critical(this,tr("Error"),tr("Seems that Maxima process died calculating the result.")) ;
    if  (value!=QString("died")) {
        uid.result->setText(value);
        drawpl();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (uid.genalg->isChecked() && uid.maxIters->value()>genalglimit && genalglimitalert != "done") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Suggestion"),tr("You have set a number of interations higher than ")+QString::number(genalglimit)+tr(" with genetic algorithms. This could take a lot of time. Are you sure want to proceed?"), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
        genalglimitalert = "done";
    }

    bool netcheck = true;
    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        if (!uid.tableWidget->item(i,0) || uid.tableWidget->item(i,0)->text().isEmpty()) {
            break;
        } else {
            if (uid.tableWidget->item(i,0)->text().toDouble()<0 or uid.tableWidget->item(i,0)->text().toDouble()>1 or uid.tableWidget->item(i,1)->text().toDouble()<0 or uid.tableWidget->item(i,1)->text().toDouble()>1 ) netcheck = false;
        }
    }
    if (netcheck==false) {
        //appears only at the first run
        if (netcheckalert != "done") QMessageBox::information(this, tr("Warning"),tr("The neural network works better if used only with points between 0 and 1: you can divide them for a power of 10, to make them be in this interval. The network will try anyway to work with the points you have provided, but the resulting curve may not be perfect."));
        netcheckalert = "done";
        /*reply = QMessageBox::question(this, tr("Warning"),tr("The neural network is able to use correctly only points between 0 and 1: you can divide them for a power of 10, to make them be in this interval. The network will try anyway to work with the points you have provided, but the resulting curve may not be correct. Would you like to automatically divide the numbers into the 0-1 range"), QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {

        for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
            QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
            QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
            if (!titem || titem->text().isEmpty()) {
                //break;
            } else {
                double yvaluerq = titemo->data(Qt::DisplayRole).toDouble() ;
                double valuerq = titem->data(Qt::DisplayRole).toDouble() ;
            }
        }
                }
        }*/
    }
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
    if (mycalcs.m_myReport.isEmpty()) QMessageBox::information(this,"Maxima Report",tr("No report to show."));
    if (!mycalcs.m_myReport.isEmpty()) QMessageBox::information(this,"Maxima Report",mycalcs.m_myReport);
}

void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();
}


void MainWindow::plot(QTableWidget *table, QString function, bool original, bool funz, bool fittoorig) {

    //this function plots the original points and the best fit curve
    mycalcs.m_width = int(mycalcs.m_xmax - mycalcs.m_xmin);

    //now I'm preparing the kplot widget
    uid.qcplotwidget->clearGraphs();
    uid.qcplotwidget->clearItems();
    //now I need to set the limits of the plot
    uid.qcplotwidget->xAxis->setRange(mycalcs.m_xmin, mycalcs.m_xmax);
    uid.qcplotwidget->yAxis->setRange(mycalcs.m_ymin, mycalcs.m_ymax);

    uid.qcplotwidget->addGraph();
    uid.qcplotwidget->graph(0)->setPen(QPen(Qt::green));

    uid.qcplotwidget->addGraph();
    uid.qcplotwidget->graph(1)->setPen(QPen(Qt::blue));
    uid.qcplotwidget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    
    mycalcs.m_greenPlot = "<polyline points=\"";
    mycalcs.m_bluePlot = "<polyline points=\"";
    mycalcs.m_greenPlotLatex = "\\psline[linecolor=green, showpoints=false]";
    mycalcs.m_bluePlotLatex = "\\psline[linecolor=blue, showpoints=true]";
    
    if (!table->item(0,0) || table->item(0,0)->text().isEmpty())
    {
        //go on
    } else {
        
        if (uid.showerror->isChecked() && !(function.isEmpty())) {
            QString error = tr("RMS error: ");
            //if the error is too little, it is quite zero
            myerr = mycalcs.rmsError(table, function);
            if (myerr<pow(10,-10)) myerr = 0.0;
            error = error +  QString::number(myerr);
            QCPItemText *textLabelRMS = new QCPItemText(uid.qcplotwidget);
            uid.qcplotwidget->addItem(textLabelRMS);
            textLabelRMS->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabelRMS->position->setType(QCPItemPosition::ptAxisRectRatio);
            textLabelRMS->position->setCoords(0.5, 0); // place position at center/top of axis rect
            textLabelRMS->setText(error);
            textLabelRMS->setFont(QFont(font().family(), 10));
            textLabelRMS->setPen(QPen(Qt::red)); // show black border around text
        }
        //now we can plot the values
        int totaldata=0;
        for (int i=0; i<table->rowCount() ; i++) {
            if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                break;
            } else {
                totaldata++;
                if (original==true) uid.qcplotwidget->graph(1)->addData(table->item(i,0)->data(Qt::DisplayRole).toDouble(), table->item(i,1)->data(Qt::DisplayRole).toDouble());
                mycalcs.m_bluePlot = mycalcs.m_bluePlot + ' ' + QString::number((table->item(i,0)->data(Qt::DisplayRole).toDouble()*10)+5).replace(QString(","), QString(".")) + ',' + QString::number((mycalcs.m_ymax-table->item(i,1)->data(Qt::DisplayRole).toDouble())*10).replace(QString(","), QString("."));
                mycalcs.m_bluePlotLatex = mycalcs.m_bluePlotLatex + '(' + QString::number((table->item(i,0)->data(Qt::DisplayRole).toDouble())).replace(QString(","), QString(".")) + ',' + QString::number((table->item(i,1)->data(Qt::DisplayRole).toDouble())).replace(QString(","), QString("."))+')';
            }
        }
    }
    
    if ((funz==true) && (!(function.isEmpty()))) {
        if (mycalcs.check(function)==false) return;
        //if (!(function.at(0)=='y' and function.at(1)=='=')) return; //the function must start with "y="
        //if (function.lastIndexOf("=")!=1) return;  //we need only one "=" and it must be the second char
        //THIS IS THE PLOT OF BEST FIT CURVE
        double definition = mycalcs.m_resolution;
        double id = (mycalcs.m_xmin);
        double tmpranged = mycalcs.m_xmax-mycalcs.m_xmin;
        int tmprange = int(mycalcs.m_xmax-mycalcs.m_xmin);
        double fraction = tmpranged - ((long)tmpranged);
        if (fraction>0.49) tmprange++;
        if (tmprange<1) tmprange = 1;
        int totalx = (tmprange*definition)+1;
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
            if ((tvalue==tvalue) && (tvalue!=log(0)) && (tvalue!=-log(0))) uid.qcplotwidget->graph(0)->addData((id), (tvalue));  //remember: tvalue == tvalue is needed to check if tvalue is NaN or not, while log(0) is the infinity
            mycalcs.m_greenPlot = mycalcs.m_greenPlot + ' ' + QString::number((id*10)+5).replace(QString(","), QString(".")) + ',' + QString::number((mycalcs.m_ymax-tvalue)*10).replace(QString(","), QString("."));
            mycalcs.m_greenPlotLatex = mycalcs.m_greenPlotLatex + '(' + QString::number(id).replace(QString(","), QString(".")) + ',' + QString::number((tvalue)).replace(QString(","), QString("."))+')';
            id = id + (1/definition);
        }
    }
    


    if (fittoorig == true) uid.qcplotwidget->graph(1)->rescaleAxes();
    uid.qcplotwidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    uid.qcplotwidget->replot();
    mycalcs.m_bluePlot = mycalcs.m_bluePlot + "\" style=\"stroke:blue;fill:none\"/> ";
    mycalcs.m_greenPlot = mycalcs.m_greenPlot + "\" style=\"stroke:green;fill:none\"/> ";
    //qDebug()<< mycalcs.m_bluePlot;
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
    uid.tableWidget->setRowCount(uid.totalrows->value());
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
    //uid.resolution->setValue(1.00);
    plot(uid.tableWidget, "", uid.originalplot->isChecked(),uid.fitplot->isChecked());
    setWindowTitle("");
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

    QString list = "0.05|0.1025|0.1|0.09|0.2|0.14|0.3|0.19|0.368|0.235|0.415|0.272|0.49|0.34|0.53|0.42|0.7|0.65|";
    int rows = list.count(QString("|"))/2;
    uid.totalrows->setValue(rows);
    on_actionNew_triggered();

    int st = -1;
    for (int i = 0; i<rows+1; i++) {
        QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
        if (titem) titem->setText(list.mid(st+1,(list.indexOf("|",st+1)-st-1))) ;
        st = list.indexOf("|",st+1);
        QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
        if (titemo) titemo->setText(list.mid(st+1,(list.indexOf("|",st+1)-st-1))) ;
        st = list.indexOf("|",st+1);
    }
    
    uid.function->setText("y=(a*(x^2))+(b*x)+c");
    //uid.function->setText("y=(a*(x^3))+(b*(x^2))+(c*x)+d");
    //uid.function->setText("y=a*sin(x)+b");
    uid.showerror->setChecked(true);
    delay(100);
    drawpl();
    delay(1000); //apparently, qcplotwidget crashed the entire application if multiple replots are called immediately one after the other... we need to wait a little before being able to replot without problems
    on_fittoorig_clicked();
    uid.backprop->setChecked(true);
    uid.genalg->setChecked(true);
    uid.maxIters->setValue(genalglimit);
    uid.comboBox->setCurrentIndex(2);
    uid.tabWidget->setCurrentIndex(1);
}

void MainWindow::on_actionOpen_triggered() {
    mycalcs.m_file = QFileDialog::getOpenFileName(this, tr("Open work"), QDir::currentPath(), "*.kartesio|Kartesio File (*.kartesio)"); //     getOpenFileName(this,"Open work","","Kartesio File (*.kartesio)");
    Openfile();
}

void MainWindow::Openarg(QString filename) {
    //remove the first 7 characters
    if (filename.right(9)== ".kartesio") {
        mycalcs.m_file = filename;
        if (filename.mid(0,5)=="file:")    mycalcs.m_file = filename.remove(0,7);
        Openfile();
    }
    if (filename.right(4)== ".csv")  opencsv(filename);
}

void MainWindow::Openfile() {
    //loads all the cells text from a file prevoiusly saved
    if (!(mycalcs.m_file.isEmpty())) {
        setWindowTitle(mycalcs.m_file); //for some reason I don't get, this method doesn't work
        QFile file(mycalcs.m_file);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        int filerows = 0;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line == "table1|") filerows = 0;
            if (line == "function|") break;
            filerows++;
        }
        file.close();
        qDebug() << filerows;

        QByteArray bac = mycalcs.m_file.toLatin1();
        char *filec = bac.data();
        ifstream texto(filec);
        if (!texto) QMessageBox::critical(this,tr("Error"),tr("Unable to open ")+mycalcs.m_file) ;
        if (texto) {
            uid.totalrows->setValue(filerows/2);
            on_actionNew_triggered();
            QString tempyval;
            char tmpchr;
            int i = 0;
            //QString tempya;
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
                            qDebug() << tempyval;
                        } else {
                            //int val = uid.totalrows->value() + 1;
                            //uid.totalrows->setValue(val);
                            QTableWidgetItem *titem = uid.tableWidget->item((i/2),0) ;
                            if (titem) titem->setText(tempyval) ;
                            qDebug() << tempyval;
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
        if (!out) QMessageBox::critical(this,tr("Error"),tr("Unable to create ")+mycalcs.m_file) ;
        out << strsave;
        out.close();
    }
}
void MainWindow::on_actionSaveAs_triggered() {
    //save as
    mycalcs.m_file = QFileDialog::getSaveFileName(this, tr("Save work"), QDir::currentPath(), "*.kartesio|Kartesio File (*.kartesio)"); //"*.cpp|Sources (*.cpp)" (this,"Save work","","Kartesio File (*.kartesio)");
    setWindowTitle(mycalcs.m_file);
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
    
    QString files = QFileDialog::getSaveFileName(this, tr("Save plot"), QDir::currentPath(), "*.svg|Svg image (*.svg)"); //(this,"Save plot","","Svg image (*.svg)");
    if (!(files.isEmpty())) {
        QByteArray svgt = svgcomplete.toLatin1();
        char *strsave = svgt.data();
        QByteArray ban = files.toLatin1();
        char *filec = ban.data();
        
        ofstream out(filec);
        if (!out) QMessageBox::critical(this,tr("Error"),tr("Unable to create ")+files) ;
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
    
    QString files = QFileDialog::getSaveFileName(this, tr("Save plot"), QDir::currentPath(), "*.tex|Latex document (*.tex)"); //getSaveFileName(this,"Save plot","","Latex document (*.tex)");
    if (!(files.isEmpty())) {
        QByteArray tex = texcomplete.toLatin1();
        char *strsave = tex.data();
        QByteArray ban = files.toLatin1();
        char *filec = ban.data();
        
        ofstream out(filec);
        if (!out) QMessageBox::critical(this,tr("Error"),tr("Unable to create ")+files) ;
        out << strsave;
        out.close();
        if (out) QMessageBox::information(this,tr("Well done"),tr("Please note that you can't use pdflatex to convert this file directly into a pdf file. You can convert it only into dvi, and then it will be possible to create a pdf.")) ;
    }
    
}

void MainWindow::on_fittoorig_clicked()
{
    uid.qcplotwidget->graph(1)->rescaleAxes();
    uid.qcplotwidget->replot();
}

void MainWindow::on_totalrows_valueChanged(int arg1)
{
    uid.tableWidget->setRowCount(arg1);
}

void MainWindow::on_sortxaxis_clicked()
{
    uid.tableWidget->sortByColumn(0, Qt::DescendingOrder);
    //first we need to delete empty rows
    int counter = 0;
    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
        if (!titem) {
            uid.tableWidget->removeRow(i);
        } else {
            bool ok;
            double value = titem->text().replace(",", ".").toDouble(&ok);
            if (titem->text().isEmpty() || titem->text().isNull() || titem->text()=="" || !ok) {
                uid.tableWidget->removeRow(i);
            } else {
                //titem->setText(QString::number(value));
                uid.tableWidget->setItem(i,0, new TableNumberItem(QString::number(value)));
                counter++;
            }
        }
    }

    //put every empty row in the lower part
    uid.tableWidget->sortByColumn(0, Qt::DescendingOrder);
    uid.totalrows->setValue(counter);


    uid.tableWidget->sortByColumn(1, Qt::DescendingOrder);

    //first we need to delete empty rows
    counter = 0;
    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        QTableWidgetItem *titem = uid.tableWidget->item(i,1) ;
        if (!titem) {
            uid.tableWidget->removeRow(i);
        } else {
            bool ok;
            double value = titem->text().replace(",", ".").toDouble(&ok);
            if (titem->text().isEmpty() || titem->text().isNull() || titem->text()=="" || !ok) {
                uid.tableWidget->removeRow(i);
            } else {
                //titem->setText(QString::number(value));
                uid.tableWidget->setItem(i,1, new TableNumberItem(QString::number(value)));
                counter++;
            }
        }
    }

    //put every empty row in the lower part
    uid.tableWidget->sortByColumn(1, Qt::DescendingOrder);
    uid.totalrows->setValue(counter);

    //now we sort in ascending order
    uid.tableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void MainWindow::on_resolution_valueChanged(int arg1)
{
    mycalcs.m_resolution = arg1;
    drawpl();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,tr("About Kartesio"),tr("Kartesio is a program for calculating best fit curves for a set of points. Kartesio uses QCustomPlot widget to plot data, and Maxima to solve expressions. The icons used in Kartesio have been designed by KDE Oxygen team. \nKartesio has been written by Luca Tringali (tringalinvent@libero.it) in 2011 and is released under GPL3."));
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}

void MainWindow::on_actionAboutZorbaNN_triggered()
{
    QMessageBox::about(this,tr("About ZorbaNN"),tr("ZorbaNeuralNetwork is a library for easy neural networks construction, and supports training with both Widrow-Hoff algoritm and genetic algorithm. In Kartesio, ZorbaNN is used for recognition of the best fit curve. \nZorbaNN has been created by Luca Tringali (tringalinvent@libero.it) in 2006 and is released under GPL3."));
}

void MainWindow::on_loadcsv_clicked()
{
    QString csvfile = QFileDialog::getOpenFileName(this, tr("Open CSV"), QDir::currentPath(), "*.csv|CSV File (*.csv)");
    opencsv(csvfile);
}

void MainWindow::on_exportcsv_clicked()
{
    QString filen = QFileDialog::getSaveFileName(this, tr("Save CSV"), QDir::currentPath(), "*.csv|CSV (*.csv)");
    savecsv(filen);
}

void MainWindow::opencsv(QString csvfile) {
    QString sep = QInputDialog::getText(this, tr("Open CSV"), tr("Which is the field separator of this CSV file? (If you don't know what to answer, just leave the suggested one and press OK.)"), QLineEdit::Normal,",");
    QFile file(csvfile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    QStringList firstcolumn;
    QStringList secondcolumn;
    while (!in.atEnd()) {
        QString line = in.readLine();
        int first = line.indexOf(sep,0);
        int second = line.indexOf(sep,first+1);
        firstcolumn.append(line.mid(0,first));
        secondcolumn.append(line.mid(first+1,second-first-1));
    }

    uid.totalrows->setValue(firstcolumn.count());
    on_actionNew_triggered();
    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        QTableWidgetItem *titem = new QTableWidgetItem ;
        titem->setText(firstcolumn.at(i));
        uid.tableWidget->setItem(i,0,titem);
        QTableWidgetItem *titemo = new QTableWidgetItem ;
        titemo->setText(secondcolumn.at(i));
        uid.tableWidget->setItem(i,1,titemo);
    }
}

void MainWindow::savecsv(QString filen) {
    QString sep = QInputDialog::getText(this, tr("Save CSV"), tr("Which is the field separator of this CSV file? (If you don't know what to answer, just leave the suggested one and press OK.)"), QLineEdit::Normal,",");

    QFile file(filen);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
        QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
        if (!titem || titem->text().isEmpty()) {
            //break;
        } else {
            QString yvaluerq = titemo->data(Qt::DisplayRole).toString() ;
            QString valuerq = titem->data(Qt::DisplayRole).toString() ;
            out << valuerq << sep << yvaluerq << sep << "\n";
        }
    }
    file.close();

}

void MainWindow::on_xrangecng(QCPRange newRange){
    if (newRange.lower != uid.xmin->value()) uid.xmin->setValue(newRange.lower);
    if (newRange.upper != uid.xmax->value()) uid.xmax->setValue(newRange.upper);
}

void MainWindow::on_yrangecng(QCPRange newRange){
    if (newRange.lower != uid.ymin->value()) uid.ymin->setValue(newRange.lower);
    if (newRange.upper != uid.ymax->value()) uid.ymax->setValue(newRange.upper);
}

void MainWindow::on_actionFind_triggered() {
    QString findstr = QInputDialog::getText(this, tr("Find"), tr("What should I look for?"), QLineEdit::Normal);

    //if findstr is empty, just set color to blank
    if (findstr == "") {
        for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
            QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
            QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
            if (!titem || titem->text().isEmpty()) {
                //break;
            } else {
                titemo->setBackgroundColor(QColor(255,255,255));
                titem->setBackgroundColor(QColor(255,255,255));
            }
        }
    } else {
        for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
            QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
            QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
            if (!titem || titem->text().isEmpty()) {
                //break;
            } else {
                QString yvaluerq = titemo->data(Qt::DisplayRole).toString() ;
                QString valuerq = titem->data(Qt::DisplayRole).toString() ;
                if (yvaluerq.indexOf(findstr)!=-1) titemo->setBackgroundColor(QColor(255,255,0));
                if (valuerq.indexOf(findstr)!=-1) titem->setBackgroundColor(QColor(255,255,0));
            }
        }
    }
}

void MainWindow::on_actionReplace_triggered(){
    QString findstr = QInputDialog::getText(this, tr("Find"), tr("What should I look for?"), QLineEdit::Normal);
    QString replacestr = QInputDialog::getText(this, tr("Find"), tr("What should I replace it with?"), QLineEdit::Normal);

    if (findstr == "") {
        for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
            QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
            QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
            if (!titem || titem->text().isEmpty()) {
                //break;
            } else {
                titemo->setBackgroundColor(QColor(255,255,255));
                titem->setBackgroundColor(QColor(255,255,255));
            }
        }
    } else {
        for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
            QTableWidgetItem *titem = uid.tableWidget->item(i,0) ;
            QTableWidgetItem *titemo = uid.tableWidget->item(i,1) ;
            if (!titem || titem->text().isEmpty()) {
                //break;
            } else {
                QString yvaluerq = titemo->data(Qt::DisplayRole).toString() ;
                QString valuerq = titem->data(Qt::DisplayRole).toString() ;
                if (yvaluerq.indexOf(findstr)!=-1) titemo->setText(yvaluerq.replace(findstr,replacestr));
                if (valuerq.indexOf(findstr)!=-1) titem->setText(valuerq.replace(findstr,replacestr));
            }
        }
    }
}

void MainWindow::on_autorecognition_triggered() {
    mycalcs.viewweights = false;
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("View Neural Network"),tr("Would you like to view the neural network structure while it's performing the automatic recognition?"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) mycalcs.viewweights = true;

    if (mycalcs.viewweights) {
        QThread* thread = new QThread;
        Worker* worker = new Worker(mycalcs.weightssavename);
        worker->moveToThread(thread);
        //connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
        connect(thread, SIGNAL(started()), worker, SLOT(process()));
        connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
        connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
        QString att = QInputDialog::getText(this, tr("Question"), tr("How many times should I look for every type of function?"), QLineEdit::Normal,QString("5"));
        QFuture<int> f1 = QtConcurrent::run(this, &MainWindow::do_autorecognition, att);
        watcher.setFuture(f1);
    } else {
        QString att = QInputDialog::getText(this, tr("Question"), tr("How many times should I look for every type of function?"), QLineEdit::Normal,QString("5"));
        do_autorecognition(att);
        autorecofinished();
    }


}

int MainWindow::autorecofinished(){
    qDebug()<<"recognition finished";
    QString best = "";
    QString bestname = "";
    QTableWidgetItem *titemo = tmptable->item(0,0) ;
    if (titemo) bestname = titemo->text();
    QTableWidgetItem *titem = tmptable->item(0,1) ;
    if (titemo) best = titem->text();

    QMessageBox::information(this,tr("Done"), tr("The best fit curve is ")+bestname+tr(", with a RMS error of ")+best+tr("."));
    uid.result->setText(bestname);
    uid.qcplotwidget->clearItems();
    QString error = tr("RMS error: ")+best;
    QCPItemText *textLabelRMS = new QCPItemText(uid.qcplotwidget);
    uid.qcplotwidget->addItem(textLabelRMS);
    textLabelRMS->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabelRMS->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabelRMS->position->setCoords(0.5, 0); // place position at center/top of axis rect
    textLabelRMS->setText(error);
    textLabelRMS->setFont(QFont(font().family(), 10));
    textLabelRMS->setPen(QPen(Qt::red));
    uid.qcplotwidget->replot();
}

int MainWindow::do_autorecognition(QString att) {

    //QMessageBox::information(this,tr("Please wait"), tr("Please, wait while I'm trying to recognize the correct best fit curve. Kartesio may not respond to the operating system until the operation is finished. It'll need a while, take yourself a coffee."));
    uid.showerror->setChecked(true);
    drawpl();
    on_fittoorig_clicked();
    uid.backprop->setChecked(true);
    uid.genalg->setChecked(true);
    uid.tabWidget->setCurrentIndex(1);

    genalglimitalert = "";
    int attemptsforeveryfunc= att.toInt();

    tmptable = new QTableWidget(this);
    tmptable->setColumnCount(2);
    tmptable->setRowCount(uid.comboBox->count()*attemptsforeveryfunc);

    for (int i=0; i<tmptable->rowCount() ; i++) {
        QTableWidgetItem *titem = new QTableWidgetItem ;
        titem->setText("");
        tmptable->setItem(i,0,titem);
        QTableWidgetItem *titemo = new QTableWidgetItem ;
        titemo->setText("");
        tmptable->setItem(i,1,titemo);
    }

    int n = 0;
    for (int i = 0; i<uid.comboBox->count();i++){
        uid.comboBox->setCurrentIndex(i);
        for (int t= 0; t<attemptsforeveryfunc; t++) {
            on_pushButton_2_clicked();
            QTableWidgetItem *titemo = tmptable->item(n,0) ;
            if (titemo) titemo->setText(uid.result->text());
            QTableWidgetItem *titem = tmptable->item(n,1) ;
            if (titem) tmptable->setItem(n,1, new TableNumberItem(QString::number(myerr)));
            n++;
        }
    }

    tmptable->sortByColumn(1, Qt::AscendingOrder);

    QString fullresult = "";
    QString sep = "|";
    for (int i=0; i<tmptable->rowCount() +1; i++) {
        QTableWidgetItem *titem = tmptable->item(i,0) ;
        QTableWidgetItem *titemo = tmptable->item(i,1) ;
        if (!titem || titem->text().isEmpty()) {
            //break;
            qDebug() << "empty";
        } else {
            QString yvaluerq = titemo->data(Qt::DisplayRole).toString() ;
            QString valuerq = titem->data(Qt::DisplayRole).toString() ;
            fullresult = fullresult + valuerq + sep + yvaluerq + sep + "\n";
        }
    }
    qDebug() << fullresult;


    QString best = "";
    QString bestname = "";
    QTableWidgetItem *titemo = tmptable->item(0,0) ;
    if (titemo) bestname = titemo->text();
    QTableWidgetItem *titem = tmptable->item(0,1) ;
    if (titemo) best = titem->text();
    //this is if you want to know which one is the worst fit
    /*QTableWidgetItem *titemo = tmptable->item(tmptable->rowCount()-1,1) ;
     if (titemo) worst = titemo->text();*/
    return 0;
}

Worker::Worker(QString filen) {
    // you could copy data from constructor arguments to internal variables here.

    myQMainWindow = new QMainWindow();
    //We don't want a modal window Qt::WindowModal Qt::ApplicationModal
    myQMainWindow->setWindowModality(Qt::NonModal);
    //myQMainWindow->setGeometry(20,20,200,50);

    QWidget *mwidget = new QWidget();
    myQMainWindow->setCentralWidget(mwidget);
    layout = new QVBoxLayout(mwidget);
    myQMainWindow->setLayout(layout);

    weightsfile = filen;

    lineed = new QLabel();
    layout->addWidget(lineed);
    lineed->setTextFormat(Qt::RichText);
    lineed->setAlignment(Qt::AlignCenter);
    lineed->setText(weightsfile);

    myQMainWindow->show();
}

Worker::~Worker() {
    // free memory
}

void Worker::process() {
    while ( 1>0 ) {
        if (QFile(weightsfile).exists()){
            QFile file(weightsfile);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);

                QString line = in.readLine();
                QString clean = line.left(line.length()-1);
                if (clean.count("|")!=0) {
                    int spaces = int(clean.length()/clean.count("|"));

                    //the neural network has two layers: the first has a number of neurons equal to the number of curve coefficients. Instead, the last layer has only one neuron. Every neuron of first layer is linked to the neuron in the second layer with a weight.
                    QString formatted = "<b>";
                    for (int i = 0; i<clean.count("|"); i++) {
                        formatted = formatted + QString("I");
                        if (i!=clean.count("|")-1) for (int n = 0; n<spaces; n++) formatted = formatted + QString("-");
                    }
                    formatted = formatted + QString("</b><br>") + clean.left(clean.length()-1) + QString("<br>");
                    for (int i = 0; i<clean.count("|"); i++) {
                        if (i<(clean.count("|")/2)) formatted = formatted + QString("\\");
                        if (i==(clean.count("|")/2) && (clean.count("|")%2)!=0) formatted = formatted + QString("I");
                        if (i==(clean.count("|")/2) && (clean.count("|")%2)==0) formatted = formatted + QString("/");
                        if (i>(clean.count("|")/2)) formatted = formatted + QString("/");

                        if (i!=clean.count("|")-1) for (int n = 0; n<spaces; n++) formatted = formatted + QString("-");
                    }
                    formatted = formatted + QString("<br><b>O</b>");
                    lineed->setText(formatted);
                }
            }

        }
        delay(100);
    }
    //emit finished();
}

void Worker::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::on_savenet_triggered()
{

    QString tmpfile = QFileDialog::getSaveFileName(this, tr("Save Neural Network"), QDir::currentPath(), "*.znn|Kartesio File (*.znn)");
    if (tmpfile.mid(0,(tmpfile.length()-4)) != ".znn") tmpfile.append(".znn");
    QString tmpweights = tmpfile.mid(0,(tmpfile.length()-4));
    QString tmpold = tmpfile.mid(0,(tmpfile.length()-4));
    QString tmppop = tmpfile.mid(0,(tmpfile.length()-4));
    tmpweights.append("-weights.zwf");
    tmpold.append("-oldweights.zwf");
    tmppop.append("-population.zpf");

    QString otmpfile = mycalcs.NNsavename;
    QString otmpweights = otmpfile.mid(0,(otmpfile.length()-4));
    QString otmpold = otmpfile.mid(0,(otmpfile.length()-4));
    QString otmppop = otmpfile.mid(0,(otmpfile.length()-4));
    otmpweights.append("-weights.zwf");
    otmpold.append("-oldweights.zwf");
    otmppop.append("-population.zpf");

    if (QFile::exists(tmpfile))    QFile::remove(tmpfile);
    QFile::copy(otmpfile, tmpfile);

    if (QFile::exists(tmpweights))    QFile::remove(tmpweights);
    QFile::copy(otmpweights, tmpweights);

    if (QFile::exists(tmpold))    QFile::remove(tmpold);
    QFile::copy(otmpold, tmpold);

    if (QFile::exists(tmppop))    QFile::remove(tmppop);
    QFile::copy(otmppop, tmppop);

}

void MainWindow::on_loadnet_triggered()
{
    mycalcs.loadnetwork = QFileDialog::getOpenFileName(this, tr("Save Neural Network"), QDir::currentPath(), "*.znn|Kartesio File (*.znn)");
    QMessageBox::information(this, tr("Ready for best fit"),tr("Neural network loaded, now please press Best Fit button to start using it."));
}
