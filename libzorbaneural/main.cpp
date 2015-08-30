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

//  http://www.developer.nokia.com/Community/Wiki/Building_a_library_project_in_Qt_using_Qt_Creator
//  * http://qt-project.org/wiki/How_to_create_a_library_with_Qt_and_use_it_in_an_application
//  http://qt-project.org/doc/qt-4.8/sharedlibrary.html

#include "neuralnet.h"
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[])
{

    // prepare XOR traing data
    double data[][3]={
        0,	0,	0,
        0,	1,	1,
        1,	0,	1,
        1,	1,	0 };

    // prepare XOR test data
    double testData[][2]={
        1,	0,
        0,	0.87,
        0,	0.08,
        0.35,	1,
        0.79, 0.95};

    /*
//prepare addition training data
    double data[][3]={
        0,	0.2,	0.2,
        0.2,	0.1,	0.3,
        0.5,	0.3,	0.8,
        0.05,	0.03,	0.08,
        0.25,	0.16,	0.41,
        0.7,	0.11,	0.81,
        0.39,	0.49,	0.88,
        0.22,	0.38,	0.60,
        0.9,	0.1,	1,
        0,	0.1,	0.1,
        0,	0.01,	0.01,
        0.01,	0.01,	0.02,
        0.1,	0.1,	0.2,
        0.5,	0.5,	1,
        0,	1,	1,
        0,	0,	0,
        0.8,	0.2,	1 };



    // prepare addition test data
    double testData[][2]={
                0.1,	0,
                0.3,	0.4,
                0.6,	0.7,
                0.24,	0.36,
                0.5,	0.3};

*/


    //declaring vectors to substitute arrays
    std::vector < std::vector <double> > dataV;
    std::vector<std::vector<double> > testDataV;

    //resizing vectors
    dataV.resize(sizeof (data) / sizeof (data[0]));
    for (int j=0; j<(sizeof (data) / sizeof (data[0]));j++) {
        dataV[j].resize((sizeof(data[0])/ (sizeof data[0][0])));
    }

    testDataV.resize(sizeof (testData) / sizeof (testData[0]));
    for (int j=0; j<(sizeof (testData) / sizeof (testData[0]));j++) {
        testDataV[j].resize((sizeof(testData[0])/ (sizeof testData[0][0])));
    }


    //copying arrays values in vectors
    for (int j=0; j<(sizeof (data) / sizeof (data[0]));j++){
        for (int t=0; t<(sizeof(data[0])/ (sizeof data[0][0])); t++){
            dataV[j][t] = data[j][t];
        }
    }


    for (int j=0; j<(sizeof (testData) / sizeof (testData[0]));j++){
        for (int t=0; t<(sizeof(testData[0])/ (sizeof testData[0][0])); t++){
            testDataV[j][t] = testData[j][t];
        }
    }



    //you should have at least 3 layers (input, hidden, and output)
    //the first layer is the input layer, it should have as many neurons as the number of input parameters
    //the last layer is the output layer, usually it has only 1 neuron
    int layers = 4;
    int lSz[4] = {2,2,2,1};

    double learningRate = 0.3;
    double momentum = 0.1;


    // maximum no of iterations during training
    long maxIters = 2000000;
    long i;

    std::vector<int> lSzV;
    lSzV.resize(layers);
    for(int i=0;i<layers;i++){
        lSzV[i]=lSz[i];
    }


    // Creating the net
    ZorbaNN *myNet = new ZorbaNN(layers, lSzV, learningRate, momentum);

    cout << "Loading the network..." << endl;
    myNet->setNetwork("/home/luca/Progetti/archimede/zorbaneural/libzorbaneural/xor-test/testNN.znf");

    cout<< endl <<  "Now training the network...." << endl;

    //myNet->printWeights();
    //myNet->printPopulation();

    myNet->recursiveTrainBackProp(dataV, maxIters);

    int maxCrossoverNumber = 20;

    myNet->recursiveTrainGenAlg(dataV, maxIters, maxCrossoverNumber);

    //myNet->printWeights();
    //myNet->printPopulation();

    //myNet->saveNetwork("/home/luca/Progetti/archimede/zorbaneural/libzorbaneural/testNN.znf");


    cout<< "Now using the trained network to make predictions on test data...." << endl << endl;


    for ( i = 0 ; i < (sizeof (testData) / sizeof (testData[0])) ; i++ )
    {
        myNet->runNet(testDataV[i]);
        cout << testDataV[i][0]<< "  " << testDataV[i][1]<< "  " << myNet->output(0) << endl;
    }

    return 0;
}
