/* 
 * File:   imageProcess.cpp
 * Author: Matteo
 * 
 * Created on 12 maggio 2013, 17.07
 */

#include "imageProcess.hpp"

void convertImageRGBtoQSI(IplImage *imageRGB, IplImage* imageQSI)
{
    float fR, fG, fB;
    float fQ, fS, fI;
    const float FLOAT_TO_BYTE = 255.0f;
    const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
    const float MIN_Q = -0.5226f;
    const float Q_TO_BYTE = 255.0f / (MIN_Q * -2.0f);
    
    if (!imageQSI || imageRGB->depth != 8 || imageRGB->nChannels != 3) {
        printf("ERROR in convertImageRGBtoQSI()! Bad input image.\n");
        exit(1);
    }

    int h = imageRGB->height; // Pixel height.
    int w = imageRGB->width; // Pixel width.
    int rowSizeRGB = imageRGB->widthStep; // Size of row in bytes, including extra padding.
    char *imRGB = imageRGB->imageData; // Pointer to the start of the image pixels.
    int rowSizeQSI = imageQSI->widthStep; // Size of row in bytes, including extra padding.
    char *imQSI = imageQSI->imageData; // Pointer to the start of the image pixels.
    
    for (int y = 0; y < h; y++) 
    {
        for (int x = 0; x < w; x++) 
        {
            // Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
            uchar *pRGB = (uchar*) (imRGB + y * rowSizeRGB + x * 3);
            int bB = *(uchar*) (pRGB + 0); // Blue component
            int bG = *(uchar*) (pRGB + 1); // Green component
            int bR = *(uchar*) (pRGB + 2); // Red component

            // Convert from 8-bit integers to floats.
            fR = bR * BYTE_TO_FLOAT;
            fG = bG * BYTE_TO_FLOAT;
            fB = bB * BYTE_TO_FLOAT;
            
            // Convert from RGB to HSV, using float ranges 0.0 to 1.0.
            float fDelta;
            float fMin, fMax;
            int iMax;
            // Get the min and max, but use integer comparisons for slight speedup.
            if (bB < bG) {
                if (bB < bR) {
                    fMin = fB;
                    if (bR > bG) {
                        iMax = bR;
                        fMax = fR;
                    } else {
                        iMax = bG;
                        fMax = fG;
                    }
                } 
                else {
                    fMin = fR;
                    fMax = fG;
                    iMax = bG;
                }
            } 
            else {
                if (bG < bR) {
                    fMin = fG;
                    if (bB > bR) {
                        fMax = fB;
                        iMax = bB;
                    } else {
                        fMax = fR;
                        iMax = bR;
                    }
                } 
                else {
                    fMin = fR;
                    fMax = fB;
                    iMax = bB;
                }
            }
            
            fDelta = fMax - fMin;
            
            fI = ( 2*fG-fR-fB/2 )/4;
            
            if (iMax != 0) 
            { // Make sure its not pure black.                
                fS = (fR-fG)/fR;
            } 
            else {
                // color is pure Black.
                fS = 0;
            }
            
            fQ = 0.211456 * fR - 0.522591 * fG + 0.311135 * fB;

            // Convert from floats to 8-bit integers.
            int bQ = (int) (0.5f + (fQ - MIN_Q) * Q_TO_BYTE);
            int bS = (int) (0.5f + fS * 255.0f);
            int bI = (int) (0.5f + fI * 255.0f);

            // Clip the values to make sure it fits within the 8bits.
            if (bQ > 255)  bQ = 255;
            if (bQ < 0)    bQ = 0;
            if (bS > 255)  bS = 255;
            if (bS < 0)    bS = 0;
            if (bI > 255)  bI = 255;
            if (bI < 0)    bI = 0;

            // Set the QSI pixel components.
            uchar *pQSI = (uchar*) (imQSI + y * rowSizeQSI + x * 3);
            *(pQSI + 0) = bQ; // Q component
            *(pQSI + 1) = bS; // S component
            *(pQSI + 2) = bI; // I component
        }
    }
}

/// crop roi region from src to result ///
void cropImageSize(IplImage* src, IplImage* result, Rect roi)
{
    cvSetImageROI(src, roi);
    cvCopy(src, result, NULL);
    cvResetImageROI(src);

    //src = cvCloneImage(result);    
}

void cropImage2(IplImage* src, IplImage* result, Rect roi)
{
    IplImage* roiMask = cvCreateImage(cvGetSize(src), 8, src->nChannels);
    cvZero(result);

    cvRectangle( roiMask, Point(roi.x, roi.y), Point(roi.x + roi.width, roi.y + roi.height), CV_RGB(255, 255, 255), -1);

    cvAnd(src, roiMask, result, 0);
    cvReleaseImage(&roiMask);
}

void cropImageEllipse(IplImage* src, IplImage* roiMask, IplImage* result, CvPoint t, CvPoint l)
{
    //IplImage* roiMask = cvCreateImage(cvGetSize(src), 8, src->nChannels);
    cvZero(result);    
    
    cvEllipse( roiMask, cvPoint(t.x+W/2, l.y+W/2), cvSize(abs(t.x-l.x), abs(l.y-t.y)), 
                   0, 0, 360, CV_RGB(255, 255, 255), -1, 8, 0 );

    cvAnd(src, roiMask, result, 0);
    //cvReleaseImage(&roiMask);
}

void cropImagePoly(IplImage* src, IplImage* roiMask, IplImage* result, CvPoint t, CvPoint r, CvPoint b, CvPoint l)
{
    int numPts = 5;
    CvPoint pts[] = {t, r, b, l, t};
    cvFillConvexPoly(roiMask, pts, numPts, cvScalar(255));
    cvAnd(src, roiMask, result, 0);
}

Rect findMouthRect1(Rect faceRect)
{
    CvPoint topLeft, bottomRight;
    topLeft.x = faceRect.x+60;   
    topLeft.y = faceRect.y + cvRound(faceRect.height / 2.0)+45;
    bottomRight.x = faceRect.x+faceRect.width-60;
    bottomRight.y = faceRect.y+cvRound(faceRect.height)+30;
    
    Rect mouthRect( topLeft, bottomRight);

    return mouthRect;
}

/*
void findLip(IplImage* src, ObjectDetector* faceDetector, IplImage* result, bool &mouthFound)
{    
    //mouthFound=false;
    Mat frame = src;
    Mat grayImg;
    IplImage* roiMask = cvCreateImage(cvGetSize(src), 8, src->nChannels);    
    cvZero(result);
    cvtColor(frame, grayImg, CV_BGR2GRAY);

    //// find face ////
    vector<Rect> faces;
    faces.clear();
    faceDetector->detect(grayImg, faces);

    vector<Rect>::const_iterator it;
    for (it = faces.begin(); it != faces.end(); it++) 
    {
        //rectangle(frame, Point(it->x + 20, it->y - 25), Point(it->x + it->width - 20, it->y + it->height + 25), CV_RGB(255, 0, 0));
        
        Rect mouthRect = findMouthRect1(*it);        
        mouthFound = true;

        int mX1, mX2, mY1, mY2;
        mX1 = mouthRect.x;
        mY1 = mouthRect.y;
        mX2 = mouthRect.x + mouthRect.width;
        mY2 = mouthRect.y + mouthRect.height;
        
        //rectangle( frame, Point(mX1, mY1), Point(mX2, mY2), CV_RGB(255, 255, 255));
        cvRectangle( roiMask, Point(mX1+1, mY1+1), Point(mX2-2, mY2-2), CV_RGB(255, 255, 255), -1);

        cvAnd(src, roiMask, result, 0);
    }
    cvReleaseImage(&roiMask);
}
*/
void findLip(IplImage* src, ObjectDetector* faceDetector, IplImage* result, bool &mouthFound)
{    
    //mouthFound=false;
    Mat frame = src;
    Mat grayImg;
    IplImage* roiMask = cvCreateImage(cvGetSize(src), 8, src->nChannels);
    cvZero(result);
    cvtColor(frame, grayImg, CV_BGR2GRAY);

    //// find face ////
    vector<Rect> faces;
    faces.clear();
    faceDetector->detect(grayImg, faces);
    
    vector<Rect>::const_iterator it;
    for (it = faces.begin(); it != faces.end(); it++) 
    {
        //rectangle(frame, Point(it->x + 20, it->y - 25), Point(it->x + it->width - 20, it->y + it->height + 25), CV_RGB(255, 0, 0));
        
        Rect mouthRect = findMouthRect1(*it);        
        mouthFound = true;

        int mX1, mX2, mY1, mY2;
        mX1 = mouthRect.x;
        mY1 = mouthRect.y;
        mX2 = mouthRect.x + mouthRect.width + 20;
        mY2 = mouthRect.y + mouthRect.height + 20;
        
        cvRectangle( roiMask, Point(mX1, mY1), Point(mX2, mY2), CV_RGB(255, 255, 255), -1);
        //cvRectangle( roiMask, Point(mX1+1, mY1+1), Point(mX2-2, mY2-2), CV_RGB(255, 255, 255), -1);
        cvAnd(src, roiMask, result, 0);
        
        Mat myMat(result);
        Rect croppedRectangle = mouthRect;
        Mat croppedImage = myMat(croppedRectangle);
        /* visualizza finestra solo con bocca (però non è fissa ma si adatta alla grandezza dell'immagine acquisita
        namedWindow("r", WINDOW_AUTOSIZE );
        resizeWindow("r", 100, 100);
        imshow("r", croppedImage);
         */

    }
    cvReleaseImage(&roiMask);
}

void adaptiveTh(IplImage* src, IplImage* result, IplImage* ellipseMask)
{
    float fI;
    const float FLOAT_TO_BYTE = 255.0f;
    const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

    int h = src->height; // Pixel height.
    int w = src->width; // Pixel width.
    int rowSizeI = src->widthStep; // Size of row in bytes, including extra padding.
    char *imI = src->imageData; // Pointer to the start of the image pixels.
    int rowSizeResult = result->widthStep; // Size of row in bytes, including extra padding.
    char *imResult = result->imageData; // Pointer to the start of the image pixels.
    int rowSizeM = ellipseMask->widthStep; // Size of row in bytes, including extra padding.
    char *imM = ellipseMask->imageData; // Pointer to the start of the image pixels.
    
    int n=0;
    float sum=0;
    for (int y = 0; y < h; y++) 
    {
        for (int x = 0; x < w; x++) 
        {
            n++;
            // Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
            uchar *pI = (uchar*) (imI + y * rowSizeI + x);
            int bI = *(uchar*) (pI ); // Pixel Intensity
            sum += bI;   
        }
    }
    float th = 0.5*(sum/n);     //0.7
    for (int y = 0; y < h; y++) 
    {
        for (int x = 0; x < w; x++) 
        {
            // Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
            uchar *pI = (uchar*) (imI + y * rowSizeI + x);
            int bI = *(uchar*) (pI );
            fI = bI * BYTE_TO_FLOAT;
            uchar *pResult = (uchar*) (imResult + y * rowSizeResult + x);
            if (fI<th)    
            {
                uchar *pM = (uchar*) (imM + y * rowSizeM + x);
                int bM = *(uchar*) (pM ); // Pixel Intensity
                if(bM>0)
                    *(pResult) = 255;
                else
                    *(pResult) = 0;
            }
            else
                *(pResult) = 0;
        }
    }
    cvDilate(result, result, NULL, 2);
    cvErode(result, result, NULL, 2);
}

float findDarkArea( IplImage* mouthRegion, CvPoint t, CvPoint l)
{
    float area=0;
    
    IplImage* interiorLip = cvCreateImage(cvGetSize(mouthRegion), 8, 3);
    IplImage* ellipseMask = cvCreateImage(cvGetSize(mouthRegion), 8, 3);
    IplImage* ellipseMaskBin = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    
    cropImageEllipse(mouthRegion, ellipseMask, interiorLip, t, l);    
    cvCvtColor(ellipseMask, ellipseMaskBin, CV_RGB2GRAY);
    //cvShowImage("eMask", ellipseMaskBin);
    
    IplImage* interiorLipGray = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    cvCvtColor(interiorLip, interiorLipGray, CV_RGB2GRAY);
    cvShowImage("area", interiorLipGray);
    
    IplImage* interiorLipMask = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    adaptiveTh (interiorLipGray, interiorLipMask, ellipseMaskBin);
    
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contour = 0;
    cvFindContours( interiorLipMask, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    if(contour!=NULL)
        area = cvContourArea(contour, CV_WHOLE_SEQ, 0);
    cvShowImage("darkArea", interiorLipMask);
    
    cvWaitKey(0);
    cvReleaseMemStorage(&storage);
    cvReleaseImage(&interiorLip);
    cvReleaseImage(&ellipseMask);
    cvReleaseImage(&ellipseMaskBin);
    cvReleaseImage(&interiorLipGray);
    cvReleaseImage(&interiorLipMask);
    return area;
}

float findDarkArea2(IplImage* mouthRegion, CvPoint t, CvPoint r, CvPoint b, CvPoint l)
{
    double area=0;
    IplImage* mouthRegionQSI = cvCreateImage(cvGetSize(mouthRegion), 8, 3);
    IplImage* mouthRegionQSIgray = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    IplImage* polyMask = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    IplImage* interiorLip = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    IplImage* darkArea = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    
    int w = W/2;
    t.x=t.x+w;  t.y=t.y+w;
    r.x=r.x+w;  r.y=r.y+w;
    b.x=b.x+w;  b.y=b.y+w;
    l.x=l.x+w;  l.y=l.y+w;
    convertImageRGBtoQSI(mouthRegion, mouthRegionQSI);
    cvCvtColor(mouthRegionQSI, mouthRegionQSIgray, CV_RGB2GRAY);
    cvZero(polyMask); 
    int numPts = 5;
    CvPoint pts[] = {t, r, b, l, t};
    cvFillConvexPoly(polyMask, pts, numPts, cvScalar(255));
    cvZero(interiorLip);
    cvCopy( mouthRegionQSIgray, interiorLip, polyMask );
    
    adaptiveTh (interiorLip, darkArea, polyMask);
    //cvShowImage("darkArea", darkArea);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contour = 0;
    cvFindContours( darkArea, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    int size = 50;
    double largestArea = 0;
    while (contour != NULL)
    {
        double area = cvContourArea(contour, CV_WHOLE_SEQ, 0);       
        if(area>largestArea && fabs(area)>size)
        {
            largestArea = area;
        }
        contour = contour->h_next;
    }            
    //cvShowImage("mouthRegionQSI", mouthRegionQSIgray);
    //cvShowImage("interiorLip", interiorLip);    
    
    //cvWaitKey(0);
    cvReleaseMemStorage(&storage);
    cvReleaseImage(&mouthRegionQSI);
    cvReleaseImage(&mouthRegionQSIgray);
    cvReleaseImage(&polyMask);
    cvReleaseImage(&interiorLip);
    cvReleaseImage(&darkArea);
    return largestArea;
}

