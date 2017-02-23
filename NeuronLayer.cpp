
#include "NeuronLayer.h"

NeuronLayer::NeuronLayer(int c_numNeurons, int c_weightsPerNeuron)
{
    for (int i = 0; i < c_numNeurons; i++) {
        neurons.push_back(new Neuron(c_weightsPerNeuron));
    }
}

Neuron* NeuronLayer::getNeuron(int i) const {
    if (i < neurons.size()) {
        return neurons[i];
    } else {
        cout << "Error: NeuronLayer::getNeuron(): index `i` out of range." << endl;
        exit(0);
    }
}

int NeuronLayer::getNeuronsNum() const {
    return neurons.size();
}

inline double sigmoid(double input) {
    //cout << "funzione trasferimento usata: transf_sigmoid" << endl;
    return (1 / (1 + exp(-input) ));
}

inline double tanh_iper(double input) {
    return (exp(input) - exp(-input)) / (exp(input) + exp(-input));
}
