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

#include "calculations.h"

Calculations::Calculations()
{
    //
}

Calculations::~Calculations()
{
    //
}

double Calculations::rmsError(QTableWidget *table,  QString func) {
    double rms=0.0;
    
    if (!table->item(0,0) || table->item(0,0)->text().isEmpty())
    {
        //go on
    } else {
        
        
        for (int i=0; i<table->rowCount() ; i++) {
            if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                break;
            } else {
                QScriptEngine myEngine;
                QByteArray ban = func.toLatin1();
                char *tmreporto = ban.data();
                
                QString istr;
                istr.append(QString("%1").arg((table->item(i,0)->data(Qt::DisplayRole).toDouble())));
                QString myscript = solvex(tmreporto,istr);
                QScriptValue three = myEngine.evaluate(myscript);
                
                double tvalue = three.toNumber();
                double diff = table->item(i,1)->data(Qt::DisplayRole).toDouble() - tvalue;
                rms+= std::pow(diff, 2);
            }
        }
    }
    
    return std::pow((rms/table->rowCount()),0.5);
}

QString Calculations::calculate(QTableWidget *table,  QLineEdit *func) {
    //this function calculates the best fit curve from some points and a generic function
    m_resultFunction = "";
    m_myReport=QString("Values obtained by maxima:")+QString("\n");
    m_width = int(m_xmax - m_xmin);
    int totalcoeff=0;
    QStringList coeff;
    QString myfunz;
    std::vector < std::vector <double> > allcoeffs;
    
    
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
        QString yvalue = func->text();
        QString tempy;
        QString tempyn = "";
        tempy = "";
        if (check(yvalue)==false) return "";
        for (int i=0; i<yvalue.length()+1;i++) {
            if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M') tempyn = tempyn + yvalue[i];
            // every letter will be added in the variable tempyn so we can study it
            tempy = "";
            if  (yvalue[i]=='=' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) tempyn = "";
            //use the correct functions
            if (!(tempyn.isEmpty() or tempyn=="cos" or tempyn=="sin" or tempyn=="y" or tempyn=="x" or tempyn=="tan" or tempyn=="cot" or tempyn=="exp" or tempyn=="abs" or tempyn=="acos" or tempyn=="atan" or tempyn=="atan2" or tempyn=="asin" or tempyn=="ceil" or tempyn=="floor" or tempyn=="log" or tempyn=="ln" or tempyn=="max" or tempyn=="min" or tempyn=="random" or tempyn=="round" or tempyn=="sqrt")) {
                if  ((yvalue[i+1]=='+' or yvalue[i+1]=='=' or yvalue[i+1]=='-' or yvalue[i+1]=='*' or yvalue[i+1]=='^' or yvalue[i+1]=='/' or yvalue[i+1]=='(' or yvalue[i+1]==')' or yvalue[i+1]=='1' or yvalue[i+1]=='2' or yvalue[i+1]=='3' or yvalue[i+1]=='4' or yvalue[i+1]=='5' or yvalue[i+1]=='6' or yvalue[i+1]=='7' or yvalue[i+1]=='8' or yvalue[i+1]=='9' or yvalue[i+1]=='0' or yvalue[i+1]=='.' or yvalue[i+1]==' ' or yvalue[i+1]==',' or yvalue[i+1]=='=' or (i+1)==(yvalue.length())) ) {
                    coeff.append(tempyn);
                    totalcoeff++;
                }
            }
        }
        
        m_tryNumber = 0;
        
        allcoeffs.resize(totalcoeff);
        for (int j=0; j<(totalcoeff);j++) {
            allcoeffs[j].resize(1);
        }
        
        //run the procedure until every point has been used
        for (int f=0; f<(totaldata);f++) {
            //we try to prepare the command line for maxima
            QString cmd = maximap + " --batch-string=\"numer:true;solve([";
            int fir = 1;
            int i = f;
            //now i'm using the value of the points to fit the curve
            //REMEMBER that coeff.at(u) is the actual coefficient px[i] and py[i] are the x,y values of the point used
            for (int u=0; u<totalcoeff; u++) {
                QByteArray banf = func->text().toLatin1();
                char *tmfn = banf.data();
                QString xtmp;
                xtmp.setNum(px[i%totaldata]).replace(QString(","), QString("."));
                QString eqa = replacevar(tmfn,xtmp,QString("x"));
                QByteArray banfn = eqa.toLatin1();
                char *tmfns = banfn.data();
                xtmp.setNum(py[i%totaldata]).replace(QString(","), QString("."));
                QString eq = replacevar(tmfns,xtmp,QString("y"));
                if (fir==0) cmd = cmd + ','+ eq;
                if (fir==1) {
                    cmd = cmd + eq;
                    fir = 0;
                }
                i++;
            }
            
            cmd = cmd+"],[";
            fir= 1;
            for (int u=0; u<totalcoeff; u++) {
                if (fir==0) cmd = cmd + ','+coeff.at(u);
                if (fir==1) {
                    cmd = cmd + coeff.at(u);
                    fir = 0;
                }
            }

            QString tmpfilename = "";
            QTemporaryFile file;
            if (file.open()) {
                tmpfilename = file.fileName();
            }
            tmpfilename = tmpfilename +"1";
            cmd = cmd+"]);\" >> \"" + tmpfilename + "\"";
            
            //run cmd in a linux shell
            QByteArray banfc = cmd.toLatin1();
            char *tmc = banfc.data();
            QFile::remove(tmpfilename);
            int go = system(tmc);
            // if go is not 0, then it means that maxima died
            if  (go!=0) return QString("died");
            qDebug()<<tmpfilename;

            QByteArray banfcf = tmpfilename.toLatin1();
            char *tmf = banfcf.data();
            char tmpchr;
            ifstream texto(tmf);
            if (texto) {
                do {
                    texto >> tmpchr;
                    myfunz = myfunz + tmpchr;
                } while (!texto.eof());
            }
            texto.close();
            
            //now we have a string like this "...solve([1=a3+b3+c,2=a4+b4+c,3=a5+b5+c],[a,b,c])(%o2)[[a=0,b=1,c=-2]]]"
            //and we must read the values of the coefficients to store them in a list then we create the function
            //replacing the correct coefficient values into the original function written by user
            
            if (myfunz.indexOf("(%o2)")==-1) return 0;
            QString tempstr = myfunz.split("(%o2)").at(1);
            //we must delete [ and ]
            QString cancstr = tempstr.replace("[","");
            cancstr = cancstr.replace("]","");
            if (myfunz.indexOf(",")==-1 and totalcoeff>1) return 0;
            QStringList cmvalue = cancstr.split(',');
            m_myReport = m_myReport + cancstr+ '\n';
            //ignore the result if it is not a correct number
            int good = 1;
            for (int u=0; u<totalcoeff; u++) {
                for (int n=0; n<cmvalue.count() ; n++) {
                    if (cmvalue.at(n).indexOf("=")==-1) {
                        break;
                        good = 0;
                    }
                    QStringList mnval = cmvalue.at(n).split('=');
                    QScriptEngine myEnginee;
                    QScriptValue isreal = myEnginee.evaluate(mnval.at(1)+"*0");
                    if (isreal.toString()!="0") good = 0;
                }
            }
            
            //at this point, cmvalue contains the values of coefficients, for example the first element could be a=0
            
            myfunz = func->text();
            
            if (good == 1) {
                for (int u=0; u<totalcoeff; u++) {
                    int r = 0;
                    for (int n=0; n<cmvalue.count() ; n++) {
                        if (cmvalue.at(n).indexOf("=")!=-1) {
                            QStringList mnval = cmvalue.at(n).split('=');
                            if (mnval.at(0)==coeff.at(u)) {
                                //now we must sum the new value to the others
                                if (m_resultFunction=="") m_resultFunction = myfunz;
                                if (f!=0 and m_tryNumber!=0) {  //remember that f begins with 0
                                    allcoeffs.at(u).resize(r+1);
                                    allcoeffs.at(u).at(r) = mnval.at(1).toDouble();
                                    r++;
                                }
                                m_tryNumber++;
                            }
                        }
                    }
                }
                m_oldValue = cmvalue;
            }
        } //here ends the search for coefficients values
        
        if (m_resultFunction=="") m_resultFunction = myfunz;
        //mean for every coeff
        for (int u=0; u<totalcoeff; u++) {
            double mean = 0;
            for (int r=0; r<allcoeffs.at(u).size(); r++) {
                mean += allcoeffs.at(u).at(r);
            }
            mean = (mean/allcoeffs.at(u).size());
            QByteArray banf = m_resultFunction.toLatin1();
            char *tmfnz = banf.data();
            m_resultFunction = replacevar(tmfnz,QString::number(mean), coeff.at(u));
        }
        
    } //here ends the "else"
    return m_resultFunction;
}


QString Calculations::trainNN(QTableWidget *table,  QComboBox *func, bool backprop, bool genalg)
{
    //this function calculates the best fit curve from some points and a generic function
    m_resultFunction = "";
    //m_myReport="Values obtained by maxima:\n";
    m_width = int(m_xmax - m_xmin);
    int totalcoeff=0;
    //QStringList coeff;
    QString myfunz;
    std::vector<int> lSzV;
    int layers = 2;
    QString myresult = "";
    
    
    std::vector < std::vector <double> > dataV;
    
    //now we need to know which function has been selected
    
    //uid.tableWidget->sortItems(1, Qt::AscendingOrder); //seems that the sorting doesn't work correctly
    if (!table->item(0,0) || table->item(0,0)->text().isEmpty())
    {
        //go on
    } else {
        
        if (func->currentText() == "y=m*x+q") {
            totalcoeff = 2;
            dataV.resize(table->rowCount());
            for (int j=0; j<(table->rowCount());j++) {
                dataV[j].resize(totalcoeff+1);
            }
            for (int i=0; i<table->rowCount() ; i++) {
                if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                    break;
                } else {
                    dataV[i][0] = table->item(i,0)->data(Qt::DisplayRole).toDouble();  //x
                    dataV[i][1] = 1;
                    dataV[i][2] = table->item(i,1)->data(Qt::DisplayRole).toDouble();  //y
                }
            }
        }
        
        if (func->currentText() == "y=a*(x^2)+b*x+c") {
            totalcoeff = 3;
            dataV.resize(table->rowCount());
            for (int j=0; j<(table->rowCount());j++) {
                dataV[j].resize(totalcoeff+1);
            }
            for (int i=0; i<table->rowCount() ; i++) {
                if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                    break;
                } else {
                    dataV[i][0] = pow(table->item(i,0)->data(Qt::DisplayRole).toDouble(), 2);  //x^2
                    dataV[i][1] = table->item(i,0)->data(Qt::DisplayRole).toDouble();  //x
                    dataV[i][2] = 1;
                    dataV[i][3] = table->item(i,1)->data(Qt::DisplayRole).toDouble();  //y
                }
            }
        }
        
        if (func->currentText() == "y=a*(x^3)+b*(x^2)+c*x+d") {
            totalcoeff = 4;
            dataV.resize(table->rowCount());
            for (int j=0; j<(table->rowCount());j++) {
                dataV[j].resize(totalcoeff+1);
            }
            for (int i=0; i<table->rowCount() ; i++) {
                if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                    break;
                } else {
                    dataV[i][0] = pow(table->item(i,0)->data(Qt::DisplayRole).toDouble(), 3);  //x^3
                    dataV[i][1] = pow(table->item(i,0)->data(Qt::DisplayRole).toDouble(), 2);  //x^2
                    dataV[i][2] = table->item(i,0)->data(Qt::DisplayRole).toDouble();  //x
                    dataV[i][3] = 1;
                    dataV[i][4] = table->item(i,1)->data(Qt::DisplayRole).toDouble();  //y
                }
            }
        }
        
        if (func->currentText() == "y=a*(x^4)+b*(x^3)+c*(x^2)+d*x+e") {
            totalcoeff = 5;
            dataV.resize(table->rowCount());
            for (int j=0; j<(table->rowCount());j++) {
                dataV[j].resize(totalcoeff+1);
            }
            for (int i=0; i<table->rowCount() ; i++) {
                if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                    break;
                } else {
                    dataV[i][0] = pow(table->item(i,0)->data(Qt::DisplayRole).toDouble(), 4);  //x^4
                    dataV[i][1] = pow(table->item(i,0)->data(Qt::DisplayRole).toDouble(), 3);  //x^3
                    dataV[i][2] = pow(table->item(i,0)->data(Qt::DisplayRole).toDouble(), 2);  //x^2
                    dataV[i][3] = table->item(i,0)->data(Qt::DisplayRole).toDouble();  //x
                    dataV[i][4] = 1;
                    dataV[i][5] = table->item(i,1)->data(Qt::DisplayRole).toDouble();  //y
                }
            }
        }
        
        if (func->currentText() == "y=a*(e^x)+c") {
            totalcoeff = 2;
            dataV.resize(table->rowCount());
            for (int j=0; j<(table->rowCount());j++) {
                dataV[j].resize(totalcoeff+1);
            }
            for (int i=0; i<table->rowCount() ; i++) {
                if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                    break;
                } else {
                    dataV[i][0] = exp(table->item(i,0)->data(Qt::DisplayRole).toDouble());  //exp(x)
                    dataV[i][1] = 1;
                    dataV[i][2] = table->item(i,1)->data(Qt::DisplayRole).toDouble();  //y
                }
            }
        }
        
        if (func->currentText() == "y=a*ln(x)+c") {
            totalcoeff = 2;
            dataV.resize(table->rowCount());
            for (int j=0; j<(table->rowCount());j++) {
                dataV[j].resize(totalcoeff+1);
            }
            for (int i=0; i<table->rowCount() ; i++) {
                if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                    break;
                } else {
                    dataV[i][0] = log(table->item(i,0)->data(Qt::DisplayRole).toDouble());  //ln(x)
                    dataV[i][1] = 1;
                    dataV[i][2] = table->item(i,1)->data(Qt::DisplayRole).toDouble();  //y
                }
            }
        }
        
        
        if (func->currentText() == "y=(a/x)+b") {
            totalcoeff = 2;
            dataV.resize(table->rowCount());
            for (int j=0; j<(table->rowCount());j++) {
                dataV[j].resize(totalcoeff+1);
            }
            for (int i=0; i<table->rowCount() ; i++) {
                if (!table->item(i,0) || table->item(i,0)->text().isEmpty()) {
                    break;
                } else {
                    dataV[i][0] = (1/(table->item(i,0)->data(Qt::DisplayRole).toDouble()));  //1/x
                    dataV[i][1] = 1;
                    dataV[i][2] = table->item(i,1)->data(Qt::DisplayRole).toDouble();  //y
                }
            }
        }
        
        lSzV.resize(layers);
        lSzV[0] = (totalcoeff-1);
        lSzV[1] = 1;
        
        //this is the common zorba neural network definition
        
        double learningRate = 0.1;
        double momentum = 0.1;
        int maxCrossoverNumber = 200;

        ZorbaNN *myNet = new ZorbaNN(layers, lSzV, learningRate, momentum);
        myNet->minAcceptableError = pow(10,-4);
        QByteArray banfc = weightssavename.toLatin1();
        char *tmcf = banfc.data();
        myNet->fileviewweights = tmcf;
        myNet->viewweights = viewweights;

        if (loadnetwork != "") {
            std::cout << "Actual weights:";
            myNet->printWeights();
            QByteArray banfcn = loadnetwork.toLatin1();
            char *tmcfn = banfcn.data();
            myNet->setNetwork(tmcfn);
            loadnetwork = "";
            std::cout << "Loaded weights:";
            myNet->printWeights();
        }

        if (backprop==true) myNet->recursiveTrainBackProp(dataV, m_maxIters);
        if (genalg==true) myNet->recursiveTrainGenAlg(dataV, m_maxIters, maxCrossoverNumber);
        
        myresult = "y=";
        
        for (int f = 0; f<totalcoeff; f++) {
            //print the result: the number of weights is totalcoeff
            double temp = (1/(1+exp(-(myNet->getWeight(1,0,f)))));
            std::ostringstream strs;
            strs << std::fixed << std::setprecision(6) << temp;
            myresult = myresult + QString(strs.str().c_str());
            QString vars = "";
            if (func->currentText() == "y=m*x+q" && f == 0) vars = "*x+";
            
            if (func->currentText() == "y=a*(x^2)+b*x+c" && f == 0) vars = "*(x^2)+";
            if (func->currentText() == "y=a*(x^2)+b*x+c" && f == 1) vars = "*x+";
            
            if (func->currentText() == "y=a*(x^3)+b*(x^2)+c*x+d" && f == 0) vars = "*(x^3)+";
            if (func->currentText() == "y=a*(x^3)+b*(x^2)+c*x+d" && f == 1) vars = "*(x^2)+";
            if (func->currentText() == "y=a*(x^3)+b*(x^2)+c*x+d" && f == 2) vars = "*x+";
            
            if (func->currentText() == "y=a*(x^4)+b*(x^3)+c*(x^2)+d*x+e" && f == 0) vars = "*(x^4)+";
            if (func->currentText() == "y=a*(x^4)+b*(x^3)+c*(x^2)+d*x+e" && f == 1) vars = "*(x^3)+";
            if (func->currentText() == "y=a*(x^4)+b*(x^3)+c*(x^2)+d*x+e" && f == 2) vars = "*(x^2)+";
            if (func->currentText() == "y=a*(x^4)+b*(x^3)+c*(x^2)+d*x+e" && f == 3) vars = "*x+";
            
            if (func->currentText() == "y=a*(e^x)+c" && f == 0) vars = "*exp(x)+";
            
            if (func->currentText() == "y=a*ln(x)+c" && f == 0) vars = "*ln(x)+";
            
            if (func->currentText() == "y=(a/x)+b" && f == 0) vars = "/x+";
            
            myresult = myresult + vars;
        }
        
        //here I can save the entire network for reuse
        if (NNsavename != "") {
            QByteArray banfc = NNsavename.toLatin1();
            char *tmc = banfc.data();
            myNet->saveNetwork(tmc);
            //qDebug() << "Saved neural network in" << NNsavename;
        }
    } //here ends the "else"

    return myresult;
}

QString Calculations::solvex(QString yvalue, QString dnum) {
    //yvalue contains the equation of Y-axis variable
    //Remember that the function to elevate to power is Math.pow(b,e)
    //dnum is the value of x
    //the E must be changed to *10^: for example, the function
    //y=4.6374269005847954E-8*(x^3)+-1.8212280701754386E-5*(x^2)+0.002477485380117*x+0
    //should be
    //y=4.6374269005847954*10^-8*(x^3)+-1.8212280701754386*10^-5*(x^2)+0.002477485380117*x+0
    
    QString mreport;
    QString tempy;
    QString tempyold;
    QString tempyolda = "";
    QString tempyn = "";
    int olda =0;
    mreport ="";
    QString tempyval;
    tempy = "";
    if (check(yvalue)==false) return "";
    for (int i=0; i<yvalue.length()+1;i++) {
        if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M') tempyn = tempyn + yvalue[i];
        // every letter will be added in the variable tempyn so we can study it
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
        if (tempyn=="E") {
            tempy = "*Math.pow(10,";
            do{
                i++;
                tempy = tempy + yvalue[i];
            }while ((QString(yvalue[i+1])!=QString("*")));//until i+1=="*"
            tempy = tempy + ')';
        }
        //the simbol ^ should be replaced by Math.pow(base, exp)
        if (olda==1) {
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
            if ((yvalue[i+1]!='^') and (yvalue[i]!='^') and !(tempy.isEmpty())) {
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
    return mreport;
}

QString Calculations::replacevar(QString yvalue, QString dnum, QString var) {
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
    if (check(yvalue)==false) return "";
    for (int i=0; i<yvalue.length()+1;i++) {
        if (yvalue[i]=='q' or yvalue[i]=='w' or yvalue[i]=='e' or yvalue[i]=='r' or yvalue[i]=='t' or yvalue[i]=='y' or yvalue[i]=='u' or yvalue[i]=='i' or yvalue[i]=='o' or yvalue[i]=='p' or yvalue[i]=='a' or yvalue[i]=='s' or yvalue[i]=='d' or yvalue[i]=='f' or yvalue[i]=='g' or yvalue[i]=='h' or yvalue[i]=='j' or yvalue[i]=='k' or yvalue[i]=='l' or yvalue[i]=='z' or yvalue[i]=='x' or yvalue[i]=='c' or yvalue[i]=='v' or yvalue[i]=='b' or yvalue[i]=='n' or yvalue[i]=='m' or yvalue[i]=='Q' or yvalue[i]=='W' or yvalue[i]=='E' or yvalue[i]=='R' or yvalue[i]=='T' or yvalue[i]=='Y' or yvalue[i]=='U' or yvalue[i]=='I' or yvalue[i]=='O' or yvalue[i]=='P' or yvalue[i]=='A' or yvalue[i]=='S' or yvalue[i]=='D' or yvalue[i]=='F' or yvalue[i]=='G' or yvalue[i]=='H' or yvalue[i]=='J' or yvalue[i]=='K' or yvalue[i]=='L' or yvalue[i]=='Z' or yvalue[i]=='X' or yvalue[i]=='C' or yvalue[i]=='V' or yvalue[i]=='B' or yvalue[i]=='N' or yvalue[i]=='M') tempyn = tempyn + yvalue[i];
        // every letter will be added in the variable tempyn so we can study it
        tempy = "";
        if  (yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) tempyn = "";
        if  ((yvalue[i+1]=='+' or yvalue[i+1]=='=' or yvalue[i+1]=='-' or yvalue[i+1]=='*' or yvalue[i+1]=='^' or yvalue[i+1]=='/' or yvalue[i+1]=='(' or yvalue[i+1]==')' or yvalue[i+1]=='1' or yvalue[i+1]=='2' or yvalue[i+1]=='3' or yvalue[i+1]=='4' or yvalue[i+1]=='5' or yvalue[i+1]=='6' or yvalue[i+1]=='7' or yvalue[i+1]=='8' or yvalue[i+1]=='9' or yvalue[i+1]=='0' or yvalue[i+1]=='.' or yvalue[i+1]==' ' or yvalue[i+1]==',' or yvalue[i+1]=='=' or (i+1)==(yvalue.length())) ) {
            if (tempyn==var) tempyval = tempyval + dnum; //replace every x with the correct value
            if (tempyn!=var) tempyval = tempyval + tempyn;
            tempyn = "";
        }
        //use the correct functions
        //if the carachter is a number or a mathematic simbol we simply copy it
        if  ((yvalue[i]=='=' or yvalue[i]=='+' or yvalue[i]=='-' or yvalue[i]=='*' or yvalue[i]=='^' or yvalue[i]=='/' or yvalue[i]=='(' or yvalue[i]==')' or yvalue[i]=='1' or yvalue[i]=='2' or yvalue[i]=='3' or yvalue[i]=='4' or yvalue[i]=='5' or yvalue[i]=='6' or yvalue[i]=='7' or yvalue[i]=='8' or yvalue[i]=='9' or yvalue[i]=='0' or yvalue[i]=='.' or yvalue[i]==',' ) and (olda!=1) ) tempyval = tempyval + yvalue[i];
        
        if (tempyval!="") mreport = tempyval;
    }
    return mreport;
}

bool Calculations::check(QString func) { //returns true if it does not contain dangerous chars, false if there is one
    
    bool result = true;
    if (func.isEmpty()) result = false;
    
    for (int i=0; i<func.length();i++) {
        if (!(func.at(i).isLetter() or func.at(i).isNumber() or func.at(i)=='+' or func.at(i)=='-' or func.at(i)=='^' or func.at(i)=='*' or func.at(i)=='/' or func.at(i)=='(' or func.at(i)==')' or func.at(i)=='.' or func.at(i)==','or func.at(i)=='=')) result = false;
    }
    if (func.indexOf("**")!=-1) result = false;
    if (func.indexOf("==")!=-1) result = false;
    if (func.indexOf("/*")!=-1) result = false;
    if (func.indexOf("//")!=-1) result = false;
    if (func.indexOf("\\")!=-1) result = false;
    if (func.indexOf("..")!=-1) result = false;
    if (func.indexOf(",,")!=-1) result = false;
    
    return result;
}

