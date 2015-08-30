/***************************************************************************
 *   Zorba Neural Network (ZorbaNeural, or ZorbaNN) is a feed forward      *
 *   neural network. The network is pretty flexible and can be used        *
 *   almost for every I.A. application. The network supports multiple      *
 *   fully connected hidden layers and training with backpropagation       *
 *   and genetic algorithms.                                               *
 *                                                                         *
 *                   Zorba Neural has been created by                      *
 *                Luca Tringali, TRINGALINVENT@libero.it                   *
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


#include "neuralnet.h"


ZorbaNN::ZorbaNN(int nl,std::vector<int> sz, double learn,double mom)
{

    learningRate = learn;
    momentum = mom;
    layers=nl;

    //resizing vectors

    lsize.resize(layers);

    for(int i=0;i<layers;i++){
        lsize[i]=sz[i];
    }

    resizeVectors();

    //generate new weights
    generateWeights();

}



ZorbaNN::~ZorbaNN()
{
    int i;
    for(i=0;i<layers;i++)
        delete &out[i];
    delete &out;

    for(i=1;i<layers;i++)
        delete &delta[i];
    delete &delta;

    for(i=1;i<layers;i++)
        for(int j=0;j<lsize[i];j++)
            delete &weights[i][j];
    for(i=1;i<layers;i++)
        delete &weights[i];
    delete &weights;

    for(i=1;i<layers;i++)
        for(int j=0;j<lsize[i];j++)
            delete &oldWeights[i][j];
    for(i=1;i<layers;i++)
        delete &oldWeights[i];
    delete &oldWeights;

    for(i=1;i<MAXpopulation;i++)
        delete &population[i];
    delete &population;

    delete &lsize;

}

//	sigmoid function
double ZorbaNN::sigmoid(double in)
{
    return (1/(1+exp(-in)));
}

double ZorbaNN::getWeight(int i, int j, int k)
{
    return weights[i][j][k];
}

//	calculate root mean square error of the actual weights
double ZorbaNN::rmsError(double wanted) const
{
    //root mean square
    double rmsError=0;
    for(int i=0;i<lsize[layers-1];i++){
        rmsError+= std::pow((wanted-out[layers-1][i]), 2);
    }
    return std::pow((rmsError/lsize[layers-1]),0.5);
}

//	return content of out vector
double ZorbaNN::output(int i) const
{
    return out[layers-1][i];
}

// feed forward
void ZorbaNN::runNet(std::vector<double> in)
{
    double sum;
    int i;

    for(i=0;i<lsize[0];i++)
        out[0][i]=in[i];

    // i = current layer, j= current neuron, k = current input = number of neurons in the preceeding layer
    // k'th input of j'th neuron in i'th layer
    for(i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            sum=0.0;
            for(int k=0;k<lsize[i-1];k++){
                sum+= out[i-1][k]*weights[i][j][k];
            }
            sum+=weights[i][j][lsize[i-1]];
            out[i][j]=sigmoid(sum);
        }
    }
}




void ZorbaNN::recursiveTrainBackProp(std::vector<std::vector<double> > data, long maxIters)
{
    long i = 0;
    int j = data.size();
    int t = data.at(0).size() -1;
    for (i=0; i<maxIters ; i++)
    {
        trainBackProp(data[i%j], data[i%j][t]);
        //TODO is it possible to show somewhere the actual weights?
        //if (viewweights) saveWeights(fileviewweights);
    }
    //std::cout << std::endl << i << " iterations completed with back propagation "   << "Mean Error: " << rmsError(data[(i-1)%j][t]) << std::endl;
}

void ZorbaNN::trainBackProp(std::vector <double> in, double wanted)
{
    double sum;
    int i;

    runNet(in);

    //	calculate delta
    for(i=0;i<lsize[layers-1];i++){
        delta[layers-1][i]=out[layers-1][i]*(1-out[layers-1][i])*(wanted-out[layers-1][i]);
    }

    for(i=layers-2;i>0;i--){
        for(int j=0;j<lsize[i];j++){
            sum=0.0;
            for(int k=0;k<lsize[i+1];k++){
                sum+=delta[i+1][k]*weights[i+1][k][j];
            }
            delta[i][j]=out[i][j]*(1-out[i][j])*sum;
        }
    }

    // considering momentum
    for(i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1];k++){
                weights[i][j][k]+=momentum*oldWeights[i][j][k];
                if (viewweights) saveWeights(fileviewweights);
            }
            weights[i][j][lsize[i-1]]+=momentum*oldWeights[i][j][lsize[i-1]];
        }
    }

    // considering learning rate
    for(i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1];k++){
                oldWeights[i][j][k]=learningRate*delta[i][j]*out[i-1][k];
                weights[i][j][k]+=oldWeights[i][j][k];
            }
            oldWeights[i][j][lsize[i-1]]=learningRate*delta[i][j];
            weights[i][j][lsize[i-1]]+=oldWeights[i][j][lsize[i-1]];
        }
    }
}

std::vector <double> ZorbaNN::weightsToRNA(std::vector<std::vector<std::vector<double> > > tmpWeights){

    int length = 0;  //total weight number
    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                length++;
            }
        }
    }

    std::vector <double> tmpRNA;
    tmpRNA.resize( length+1 );
    tmpRNA[0] = actualMaxError+1.0; //the first element is the fitness

    int n = 1;
    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                tmpRNA[n] = tmpWeights[i][j][k];
                n++;
            }
        }
    }
    return tmpRNA;
}

void ZorbaNN::crossover(std::vector <double> RNAA, std::vector <double> RNAB, int length, std::vector <double> in, double wanted){

    int crosspoint = ((int)rand())%length;


    std::vector <double> crossRNA1;
    crossRNA1.resize(length+1);
    crossRNA1[0] = actualMaxError;


    std::vector <double> crossRNA2;
    crossRNA2.resize(length+1);
    crossRNA2[0] = actualMaxError;


    for (int i = 0; i<crosspoint; i++) {
        crossRNA1[i+1] = RNAA[i];
    }
    for (int i = crosspoint; i<crossRNA1.size(); i++) {
        crossRNA1[i] = RNAB[i];
    }
    for (int i = 0; i<crosspoint; i++) {
        crossRNA2[i+1] = RNAB[i];
    }
    for (int i = crosspoint; i<crossRNA2.size(); i++) {
        crossRNA2[i] = RNAA[i];
    }

    //introducing a casual mutation
    if (((rand() % 10 + 1)/10) <= mutationRate) {
        int s = -1;
        if (rand()%2) s = 1;
        int index;
        index = rand() % crossRNA1.size();
        if (((int)rand())%2) crossRNA1[index] += (s*(rand() % 2000 + 1)/1000);
        if (((int)rand())%2) crossRNA2[index] += (s*(rand() % 2000 + 1)/1000);
    }



    //calculating crossRNA1 and crossRNA2 fitness

    int d = 1; //the first element is the fitness
    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                weights[i][j][k] = crossRNA1[d];
                d++;
            }
        }
    }


    runNet(in);
    double error1 = rmsError(wanted);

    d = 1; //the first element is the fitness
    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                weights[i][j][k] = crossRNA2[d];
                d++;
            }
        }
    }

    runNet(in);
    double error2 = rmsError(wanted);

    //now I save the fitness as first element of the vector
    crossRNA1[0] = error1;
    crossRNA2[0] = error2;

    if ((crossRNA2[0]<actualMinError)&&(crossRNA2[0]>(minAcceptableError))) {     //if the error is too little, something is going wrong
        //adding first chromosome at the population vector
        for (int i = 0; i<MAXpopulation; i++) {
            if ((population[i][0]==actualMaxError)||(i==(MAXpopulation-1))) {
                for (int h = 0; h<(length+1); h++) {
                    population[i][h] = crossRNA1[h];
                }
                break;
            }
        }
    }

    //calculating actual min error and max error
    actualMinError = actualMaxError;
    actualMaxError = 0;
    for (int i = 0; i<MAXpopulation; i++) {
        if (population[i][0]>actualMaxError) actualMaxError = population[i][0];
        if ((population[i][0]<actualMinError)) actualMinError = population[i][0];
    }

    //doing the same for the crossRNA2
    if ((crossRNA2[0]<actualMinError)&&(crossRNA2[0]>(minAcceptableError))) {      //if the error is too little, something is going wrong
        for (int i = 0; i<MAXpopulation; i++) {
            if ((population[i][0]==actualMaxError)||(i==(MAXpopulation-1))) {
                for (int h = 0; h<(length+1); h++) {
                    population[i][h] = crossRNA2[h];
                }
                break;
            }
        }
    }

    //calculating actual min error e max error
    actualMinError = actualMaxError;
    actualMaxError = 0;
    for (int i = 0; i<MAXpopulation; i++) {
        if (population[i][0]>actualMaxError) actualMaxError = population[i][0];
        if ((population[i][0]<actualMinError)) actualMinError = population[i][0];
    }

    //std::cout << "Actual minimum error: " << actualMinError << std::endl;
}



void ZorbaNN::trainGenAlg(std::vector <double> in, double wanted, long maxCrossoverNumber)
{


    int length = 0;  //total weights number

    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                length++;
            }
        }
    }


    //the first RNA couple come from weights and oldWeights
    std::vector <double> firstRNAA;
    std::vector <double> firstRNAB;
    firstRNAA = weightsToRNA(weights);
    firstRNAB = weightsToRNA(oldWeights);

    runNet(in);
    if (actualMinError == 1000.0){
        firstRNAA[0] = rmsError(wanted);
        firstRNAB[0] = firstRNAA[0]+1;
        actualMinError = firstRNAA[0];
        actualMaxError = firstRNAB[0];

        //filling population vector with initial RNA
        for (int i = 0; i<MAXpopulation; i++) {
            if ((i%2==0) || i==0) {
                for (int h = 0; h<(length+1); h++) {
                    population[i][h] = firstRNAA[h];
                }
            } else {
                for (int h = 0; h<(length+1); h++) {
                    population[i][h] = firstRNAB[h];
                }
            }
        }

    } else {
        firstRNAA[0] = actualMinError;
        firstRNAB[0] = actualMaxError;
    }


    //the first time we must execute the cross over
    crossover(firstRNAA, firstRNAB, length, in, wanted);

    //esecuting maximum crossover number
    for (int g = 0; g<maxCrossoverNumber; g++) {
        //choosing RNAB casually, but RNAA is always the best
        int b = (rand() % MAXpopulation);
        int a = 0;
        for (int t = 0; t<MAXpopulation; t++) {
            if ((population[t][0]==actualMinError)||(t==(MAXpopulation-1))) {
                a = t;
                break;
            }
        }
        //executing casually the crossover
        if (((rand() % 10 + 1)/10) <= crossoverRate)   crossover(population[a], population[b], length, in, wanted);
    }


    //moving the RNA in the weights vector
    for (int t = 0; t<MAXpopulation; t++) {
        if ((population[t][0]==actualMinError)||(t==MAXpopulation-1)) {
            int n = 1; //the first element is the fitness
            for(int i=1;i<layers;i++){
                for(int j=0;j<lsize[i];j++){
                    for(int k=0;k<lsize[i-1]+1;k++){
                        weights[i][j][k] = population[t][n];
                        n++;
                        if (viewweights) saveWeights(fileviewweights);
                    }
                }
            }
            break;
        }
    }

    //now training is done and weights have been loaded

    //std::cout << "Wanted|Result|Error: " << wanted << "|"  << wanted-out[layers-1][0] << "|" << actualMinError << std::endl;
}

void ZorbaNN::recursiveTrainGenAlg(std::vector<std::vector<double> > data, long maxIters, long maxCrossoverNumber)
{
    actualMinError = 1000.0;
    actualMaxError = actualMinError + 1.0;
    int i = 0;
    int j = data.size();
    int t = data.at(0).size() -1;
    for (i=0; i<maxIters ; i++)
    {
        trainGenAlg(data[i%j],data[i%j][t], maxCrossoverNumber);
        //TODO is it possible to show somewhere the actual weights?
        //if (viewweights) saveWeights(fileviewweights);
    }
    //std::cout << std::endl << i << " iterations completed with genetic algorithm "   << "Minimum Error: " << myNet->actualMinError << std::endl;
}

//this function will take a standard zorbaneural weights file to fill the oldWeights vector
void ZorbaNN::setOldWeights( const char filec[])
{
    /*the structure of a common zorbaneural weights file is
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&$
     * Because $ means the end of the row (first dimension), & means end of the line (second dimension), and | means end of the profondity (third dimension)
    */
    std::ifstream texto(filec);
    char tmpchr;
    std::string tempval;
    int in = 1;
    int jn = 0;
    int tn = 0;
    if (texto) {
        do {
            texto >> tmpchr;
            if (tmpchr!='|' && tmpchr!='&' && tmpchr!='$') tempval = tempval + tmpchr;
            if (tmpchr=='|') {
                double tempnum =  atof(tempval.c_str());
                oldWeights[in][jn][tn] = tempnum;
                tn++;
                tempval = "";
            }
            if (tmpchr=='&') {
                tn=0;
                jn++;
            }
            if (tmpchr=='$') {
                jn=0;
                tn=0;
                in++;
            }
        } while (!texto.eof());
        texto.close();
    }

}

//this function will create a standard zorbaneural oldWeights file
void ZorbaNN::saveOldWeights( const char filec[])
{
    /*the structure of a common zorbaneural weights file is
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&$
     * Because $ means the end of the row (first dimension), & means end of the line (second dimension), and | means end of the profondity (third dimension)
    */
    std::string endval;
    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                std::ostringstream strs;
                strs << std::fixed << std::setprecision(6) << oldWeights[i][j][k];
                endval = endval + strs.str() + "|";
            }
            endval = endval + "&\n";
        }
        endval = endval + "$\n";
    }

    char *strsave = (char*)endval.c_str();
    std::ofstream out(filec);
    out << strsave;
    out.close();
}


//this function will take a standard zorbaneural weights file to fill the weights vector
void ZorbaNN::setWeights( const char filec[])
{
    /*the structure of a common zorbaneural weights file is
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&$
     * Because $ means the end of the row (first dimension), & means end of the line (second dimension), and | means end of the profondity (third dimension)
    */
    std::ifstream texto(filec);
    char tmpchr;
    std::string tempval;
    int in = 1;
    int jn = 0;
    int tn = 0;
    if (texto) {
        do {
            texto >> tmpchr;
            if (tmpchr!='|' && tmpchr!='&' && tmpchr!='$') tempval = tempval + tmpchr;
            if (tmpchr=='|') {
                double tempnum =  atof(tempval.c_str());
                weights[in][jn][tn] = tempnum;
                tn++;
                tempval = "";
            }
            if (tmpchr=='&') {
                tn=0;
                jn++;
            }
            if (tmpchr=='$') {
                jn=0;
                tn=0;
                in++;
            }
        } while (!texto.eof());
        texto.close();
    }

}

//print on the screen the content of weights array
void ZorbaNN::printWeights(){
    std::string endval;
    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                std::ostringstream strs;
                strs << std::fixed << std::setprecision(6) << weights[i][j][k];
                endval = endval + strs.str() + "|";
            }
            endval = endval + "&\n";
        }
        endval = endval + "$\n";
    }

    std::cout << endval.c_str();
}


//this function will create a standard zorbaneural weights file
void ZorbaNN::saveWeights( const char filec[])
{
    /*the structure of a common zorbaneural weights file is
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&$
     * Because $ means the end of the row (first dimension), & means end of the line (second dimension), and | means end of the profondity (third dimension)
    */
    std::string endval;
    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                std::ostringstream strs;
                strs << std::fixed << std::setprecision(6) << weights[i][j][k];
                endval = endval + strs.str() + "|";
            }
            endval = endval + "&\n";
        }
        endval = endval + "$\n";
    }

    char *strsave = (char*)endval.c_str();
    std::ofstream out(filec);
    out << strsave;
    out.close();
}


//resize vectors
void ZorbaNN::resizeVectors(){


    //std::vector<std::vector<double> > out;
    out.resize(layers);
    for (int j=0; j<layers;j++) {
        out[j].resize(lsize[j]);
    }

    //std::vector<std::vector<double> > delta;
    delta.resize(layers);
    for (int j=0; j<layers;j++) {
        delta[j].resize(lsize[j]);
    }

    //std::vector<std::vector<std::vector<double> > > weights;
    weights.resize(layers);
    for (int i=0; i<layers;i++) {
        weights[i].resize(lsize[i]);
        for (int j=0; j<lsize[i];j++) {
            if (lsize[i-1]+1 > 0) weights[i][j].resize(lsize[i-1]+1);
            if (lsize[i-1]+1 <= 0) weights[i][j].resize(1);
        }
    }


    //std::vector<std::vector<std::vector<double> > > oldWeights;
    oldWeights.resize(layers);
    for (int i=0; i<layers;i++) {
        oldWeights[i].resize(lsize[i]);
        for (int j=0; j<lsize[i];j++) {
            if (lsize[i-1]+1 > 0) oldWeights[i][j].resize(lsize[i-1]+1);
            if (lsize[i-1]+1 <= 0) oldWeights[i][j].resize(1);
        }
    }

    int length = 0;  //total weights number

    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                length++;
            }
        }
    }

    population.resize(MAXpopulation);
    for (int j=0; j<MAXpopulation;j++) {
        population[j].resize(length+1);
    }

}

//generate initial weights
void ZorbaNN::generateWeights(){

    //	calculate random weights
    srand((unsigned)(time(NULL)));
    for(int i=1;i<layers;i++)
        for(int j=0;j<lsize[i];j++)
            for(int k=0;k<lsize[i-1]+1;k++)
                weights[i][j][k]=(rand())/(RAND_MAX/2) - 1;

    //	initialize old weights to 0
    for(int i=1;i<layers;i++)
        for(int j=0;j<lsize[i];j++)
            for(int k=0;k<lsize[i-1]+1;k++)
                oldWeights[i][j][k]=0.0;

}

//print on the screen the content of population vector
void ZorbaNN::printPopulation(){
    int length = 0;  //total weights number

    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                length++;
            }
        }
    }

    std::string endval;
    for (int t = 0; t<MAXpopulation; t++) {
        for (int n = 0; n<(length+1); n++) {
            std::ostringstream strs;
            strs << std::fixed << std::setprecision(6) << population[t][n];
            endval = endval + strs.str() + "|";
        }
        endval = endval + "\n";
    }

    std::cout << endval.c_str();
}

//this function will fill the population vector
void ZorbaNN::setPopulation( const char filec[])
{
    /*the structure of a common zorbaneural population file is
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
    */
    std::ifstream texto(filec);
    char tmpchr;
    std::string tempval;
    int in = 0;
    int jn = 0;
    if (texto) {
        do {
            texto >> tmpchr;
            if (tmpchr!='|' && tmpchr!='&') tempval = tempval + tmpchr;
            if (tmpchr=='|') {
                double tempnum =  atof(tempval.c_str());
                population[in][jn] = tempnum;
                jn++;
                tempval = "";
            }
            if (tmpchr=='&') {
                jn=0;
                in++;
            }
        } while (!texto.eof());
        texto.close();
    }

}


//this function will create a standard zorbaneural population file
void ZorbaNN::savePopulation( const char filec[])
{
    /*the structure of a common zorbaneural population file is
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
    */
    int length = 0;  //total weights number

    for(int i=1;i<layers;i++){
        for(int j=0;j<lsize[i];j++){
            for(int k=0;k<lsize[i-1]+1;k++){
                length++;
            }
        }
    }

    std::string endval;
    for (int t = 0; t<MAXpopulation; t++) {
        for (int n = 0; n<(length+1); n++) {
            std::ostringstream strs;
            strs << std::fixed << std::setprecision(6) << population[t][n];
            endval = endval + strs.str() + "|";
        }
        endval = endval + "&\n";
    }

    char *strsave = (char*)endval.c_str();
    std::ofstream out(filec);
    out << strsave;
    out.close();
}

//this function will load the entire network
void ZorbaNN::setNetwork( const char filec[])
{
    /*the structure of a common zorbaneural population file is
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
     * VALUE | VALUE | VALUE |&
    */

    std::string tmpfile(filec);
    std::string tmpweights(tmpfile.substr(0,(tmpfile.length()-4)));
    std::string tmpold(tmpfile.substr(0,(tmpfile.length()-4)));
    std::string tmppop(tmpfile.substr(0,(tmpfile.length()-4)));
    tmpweights.append("-weights.zwf");
    tmpold.append("-oldweights.zwf");
    tmppop.append("-population.zpf");


    std::ifstream texto(filec);
    char tmpchr;
    std::string tempval;
    int in = 0;
    //int jn = 0;
    int cont = 0;
    if (texto) {
        do {
            texto >> tmpchr;
            if (tmpchr!='|' && tmpchr!='&') tempval = tempval + tmpchr;
            if (tmpchr=='|') {
                double tempnum =  atof(tempval.c_str());
                if (cont == 0) {
                    layers = tempnum;
                    lsize.resize(layers);
                }
                if (cont == 1) MAXpopulation = tempnum;
                if (cont == 2) learningRate = tempnum;
                if (cont == 3) momentum = tempnum;
                if (cont == 4) actualMinError = tempnum;
                if (cont == 5) actualMaxError = tempnum;
                if (cont == 6) minAcceptableError = tempnum;
                if (cont == 7) mutationRate = tempnum;
                if (cont == 8) crossoverRate = tempnum;
                if (cont == 9) {
                    lsize[in] = tempnum;
                    in++;
                }
                //jn++;
                tempval = "";
            }
            if (tmpchr=='&') {
                //jn=0;
                //in++;
                cont++;
            }
        } while (!texto.eof());
        texto.close();
    }

    //resize main vectors
    resizeVectors();

    setPopulation(tmppop.c_str());
    setWeights(tmpweights.c_str());
    setOldWeights(tmpold.c_str());

}

//this function will save the entire network
void ZorbaNN::saveNetwork( const char filec[])
{

    std::string tmpfile(filec);
    std::string tmpweights(tmpfile.substr(0,(tmpfile.length()-4)));
    std::string tmpold(tmpfile.substr(0,(tmpfile.length()-4)));
    std::string tmppop(tmpfile.substr(0,(tmpfile.length()-4)));
    tmpweights.append("-weights.zwf");
    tmpold.append("-oldweights.zwf");
    tmppop.append("-population.zpf");

    savePopulation(tmppop.c_str());
    saveWeights(tmpweights.c_str());
    saveOldWeights(tmpold.c_str());



    std::string endval;

    std::ostringstream strs;

    strs << std::fixed << layers;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << MAXpopulation;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << std::setprecision(6) << learningRate;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << std::setprecision(6) << momentum;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << std::setprecision(6) << actualMinError;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << std::setprecision(6) << actualMaxError;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << std::setprecision(6) << minAcceptableError;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << std::setprecision(6) << mutationRate;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    strs << std::fixed << std::setprecision(6) << crossoverRate;
    endval = endval + strs.str() + "|";
    endval = endval + "&\n";
    strs.str("");

    //	vector of each layer's elements
    //std::vector<int> lsize;

    for (int t = 0; t<(lsize.size()); t++) {
        std::ostringstream strsl;
        strsl << std::fixed << lsize[t];
        endval = endval + strsl.str() + "|";
    }
    endval = endval + "&\n";


    char *strsave = (char*)endval.c_str();
    std::ofstream out(filec);
    out << strsave;
    out.close();
}


bool ZorbaNN::fileExists (char *filec) {
    std::ifstream in(filec);
    if (!in) {
        in.close();
        return 0;
    } else {
        in.close();
        return 1;
    }
}
