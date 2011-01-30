#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{

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

    connect(clearAction, SIGNAL(triggered(bool)),this, SLOT(on_actionNew_triggered()));
    connect(plotAction, SIGNAL(triggered(bool)),this, SLOT(on_pushButton_clicked()));
    connect(openAction, SIGNAL(triggered(bool)),this, SLOT(on_actionOpen_triggered()));
    connect(saveAction, SIGNAL(triggered(bool)),this, SLOT(on_actionSave_triggered()));
    connect(saveasAction, SIGNAL(triggered(bool)),this, SLOT(on_actionSaveAs_triggered()));
    connect(expsvgAction, SIGNAL(triggered(bool)),this, SLOT(on_actionSvg_triggered()));
    connect(exptexAction, SIGNAL(triggered(bool)),this, SLOT(on_actionTex_triggered()));
    connect(exampleAction, SIGNAL(triggered(bool)),this, SLOT(on_actionShow_example_triggered()));
    
    
//setupGUI();
setupGUI(Default, "kartesioui.rc");
    QWidget *widget = new QWidget( this );

    // create the user interface, the parent widget is "widget"
    uid.setupUi(widget);
    setCentralWidget(widget);

    xmin = 0;
    xmax = 50;
    ymin = 0;
    ymax = 50;
    width = int(xmax - xmin);
    file = "";

    plot(uid.tableWidget, "",uid.originalplot->isChecked(),uid.fitplot->isChecked());


    connect( uid.pushButton, SIGNAL( clicked() ),this, SLOT( on_pushButton_clicked() ) );
    connect( uid.xmin, SIGNAL( valueChanged(double ) ),this, SLOT( on_xmin_valueChanged(double ) ) );
    connect( uid.xmax, SIGNAL( valueChanged(double ) ),this, SLOT( on_xmax_valueChanged(double ) ) );
    connect( uid.ymin, SIGNAL( valueChanged(double ) ),this, SLOT( on_ymin_valueChanged(double ) ) );
    connect( uid.ymax, SIGNAL( valueChanged(double ) ),this, SLOT( on_ymax_valueChanged(double ) ) );
    connect( uid.fitplot, SIGNAL( stateChanged(int ) ),this, SLOT( on_fitplot_stateChanged(int ) ) );
    connect( uid.originalplot, SIGNAL( stateChanged(int ) ),this, SLOT( on_originalplot_stateChanged(int ) ) );

}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //if (uid.function->text()!="") {
      QString value = calculate(uid.tableWidget,  uid.function);
      uid.result->setText(value);
      //for some reason if we call directly the function plot, the program crashes
      drawpl();
    //}
    
}

void MainWindow::drawpl(){
    QString tempstr = "";
    if (uid.result->text()!="") tempstr = uid.result->text().split("=").at(1);
    plot(uid.tableWidget, tempstr,uid.originalplot->isChecked(),uid.fitplot->isChecked());
}

QString MainWindow::calculate(QTableWidget *table,  QLineEdit *func){
    //this function calculates the best fit curve from some points and a generic function
    resfunz = "";
    width = int(xmax - xmin);
    int totalcoeff=0;
    QStringList coeff;
    //uid.label->setText("");
    QString myfunz;
    //uid.tableWidget->sortItems(1, Qt::AscendingOrder); //seems that the sorting doesn't work correctly
    if (!table->item(0,0) || table->item(0,0)->text().isEmpty())
    {
        //go on
    } else {
        //now we can plot the values
        QVarLengthArray<double, 64> px(table->rowCount());
        QVarLengthArray<double, 64> py(table->rowCount());
        int totaldata=0;
        for (int i=0; i<table->rowCount() ; i++) {
            if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                break;
            } else {
                totaldata++;
                py[i] = table->item(i,1)->data(Qt::DisplayRole).toDouble();
                px[i] = table->item(i,0)->data(Qt::DisplayRole).toDouble();
            }
        }
        //now in the arrays px and py are stored the values of the points
        //we need to read the function and find a way to invert it

        //first of all we need to create an array of the coefficients
        //then, for every coefficient (e.g.: "a") we force the passage for one point
        //then we must solve the system
        //example of a correct system: solve([7=(a*(2^2))+(b*2)+c,13=(a*(3^2))+(b*3)+c,21=(a*(4^2))+(b*4)+c],[a,b,c]);
        //this procedure is replicated until every point has been used. Then we make the medium between the coefficient values
        //We will use the following command: maxima --batch-string="solve([7=(a*(2^2))+(b*2)+c,13=(a*(3^2))+(b*3)+c,21=(a*(4^2))+(b*4)+c],[a,b,c]);"

        //Try to fill the coeff list with all the coefficients and totalcoeff with the number of coefficients if there is a generic equation
        if (func->text()=="") return 0;
        QByteArray bat = func->text().toLatin1();
        char *yvalue = bat.data();
        QString tempy;
        QString tempyn = "";
        tempy = "";
        for (int i=0; strlen(yvalue)+1;i++) {
            if (!(yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==','or yvalue[i]=='=')) break; //if current value is not a permitted value, this means that something is wrong
            if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M') tempyn = tempyn + yvalue[i]; // every letter will be added in the variable tempyn so we can study it
            tempy = "";
            if  (yvalue[i]=='=' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) tempyn = "";
            //use the correct functions
            if (!(tempyn=="" or tempyn=="cos" or tempyn=="sin" or tempyn=="y" or tempyn=="x" or tempyn=="tan" or tempyn=="cot" or tempyn=="exp" or tempyn=="abs" or tempyn=="acos" or tempyn=="atan" or tempyn=="atan2" or tempyn=="asin" or tempyn=="ceil" or tempyn=="floor" or tempyn=="log" or tempyn=="ln" or tempyn=="max" or tempyn=="min" or tempyn=="random" or tempyn=="round" or tempyn=="sqrt")) {
                if  ((yvalue[i+1]=='+' or yvalue[i+1]=='=' or yvalue[i+1]=='-' or yvalue[i+1]=='*' or yvalue[i+1]=='^' or yvalue[i+1]=='/' or yvalue[i+1]=='(' or yvalue[i+1]==')' or yvalue[i+1]=='1' or yvalue[i+1]=='2' or yvalue[i+1]=='3' or yvalue[i+1]=='4' or yvalue[i+1]=='5' or yvalue[i+1]=='6' or yvalue[i+1]=='7' or yvalue[i+1]=='8' or yvalue[i+1]=='9' or yvalue[i+1]=='0' or yvalue[i+1]=='.' or yvalue[i+1]==' ' or yvalue[i+1]==',' or yvalue[i+1]=='=' or (i+1)==(strlen(yvalue))) ) {
                    coeff.append(tempyn);
                    totalcoeff++;
                }
            }
        }

        tryn = 0;
        //run the procedure until every point has been used
        for (int f=0; f<(totaldata-totalcoeff+2);f++) {
        //we try to prepare the command line for maxima
        QString cmd = "maxima --batch-string=\"solve([";
        int fir = 1;
        //tryn = f;
        int i = f;
            //now i'm using the value of the points to fit the curve
            //REMEMBER that coeff.at(u) is the actual coefficient px[i] and py[i] are the x,y values of the point used
            for (int u=0; u<totalcoeff; u++){
                QByteArray banf = func->text().toLatin1();
                char *tmfn = banf.data();
                QString xtmp;
                xtmp.setNum(px[i]);
                QString eqa = replacevar(tmfn,xtmp,QString("x"));
                QByteArray banfn = eqa.toLatin1();
                char *tmfns = banfn.data();
                xtmp.setNum(py[i]);
                QString eq = replacevar(tmfns,xtmp,QString("y"));
                if (fir==0) cmd = cmd + ","+ eq;
                if (fir==1) {
                    cmd = cmd + eq;
                    fir = 0;
                }
                i++;
            }

        cmd = cmd+"],[";
        fir= 1;
        for (int u=0; u<totalcoeff; u++){
            if (fir==0) cmd = cmd + ","+coeff.at(u);
            if (fir==1) {
                cmd = cmd + coeff.at(u);
                fir = 0;
            }
        }
        cmd = cmd+"]);\" >> \"/tmp/kartesiotmp.txt\"";
	
        //run cmd in a linux command line
        QByteArray banfc = cmd.toLatin1();
        char *tmc = banfc.data();
        int go = system("rm /tmp/kartesiotmp.txt");
        go = system(tmc);
	// if go is not 0, then it means that maxima died
	if  (go!=0) QMessageBox::critical(this,"Error","Seems that Maxima process died calculating the result.") ;
	
        char tmpchr;
        ifstream texto("/tmp/kartesiotmp.txt");
        if (texto) {
            do {
                texto >> tmpchr;
                myfunz = myfunz + tmpchr;
            } while (!texto.eof());
        }
        texto.close();

        //now we have a string like this "...solve([1=a3+b3+c,2=a4+b4+c,3=a5+b5+c],[a,b,c])(%o1)[[a=0,b=1,c=-2]]]"
        //and we must read the values of the coefficients to store them in a list then we create the function
        //replacing the correct coefficient values into the original function written by user

	if (myfunz.indexOf("(%o1)[[")==-1) return 0;
        QString tempstr = myfunz.split("(%o1)[[").at(1);
        QString cancstr = tempstr.replace("]","");
	if (myfunz.indexOf(",")==-1 and totalcoeff>1) return 0;
        QStringList cmvalue = cancstr.split(",");

        //ignore the result if it is not a correct number
        int good = 1;
        for (int u=0; u<totalcoeff; u++){
            for (int n=0; n<cmvalue.count() ; n++){
	    if (cmvalue.at(n).indexOf("=")==-1) {
	      break;
	      good = 0;
	    }
            QStringList mnval = cmvalue.at(n).split("=");
            QScriptEngine myEnginee;
            QScriptValue isreal = myEnginee.evaluate(mnval.at(1)+"*0");
            //QString eef = uid.label_2->text();
            //uid.label_2->setText(eef + "|" + isreal.toString());
            if (isreal.toString()!="0") good = 0;
            }
        }
	

        //QString rrf = uid.label->text();
        //uid.label->setText(rrf+"|"+cancstr);

        myfunz = func->text();
        //resfunz = myfunz;
        if (good == 1){
        //myfunz = func->text();
        for (int u=0; u<totalcoeff; u++){
            for (int n=0; n<cmvalue.count() ; n++){
	      if (cmvalue.at(n).indexOf("=")!=-1) {
                QStringList mnval = cmvalue.at(n).split("=");
                if (mnval.at(0)==coeff.at(u)){
                    //now we must sum the new value to the others
                    QString newvar = mnval.at(1);
                    //if (f==1 or tryn == 0) resfunz = myfunz;
                    if (resfunz=="") resfunz = myfunz;
                    if (f!=0 and tryn!=0) {  //remember that f begins with 0
                        QStringList mnvalo = oldcvalue.at(n).split("=");
                        //double newval = (((mnvalo.at(1).toDouble()*(f))+mnval.at(1).toDouble())/(f+1));
                        double newval = (((mnvalo.at(1).toDouble()*(tryn))+mnval.at(1).toDouble())/(tryn+1));
                        newvar.setNum(newval);
                    }
                    QByteArray banf = resfunz.toLatin1();
                    char *tmfnz = banf.data();
                    //myfunz = replacevar(tmfnz,newvar, coeff.at(u));
                    resfunz = replacevar(tmfnz,newvar, coeff.at(u));
                    tryn++;
                }
	    }
        }
        }
        oldcvalue = cmvalue;
        }
    } //here ends the search for coefficients values
    } //here ends the "else"
    return resfunz;
    resfunz = "";
}

void MainWindow::plot(QTableWidget *table, QString function, bool original, bool funz){

    //this function plots the original points and the best fit curve
    width = int(xmax - xmin);
    //now I'm preparing the kplot widget
    uid.kplotwidget->removeAllPlotObjects();
    uid.kplotwidget->setLimits( xmin, xmax, ymin, ymax ); //now I need to set the limits of the plot

    KPlotObject *kpor = new KPlotObject( Qt::red, KPlotObject::Lines );
    KPlotObject *kpog = new KPlotObject( Qt::green, KPlotObject::Lines );
    KPlotObject *kpob = new KPlotObject( Qt::blue, KPlotObject::Lines );
    redplot = "<polyline points=\"";
    greenplot = "<polyline points=\"";
    blueplot = "<polyline points=\"";

    //uid.tableWidget->sortItems(1, Qt::AscendingOrder); //seems that the sorting doesn't work correctly
    if (!table->item(0,0) || table->item(0,0)->text().isEmpty())
    {
        //go on
    } else {
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
                blueplot = blueplot + " " + QString::number((table->item(i,0)->data(Qt::DisplayRole).toDouble()*10)+5).replace(QString(","), QString(".")) + "," + QString::number((ymax-table->item(i,1)->data(Qt::DisplayRole).toDouble())*10).replace(QString(","), QString("."));
            }
        }
        //THIS IS THE PLOT OF BEST FIT CURVE
        for (int i=int(xmin); i<(int(xmax)); i++) {
            double id = i;
            QString mreporto = function;
            QScriptEngine myEngine;
            QByteArray ban = mreporto.toLatin1();
            char *tmreporto = ban.data();

            QString istr;
            istr.append(QString("%1").arg((id)));
            //now i'm using QScript language to solve the expression
            //in a future we can consider to change it supporting some backends, but it's really complex
            QString myscript = solvex(tmreporto,istr); //myscript is the equation converted in QScript language and with the value of x axis (istr) instead of "x" variable
            QScriptValue three = myEngine.evaluate(myscript);

            double tvalue = three.toNumber();
            if (funz==true) kpog->addPoint(id, tvalue);
            greenplot = greenplot + " " + QString::number((id*10)+5).replace(QString(","), QString(".")) + "," + QString::number((ymax-tvalue)*10).replace(QString(","), QString("."));

        }
    } //here ends the experimental values mode

    uid.kplotwidget->addPlotObject(kpor);
    uid.kplotwidget->addPlotObject(kpog);
    uid.kplotwidget->addPlotObject(kpob);

    redplot = redplot + "\" style=\"stroke:red;fill:none\"/> ";
    blueplot = blueplot + "\" style=\"stroke:blue;fill:none\"/> ";
    greenplot = greenplot + "\" style=\"stroke:green;fill:none\"/> ";

}

QString MainWindow::solvex(char *yvalue, QString dnum) {
    //yvalue contains the equation of Y-axis variable
    //Remember that the function to elevate to power is Math.pow(b,e)
    //dnum is the value of x
    QString mreport;
    QString tempy;
    QString tempyold;
    QString tempyolda = "";
    QString tempyn = "";
    int olda =0;
    mreport ="";
    QString tempyval;
    tempy = "";
    for (int i=0; strlen(yvalue)+1;i++) {
        if (!(yvalue[i]=='=' or yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',')) break; //if current value is not a permitted value, this means that something is wrong
        if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M') tempyn = tempyn + yvalue[i]; // every letter will be added in the variable tempyn so we can study it
        tempy = "";
        if  (yvalue[i]=='=' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) tempyn = "";
        if (tempyn=="x") tempy=dnum; //replace every x with the correct numerical value
        //use the correct functions
        if (tempyn=="cos") tempy = "Math.cos";
        if (tempyn=="sin") tempy = "Math.sin";
        if (tempyn=="tan") tempy = "Math.tan";
        if (tempyn=="cot") tempy = "Math.cot";
        if (tempyn=="exp") tempy = "Math.exp";
        if (tempyn=="abs") tempy = "Math.abs";
        if (tempyn=="acos") tempy = "Math.acos";
        if (tempyn=="atan") tempy = "Math.atan";
        if (tempyn=="atan2") tempy = "Math.atan2";
        if (tempyn=="asin") tempy = "Math.asin";
        if (tempyn=="ceil") tempy = "Math.ceil";
        if (tempyn=="floor") tempy = "Math.floor";
        if (tempyn=="log") tempy = "Math.log"; //here's the usual problem: some people call log the logarithm in e, others call it ln
        if (tempyn=="ln") tempy = "Math.log"; //here's the usual problem: some people call log the logarithm in e, others call it ln
        if (tempyn=="max") tempy = "Math.max";
        if (tempyn=="min") tempy = "Math.min";
        if (tempyn=="random") tempy = "Math.random";
        if (tempyn=="round") tempy = "Math.round";
        if (tempyn=="sqrt") tempy = "Math.sqrt";

        //the simbol ^ should be replaced by Math.pow(base, exp)
        if (olda==1){
            //we need to know when we get a simbol to know the power exponent is ended
            tempyold = tempyold + yvalue[i];
            if  (yvalue[i]=='(' or yvalue[i]==')' ) {
                tempyval = tempyval + QString("Math.pow(") + tempyolda + QString(",") + tempyold + QString(")");
                tempyolda = "";
                tempyold  = "";
                olda = 0;
            }
        } else {
        //if the character is a number or a mathematic simbol we simply copy it
        if  ((yvalue[i]=='=' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) and (olda!=1) ) tempyval = tempyval + yvalue[i];
        if (yvalue[i]=='^') {
            olda =1;
        }
        if ((yvalue[i+1]!='^') and (yvalue[i]!='^') and (tempy!="")) {
            tempyval = tempyval + tempy;
            tempy ="";
        }
        if (yvalue[i+1]=='^') {
            tempyolda = tempy;
            tempy ="";
        }
        if (tempyval!="") mreport = tempyval;
    }
    }
    //QMessageBox::information(this,"report",mreport);
    return mreport;
}

QString MainWindow::replacevar(char *yvalue, QString dnum, QString var) {
    //yvalue contains the equation of Y-axis variable
    //Remember that the function to elevate to power is Math.pow(b,e)
    //dnum is the value of x
    QString mreport;
    QString tempy;
    QString tempyn = "";
    int olda =0;
    mreport ="";
    QString tempyval;
    tempy = "";
    for (int i=0; strlen(yvalue)+1;i++) {
        if (!(yvalue[i]=='=' or yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='^' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',')) break; //if current value is not a permitted value, this means that something is wrong
        if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M') tempyn = tempyn + yvalue[i]; // every letter will be added in the variable tempyn so we can study it
        tempy = "";
        if  (yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) tempyn = "";
        if  ((yvalue[i+1]=='+' or yvalue[i+1]=='=' or yvalue[i+1]=='-' or yvalue[i+1]=='*' or yvalue[i+1]=='^' or yvalue[i+1]=='/' or yvalue[i+1]=='(' or yvalue[i+1]==')' or yvalue[i+1]=='1' or yvalue[i+1]=='2' or yvalue[i+1]=='3' or yvalue[i+1]=='4' or yvalue[i+1]=='5' or yvalue[i+1]=='6' or yvalue[i+1]=='7' or yvalue[i+1]=='8' or yvalue[i+1]=='9' or yvalue[i+1]=='0' or yvalue[i+1]=='.' or yvalue[i+1]==' ' or yvalue[i+1]==',' or yvalue[i+1]=='=' or (i+1)==(strlen(yvalue))) ) {
            if (tempyn==var) tempyval = tempyval + dnum; //replace every x with the correct value
            if (tempyn!=var) tempyval = tempyval + tempyn;
            tempyn = "";
        }
        //use the correct functions
        //if the carachter is a number or a mathematic simbol we simply copy it
        if  ((yvalue[i]=='=' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='^' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) and (olda!=1) ) tempyval = tempyval + yvalue[i];

        if (tempyval!="") mreport = tempyval;
    }
    //QMessageBox::information(this,"report",mreport);
    return mreport;
}


void MainWindow::on_xmin_valueChanged(double val)
{
    xmin = val;
    drawpl();
}

void MainWindow::on_xmax_valueChanged(double val)
{
    xmax = val;
    drawpl();
}

void MainWindow::on_ymin_valueChanged(double val)
{
    ymin = val;
    drawpl();
}

void MainWindow::on_ymax_valueChanged(double val)
{
    ymax = val;
    drawpl();
}


void MainWindow::on_actionNew_triggered()
{
    //set all the table cells as empty ("")
    for (int i=0; i<uid.tableWidget->rowCount() ; i++) {
        QTableWidgetItem *titem = new QTableWidgetItem ;
        titem->setText("");
        uid.tableWidget->setItem(i,0,titem);
        QTableWidgetItem *titemo = new QTableWidgetItem ;
        titemo->setText("");
        uid.tableWidget->setItem(i,1,titemo);
    }
    uid.function->setText("") ;
    uid.result->setText("") ;
    plot(uid.tableWidget, "",uid.originalplot->isChecked(),uid.fitplot->isChecked());
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
    titemo->setText("7,19");
    titemo = uid.tableWidget->item(0,1);
    titemo->setText("30");
    titemo = uid.tableWidget->item(1,0);
    titemo->setText("7,64");
    titemo = uid.tableWidget->item(1,1);
    titemo->setText("30,5");
    titemo = uid.tableWidget->item(2,0);
    titemo->setText("10,02");
    titemo = uid.tableWidget->item(2,1);
    titemo->setText("31");
    titemo = uid.tableWidget->item(3,0);
    titemo->setText("10,45");
    titemo = uid.tableWidget->item(3,1);
    titemo->setText("31,5");

    uid.function->setText("y=(a*(x^3))+(b*(x^2))+(c*x)+d");
    uid.xmin->setValue(30.00);
    uid.xmax->setValue(32.00);
    uid.ymin->setValue(6.00);
    uid.ymax->setValue(13.00);
}

void MainWindow::on_actionOpen_triggered(){
  //open
  //loads all the cells text from a file prevoiusly saved
    file = QFileDialog::getOpenFileName(this,"Open work","","Kartesio File (*.kartesio)");
    if (file!="") {
        QByteArray bac = file.toLatin1();
        char *filec = bac.data();
        ifstream texto(filec);
        if (!texto) QMessageBox::critical(this,"Error","Unable to open "+file) ;
        if (texto) {
            on_actionNew_triggered();
            QString tempyval;
            char tmpchr;
            int i = 0;
            QString tempya;
            int tablea = 0;
            int xax = 0;
            do {
                texto >> tmpchr;
                if (tmpchr!='|') tempyval = tempyval + tmpchr;
                if (tmpchr=='|') {
                    if ((tablea==1) and (tempyval != QString("table1"))  and (tempyval != QString("function"))  ) {
                        if ((i%2)!=0) {
                            QTableWidgetItem *titemo = uid.tableWidget->item((i-1)/2,1) ;
                            if (titemo) titemo->setText(tempyval) ;
                        } else {
                            QTableWidgetItem *titem = uid.tableWidget->item((i/2),0) ;
                            if (titem) titem->setText(tempyval) ;
                        }
                        i++;
                    }

         
                    if ((xax==1) and (tempyval != QString("table1"))  and (tempyval != QString("function"))  ) {
                        uid.function->setText(tempyval);
			xax = 0;
                    }
                    
                    if (tempyval == QString("table1")) {
                        i=0;
                        tablea=1;
                        xax = 0;
                    }
                    if (tempyval == QString("function"))  {
                        tablea=0;
                        xax = 1;
                    }
                    
                    tempyval = "";
                }
            } while (!texto.eof());
            texto.close();
        }
    }
    
}

void MainWindow::on_actionSave_triggered(){
  if (file == "") {
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
    
    tempyval =  tempyval + QString("\nthe end|\n");

    if (file!="") {
        QByteArray ba = tempyval.toLatin1();
        char *strsave = ba.data();
        QByteArray bac = file.toLatin1();
        char *filec = bac.data();

        ofstream out(filec);
        /*cout << "|";
        cout << filec;
        cout << "|";*/
        if (!out) QMessageBox::critical(this,"Error","Unable to create "+file) ;
        out << strsave;
        out.close();
        //if(out) QMessageBox::information(this, "Information", "File "+file + " succesfully saved.");
    }
}
void MainWindow::on_actionSaveAs_triggered(){
  //save as
  file = QFileDialog::getSaveFileName(this,"Save work","","Kartesio File (*.kartesio)");
  on_actionSave_triggered();
}
void MainWindow::on_actionSvg_triggered(){
    //This function saves the plot into a SVG file
    QString svgheader = "<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"no\"?> <!DOCTYPE svg PUBLIC \"-//W3C//Dtd SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/Dtd/svg11.dtd\"> <svg width=\""+ QString::number((xmax*10)+5)+ "\" height=\""+ QString::number((ymax*10)+5)+ "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\"><polyline points=\"5," + QString::number(ymax*10) + " " + QString::number((xmax*10)-5) + "," + QString::number(ymax*10) + " " + QString::number((xmax*10)-5) + "," + QString::number((ymax*10)-5) + " " + QString::number(xmax*10) + "," + QString::number(ymax*10) + " " + QString::number((xmax*10)-5) + "," + QString::number((ymax*10)+5) + " " + QString::number((xmax*10)-5) + "," + QString::number(ymax*10) + "\" style=\"stroke:black;fill:none\"/> <polyline points=\"5," + QString::number(ymax*10) + " 5,5 10,5 5,0 0,5 5,5\" style=\"stroke:black;fill:none\"/> ";    
    //QString svgcomplete = svgheader + greenplot + blueplot + "</svg> ";
    QString svgcomplete = svgheader ;
    if (uid.fitplot->isChecked()) svgcomplete = svgcomplete + greenplot ; 
    if (uid.originalplot->isChecked()) svgcomplete = svgcomplete + blueplot; 
    svgcomplete = svgcomplete + "</svg> ";
        
    QString files = QFileDialog::getSaveFileName(this,"Save plot","","Svg image (*.svg)");
    if (files!="") {
      QByteArray svgt = svgcomplete.toLatin1();
      char *strsave = svgt.data();
      QByteArray ban = files.toLatin1();
      char *filec = ban.data();
      
      ofstream out(filec);
      if (!out) QMessageBox::critical(this,"Error","Unable to create "+files) ;
      out << strsave;
      out.close();
    }
}
void MainWindow::on_actionTex_triggered(){
  //save latex
}