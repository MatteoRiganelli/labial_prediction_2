
#include "Neuron.h"

Neuron::Neuron(int c_numWeights) {
    //cout << "Neuron: creo neurone con " << c_numWeights << " pesi" << endl;
    for (int i = 0; i < c_numWeights + 1; i++) {
        weights.push_back(wRand());
    }
}

void Neuron::setValue(double v) {
    value = v;
}

double Neuron::getValue() const {
    return value;
}

void Neuron::setWeight(int i, double v) {
    if (i < weights.size()) {
        weights[i] = v;
    } else {
        cout << "Error: Neuron::setWeight(): index `i` out of range." << endl;
        exit(0);
    }
}

double Neuron::getWeight(int i) const {
    if (i < weights.size()) {
        return weights[i];
    } else {
        cout << "Error: Neuron::getWeight(): index `i` out of range." << endl;
        exit(0);
    }
}

int Neuron::getWeightsNum(bool bias) const {
    return (bias) ? weights.size() : weights.size() - 1; // Se bias == true ritorna i pesi contando anche il bias, altrimenti senza contarlo
}
