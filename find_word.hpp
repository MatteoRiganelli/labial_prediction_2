/* 
 * File:   find_word.hpp
 * Author: Fred
 *
 * Created on 9 luglio 2013, 10.31
 */

#ifndef FIND_WORD_HPP
#define	FIND_WORD_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <list>
#include <string.h>

#define VISEME_CLOCK 2 // prima era 6

using namespace std;

class Find_Word
{
public:
    Find_Word();
    bool addViseme(int visemeCode);      // return 1 or 0: 1=word complete, 0=word uncomplete.
    vector<string> searchWord();        // restituisce le parole trovate
    bool clearVisemeList();
    int sizeVisemeList();
    void add_classified_visemes(int n);
    void clear_classified_visemes();
    int last_classified_visemes();
    int size_classified_visemes();
    int x_element_classified_visemes(int t);
    
    
private:
    struct visema
    {
        int value;
        int power;
    };
    
    void loadCoreset(string inputFile);
    
    vector<string> coreset;         // database delle parole
    vector<int> visemeList;         // lista dei visemi da analizzare
    visema prevViseme;              // ultimo visema inserito nella lista dei visemi da analizzare
    vector<int> classified_visemes; 
   
    
};

#endif	/* FIND_WORD_HPP */

