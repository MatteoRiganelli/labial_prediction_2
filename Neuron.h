/* 
 * File:   Neuron.h
 * Author: Fred
 *
 * Created on 2 dicembre 2012, 11.43
 */

#ifndef NEURON_H
#define	NEURON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

#include "Misc.h"

using namespace std;

class Neuron    // classe che crea un singolo neurone
{
public:
    // crea un neurone con c_numWeights canali in ingresso.
    Neuron(int c_numWeights); 
    
    // memorizza il valore trasmesso dal neurone. 
    // Il valore è calcolato come y=fun_di_attiv( sum(x_i*p_i) )        ,x_i=canale di ingresso, p_i=peso associato al canale 
    void setValue(double v); 
    double getValue() const;
    
    // memorizza il peso v, del canale i
    void setWeight(int i, double v);
    double getWeight(int i) const; // weight = ingresso i del neurone W_i
    
    // calcola il valore restituito dalla funz di att del neurone, getWeightsNum=funz_di_att( sum(x_i*p_i) ) 
    int getWeightsNum(bool bias) const;
    
private:
    // valore di uscita del neurone
    double value;
    // vettore dei pesi in ingresso al neurone
    vector<double> weights;
};
#endif	/* NEURON_H */

