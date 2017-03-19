/// VISEME MAPPING ///
// 0 = a, l, n //
// 1 = e, ca, ga, che //
// 2 = i, chi //
// 3 = o, go, co //
// 4 = u, qu, cu, gu, tu, su //
// 5 = f, v //
// 6 = t, d, z, s, r //
// 7 = p, m, b //
// 8 = ci, gi, ce, ge, sc //
// 9 = neut //

#include "find_word.hpp"
#include <iostream>
#include <fstream>

Find_Word::Find_Word()
{
    loadCoreset("coreset_personale.txt");
    prevViseme.power=0;
    prevViseme.value=-1;
}

void Find_Word::loadCoreset(string inputFile)
{
    fstream in(inputFile.c_str(), ios::in);
    string s = "";
    string value;
    cout << "loadCoreSet: input...\n";
    while(getline(in, s)) 
    {
        istringstream iss(s);
        while (iss >> value) {
            coreset.push_back(value);
        }
    }
    in.close();
    if (coreset.size() == 0){
        cout << "ERROR: nessun dato in input" << endl;
        exit(0);
    }
}


/*
bool Find_Word::addViseme(int visemeCode)
{

        if (visemeCode == 9){
                cout << "fine parola\n";
                visemeList.push_back(visemeCode);
                cout << "STAMPA visemeList " << endl;

                int i = 0;
                for (i = 0; i< visemeList.size(); i++){
                        cout << "ELEMENTO " << i << " -> " << visemeList[i] << endl;
                }
                return true;
        }
	
	else
	{
		cout << "visema diverso\n";
		cout << "primo visema inserito\n";
		visemeList.push_back(visemeCode); //prevViseme.value
		int i = 0;
		for (i = 0; i< visemeList.size(); i++){
			cout << "ELEMENTO " << i << " -> " << visemeList[i] << endl;
		}

	}
	return false;
}
*/
bool Find_Word::addViseme(int visemeCode)
// return: true=word complete, false=word uncomplete.
{

	if (visemeCode == prevViseme.value)
	{
		
		cout << "PrevPOWER= " << prevViseme.power << endl;
		cout << "visemeLIST= " << visemeList.size() << endl;
		prevViseme.power++;
		if (prevViseme.value == 9 && prevViseme.power >= 3 && visemeList.size()>1)
			// trovata una sequenza di "NEUT" => parola completata
		{
			cout << "fine parola\n";
			visemeList.push_back(visemeCode);
			return true;
		}
	}
	else
	{
		cout << "visema diverso\n";
		if (visemeList.size() == 0)
		{
			if (prevViseme.power>VISEME_CLOCK && prevViseme.value != 9)
			{
				cout << "primo visema inserito\n";
				visemeList.push_back(prevViseme.value);
			}
		}
		else if (prevViseme.power>VISEME_CLOCK && visemeList[visemeList.size() - 1] != prevViseme.value)
		{
			cout << "altro visema inserito\n";
			visemeList.push_back(prevViseme.value);
		}
		prevViseme.value = visemeCode;
		prevViseme.power = 1;
	}
	return false;
}



vector<string> visemeMap(int visemeCode)
{
    vector<string> vChar;
    switch(visemeCode)
    {                
        case(0):                // A_L_N
            vChar.push_back("a");
            vChar.push_back("l");
            vChar.push_back("n");
            vChar.push_back("ne");
            vChar.push_back("va");
            //vChar.push_back("ca");
        break;
        case(1):                // E_CA_GA_CHE
            vChar.push_back("e");
            vChar.push_back("ca");
            vChar.push_back("ga");
            vChar.push_back("che");
            //vChar.push_back("n");
            vChar.push_back("f"); //AGGIUNTO
            //vChar.push_back("fi");//AGGIUNTO
        break;
        case(2):                // I_CHI
            vChar.push_back("i");
            vChar.push_back("chi");
            vChar.push_back("bi");
            vChar.push_back("r"); //AGGINTO
            vChar.push_back("s"); //AGGINTO
            vChar.push_back("t"); //AGGINTO
            //vChar.push_back("ti");//AGGIUNTO
            vChar.push_back("d");
            //vChar.push_back("ne"); //AGGIUNTO
            vChar.push_back("z");//AGGIUNTO
        break;
        case(3):                // O_CO_GO
            vChar.push_back("o");
            vChar.push_back("co");
            vChar.push_back("go");
            vChar.push_back("to");
            //vChar.push_back("so");
            vChar.push_back("do"); //AGGIUNTO
            vChar.push_back("po"); //AGGIUNTO
            vChar.push_back("ro"); //AGGIUNTO
        break;
        case(4):                // U_QU_CU_GU_TU_SU
            vChar.push_back("u");
            vChar.push_back("qu");
            vChar.push_back("cu");
            vChar.push_back("gu");
            vChar.push_back("tu");
            vChar.push_back("su");
            vChar.push_back("lu");
        break;
        case(5):                // F_V
            vChar.push_back("f");
            vChar.push_back("v");
        break;
        case(6):                // T_D_Z_S_R
            vChar.push_back("t");
            vChar.push_back("d");
            vChar.push_back("z");
            vChar.push_back("s");
            vChar.push_back("r");
            vChar.push_back("f");
        break;
        case(7):                // P_M_B
            vChar.push_back("p");
            vChar.push_back("m");
            vChar.push_back("b");
        break;
        case(8):                // CI_GI_CE_GE_SC
            vChar.push_back("gi");
            vChar.push_back("ci");
            vChar.push_back("ce");
            vChar.push_back("ge");
            vChar.push_back("sc");
            //vChar.push_back("so"); //AGGIUNTO
        break;
        case(9):               // NEUT
            vChar.push_back("stop");
        break;
    }
    return vChar;
}

int minimum(int a,int b,int c) 
/* funzione che calcola il minimo di 3 valori */ 
{
    int min = a;
    if (b < min)
        min = b;
    if (c < min)
        min = c;
    return min; 
}

int Levenshtein_distance(const char *x, const char *y)
{
    int m=strlen(x);
    int n=strlen(y);
 
    register int i,j;
    int distance;
 
    int *prev = static_cast<int *>( malloc((n+1)*sizeof(int)) );
    int *curr = static_cast<int *>( malloc((n+1)*sizeof(int)) );
    int *tmp=0;
 
    for(i=0;i<=n;i++)
        prev[i]=i; 
 
    for(i=1;i<=m;i++){
        curr[0]=i;
        for(j=1;j<=n;j++){
            if(x[i-1]!=y[j-1]){
                int k=minimum(curr[j-1],prev[j-1],prev[j]);
                curr[j]=k+1;
            }else{
                curr[j]=prev[j-1];
            }
        } 
 
        tmp=prev; 
        prev=curr; 
        curr=tmp;
 
        memset((void*)curr,0,sizeof(int)*(n+1)); 
    }
 
    distance=prev[n];
 
    free(curr);
    free(prev);
 
    return distance;
}

void deleteTwin(vector<string> &wordsFound)
{
    /*for(int i=0; i<wordsFound.size(); i++)
    {
        for(int j=i+1; j<wordsFound.size(); j++)
        {
            if( wordsFound[i] == wordsFound[j] )
            {
                wordsFound.erase(wordsFound.begin()+j);
            }
        }
    }*/
    vector<string>::iterator it = wordsFound.begin();
    for (; it != wordsFound.end(); ++it) 
    {
        vector<string>::iterator it2 = it+1;
        for (; it2 != wordsFound.end();) 
        {
            if (*it == *it2) {
                it2 = wordsFound.erase(it2);
            } else {
                ++it2;
            }
        }
    }    
}

vector<string> Find_Word::searchWord()
{
    
    vector<string> wordsFound;
    string charSequence = ""; 
    vector< vector<string> > visemeChar;
    
    cout << "parola: " ;
    for (int j=0; j<visemeList.size(); j++)
    {
        cout << visemeList[j] << " ";
    }
    cout << endl;
    
    visemeChar.clear();
    for (int i=0; i<visemeList.size(); i++)
    {
        visemeChar.push_back( visemeMap(visemeList[i]) );
    }
    for (int i=0; i<visemeChar.size(); i++)
    {
        cout << "visema_" << i << ": ";
        for (int j=0; j<visemeChar[i].size(); j++)
                cout << visemeChar[i][j] << ", ";
        cout << endl;
    }
    
    int l = 0;          // livello dell'albero di ricerca
    vector <int> nL;    // numero figli di ogni nodo
    
    for (int g=0; g<visemeChar.size(); g++)
        nL.push_back(0);

    cout << "findWords: inizio ricerca parole..." << endl;
    
    while (nL[0]<visemeChar[0].size())
    {
        charSequence += visemeChar[l][nL[l]];
        cout << "charSequence=" << charSequence <<endl;
        
        if (l<visemeChar.size()-2)         // continua la ricerca in profondità
        {
            l++;
        }
        else if (nL[l]<visemeChar[l].size()-1)   // raggiunta una foglia ma non ho visitato tutti i nodi  // continua la ricerca in ampiezza
        {
            //cout << "raggiunta una foglia, cerco: " << charSequence << endl;
            int dist=charSequence.size();
            const char *cstr1 = charSequence.c_str();
            for(int c=0; c<coreset.size(); c++)
            {
                const char *cstr2 = coreset[c].c_str();
                dist = Levenshtein_distance(cstr2, cstr1);
                if (dist==0)
                {
                    //cout << "trovato parola uguale: " << coreset[c] << endl;
                    wordsFound.insert(wordsFound.begin(), coreset[c]);
                }
                else if (dist<2)
                {
                    //cout << "trovato parola simile: " << coreset[c] << endl;
                    wordsFound.push_back(coreset[c]);
                }
            }
            
            //charSequence -= visemeChar[l][nL[l]];
            charSequence.erase( charSequence.size() - visemeChar[l][nL[l]].size() );
            //cout << "cancello ultima lettera, charSequence=" << charSequence << endl;
            nL[l]++;
        }
        else // raggiunta una foglia e ho visitato tutti i nodi  // interrompi ricerca e torna indietro
        {
            //cout << "raggiunta una foglia, cerco: " << charSequence << endl;
            int dist=charSequence.size();
            const char *cstr1 = charSequence.c_str();
            for(int c=0; c<coreset.size(); c++)
            {
                const char *cstr2 = coreset[c].c_str();
                dist = Levenshtein_distance(cstr2, cstr1);
                if (dist==0)
                    wordsFound.insert(wordsFound.begin(), coreset[c]);
                else if (dist<2)
                    wordsFound.push_back(coreset[c]);
            }
            
            //cout << "visitati tutti i nodi al livello " << l << endl;
            if (l==0)
                break; //return;
            charSequence.erase( charSequence.size() - visemeChar[l][nL[l]].size() );
            //cout << "cancello ultima lettera, charSequence=" << charSequence << endl;
            nL[l]=0;
            l--;
            //charSequence -= visemeChar[l][nL[l]];
            charSequence.erase( charSequence.size() - visemeChar[l][nL[l]].size() );
            //cout << "cancello ultima lettera, charSequence=" << charSequence << endl;
            nL[l]++;
            while (nL[l]>=visemeChar[l].size()) 
            {
                //cout << "visitati ANCHE tutti i nodi al livello " << l << endl;
                if (l==0)
                    break; //return;
                nL[l]=0;
                l--;
                charSequence.erase( charSequence.size() - visemeChar[l][nL[l]].size() );
                //cout << "cancello ultima lettera, charSequence=" << charSequence << endl;
                nL[l]++;
            }
        }
    }      
    
    deleteTwin(wordsFound);
    cout << " --- Parole Possibili --- \n";
    for(int f=0; f<wordsFound.size(); f++)
    {
        cout << wordsFound[f] << "; ";
    }
    cout << endl;
    
    prevViseme.power=0;
    prevViseme.value=-1;
    visemeList.clear();
    
    return wordsFound;
}

bool Find_Word::clearVisemeList(){
    visemeList.clear();
    return true;
}

int Find_Word::sizeVisemeList(){
    return visemeList.size();
}

void Find_Word::add_classified_visemes(int n){
    classified_visemes.push_back(n);
}

void Find_Word::clear_classified_visemes(){
    classified_visemes.clear();
}

int Find_Word::last_classified_visemes(){
    return classified_visemes.back();
}

int Find_Word::size_classified_visemes(){
    return classified_visemes.size();
}

int Find_Word::x_element_classified_visemes(int t){
    return classified_visemes.at(t);
}