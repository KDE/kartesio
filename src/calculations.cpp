#include "calculations.h"

Calculations::Calculations()
{
//
}

Calculations::~Calculations()
{
    //
}

QString Calculations::calculate(QTableWidget *table,  QLineEdit *func) {
    //this function calculates the best fit curve from some points and a generic function
    m_resultFunction = "";
    m_myReport="Values obtained by maxima:\n";
    m_width = int(m_xmax - m_xmin);
    int totalcoeff=0;
    QStringList coeff;
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

        m_tryNumber = 0;
        //run the procedure until every point has been used
        for (int f=0; f<(totaldata);f++) {
            //we try to prepare the command line for maxima
            QString cmd = "maxima --batch-string=\"numer:true;solve([";
            int fir = 1;
            int i = f;
            //now i'm using the value of the points to fit the curve
            //REMEMBER that coeff.at(u) is the actual coefficient px[i] and py[i] are the x,y values of the point used
            for (int u=0; u<totalcoeff; u++) {
                QByteArray banf = func->text().toLatin1();
                char *tmfn = banf.data();
                QString xtmp;
                xtmp.setNum(px[i%totalcoeff]).replace(QString(","), QString("."));
                QString eqa = replacevar(tmfn,xtmp,QString("x"));
                QByteArray banfn = eqa.toLatin1();
                char *tmfns = banfn.data();
                xtmp.setNum(py[i%totalcoeff]).replace(QString(","), QString("."));
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
            for (int u=0; u<totalcoeff; u++) {
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
            //if  (go!=0) QMessageBox::critical(this,"Error","Seems that Maxima process died calculating the result.") ;
            if  (go!=0) return QString("died");

            char tmpchr;
            ifstream texto("/tmp/kartesiotmp.txt");
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
            //QString cancstr = tempstr.replace(" ","");
            QString cancstr = tempstr.replace("[","");
            cancstr = cancstr.replace("]","");
            if (myfunz.indexOf(",")==-1 and totalcoeff>1) return 0;
            QStringList cmvalue = cancstr.split(",");
            m_myReport = m_myReport + cancstr+ "\n";
            //ignore the result if it is not a correct number
            int good = 1;
            for (int u=0; u<totalcoeff; u++) {
                for (int n=0; n<cmvalue.count() ; n++) {
                    if (cmvalue.at(n).indexOf("=")==-1) {
                        break;
                        good = 0;
                    }
                    QStringList mnval = cmvalue.at(n).split("=");
                    QScriptEngine myEnginee;
                    QScriptValue isreal = myEnginee.evaluate(mnval.at(1)+"*0");
                    if (isreal.toString()!="0") good = 0;
                }
            }


            myfunz = func->text();

            if (good == 1) {
                for (int u=0; u<totalcoeff; u++) {
                    for (int n=0; n<cmvalue.count() ; n++) {
                        if (cmvalue.at(n).indexOf("=")!=-1) {
                            QStringList mnval = cmvalue.at(n).split("=");
                            if (mnval.at(0)==coeff.at(u)) {
                                //now we must sum the new value to the others
                                QString newvar = mnval.at(1);
                                if (m_resultFunction=="") m_resultFunction = myfunz;
                                if (f!=0 and m_tryNumber!=0) {  //remember that f begins with 0
                                    QStringList mnvalo = m_oldValue.at(n).split("=");
                                    //double newval = (((mnvalo.at(1).toDouble()*(f))+mnval.at(1).toDouble())/(f+1));
                                    double newval = (((mnvalo.at(1).toDouble()*(m_tryNumber))+mnval.at(1).toDouble())/(m_tryNumber+1));
                                    newvar.setNum(newval);
                                }
                                QByteArray banf = m_resultFunction.toLatin1();
                                char *tmfnz = banf.data();
                                //myfunz = replacevar(tmfnz,newvar, coeff.at(u));
                                m_resultFunction = replacevar(tmfnz,newvar, coeff.at(u));
                                m_tryNumber++;
                            }
                        }
                    }
                }
                m_oldValue = cmvalue;
            }
        } //here ends the search for coefficients values
    } //here ends the "else"
    return m_resultFunction;
    //m_resultFunction = "";
}

QString Calculations::solvex(char *yvalue, QString dnum) {
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
	//if (tempyn=="E") tempy = "*10^";
	if (tempyn=="E") {
	  tempy = "*Math.pow(10,";
	  do{
	    i++;
	    tempy = tempy + yvalue[i];
	  }while ((QString(yvalue[i+1])!=QString("*")));//until i+1=="*"
	  tempy = tempy + ")";
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
    return mreport;
}

QString Calculations::replacevar(char *yvalue, QString dnum, QString var) {
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
    return mreport;
}
