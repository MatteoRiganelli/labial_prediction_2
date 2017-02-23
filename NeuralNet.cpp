
#include "NeuralNet.h"

double sigmoid(double input) {
    //cout << "funzione trasferimento usata: transf_sigmoid" << endl;
    return (double)(1 / (1 + exp(-input) ));
}

double tanh_iper(double input) {
    return (exp(input) - exp(-input)) / (exp(input) + exp(-input));
}

double tanh_iper2(double input) {
    if (1 + exp(-input)==0)
    {
        cout << "tanh_iper2 divisione per zero" << endl;
        getchar();
    }
    double ret = (1 - exp(-input)) / (1 + exp(-input));
    //cout << ret << " ";
    return ret;
}

NeuralNet::NeuralNet(int c_numInputNeurons, int c_numOutputNeurons, int c_numHiddenLayers, int c_numNeuronsPerLayer) {
    numInputNeurons = c_numInputNeurons;
    numOutputNeurons = c_numOutputNeurons;
    numHiddenLayers = c_numHiddenLayers;
    numNeuronsPerLayer = c_numNeuronsPerLayer;
}

void NeuralNet::createNet() 
{
    cout << endl << "NeuralNet::createNet(): inizio creazione layer..." << endl;
    
    // Se ci sono layer nascosti li crea
    if (numHiddenLayers > 0) 
    {   
        // crea il primo layer nascosto che a differenza degli altri ha un numero di canali in ingresso uguale al numero di neuroni della layer di input
        cout << "createNet():: creo primo layer nascosto con: " << numNeuronsPerLayer << " neuroni e " << numInputNeurons << " input per neurone" << endl;
        NeuronLayer* HL = new NeuronLayer(numNeuronsPerLayer, numInputNeurons);
        layers.push_back(HL);
        
        // crea gli altri layer nascosti che hanno un numero di canali in ingresso uguale al numero di neuroni del layer precedente
        for (int i = 1; i < numHiddenLayers; i++) 
        { 
            cout << "createNet():: creo layer nascosto con: " << numNeuronsPerLayer << " neuroni e " << numNeuronsPerLayer << " input per neurone" << endl;
            NeuronLayer* HL = new NeuronLayer(numNeuronsPerLayer, numNeuronsPerLayer);
            layers.push_back(HL);
        }
        
        cout << "createNet():: creo layer OUTPUT con: " << numOutputNeurons << " neuroni e " << numNeuronsPerLayer << " input per neurone" << endl;
        NeuronLayer* OL = new NeuronLayer(numOutputNeurons, numNeuronsPerLayer);
        layers.push_back(OL);
    } 
    else 
    {
        // Se non ci sono layer nascosti crea direttamente il layer di output
        cout << "createNet(): creo solo layer OUTPUT con: " << numOutputNeurons << " neuroni, " << numInputNeurons << " input per neurone." << endl;
        NeuronLayer* OL = new NeuronLayer(numOutputNeurons, numInputNeurons);
        layers.push_back(OL);
    }
    
    cout << "layersTOT=" <<layers.size() << " ";
    
    prevDiffWeight = new double**[layers.size()];
    for (int p=0; p<layers.size(); p++)
    {
        prevDiffWeight[p] = new double*[layers[p]->getNeuronsNum()];
    }
    
    for (int p=0; p<layers.size(); p++)
    {
        for (int r=0; r<layers[p]->getNeuronsNum(); r++)
        {
            prevDiffWeight[p][r] = new double[layers[p]->getNeuron(r)->getWeightsNum(BIAS)];
        }
        
    }
    
    // inizialmente assegna pesi casuali
    for (int i = 0; i < layers.size(); i++) 
    { // Per ogni livello...
        for (int j = 0; j < layers[i]->getNeuronsNum(); j++) 
        { // Per ogni neurone...
            for (int k = 0; k < layers[i]->getNeuron(j)->getWeightsNum(BIAS); k++) 
            { // Per ogni canale in ingresso al neurone j, aggiorna il peso k  
                double weight = wRand();
                layers[i]->getNeuron(j)->setWeight(k, weight);     
                prevDiffWeight[i][j][k] = 0;
                //layers[i]->getNeuron(j)->setWeight(k, wRand());                
            }
        }
    }
    
    cout << "creazione layer completata." << endl;
    cout << "assegnazione casuale dei pesi...";
        
    delta = new double*[layers.size()];
    
    for(int i=0; i < layers.size(); i++)
    {
        delta[i] = new double[layers[i]->getNeuronsNum()];
    }
    
    cout << "OK" << endl;
    
    cout << "NeuralNet::createNet(): ...creazione rete comletata." << endl << endl;
}

// calcola il valore di output per ogni neurone della rete
void NeuralNet::feedForward(vector<double> input)
{
    //stampaPesi();
    if (input.size() != numInputNeurons) 
    {
        cout << "Error: NeuralNet::feedForward(): wrong input array size." << endl;
        cout << "       expected " << numInputNeurons << " instead of " << input.size() << endl;
        exit(0);
    }
    
    double totInput=0;
    double biasValue = 0;
    for (int i=0; i<layers.size(); i++)     
    { // Per ogni livello        
        if (i == 0)     // INPUT --> HIDDEN , primo hidden layer -> prendo i valori dal layer di input, totInput += ... * input[k];
        {
            for (int j=0; j<layers[i]->getNeuronsNum(); j++) 
            { // Per ogni neurone;
                totInput = 0;
                biasValue= 0;
                for (int k=0; k<layers[i]->getNeuron(j)->getWeightsNum(BIAS); k++)
                { // per ogni canale in ingresso 
                    totInput += layers[i]->getNeuron(j)->getWeight(k) * input[k];       // totInput = valore da passare in ingresso alla funzione di attivazione
                }
                
                if (BIAS) {
                    biasValue = layers[i]->getNeuron(j)->getWeight( layers[i]->getNeuron(j)->getWeightsNum(BIAS) - 1);
                }                
                //layers[i]->getNeuron(j)->setValue(totInput);
                //layers[i]->getNeuron(j)->setValue(totInput - biasValue);       
                //layers[i]->getNeuron(j)->setValue(tanh_iper(totInput) - biasValue);
                layers[i]->getNeuron(j)->setValue(sigmoid(totInput) - biasValue);
            }
        }
        
        else   // HIDDEN --> HIDDEN o HIDDEN --> OUTPUT
        {      // ..altrimenti prendo i valori dai neuroni del layer precedente totInput += ... * layers[i - 1]->getNeuron(k)->getValue();
            for (int j=0; j<layers[i]->getNeuronsNum(); j++) 
            { // Per ogni neurone                
                totInput = 0;
                biasValue= 0;
                for (int k=0; k<layers[i]->getNeuron(j)->getWeightsNum(BIAS); k++) 
                { 
                    totInput += layers[i]->getNeuron(j)->getWeight(k) * layers[i-1]->getNeuron(k)->getValue();
                }
                
                if (BIAS) {
                    biasValue = layers[i]->getNeuron(j)->getWeight(layers[i]->getNeuron(j)->getWeightsNum(BIAS) - 1); 
                }            
                /// OUTPUT layer ///
                if (i==layers.size()-1) 
                {
                    //cout << "feedForward:: OUTPUT layer valore di uscita neurone" << j << "=" << layers[i]->getNeuron(j)->getValue() << endl;
                    //layers[i]->getNeuron(j)->setValue(totInput - biasValue);
                    //cout << layers[i]->getNeuron(j)->getValue() << " " ;
                    layers[i]->getNeuron(j)->setValue(sigmoid(totInput) - biasValue);
                }
                /// HIDDEN layers ///
                else 
                {
                    //cout << "feedForward:: HIDDEN layer valore di uscita neurone" << j << "=" << layers[i]->getNeuron(j)->getValue() << endl;
                    layers[i]->getNeuron(j)->setValue(sigmoid(totInput) - biasValue);
                }
                
                //layers[i]->getNeuron(j)->setValue(totInput);
                //layers[i]->getNeuron(j)->setValue(totInput - biasValue);                 
                //layers[i]->getNeuron(j)->setValue(sigmoid(totInput));
                //layers[i]->getNeuron(j)->setValue(sigmoid(totInput) - biasValue);                
                //layers[i]->getNeuron(j)->setValue(tanh_iper(totInput));
                
            }
        }
    }
}

void NeuralNet::feedForwardSoft(double input[12])
{
    //stampaPesi();
    if (12 != numInputNeurons) 
    {
        cout << "Error: NeuralNet::feedForward(): wrong input array size." << endl;
        cout << "       expected " << numInputNeurons << " instead of " << 12 << endl;
        exit(0);
    }
    
    double totInput=0;
    double biasValue = 0;
    double max = -10;
    double scale = 0;
    for (int i=0; i<layers.size(); i++)     
    { // Per ogni livello        
        if (i == 0)     // INPUT --> HIDDEN , primo hidden layer -> prendo i valori dal layer di input, totInput += ... * input[k];
        {
            for (int j=0; j<layers[i]->getNeuronsNum(); j++) 
            { // Per ogni neurone;
                totInput = 0;
                biasValue= 0;
                for (int k=0; k<layers[i]->getNeuron(j)->getWeightsNum(BIAS); k++)
                { // per ogni canale in ingresso 
                    totInput += layers[i]->getNeuron(j)->getWeight(k) * input[k];       // totInput = valore da passare in ingresso alla funzione di attivazione
                }
                
                if (BIAS) {
                    biasValue = layers[i]->getNeuron(j)->getWeight( layers[i]->getNeuron(j)->getWeightsNum(BIAS) - 1);
                }                
                layers[i]->getNeuron(j)->setValue(sigmoid(totInput) - biasValue);
            }
        }
        
        else   // HIDDEN --> HIDDEN o HIDDEN --> OUTPUT
        {      // ..altrimenti prendo i valori dai neuroni del layer precedente totInput += ... * layers[i - 1]->getNeuron(k)->getValue();
            for (int j=0; j<layers[i]->getNeuronsNum(); j++) 
            { // Per ogni neurone                
                totInput = 0;
                biasValue= 0;
                for (int k=0; k<layers[i]->getNeuron(j)->getWeightsNum(BIAS); k++) 
                { 
                    totInput += layers[i]->getNeuron(j)->getWeight(k) * layers[i-1]->getNeuron(k)->getValue();
                }
                
                if (BIAS) {
                    biasValue = layers[i]->getNeuron(j)->getWeight(layers[i]->getNeuron(j)->getWeightsNum(BIAS) - 1); 
                }            
                /// OUTPUT layer ///
                if (i==layers.size()-1) 
                {
                    //cout << "feedForward:: OUTPUT layer valore di uscita neurone" << j << "=" << layers[i]->getNeuron(j)->getValue() << endl;
                    layers[i]->getNeuron(j)->setValue(totInput);
                    if (totInput>max)
                        max=totInput;
                }
                /// HIDDEN layers ///
                else 
                {
                    //cout << "feedForward:: HIDDEN layer valore di uscita neurone" << j << "=" << layers[i]->getNeuron(j)->getValue() << endl;
                    layers[i]->getNeuron(j)->setValue(sigmoid(totInput) - biasValue);
                }                
            }
        }
    }
    scale=0;
    int l = layers.size()-1;
    for (int s=0; s<layers[l]->getNeuronsNum(); s++)
    {
        scale += exp( layers[l]->getNeuron(s)->getValue()-max );
    }
    for (int s=0; s<layers[l]->getNeuronsNum(); s++)
    {
        layers[l]->getNeuron(s)->setValue( exp(layers[l]->getNeuron(s)->getValue()-max)/scale );
    }
}

void NeuralNet::backPropagation(vector<double> input, vector<double> output)
{
    double out = 0;
    double sum = 0;
    for (int i=layers.size()-1; i>=0; i--)     
    { // Per ogni livello  
        
        /// HIDDEN layers ///
        if ( i != layers.size()-1 )
        {
            for (int k=0; k<layers[i]->getNeuronsNum(); k++) 
            { // Per ogni neurone;
                
                sum=0;
                for (int h=0; h<layers[i+1]->getNeuronsNum(); h++)
                {
                    sum += delta[i+1][h] * layers[i+1]->getNeuron(h)->getWeight(k);     // somma dei pesi che collegano il neurone h del layer i * delta del neurone h del layer i+1: sum+= w*DELTA
                }                
                out = layers[i]->getNeuron(k)->getValue();
                delta[i][k] = out * ( 1 - out ) * sum;
                //cout.precision(6);
                //cout << fixed << "delta del neurone_"<< k << " , del layer_" << i << " = " << delta[i][k] <<endl;
            }
            
        }
        
        /// layer di OUTPUT ///
        else    
        {
            for (int k=0; k<layers[i]->getNeuronsNum(); k++) 
            { // Per ogni neurone del livello OUTPUT;                
                out = layers[i]->getNeuron(k)->getValue();
                delta[i][k] = out * ( 1 - out ) * ( output[k] - out );
                //delta[i][k] = out * ( 1 - out ) * ( output[k]/out + (1-output[k])/(1-out) )*-1;
                //delta[i][k] = output[k]-out;
                
                //cout.precision(6);
                //cout << fixed << "delta del neurone_"<< k << ", del layer_" << i << "=" << delta[i][k] << "; out=" << out <<endl;
            }
        }
    }
    updateWeights(input);
}

void NeuralNet::updateWeights(vector<double> input) 
{
    //stampaPesi();
    double diffWeight = 0;
    double newWeight = 0;
    
    /// INPUT --> HIDDEN ///
    /*for(int i=0; i<numInputNeurons; i++)
    {
        for(int j=0; j<layers[0]->getNeuronsNum(); j++)
        {
            diffWeight = LEARNING_RATE * delta[0][j] * input[i] + prevDiffWeight[0][j][i]*MOMENTUM;     
            newWeight = layers[0]->getNeuron(j)->getWeight(i) + diffWeight;
            //cout << "delta[0][" << j << "]=" << delta[0][j] << "; prevDiffWeight[0][j][" << i << "]=" << prevDiffWeight[0][j][i] << endl;
            
            prevDiffWeight[0][j][i] = diffWeight;
            
            layers[0]->getNeuron(j)->setWeight(i, newWeight);
            //cout << "aggiorno peso_" << i << " del neurone_" << j << " del layer_0: " <<  layers[0]->getNeuron(j)->getWeight(i) << " + " << diffWeight << endl;
        }
    }*/
    
    /// HIDDENs --> OUTPUT ///
    /*for(int i=0; i<layers.size()-1; i++)
    {   //per ogni layer i, escluso il layer di output
        for(int k=0; k<layers[i]->getNeuronsNum(); k++)
        {   //per ogni neurone k del livello i
            for(int j=0; j<layers[i+1]->getNeuronsNum(); j++)
            {   // per ogni neurone j del layer i+1
                diffWeight = LEARNING_RATE * delta[i+1][j] * layers[i]->getNeuron(k)->getValue() + prevDiffWeight[i+1][j][k]*MOMENTUM;                
                newWeight = layers[i+1]->getNeuron(j)->getWeight(k) + diffWeight;
                
                prevDiffWeight[i+1][j][k] = diffWeight;                
                //cout << "aggiorno peso_" << k << " del neurone_" << j << " del layer_" << i+1 << ": " <<  layers[i+1]->getNeuron(j)->getWeight(k) << " + " << diffWeight << endl;
                
                layers[i+1]->getNeuron(j)->setWeight(k, newWeight);
                
                //cout.precision(6);
                //cout << fixed << "nuovo peso_" << k << " , del neurone_"<< j << " , del layer_" << i+1 << " = " << newWeight <<endl;
            }
        }
    }*/
    double momentum=0;
    for(int j=0; j<layers[0]->getNeuronsNum(); j++)
    {
        for(int k=0; k<numInputNeurons; k++)
        {
            momentum = prevDiffWeight[0][j][k]*MOMENTUM;
            prevDiffWeight[0][j][k]=LEARNING_RATE*delta[0][j]*input[k];
            double weightIJK = layers[0]->getNeuron(j)->getWeight(k);
            layers[0]->getNeuron(j)->setWeight(k, weightIJK+prevDiffWeight[0][j][k]+momentum);
        }
    }
    for(int i=1; i<layers.size(); i++)
    {   //per ogni layer i, escluso il layer di input
        for(int j=0; j<layers[i]->getNeuronsNum(); j++)
        {   //per ogni neurone j
            for(int k=0; k<layers[i-1]->getNeuronsNum(); k++)
            {   // per ogni neurone k del layer precedente              
                //cout.precision(6);
                //cout << fixed << "nuovo peso del neurone_"<< k << " , del layer_" << i << " = " << prevWeight+diffWeight <<endl;
                momentum = prevDiffWeight[i][j][k]*MOMENTUM;
                prevDiffWeight[i][j][k]=LEARNING_RATE*delta[i][j]*layers[i-1]->getNeuron(k)->getValue();
                double weightIJK = layers[i]->getNeuron(j)->getWeight(k);
                layers[i]->getNeuron(j)->setWeight(k, weightIJK+prevDiffWeight[i][j][k]+momentum);
                
            }                        
        }
    }
}

// restituisce la somma degli errori di tutte le righe di input
// vector<double> des = vettore degli output
double NeuralNet::globalError(vector<double> des) 
{
    /*double sum = 0;
    for (int i = 0, l = layers.size() - 1; i < layers[l]->getNeuronsNum(); i++) 
    {
        //err += abs(des[i] - layers[l]->transf(layers[l]->getNeuron(i)->getValue()));
        //err += abs(des[i] - layers[l]->getNeuron(i)->getValue());
        sum += pow( des[i] - layers[l]->getNeuron(i)->getValue(), 2);
    }
    double err = sqrt( (1/des.size())*sum );
    return err;*/
    double sum = 0;
    for (int i=0, l=layers.size()-1; i<layers[l]->getNeuronsNum(); i++) 
    {
        //err += abs(des[i] - layers[l]->transf(layers[l]->getNeuron(i)->getValue()));
        //err += abs(des[i] - layers[l]->getNeuron(i)->getValue());
        //sum += fabs(des[i] - layers[l]->getNeuron(i)->getValue());
        sum += pow(des[i] - layers[l]->getNeuron(i)->getValue(),2);
        //cout << " neuron_" << i << "=" << layers[l]->getNeuron(i)->getValue();
    }       
    return sum;               // sum/5
}
double NeuralNet::globalCrossError(vector<double> des) 
{
    double sum = 0;
    for (int i=0, l=layers.size()-1; i<layers[l]->getNeuronsNum(); i++) 
    {
        if(des[i]!=0)
        {
            if(layers[l]->getNeuron(i)->getValue()!=0)
                sum -= des[i] * log(layers[l]->getNeuron(i)->getValue());
            else
                sum-=-730;
            //cout <<"value neuron_"<<i<<"="<<layers[l]->getNeuron(i)->getValue()<<"; sum="<<log(layers[l]->getNeuron(i)->getValue());
        }
    }       
    return sum;               // sum/5
}

int NeuralNet::totalNumWeights(bool bias) 
{
    int n = 0;
    for (int i = 0; i < layers.size(); i++) 
    { // per ogni layer...
        for (int j = 0; j < layers[i]->getNeuronsNum(); j++) 
        { // per ogni neurone...
            //cout << "NeuralNet::totalNumWeights: Il neurone " << j << ", del layer " << i << ") ha " << layers[i]->getNeuron(j)->getWeightsNum(bias) << " pesi." << endl;
            n += layers[i]->getNeuron(j)->getWeightsNum(bias);
        }
    }
    return n;
}

int NeuralNet::numInputs() const {
    return numInputNeurons;
}

int NeuralNet::numOutputs() const {
    return numOutputNeurons;
}

int NeuralNet::numLayers() const {
    return layers.size();
}

NeuronLayer* NeuralNet::getLayer(int i) {
    if (i < layers.size()) {
        return layers[i];
    } else {
        cout << "Error: NeuralNet::getLayer(): index `i` out of range." << endl;
        exit(0);
    }
}

void NeuralNet::dump() {
    for (int i = 0; i < layers.size(); i++) 
    { // Per ogni livello
        for (int j = 0; j < layers[i]->getNeuronsNum(); j++) 
        { // Per ogni neurone
            cout << layers[i]->getNeuron(j)->getValue() << " ";
        }
        cout << endl;
    }
}

void NeuralNet::stampaPesi()
{
    cout << "--- PESI RETE ---" << endl;
    for (int i = 0; i < layers.size(); i++) 
    { // Per ogni livello
        cout << "livello_" << i << endl;
        for (int j = 0; j < layers[i]->getNeuronsNum(); j++) 
        { // Per ogni neurone
            cout << "neurone_" << j << endl;
            for (int k = 0; k < layers[i]->getNeuron(j)->getWeightsNum(BIAS); k++) 
            {
                cout << "peso_" << k << ": " <<layers[i]->getNeuron(j)->getWeight(k) << " ";
            } 
            cout << endl;
        }
        cout << endl;
    }
}

void NeuralNet::printOutput() {
    int l = layers.size() - 1;    
    for (int i = 0; i < layers[l]->getNeuronsNum(); i++) {
        cout << layers[l]->getNeuron(i)->getValue() << endl;
        //cout << "valore output= " << layers[l]->transf(layers[l]->getNeuron(i)->getValue()) << " ";
        //cout << transf_sigmoid(12.3) << " ";
    }
}

double NeuralNet::results() 
{
    int l = layers.size() - 1;    
    return layers[l]->getNeuron(0)->getValue();
}

