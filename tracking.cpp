#include "imageProcess.hpp"
//#include "poiDetection.hpp"
//#include "initializeNN.hpp"
#include "find_word.hpp"
#include "eye_detect.hpp"
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\video\background_segm.hpp>
#include <Windows.h>
#include <fstream>
#include "imageProcess.hpp"
#include "find_word.hpp"

#include <cstdlib>
#include <stdio.h>     
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>

#include <limits>
#include <vector>
#include <sstream>
#include <cstring>



String inttostr(int input);
string GetStdoutFromCommand(string cmd);
string RemoveChar(string str, char c);

int N_MIN = 0;
int NUM_CLASSI = 9;

IplImage *mouthRegion;


int lipTracking() 
{
    
    ObjectDetector* faceDetector = new HaarCascadeObjectDetector("haarcascade_frontalface_alt.xml");

    int key;
    int key2;
    IplImage *src;                                      // immagine presa dalla webcam
    IplImage *result;                                   // immagine che mostra il risultato del tracking
    Mat myMat;
    Mat croppedImage;
    Rect croppedRectangle;
    bool cattura = false;
    Find_Word FW = Find_Word();
    EyeDetector ED = EyeDetector();
    int visemeCode;
    bool completeWord = false;
    Mat word = Mat::zeros(480, 650, CV_8UC3);
    Mat frase = Mat::zeros(200, 600, CV_8UC3);
    cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
    
    unsigned int parola=0;
    
    CvFont font1;
    cvInitFont(&font1,CV_FONT_HERSHEY_DUPLEX,1,1,0.2,2,8);
    int fontFace = CV_FONT_HERSHEY_DUPLEX;
   
    vector<string> wordsFound;
    
    CvCapture *capture = cvCaptureFromCAM(0);
    
    if (!capture) {
        cout << "WEBCAM ERROR: no signal \n";
        return -1;
    }
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    src = cvQueryFrame(capture);
    result = cvCloneImage(src);  
    CvSize s = cvGetSize(src);
    int d = src->depth;
    int c = src->nChannels;
    mouthRegion=cvCreateImage(s, d, c); // (s, 8, 3)    
 
    result=cvCreateImage(s, d, c); // (s, 8, 3)   

    bool mouthFound = false;
    
    //mi sposto sulla directory to 'caffe' per eseguire i comandi
    chdir("/Projects/prova/caffe");
    
    //vettore contenente i visemi classificati
    vector<int> classified_visemes;
    vector<string> immagini;
    
    classified_visemes.clear();
    
    immagini.clear();
    
    //examples/viseme3c
    //string proto_path = "models\\viseme3\\lenet.prototxt";
    //string model_path = "examples\\viseme3\\viseme_snap_iter_110.caffemodel";
    //string binaryproto_path = "data\\viseme\\imagenet_mean.binaryproto";
    //string synsetwords_path = "data\\viseme\\synset_words.txt";

    //examples/viseme4
    //string proto_path = "examples\\viseme4\\lenet.prototxt";
    //string model_path = "examples\\viseme4\\viseme_snap_iter_110.caffemodel";
    //string binaryproto_path = "data\\viseme2\\imagenet_mean.binaryproto";
    //string synsetwords_path = "data\\viseme2\\synset_words.txt";

    //examples/viseme5 (con meno luce)
    //string proto_path = "examples\\viseme5\\lenet.prototxt";
    //string model_path = "examples\\viseme5\\viseme_snap_iter_150.caffemodel";
    //string binaryproto_path = "data\\viseme3\\imagenet_mean.binaryproto";
    //string synsetwords_path = "data\\viseme3\\synset_words.txt";

    ////examples/viseme6
    //string proto_path = "examples\\viseme6\\lenet.prototxt";
    //string model_path = "examples\\viseme6\\viseme_snap_iter_150.caffemodel";
    //string binaryproto_path = "data\\viseme4\\imagenet_mean.binaryproto";
    //string synsetwords_path = "data\\viseme4\\synset_words.txt";

    //examples/viseme7 (con più luce)
    string proto_path = "examples\\viseme7\\lenet.prototxt";
    string model_path = "examples\\viseme7\\viseme_snap_iter_150.caffemodel";
    string binaryproto_path = "data\\viseme5\\imagenet_mean.binaryproto";
    string synsetwords_path = "data\\viseme5\\synset_words.txt";
    
    int photocount = 0; //initialize image counter
    String imagename;
    
    vector<int> compression_params;
    //vector that stores the compression parameters of the image
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    //specify the compression technique
    compression_params.push_back(100); //specify the compression quality
    
    long int framenumber = 0;
    
    while ((key = waitKey(30)) != 27 )
    {
        framenumber += 1;
        vector<string> wordsFound;
        //cout << "FRAMENUMBER" << framenumber << endl;
        src = cvQueryFrame(capture); 
        
        if (src == NULL) { cout << "---EMPTY---" << endl; }
        
        cvFlip(src, src, 180);
        
        cvCopy( src, result, NULL ); 
        
        /* check if mouth is visible in scene */
        try{
            findLip(src, faceDetector, mouthRegion, mouthFound);
        }
        catch(cv::Exception &e){cout << e.what() << endl;}
        
        if(!mouthFound)  continue;
        
        myMat = mouthRegion;
        croppedRectangle = Rect(300,300,100,100); //coordinate crop + width,height crop 
        croppedImage = myMat(croppedRectangle);
        /// display images ///
        resizeWindow("cropped", 100, 100);
        imshow("cropped", croppedImage);
        cvShowImage("result", result);
        
        imagename= "";
        if (key == 'c') 
        {    
            if (cattura){
                cattura=false;
                FW.clear_classified_visemes();
                //classified_visemes.clear();
                FW.clearVisemeList();
                photocount = 0;
                cout << "RI-PREMUTO" << endl;
            }
            else{
                //CLASSIFICO VISEMI CON CAFFE
                /*
                 */
                //CLASSIFICO PAROLA DA VISEMI                  
                //visemeList << visemeCode << endl;
                //
                //CANCELLO I FILE UTILIZZATI
                /**/
                cattura=true;
                cout << "PREMUTO" << endl;
                
            }
        }
                    if(cattura && framenumber % 8 == 0){
                    photocount++; // increment image number for each capture
                    imagename = "images/image" + inttostr(photocount) + ".JPEG";
                    imwrite(imagename, croppedImage, compression_params);
                    immagini.push_back("images\\image" + inttostr(photocount) + ".JPEG");
                    //stampa le immagini da controllare
                    for(int t=0;t<immagini.size();++t){
                            cout<< "Eccolo->" << immagini.at(t) << endl;
                    }
                    //per ogni immagine dal classificare
                    for(int t=0;t<immagini.size();++t){
                        fstream file("result.txt", ios::out); //apre il file in scrittura (cancellando quello che già c'era)
                        if(!file) {
                            cout<<"Impossibile aprire file di output!";
                            return -1;
                        }
                        string image_path = immagini.at(t);
                        //cout << "IMAGE_PATH= "<<image_path << endl;
                        //comando da eseguire
                        string result = GetStdoutFromCommand("build\\examples\\cpp_classification\\Release\\classification.exe " + proto_path + " " + model_path + " " + binaryproto_path +  " " + synsetwords_path + " " + image_path);
                        //cout << "RESULT" << endl;
                        //cout << result << endl;
                        //cout << "---" << endl;

                        //scrivo rsultato su file .txt
                        file << result << endl;

                        vector<string> righe;
                        righe.clear();
                        fstream in("result.txt", ios::in);
                        string s = "";
                        string value;
                        while(getline(in, s)) 
                        {
                            istringstream iss(s);
                            while (iss >> value) {
                                righe.push_back(value);
                            }
                        }
                        //cout << endl;
                        for(int i=0; i< righe.capacity(); i++){
                            if(i==7){ //se sono al elemento 7, quindi la l'elemento dove c'è il risultato
                                cout << i << " - " << righe[i] << endl;
                                //cout << righe[i] << endl;
                                string s = righe[i];
                                //cout << s << endl;
                                s = RemoveChar(s,'"'); //rimuovo carattetere ' " '
                                //cout << "s=" << s << endl;
                                int n = atoi(s.c_str());
                                printf("NUMBER=%d\n", n);
                                //classified_visemes.push_back(n);
                                FW.add_classified_visemes(n);
                            }
                        }
                        in.close();
                        file.close();

                    }
                    for(int t=0;t<FW.size_classified_visemes();++t){
                            cout<<"CLASSIFIED_VISEME->" << t << " -> " << FW.x_element_classified_visemes(t) << endl;
                    }
                    
                    immagini.clear();
                    
                    //VISEMA CLASSIFICATO
                    visemeCode = FW.last_classified_visemes();
                    //visemeCode = classified_visemes.back(); // passo sempre l'ultimo visema classificato
                    /*
                    if((visemeCode==7 || visemeCode==5 || visemeCode==6) && FW.sizeVisemeList() >=5){ // er aggiustare il 7 finale, provare
                        visemeCode = 9;
                    }
                    */
                    completeWord = FW.addViseme(visemeCode);
                    cout << "COMPLETE_WORD= " << completeWord << endl;
                    
                    resizeWindow("wordWindow", 800, 300);
                    if (completeWord)
                    {
                        cvNamedWindow("wordWindow", CV_WINDOW_AUTOSIZE);
                        //int fontFace = CV_FONT_HERSHEY_DUPLEX;
                        int fontFace = CV_FONT_ITALIC;
                        double fontScale = 1.5;
                        int thickness = 3;  
                        cvPutText(result, "Attendere, ricerca parola in corso...", Point(20, 100), &font1, cvScalar(255, 0, 0, 0));
                        cvShowImage("result", result);
                        wordsFound.clear();
                        wordsFound = FW.searchWord();
                        if(wordsFound.size()==0)
                        {
                            putText(word, ":( - Nessuna parola trovata,", Point(20, 50), fontFace, 1, cvScalar(0, 0, 255, 0), 2,8);
                            putText(word, "chiudere gli occhi per continuare", Point(20, 100), fontFace, 1, cvScalar(255, 0, 0, 0), 2,8);
                            imshow("wordWindow", word);
                            while ( (ED.detect()==0) ); //mettere anche  || ((key = waitKey(30)) != 'd')      // attende che l'utente chiuda gli occhi (ED.detect()==1) per continuare
                                sndPlaySound("C://Users//Matteo//Documents//NetBeansProjects//Classification2//beep_yes", SND_ASYNC);

                        }
                        else
                        {   Mat dst;
                            putText(word, "chiudi gli occhi per eliminare la parola", Point(20, 50), fontFace, 0.9, Scalar::all(255), 2,8);
                            putText(word, wordsFound[0], Point(50, 150), fontFace, 1.2, cvScalar(0, 255, 0, 0), 2, 8);
                            int raw=1, col=0;
                            
                            for(int w=1; w<wordsFound.size(); w++)
                            {
                                Point textPos(50+200*col, 150+raw*100);
                                putText(dst, wordsFound[w]+" ", textPos, fontFace, 1.2, Scalar::all(255), 2,8);
                                col++;
                                if(col==3) {
                                    raw++;
                                    col=0;
                                }
                            }
                            
                            
                            imshow("wordWindow", word);

                            while (ED.detect()==1)
                            // se si chiudono gli occhi(ED.detect()==1) si cancella la prima parola e si prosegue con la selezione
                            // se si rimane con gli occhi aperti (ED.detect()==0) si seleziona la parola
                            {
                                sndPlaySound("C://Users//Matteo//Documents//NetBeansProjects//Classification2//beep_no", SND_ASYNC);
                                wordsFound.erase(wordsFound.begin());
                                word.zeros(word.rows, word.cols, word.type());
                                word = Scalar(0, 0, 0);
                                if (wordsFound.size()==0)
                                {
                                    putText(word, ":( - Suggerimenti finiti,", Point(20, 50), fontFace, 1, cvScalar(0, 0, 255, 0), 2,8);
                                    putText(word, "chiudere gli occhi per continuare", Point(20, 100), fontFace, 1, cvScalar(0, 0, 255, 0), 2,8);
                                    imshow("wordWindow", word);
                                    while (ED.detect()==0);     // attende che l'utente chiuda gli occhi (ED.detect()==1) per continuare
                                    sndPlaySound("C://Users//Matteo//Documents//NetBeansProjects//Classification2//beep_yes", SND_ASYNC);
                                    break;
                                }
                                else
                                {
                                    putText(word, "chiudi gli occhi per eliminare la parola", Point(20, 50), fontFace, 0.9, Scalar::all(255), 2,8);
                                    putText(word, wordsFound[0], Point(50, 150), fontFace, 1.5, cvScalar(0, 255, 0, 0), 2, 8);
                                    int raw=1, col=0;
                                    for(int w=1; w<wordsFound.size(); w++)
                                    {
                                        Point textPos(50+200*col, 150+raw*100);
                                        putText(word, wordsFound[w]+" ", textPos, fontFace, 1.2, Scalar::all(255), 2,8);
                                        col++;
                                        if(col==3) {
                                            raw++;
                                            col=0;
                                        }
                                    }
                                    imshow("wordWindow", word);
                                }
                            }
                            sndPlaySound("C://Users//Matteo//Documents//NetBeansProjects//Classification2//beep_yes", SND_ASYNC);
                            if(wordsFound.size()!=0){
                                putText(frase, wordsFound[0]+" ", Point(20+150*parola, 50), fontFace, 1, Scalar::all(255), 2,8);
                                parola++;
                                cout << "PAROLAAAAA = " << parola << endl;
                                imshow("frase", frase);
                            }
                        }
                        word.zeros(word.rows, word.cols, word.type());
                        word = Scalar(0, 0, 0);
                        cvDestroyWindow("wordWindow");
                        //photocount = 0;
                        FW.clear_classified_visemes();
                        completeWord = false;
                    }   
                                
                    cout << imagename << " " << N_MIN << endl;
                    //f << imagename << " " << N_MIN << endl;
                    //namedWindow("result", WINDOW_AUTOSIZE );
                    //resizeWindow("result", 100, 100);
                    
                    //imshow("result", croppedImage);
                }
        
        
        
        
        
        
 

    }
    
    cvDestroyWindow("result");
    cvDestroyWindow("cropped");
   
    myMat.release();
    croppedImage.release();
    cvReleaseImage(&src);
    cvReleaseImage(&result);
    
    cvReleaseCapture(&capture);
    exit( 0 );
    cout << "QUI" << endl;
    return 0;
     
    }    



String inttostr(int input)
{
       stringstream ss;
       ss << input;
       return ss.str();
}

//restituisce lo std output di un comando eseguito da terminale sottoforma di stringa
string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
    }
    return data;
}

string RemoveChar(string str, char c) 
{
   string result;
   for (size_t i = 0; i < str.size(); i++) 
   {
          char currentChar = str[i];
          if (currentChar != c)
              result += currentChar;
   }
       return result;
}



