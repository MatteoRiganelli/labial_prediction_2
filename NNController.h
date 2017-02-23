/* 
 * File:   NNController.h
 * Author: Fred
 *
 * Created on 2 dicembre 2012, 12.00
 */

#ifndef NNCONTROLLER_H
#define	NNCONTROLLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <vector>
#include <stdio.h>

#include "NeuralNet.h" 
//#include "Misc.h"

using namespace std;

class NNController 
{
public:
    // costruisce la rete con i parametri specificati
    NNController(int c_numInputNeurons, int c_numOutputNeurons, int c_numHiddenLayers, int c_numNeuronsPerLayer);
    
    void loadTrainingSet(string inputFile, string outputFile);
    void loadValidationSet(string validationFile);
    
    // metodo che addestra la rete
    void train();
    
    void saveWeights(string file);
    void loadWeights(string file);
    
    void test(); // Permette di testare la rete neurale una volta che è stata allenata
    void testSet();
    int classify(double nnInput[12]);
    
private:
    vector< vector<double> > inputData;     // valori di INPUT del Traning Set
    vector< vector<double> > outputData;    // valori di OUTPUT del Traning Set
    vector< vector<double> > validationData;    // valori di OUTPUT del Traning Set
    NeuralNet* N;
};

#endif	/* NNCONTROLLER_H */

