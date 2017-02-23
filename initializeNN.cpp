/* 
 * File:   main.cpp
 * Author: Fred
 *
 * Created on 2 dicembre 2012, 11.41
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cmath>

//#include "NNController.h"
#include "initializeNN.hpp"

using namespace std;

NNController initializeNN() 
{
    cout << "Main_1: creo NNController." << endl;
    NNController NC(12, 10, 2, 60);                     // NC(c_numInputNeurons, c_numOutputNeurons, c_numHiddenLayers, c_numNeuronsPerLayer);
    
    NC.loadWeights("pesi.txt");
    
    return NC;
}

