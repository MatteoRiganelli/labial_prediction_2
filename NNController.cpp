
#include "NNController.h"


NNController::NNController(int c_numInputNeurons, int c_numOutputNeurons, int c_numHiddenLayers, int c_numNeuronsPerLayer){
    cout << endl << "NNController_1: costruisco una rete con... " 
            << endl << "neuroni in input = " << c_numInputNeurons
            << endl << "neuroni di output = " << c_numOutputNeurons
            << endl << "layer nascosti = " << c_numHiddenLayers
            << endl << "neuroni per layer = " << c_numNeuronsPerLayer << endl;
    
    srand(time(NULL));
    N = new NeuralNet(c_numInputNeurons, c_numOutputNeurons, c_numHiddenLayers, c_numNeuronsPerLayer);
    N->createNet();
    
    cout << endl << "NNController_3: costruzione rete completata." << endl;
}

void NNController::loadTrainingSet(string inputFile, string outputFile)
{
    cout << "NNController::loadTrainigSet: carico training set... " << endl;
    fstream in(inputFile.c_str(), ios::in), out(outputFile.c_str(), ios::in);
    string s = "";
    double value;
    vector<double> data;
    int i = 1;
    int count = 0;
    cout << "NNController::loadTrainigSet: input...";
    while(getline(in, s)) {
        istringstream iss(s);
        //cout << "riga " <<  i << ":";   i++;
        while (iss >> value) {
            data.push_back(value);
            if(i==1)
                count++;
            //cout << value << " " ;
        }
        //cout << endl;
        inputData.push_back(data);
        data.clear();
        i++;
    }
    in.close();
    if (inputData.size() == 0){
        cout << "ERROR: nessun dato in input" << endl;
        exit(0);
    }
    if(count!=N->numInputs()){
        cout << "ERROR: vettore input="<< count << "neuroni layer input" << N->numInputs() << endl;
        exit(0);
    }
    cout << "OK" << endl;

    cout << "NNController::loadTrainigSet: output...";
    i=1;
    count = 0;
    while (getline(out, s)) 
    { // Carica il file di output nell'array
        istringstream iss(s);
        //cout << "riga " <<  i << ":";   i++;
        while (iss >> value) {
            data.push_back(value);
            //cout << value << " " ;
            if(i==1)
                count++;
        }
        //cout << endl;
        outputData.push_back(data);
        data.clear();
        i++;
    }
    out.close();
    if (outputData.size() == 0){
        cout << "ERROR: nessun dato in output" << endl;
        exit(0);
    }
    if(count!=N->numOutputs()){
        cout << "ERROR: vettore output="<< count << "neuroni layer output" << N->numOutputs() << endl;
        exit(0);
    }
    cout << "OK" << endl;
}

void NNController::loadValidationSet(string validationFile)
{
    cout << "NNController::loadValidationSet: carico validation set... ";
    fstream in(validationFile.c_str(), ios::in);
    string s = "";
    double value;
    vector<double> data;
    
    int i = 1;
    
    while(getline(in, s)) {
        istringstream iss(s);
        //cout << "riga " <<  i << ": ";
        i++;
        while (iss >> value) 
        {
            data.push_back(value);
            //cout << value << " " ;
        }
        //cout << endl;
        validationData.push_back(data);
        data.clear();
    }
    in.close();
    cout << ".fine validation set." << endl;
}

//allena la rete
void NNController::train()
{
    cout << "NNController::train: inizio allenamento..." << endl;
    
    int i=0;
    double MSE = 100;
    double minMSE = 100;
    
    while (MSE>MAX_ERROR && i<MAX_ITER) 
    {
        MSE = 0;
        for (int k=0; k<inputData.size(); k++) 
        {            
            //cout << "elaboro input_" << k << endl;
            N->feedForward(inputData[k]);
            //N->feedForwardSoft(inputData[k]);
            N->backPropagation(inputData[k], outputData[k]);            
                        
            //MSE += pow( N->globalError(outputData[k]), 2 );
            MSE += N->globalError(outputData[k]);
            //MSE += N->globalCrossError(outputData[k]);
            //cout << " MSE="<<MSE<<endl;
            //getchar();
        }
        MSE = MSE / inputData.size();
        //MSE = sqrt(MSE);
        
        if(MSE<minMSE && i>50)
        {
            cout << i << ". MSE = " << MSE << endl;
            string file = "pesi.txt";
            //stringstream stream;
            //stream << i << file;
            //file = stream.str();
            minMSE=MSE;
            saveWeights(file);
        }
        
        /*if(i % UPDATE == 0) 
        {
            cout << i << ". MSE = " << MSE << endl;
            string file = "_pesi.txt";
            stringstream stream;
            stream << i << file;
            file = stream.str();            
            saveWeights(file);
            //getchar();
        }*/
        cout << i << ". MSE = " << MSE << endl;
        //getchar();
        i++;   
    }    
    cout << "Finito con MSE: "<< minMSE << endl;
}

void NNController::saveWeights(string file) 
{
    cout << "Salvo pesi...";
    fstream f(file.c_str(), ios::out);
    for (int i = 0; i < N->numLayers(); i++) 
    { // Per ogni livello
        //cout << "livello  "<< i << endl;
        
        for (int j=0; j<N->getLayer(i)->getNeuronsNum(); j++) 
        { // Per ogni neurone
            //cout << "neurone  "<< j << ": ";
            
            for (int k = 0; k < N->getLayer(i)->getNeuron(j)->getWeightsNum(BIAS); k++) 
            {
                f << N->getLayer(i)->getNeuron(j)->getWeight(k) << endl << " ";
                cout.precision(10);
                //cout << fixed << N->getLayer(i)->getNeuron(j)->getWeight(k) << " ";
            }
            //cout << endl;
        }
    }
    f.close();
    //cout << "OK" << endl;
}

void NNController::loadWeights(string file) {
    cout << "loadWeights: carico pesi... ";
    fstream f(file.c_str(), ios::in);
    double val;
    for (int i = 0; i < N->numLayers(); i++) { // Per ogni livello
        for (int j = 0; j < N->getLayer(i)->getNeuronsNum(); j++) { // Per ogni neurone
            for (int k = 0; k < N->getLayer(i)->getNeuron(j)->getWeightsNum(BIAS); k++) {
                f >> val;
                N->getLayer(i)->getNeuron(j)->setWeight(k, val);
            }
        }
    }
    f.close();
    cout << "OK" << endl;
    //N->stampaPesi();
}

void NNController::test() 
{
    cout << "test: inizio test... " << endl;
    //vector<double> input;
    double val;
    cout << "Premi Ctrl+C per terminare il test ed il programma." << endl;
    while (true) 
    {
        vector<double> input;
        cout << "Inserisci " << N->numInputs() << " numeri in input." << endl;
        for (int i = 0; i < N->numInputs(); i++) {
            cin >> val;
            input.push_back(val);
        }
        N->feedForward(input);
        double result = N->results();
        cout << result << endl;
        input.clear();
    }
}

int NNController::classify(double nnInput[12])
{   
    //cout << "NNController::classify.\n";
    fstream file("outputNN.txt", ios::out);
    double result=0;
    cout.precision(5);
    int max = -1;
    double maxV=0;
    string viseme[] = {"A_L_N", "E_CA_GA", "I", "O", "U", "F_V", "T_D_Z_S", "P_M_B", "C_G", "NEUT"};

    N->feedForwardSoft(nnInput);
    
    for (int j=0; j<N->numOutputs(); j++)
    {            
        result = N->getLayer(N->numLayers()-1)->getNeuron(j)->getValue();
        if(result>maxV)
        {
            maxV=result;
            max=j;
        }
    }
    cout << max << " = " << viseme[max] << endl;
    file << max << endl;
        
    return max;
}