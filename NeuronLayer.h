/* 
 * File:   NeuronLayer.h
 * Author: Fred
 *
 * Created on 2 dicembre 2012, 11.44
 */

#ifndef NEURONLAYER_H
#define	NEURONLAYER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "Neuron.h"

//#include "Misc.h"

using namespace std;

class NeuronLayer {
public:
    // c_numNeurons = numero di neuroni del livello,
    // c_weightsPerNeuron = numero di canali in input per ogni neurone
    // (*t) = funzione di attivazione utilizzata dal livello
    NeuronLayer(int c_numNeurons, int c_weightsPerNeuron);   
    
    // restituisce il neuroni i
    Neuron* getNeuron(int i) const;
    int getNeuronsNum() const;             // ritorna il numero di neuroni che ha il layer
    
private:
    // vettore in cui sono memorizzati i neuroni
    vector<Neuron*> neurons;
};

#endif	/* NEURONLAYER_H */

