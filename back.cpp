int NeuralNet::Train_BackPropagation(vector<_PRECISION> des_out) {
	/* Backpropagation case */

	// Verifiy that the hidden layer exists
	if(des_out.size() != numOutputNeurons || numHiddenLayers <= 0 || numHiddenNeuronsPerLayer <= 0) return -1;

	_PRECISION error, delta_weight, prev_bp_err = 0, cur_bp_err = 0;

	for(u_int32_t l = layers.size()-1; l >= 0; l--) {	// For each layer, begin from output
		prev_bp_err = cur_bp_err;
		cur_bp_err = 0;
		if(!l) break;	// Input layer has no weights
		if(l==layers.size()-1) {// Output layer
			for(u_int32_t n = 0;n < layers[l]->getNeuronNum();n++) {	// For each neuron
				// Calculate delta error, dk = (rk - ok)*f'(netk)

				if(n==layers[l]->getNeuronNum()-1 && BIAS){				// for only bias neuron
					error  =
						(des_out[n-1] - layers[l]->getNeuron(n)->getPotential())*
						layers[l]->getNeuron(n)->trans_func_derivate(layers[l]->getNeuron(n)->getPotential());
				} else {
					error  =
						(des_out[n] - layers[l]->getNeuron(n)->getPotential())*
						layers[l]->getNeuron(n)->trans_func_derivate(layers[l]->getNeuron(n)->getPotential());
				}

				for(u_int32_t w = 0;w < layers[l]->getNeuron(n)->getWeightsNum(); w++) {// For each weight
					// Calculate delta weight, Dw = Wjk + n*dk*oj
					delta_weight = layers[l]->getNeuron(n)->getWeight(w) + l_rate*error*layers[l-1]->getNeuron(w)->getPotential();
					// Update weight
					layers[l]->getNeuron(n)->setWeight(w,delta_weight);
					// Product smmation of current error and current weight
					cur_bp_err += layers[l]->getNeuron(n)->getWeight(w)*error;
				}
			}
		} else { // Hidden layers and input
			for(u_int32_t n = 0;n < layers[l]->getNeuronNum(); n++) {// For each neuron
				// Calculate hidden layer error, remembering the previous propagated error
				error = layers[l]->getNeuron(n)->trans_func_derivate(layers[l]->getNeuron(n)->getPotential())*prev_bp_err;
				for(u_int32_t w = 0;w < layers[l]->getNeuron(n)->getWeightsNum(); w++) {// For each weight
					// Calculate delta weight, Dw = W + n*d*o
					delta_weight = layers[l]->getNeuron(n)->getWeight(w) + l_rate*error*layers[l]->getNeuron(n)->getPotential();
					// Update weight
					layers[l]->getNeuron(n)->setWeight(w,delta_weight);
					// Product smmation of current error and current weight
					cur_bp_err += layers[l]->getNeuron(n)->getWeight(w)*error;
				}
			}
		}
	}
	return 0;
}