#include "imageProcess.hpp"
#include "poiDetection.hpp"
#include "initializeNN.hpp"
#include <fstream>

#define DHmin 83.0
#define DHdiff 32.0
#define DVmin 25.0
#define DVdiff 55.0
#define DAmin 0.0
#define DAdiff 4160.0
#define DPmin 8.0
#define DPdiff 22.0

IplImage *mouthRegion, *img1;
CvPoint pointT, pointB, pointR, pointL;
int pointSel = 0;
int key = 0;

const char* viseme[] = {"0=A_L_N", "1=E_CA_GA", "2=I_CHI", "3=O_CO_GO", "4=U_CU_QU_GU", "5=F_V", "6=T_D_Z_S_R", "7=P_M_B", "8=CI_GI_CE_GE_SC", "9=NEUT"};

void mouseHandler(int event, int x, int y, int flags, void* param) 
{
    /* user release left button */
    if (event == CV_EVENT_LBUTTONUP) 
    {
        if(pointSel==0){
            pointT.x=x-W/2;
            pointT.y=y-W/2;
            cvRectangle(img1, cvPoint(pointT.x, pointT.y), cvPoint(pointT.x+W, pointT.y+W), cvScalar(0, 0, 255, 0), 1, 0, 0);
            cvLine(img1, cvPoint(x,y), cvPoint(x, y+35), cvScalar(255,255,255,255), 1, 8);
            cvLine(img1, cvPoint(x-50,y+15), cvPoint(x+50, y+15), cvScalar(255,255,255,255), 1, 8);
        }
        else if (pointSel==1){
            pointR.x=x-W/2;
            pointR.y=y-W/2;
            cvRectangle(img1, cvPoint(pointR.x, pointR.y), cvPoint(pointR.x+W, pointR.y+W), cvScalar(255, 0, 0, 0), 1, 0, 0);
            //cvLine(img1, cvPoint(x,y), cvPoint(x-85, y), cvScalar(255,255,255,255), 1, 8);
        }
        else if (pointSel==2){
            pointB.x=x-W/2;
            pointB.y=y-W/2;
            cvRectangle(img1, cvPoint(pointB.x, pointB.y), cvPoint(pointB.x+W, pointB.y+W), cvScalar(0, 255, 0, 0), 1, 0, 0);            
        }
        else{
            pointL.x=x-W/2;
            pointL.y=y-W/2;
            cvRectangle(img1, cvPoint(pointL.x, pointL.y), cvPoint(pointL.x+W, pointL.y+W), cvScalar(255, 255, 0, 0), 1, 0, 0);
        }        
        cvShowImage("mouseSelector", img1);
        pointSel++;
    }

    /* user click right button: reset all */
    if (event == CV_EVENT_RBUTTONUP) {
        cvShowImage("mouseSelector", mouthRegion);
        pointSel=0;
    }
}

int lipTracking() 
{
    
    NNController NC = initializeNN();
    cout << "Creazione rete neurale completata.\n";
    cout << "Premere un tasto per iniziare il tracking...";
    getchar();
    cout << "OK\n";
    
    ObjectDetector* faceDetector = new HaarCascadeObjectDetector("haarcascade_frontalface_alt.xml");
    
    IplImage *src;                                      // immagine presa dalla webcam
    IplImage *tplT, *tplR, *tplB, *tplL;                // immagini in cui salvare i templates da seguire
    IplImage *resT, *resR, *resB, *resL;                // immagine necessarie per la computazione del template matching
    IplImage *searchT, *searchR, *searchB, *searchL;    // porzione dell'immgaine dove posiozionare i templates
    IplImage *result;                                   // immagine che mostra il risultato del tracking
    
    CvPoint minlocT, maxlocT;                           // salva la posizione ottimale dopo il template matching 
    CvPoint minlocR, maxlocR;
    CvPoint minlocB, maxlocB;
    CvPoint minlocL, maxlocL;
    
    double minvalT=0, maxvalT=0;                        // valore di cross-correlation massimo e minimo
    double minvalR, maxvalR;
    double minvalB, maxvalB;
    double minvalL, maxvalL;
    double powT, powB, powR, powL;                      // potenziale di un punto dopo il template matching
    
    int img_width, img_height;
    int tpl_width, tpl_height;
    int res_width, res_height;
    Rect roiT, roiB, roiR, roiL;
    
    double nnInput[12];
    for(int t=0; t<12; t++)
    {
        nnInput[t]=-1;
    }
    
    cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
    cvNamedWindow( "searchT", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "searchB", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "searchR", CV_WINDOW_AUTOSIZE );
    cvNamedWindow( "searchL", CV_WINDOW_AUTOSIZE );
    cvNamedWindow("tmpT", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("tmpB", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("tmpR", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("tmpL", CV_WINDOW_AUTOSIZE);

    /// INIZIALIZZAZIONE PARAMETRI ///
    CvCapture* capture = cvCaptureFromCAM(0);
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
    img1=cvCreateImage(s, d, c); // (s, 8, 3)   
    result=cvCreateImage(s, d, c); // (s, 8, 3)   
    searchT = cvCreateImage(cvSize(W+R*2, W+R*2), d, c);
    searchB = cvCreateImage(cvSize(W+R*2, W+R*2), d, c);
    searchR = cvCreateImage(cvSize(W+R*2, W+R*2), d, c);
    searchL = cvCreateImage(cvSize(W+R*2, W+R*2), d, c);
    tplT = cvCreateImage(cvSize(W, W), d, c);
    tplB = cvCreateImage(cvSize(W, W), d, c);
    tplR = cvCreateImage(cvSize(W, W), d, c);
    tplL = cvCreateImage(cvSize(W, W), d, c);
    
    /// SELEZIONA I TEMPLATE ///
    cvNamedWindow("mouseSelector", CV_WINDOW_AUTOSIZE);
    cvSetMouseCallback("mouseSelector", mouseHandler, NULL);    
    bool mouthFound = false;
    while (key != 'q' || pointSel<4) {
        src = cvQueryFrame(capture);
        findLip(src, faceDetector, mouthRegion, mouthFound);
        if(!mouthFound)  continue;
        cvCopy( mouthRegion, img1, NULL ); 
        cvLine(mouthRegion, cvPoint(s.width/2,375-17), cvPoint(s.width/2, 375+20), cvScalar(255,255,255,255), 1, 8);
        cvLine(mouthRegion, cvPoint(s.width/2-50,375), cvPoint(s.width/2+50, 375), cvScalar(255,255,255,255), 1, 8);
        cvShowImage("mouseSelector", mouthRegion);
        key = cvWaitKey(0);
    }
    
    roiT = Rect( cvPoint(pointT.x, pointT.y), cvPoint(pointT.x+W, pointT.y+W) );
    roiB = Rect( cvPoint(pointB.x, pointB.y), cvPoint(pointB.x+W, pointB.y+W) );
    roiR = Rect( cvPoint(pointR.x, pointR.y), cvPoint(pointR.x+W, pointR.y+W) );
    roiL = Rect( cvPoint(pointL.x, pointL.y), cvPoint(pointL.x+W, pointL.y+W) );

    cropImageSize(src, tplT, roiT);
    cropImageSize(src, tplB, roiB);
    cropImageSize(src, tplR, roiR);
    cropImageSize(src, tplL, roiL);
    
    cvShowImage("tmpT", tplT);
    cvShowImage("tmpB", tplB);
    cvShowImage("tmpR", tplR);
    cvShowImage("tmpL", tplL);
    
    //cvWaitKey(0);
    /*char ok='n';
    while(ok!=13)
    {
        src = cvQueryFrame(capture);
        result = cvCloneImage(src);        
        findLip(src, faceDetector, mouthRegion, mouthFound);
        if(!mouthFound)  continue;
        ok = poiDetection(mouthRegion, tplT, tplB, tplR, tplL);
    }*/

    /* get image's properties */
    img_width = src->width;
    img_height = src->height;
    tpl_width = tplT->width;
    tpl_height = tplT->height;
    res_width = img_width - tpl_width + 1;
    res_height = img_height - tpl_height + 1;

    /* create new image for template matching computation */
    resT = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    resR = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    resB = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    resL = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    
    /// TROVA LA POSIZIONE INIZIALE DEI TEMPLATE ///
    char k;
    while(1)
    {
        src = cvQueryFrame(capture);  
        cvCopy( src, result, NULL ); 
        
        /* check if mouth is visible in scene */
        findLip(src, faceDetector, mouthRegion, mouthFound);
        if(!mouthFound)  continue;
    
        /* run template matching method */
        cvMatchTemplate(mouthRegion, tplT, resT, CV_TM_SQDIFF_NORMED);
        cvMatchTemplate(mouthRegion, tplB, resB, CV_TM_SQDIFF_NORMED);
        cvMatchTemplate(mouthRegion, tplR, resR, CV_TM_SQDIFF_NORMED);
        cvMatchTemplate(mouthRegion, tplL, resL, CV_TM_SQDIFF_NORMED);

        cvMinMaxLoc(resT, &minvalT ,&maxvalT, &minlocT, &maxlocT, 0);
        cvMinMaxLoc(resB, &minvalB ,&maxvalB, &minlocB, &maxlocB, 0);
        cvMinMaxLoc(resR, &minvalR ,&maxvalR, &minlocR, &maxlocR, 0);        
        cvMinMaxLoc(resL, &minvalL ,&maxvalL, &minlocL, &maxlocL, 0);
        
        // draw rectangle //
        cvRectangle(result, cvPoint(minlocT.x, minlocT.y), cvPoint(minlocT.x + tpl_width, minlocT.y + tpl_height), cvScalar(0, 0, 255, 0), 1, 0, 0);
        cvRectangle(result, cvPoint(minlocB.x, minlocB.y), cvPoint(minlocB.x + tpl_width, minlocB.y + tpl_height), cvScalar(0, 255, 0, 0), 1, 0, 0);
        cvRectangle(result, cvPoint(minlocR.x, minlocR.y), cvPoint(minlocR.x + tpl_width, minlocR.y + tpl_height), cvScalar(255, 0, 0, 0), 1, 0, 0);
        cvRectangle(result, cvPoint(minlocL.x, minlocL.y), cvPoint(minlocL.x + tpl_width, minlocL.y + tpl_height), cvScalar(255, 255, 0, 0), 1, 0, 0);
        
        // display images //                
        cvShowImage("result", result);  
        
        /// repeat while got correct POI position ///
        //k=cvWaitKey(0);
        k=13;
        if(k == 13) 
        {
            roiT = Rect(cvPoint(minlocT.x-R, minlocT.y-R), cvPoint(minlocT.x + tpl_width+R, minlocT.y + tpl_height+R));
            roiB = Rect(cvPoint(minlocT.x-R, minlocB.y-R), cvPoint(minlocT.x + tpl_width+R, minlocB.y + tpl_height+R));
            roiR = Rect(cvPoint(minlocR.x-R, minlocR.y-R), cvPoint(minlocR.x + tpl_width+R, minlocR.y + tpl_height+R));
            roiL = Rect(cvPoint(minlocL.x-R, minlocL.y-R), cvPoint(minlocL.x + tpl_width+R, minlocL.y + tpl_height+R));
            break;
        }
    }    
    
    /// get new image's properties ///
    img_width = searchT->width;
    img_height = searchT->height;
    res_width = img_width - tpl_width + 1;
    res_height = img_height - tpl_height + 1;

    /// create new image for template matching computation ///
    resT = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    resR = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    resB = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    resL = cvCreateImage(cvSize(res_width, res_height), IPL_DEPTH_32F, 1);
    
    int diff;
    
    float mTB, mLR, pTB, pLR;
    float dT, dB, dR, dL;
    int iter = -1;
    cout << "distH ; distV ; darkA ; propV"  << endl;
    
    /* file to save output  */
    fstream f("trackingOUT.txt", ios::out);
    if(!f) {
        cout<<"Impossibile aprire file di output!";
        return -1;
    }
    
    unsigned int bufferLenght = 0;
    float DH1, DH2, DH3;
    float DV1, DV2, DV3;
    float DA1, DA2, DA3;
    float DP1, DP2, DP3;
    int visemeCode;
    cout.precision(8);
    /// INIZIA IL TRACKING DELLE LABBRA ///
    while(1)
    {
        src = cvQueryFrame(capture);
        cvCopy( src, result, NULL ); 
        findLip(src, faceDetector, mouthRegion, mouthFound);
        if(!mouthFound)  continue;
        
        iter = -1;
        diff=21;
        while (diff>20 && iter<3)
        {
            iter++;
            diff=0;

            cropImageSize(mouthRegion, searchT, roiT);
            cropImageSize(mouthRegion, searchB, roiB);
            cropImageSize(mouthRegion, searchR, roiR);
            cropImageSize(mouthRegion, searchL, roiL);
            cvShowImage("searchT", searchT);
            cvShowImage("searchB", searchB);
            cvShowImage("searchR", searchR);
            cvShowImage("searchL", searchL);
            
            /// choose template matching method to be used ///
            cvMatchTemplate(searchT, tplT, resT, CV_TM_SQDIFF_NORMED);
            cvMatchTemplate(searchB, tplB, resB, CV_TM_SQDIFF_NORMED);
            cvMatchTemplate(searchR, tplR, resR, CV_TM_SQDIFF_NORMED);
            cvMatchTemplate(searchL, tplL, resL, CV_TM_SQDIFF_NORMED);

            cvMinMaxLoc(resT, &minvalT ,&maxvalT, &minlocT, &maxlocT, 0);
            cvMinMaxLoc(resB, &minvalB ,&maxvalB, &minlocB, &maxlocB, 0);
            cvMinMaxLoc(resR, &minvalR ,&maxvalR, &minlocR, &maxlocR, 0);        
            cvMinMaxLoc(resL, &minvalL ,&maxvalL, &minlocL, &maxlocL, 0);
            
            minlocT.x += roiT.x;  minlocT.y += roiT.y;
            minlocB.x += roiB.x;  minlocB.y += roiB.y;
            minlocR.x += roiR.x;  minlocR.y += roiR.y;
            minlocL.x += roiL.x;  minlocL.y += roiL.y;
            
            /// find new search position ///
            powT = 1-minvalT;  powB = 1-minvalB;
            powR = 1-minvalR;  powL = 1-minvalL;            
            if(powT<powB)
            {
                /// cambia la posizione in senso orizzontale ///
                diff += abs(minlocT.x-minlocB.x);
                minlocT.x = minlocB.x;
                /// cambia la posizione in senso verticale ///
                if (minlocT.y>minlocL.y || minlocT.y>minlocR.y)
                {
                    diff+=15;
                    minlocT.y=minlocL.y-15;
                }
                /*else if (powT<powR && powT<powL)
                {
                    if (minlocL.x != minlocR.x)
                        mLR = (minlocL.y - minlocR.y)/(minlocL.x - minlocR.x);
                    else
                        mLR=0;
                    pLR = minlocL.y - mLR*minlocL.x;
                    dT = fabs( (minlocT.y - mLR*minlocT.x - pLR)/(sqrt(1+pow(mLR,2))) );
                    dB = fabs( (minlocB.y - mLR*minlocB.x - pLR)/(sqrt(1+pow(mLR,2))) );
                    if (fabs(dT-dB)>20)
                    {
                        minlocT.y = minlocT.y+(dT - dB);
                        if (minlocT.y>minlocL.y || (minlocB.y-minlocT.y)<10 )
                            minlocT.y = minlocL.y-10;

                    }
                }*/
                if(powB<powL || powB<powR)
                {
                    minlocT.x = minlocL.x+((minlocR.x-minlocL.x)/2);
                    minlocB.x = minlocL.x+((minlocR.x-minlocL.x)/2);
                }
                /*if(powT<powL && powT<powR)
                {
                    minlocT.x = minlocL.x+((minlocR.x-minlocL.x)/2);
                }*/
                /// set new searchT ///
                roiT = Rect(cvPoint(minlocT.x-R, minlocT.y-R), cvPoint(minlocT.x + tpl_width+R, minlocT.y + tpl_height+R));
                roiB = Rect(cvPoint(minlocB.x-R, minlocB.y-R), cvPoint(minlocB.x + tpl_width+R, minlocB.y + tpl_height+R));
            }
            else
            {
                /// cambia la posizione in senso orizzontale ///
                diff += abs(minlocB.x-minlocT.x);
                minlocB.x = minlocT.x;
                /// cambia la posizione in senso verticale ///
                if (minlocB.y<minlocL.y || minlocB.y<minlocR.y)
                {
                    diff+=15;
                    minlocB.y=minlocL.y+15;
                }                
                else if (powB<powL && powB<powR)
                {
                    if (minlocL.x != minlocR.x)
                        mLR = (minlocL.y - minlocR.y)/(minlocL.x - minlocR.x);
                    else
                        mLR=0;
                    pLR = minlocL.y - mLR*minlocL.x;
                    dT = fabs( (minlocT.y - mLR*minlocT.x - pLR)/(sqrt(1+pow(mLR,2))) );
                    dB = fabs( (minlocB.y - mLR*minlocB.x - pLR)/(sqrt(1+pow(mLR,2))) );
                    if (fabs(dT-dB)>20)
                    {
                        minlocB.y = minlocB.y+(dT - dB);
                        if (minlocB.y<minlocL.y)
                            minlocB.y = minlocL.y+15;
                    }
                }
                
                if(powT<powL || powT<powR)
                {
                    minlocB.x = minlocL.x+((minlocR.x-minlocL.x)/2);
                    minlocT.x = minlocL.x+((minlocR.x-minlocL.x)/2);
                }
                /*if(powB<powL && powB<powR)
                {
                    minlocB.x = minlocL.x+((minlocR.x-minlocL.x)/2);
                }*/
                /// set new searchB ///                
                roiB = Rect(cvPoint(minlocB.x-R, minlocB.y-R), cvPoint(minlocB.x + tpl_width+R, minlocB.y + tpl_height+R));                
                roiT = Rect(cvPoint(minlocT.x-R, minlocT.y-R), cvPoint(minlocT.x + tpl_width+R, minlocT.y + tpl_height+R));
            }
            if(powR<powL)
            {
                /// cambia la posizione in senso verticale ///
                diff += abs(minlocL.y-minlocR.y);
                minlocR.y = minlocL.y;                
                /// cambia la posizione in senso orizzontale ///
                /*if (powR<powT && powR<powB)
                {
                    if (minlocT.x!=minlocB.x)
                    {
                        mTB = (minlocT.y - minlocB.y)/(minlocT.x - minlocB.x);
                        pTB = minlocT.y - mTB*minlocT.x;
                        dL = fabs( (minlocL.y - mTB*minlocL.x - pTB)/(sqrt(1+pow(mTB,2))) );
                        dR = fabs( (minlocR.y - mTB*minlocR.x - pTB)/(sqrt(1+pow(mTB,2))) );
                    }                    
                    else
                    {
                        dL = minlocT.x-minlocL.x;
                        dR = minlocR.x-minlocT.x;
                    }                
                    if (fabs(dL-dR)>20)
                    {
                        minlocR.x = minlocR.x+(dL - dR);
                    }
                }*/
                /// set new searchR /// 
                roiR = Rect(cvPoint(minlocR.x-R, minlocR.y-R), cvPoint(minlocR.x + tpl_width+R, minlocR.y + tpl_height+R));                
                roiL = Rect(cvPoint(minlocL.x-R, minlocL.y-R), cvPoint(minlocL.x + tpl_width+R, minlocL.y + tpl_height+R));
            }
            else
            {
                diff += abs(minlocL.y-minlocR.y);
                minlocL.y = minlocR.y;
                /// cambia la posizione in senso orizzontale ///
                /*if(powL<powT && powL<powB)
                {
                    if (minlocT.x!=minlocB.x)
                    {
                        mTB = (minlocT.y - minlocB.y)/(minlocT.x - minlocB.x);
                        pTB = minlocT.y - mTB*minlocT.x;
                        dL = fabs( (minlocL.y - mTB*minlocL.x - pTB)/(sqrt(1+pow(mTB,2))) );
                        dR = fabs( (minlocR.y - mTB*minlocR.x - pTB)/(sqrt(1+pow(mTB,2))) );
                    }                    
                    else
                    {
                        dL = minlocT.x-minlocL.x;
                        dR = minlocR.x-minlocT.x;
                    }  
                    if (fabs(dL-dR)>20)
                    {
                        minlocL.x = minlocL.x+(dL - dR);
                    }
                }*/
                roiL = Rect(cvPoint(minlocL.x-R, minlocL.y-R), cvPoint(minlocL.x + tpl_width+R, minlocL.y + tpl_height+R));                
                roiR = Rect(cvPoint(minlocR.x-R, minlocR.y-R), cvPoint(minlocR.x + tpl_width+R, minlocR.y + tpl_height+R));
            }            
            //cvWaitKey(0);
        }
        
        //float darkArea = findDarkArea( mouthRegion, minlocT, minlocL );
        float darkArea = findDarkArea2( mouthRegion, minlocT, minlocR, minlocB, minlocL );
        
        cvRectangle(result, cvPoint(minlocT.x, minlocT.y), cvPoint(minlocT.x + tpl_width, minlocT.y + tpl_height), cvScalar(0, 0, 255, 0), 1, 0, 0);
        cvRectangle(result, cvPoint(minlocB.x, minlocB.y), cvPoint(minlocB.x + tpl_width, minlocB.y + tpl_height), cvScalar(0, 255, 0, 0), 1, 0, 0);
        cvRectangle(result, cvPoint(minlocR.x, minlocR.y), cvPoint(minlocR.x + tpl_width, minlocR.y + tpl_height), cvScalar(255, 0, 0, 0), 1, 0, 0);
        cvRectangle(result, cvPoint(minlocL.x, minlocL.y), cvPoint(minlocL.x + tpl_width, minlocL.y + tpl_height), cvScalar(255, 255, 0, 0), 1, 0, 0);
        
        //cout << " " << minlocL.x-minlocR.x << " ; " << minlocB.y-minlocT.y << " ; " << darkArea << " ; " << minlocL.y-minlocT.y << endl;
        cout << " " << minlocR.x-minlocL.x << " ; " << minlocB.y-minlocT.y << " ; " << darkArea << " ; " << minlocL.y-minlocT.y << endl;

        if (bufferLenght==2)
        {
            DH3 = (double)((minlocR.x-minlocL.x)-DHmin)/DHdiff;
            DV3 = (double)((minlocB.y-minlocT.y)-DVmin)/DVdiff;
            DA3 = darkArea/DAdiff;
            DP3 = (double)((minlocL.y-minlocT.y)-DPmin)/DPdiff;  
            nnInput[8] = DH3;
            nnInput[9] = DV3;
            nnInput[10] = DA3;
            nnInput[11] = DP3;
            
            f <<fixed<< DH1 << " " << DV1 << " " << DA1 << " " << DP1 << " "
              << DH2 << " " << DV2 << " " << DA2 << " " << DP2 << " "
              << DH3 << " " << DV3 << " " << DA3 << " " << DP3 << endl;
            
            /*cout <<fixed
              << DH1 << " " << DV1 << " " << DA1 << " " << DP1 << " "
              << DH2 << " " << DV2 << " " << DA2 << " " << DP2 << " "
              << DH3 << " " << DV3 << " " << DA3 << " " << DP3 << endl;*/
            
            // passa nnInput alle rete neurale "classify(nnInput)"//
            visemeCode = NC.classify(nnInput);
            /*char *text;
            itoa(visemeCode, text, 10);
            int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
            double fontScale = 2;
            int thickness = 3;  
            Point textOrg(50, 50);
            cvPutText(result, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);*/
            
            CvFont font1;
            cvInitFont(&font1,CV_FONT_HERSHEY_DUPLEX,1,1,0.2,2,8);
            cvPutText(result, viseme[visemeCode], Point(50, 50), &font1, Scalar::all(255));
            
            //cout << "visema classificato: " << visemeCode << endl;
            
            DH1=DH2; DH2=DH3;
            DV1=DV2; DV2=DV3;
            DA1=DA2; DA2=DA3;
            DP1=DP2; DP2=DP3;
            nnInput[0] = DH1; nnInput[1] = DV1; nnInput[2] = DA1; nnInput[3] = DP1;
            nnInput[4] = DH2; nnInput[5] = DV2; nnInput[6] = DA2; nnInput[7] = DP2;
        }
        else
        {
            if (bufferLenght==0)
            {
                DH1 = (double)((minlocR.x-minlocL.x)-DHmin)/DHdiff;
                DV1 = (double)((minlocB.y-minlocT.y)-DVmin)/DVdiff;
                DA1 = darkArea/DAdiff;
                DP1 = (double)((minlocL.y-minlocT.y)-DPmin)/DPdiff;     
                //cout <<fixed<< DH1 << " " << DV1 << " " << DA1 << " " << DP1 << " ";
                nnInput[0] = DH1; nnInput[1] = DV1; nnInput[2] = DA1; nnInput[3] = DP1;
            }
            else if (bufferLenght==1)
            {
                DH2 = (double)((minlocR.x-minlocL.x)-DHmin)/DHdiff;
                DV2 = (double)((minlocB.y-minlocT.y)-DVmin)/DVdiff;
                DA2 = darkArea/DAdiff;
                DP2 = (double)((minlocL.y-minlocT.y)-DPmin)/DPdiff;  
                nnInput[4] = DH2; nnInput[5] = DV2; nnInput[6] = DA2; nnInput[7] = DP2;
            }
            bufferLenght++;
        }
            
        
        /// display images ///
        cvShowImage("result", result);
        /*cvShowImage("searchT", searchT);
        cvShowImage("searchB", searchB);
        cvShowImage("searchR", searchR);
        cvShowImage("searchL", searchL);*/
        
        if (cvWaitKey(5) == 27) break;
    }

    //* free memory *//
    f.close();
    cvDestroyWindow("result");
    cvDestroyWindow("tmpT");
    cvDestroyWindow("tmpB");
    cvDestroyWindow("tmpR");
    cvDestroyWindow("tmpL");
    cvDestroyWindow("searchT");
    cvDestroyWindow("searchB");
    cvDestroyWindow("searchR");
    cvDestroyWindow("searchL");
    cvDestroyWindow("mouseSelector");
    cvReleaseImage(&src);
    cvReleaseImage(&tplT);
    cvReleaseImage(&tplR);
    cvReleaseImage(&tplB);
    cvReleaseImage(&tplL);    
    cvReleaseImage(&resT);
    cvReleaseImage(&resR);
    cvReleaseImage(&resB);
    cvReleaseImage(&resL);

    return 0;
}