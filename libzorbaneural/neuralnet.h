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



#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <time.h>

//#include "neuralnet_global.h"
//class ZORBANN_DLLSPEC ZorbaNN

class ZorbaNN
{

    //	output of neurons
    std::vector<std::vector<double> > out;

    //	delta value
    std::vector<std::vector<double> > delta;

    //	total layers number
    int layers;

    //	vector of each layer's elements
    std::vector<int> lsize;

    //	vector of weights for each neuron
    std::vector<std::vector<std::vector<double> > > weights;

    //	weights of the previous iteration
    std::vector<std::vector<std::vector<double> > > oldWeights;

    //create an RNA vector
    std::vector <double> weightsToRNA(std::vector<std::vector<std::vector<double> > > tmpWeights);

    //execute crossover
    void crossover(std::vector <double> RNAA, std::vector <double> RNAB, int length, std::vector <double> in, double wanted);

    //the population of chromosomes
    std::vector<std::vector<double> > population;

    //save/set the main vectors, so you don't need to do the training every time
    void setWeights(const char filec[]);
    void saveWeights(const char filec[]);

    void setOldWeights(const char filec[]);
    void saveOldWeights(const char filec[]);

    void setPopulation(const char filec[]);
    void savePopulation(const char filec[]);

    void resizeVectors();

    bool fileExists(char *filec);

public:

    ~ZorbaNN();

    //	initializes and allocates memory
    ZorbaNN(int nl, std::vector<int> sz, double learn,double mom);

    double actualMinError = 1000.0;
    double actualMaxError = 1001.0;

    //would you like to view weights (this will slow all the training)
    bool viewweights =  false;
    char *fileviewweights;

    //	learning rate
    double learningRate;

    //	momentum parameter
    double momentum;

    //the error should not be too much little (at least in comparison with the result we want)
    double minAcceptableError = std::pow(10,-2);

    //	calculate sigmoid
    double sigmoid(double in);

    double getWeight(int i, int j, int k);

    void trainBackProp(std::vector<double> in, double wanted);

    void recursiveTrainBackProp(std::vector<std::vector<double> > data, long maxIters);

    //	feed forwards activations for one set of inputs
    void runNet(std::vector<double> in);

    //	returns root mean square error of the net
    double rmsError(double wanted) const;

    void trainGenAlg(std::vector<double> in, double wanted, long maxCrossoverNumber);

    void recursiveTrainGenAlg(std::vector<std::vector<double> > data, long maxIters, long maxCrossoverNumber);

    //	returns i'th output of the net
    double output(int i) const;

    //probability of a mutation
    double mutationRate = 0.6;

    //probability of a crossover
    double crossoverRate = 0.7;

    //number of population members (equal to chromosomes number)
    int MAXpopulation = 15;

    //print on the screen the weights and the population vectors
    void printWeights();
    void printPopulation();

    //usually it's a good idea to save the entire network. Anyway, it's also possible to save only the weights or the population. This could be used to start a training session and then updating only the weights and the population insted of the entire net (so it uses less resources). But this can generate some troubles, so only the functions to save/set the entire network are public.
    void setNetwork(const char filec[]);
    void saveNetwork(const char filec[]);

    //generate empty weights: this can be used to clean the weights and oldweights vector to start another training session
    void generateWeights();

};


