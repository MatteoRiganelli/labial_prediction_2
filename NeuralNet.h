/* 
 * File:   NeuralNet.h
 * Author: Fred
 *
 * Created on 2 dicembre 2012, 11.44
 */

#ifndef NEURALNET_H
#define	NEURALNET_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <vector>

#include "NeuronLayer.h"
//#include "Misc.h"

using namespace std;

// classe che crea la rete neurale
class NeuralNet {
public:
    // c_numInputNeurons = canali di ingresso per ogni neurone
    // c_numOutputNeurons = numero di neuroni del layer di output
    // c_numHiddenLayers = numero di livelli nascosti
    // c_numNeuronsPerLayer = numero di neuroni per ogni livello nascosto
    NeuralNet(int c_numInputNeurons, int c_numOutputNeurons, int c_numHiddenLayers, int c_numNeuronsPerLayer);
    
    // crea eventuali layer nascosti e il layer di output
    void createNet();
    
    // aggiorna i pesi associati ad ogni canale di ogni neurone di ogni livello
    // e calcola la funzione di attivazione per ogni neurone di ogni livello
    void run(vector<double> input, vector<double> output);
    
    void feedForward(vector<double> input);
    void feedForwardSoft(double input[12]);
    void backPropagation(vector<double> input, vector<double> output);
    
    void updateWeights(vector<double> input);
    
    int totalNumWeights(bool bias);
    void stampaPesi();
    void dump();
    double globalError(vector<double> des);
    double globalCrossError(vector<double> des);
    
    void printOutput();
    double results();
    
    int numInputs() const;
    int numOutputs() const;
    int numLayers() const;
    NeuronLayer* getLayer(int);
private:
    // vettore che memorizza i layer della rete
    vector<NeuronLayer*> layers;
    
    int numInputNeurons,
    numOutputNeurons,
    numHiddenLayers,
    numNeuronsPerLayer;
    
    //vector<double*> delta;
    //double delta[numHiddenLayers+1];
    double** delta;
    double*** prevDiffWeight;
};

#endif	/* NEURALNET_H */

