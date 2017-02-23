

#include "poiDetection.hpp"

void findLipMask(IplImage* srcQSI, IplImage* result)
{
    /*CvSize s = cvSize(srcQSI->width, srcQSI->height);
    int d = srcQSI->depth;
    IplImage* Q = cvCreateImage(s, d, 1);
    IplImage* S = cvCreateImage(s, d, 1);
    IplImage* I = cvCreateImage(s, d, 1);
    cvSplit(srcQSI, Q, S, I, NULL);    
    cvSmooth( Q, Q, CV_MEDIAN, 3, 3 );
    cvSmooth( S, S, CV_MEDIAN, 3, 3 );
    cvSmooth( I, I, CV_MEDIAN, 3, 3 );
    cvNot( I,I );*/
    
    cvCvtColor(srcQSI, result, CV_BGR2GRAY); 
    cvThreshold(result, result, 63, 255, CV_THRESH_BINARY );    // th = 63 o 100
    cvDilate(result, result, NULL, 3);
    cvErode(result, result, NULL, 3);
    //cvShowImage("grayQSI", result);
    
    /*cvReleaseImage(&Q);
    cvReleaseImage(&S);
    cvReleaseImage(&I);*/
}

void findPOI(IplImage* lipMask, CvPoint &top, CvPoint &bottom, CvPoint &left, CvPoint &right)
{
    CvScalar white = CV_RGB( 255, 255, 255 );

    IplImage* dst = cvCreateImage( cvGetSize(lipMask), 8, 1);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contour = 0;

    cvFindContours(lipMask, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    cvZero( dst );

    int size = 50;
    double largestArea = 0;
    CvSeq* largestContour = NULL;
    while (contour != NULL)
    {
        double area = cvContourArea(contour, CV_WHOLE_SEQ, 0);       
        if(area>largestArea && fabs(area)>size)
        {
            largestArea = area;
            largestContour = contour;
        }
        contour = contour->h_next;
    }        
    if(largestContour!=NULL)
    {
        cvDrawContours( dst, largestContour, white, white, 0, CV_FILLED);     
    cvInRangeS(dst, white, white, lipMask);
    
    /// get template ///
    int length = largestContour->total;
    CvPoint* point = new CvPoint[length];

    CvSeqReader reader;
    CvPoint pt = cvPoint(0, 0);
    CvSeq* contour2 = largestContour;

    cvStartReadSeq(contour2, &reader);
    for (int i=0; i<length; i++) 
    {
        CV_READ_SEQ_ELEM(pt, reader);
        point[i] = pt;
    }
    cvReleaseMemStorage(&storage);

    top=point[0]; bottom=point[1]; 
    right=point[2]; left=point[3];
    for (int i = 4; i < length; i++) 
    {
        if(point[i].y<top.y)
            top=point[i];
        else if(point[i].y>bottom.y)
            bottom=point[i];
        else if(point[i].x>right.x)
            right=point[i];
        else if(point[i].x<left.x)
            left=point[i];
    }
    
    top.x = (left.x+right.x)/2;
    bottom.x = top.x;
    
    top.x = top.x-W/2;        top.y = top.y-W/2;
    bottom.x = bottom.x-W/2;  bottom.y = bottom.y-W/2;
    right.x = right.x-W/2;    right.y = right.y-W/2;
    left.x = left.x-W/2;      left.y = left.y-W/2;
    }
    cvReleaseImage(&dst); 
} 

/// tmp* size = WxW ///
/// mouthRegion size = 640x480 ///
char poiDetection(IplImage* mouthRegion, IplImage* tmpT, IplImage* tmpB, IplImage* tmpR, IplImage* tmpL)
{
    IplImage* mouthRegionCopy = cvCloneImage(mouthRegion);
    IplImage* mouthRegionQSI = cvCreateImage(cvGetSize(mouthRegion), 8, 3);
    convertImageRGBtoQSI(mouthRegion, mouthRegionQSI);
    
    IplImage* lipMask = cvCreateImage(cvGetSize(mouthRegion), 8, 1);
    findLipMask(mouthRegionQSI, lipMask);
    
    CvPoint top, bottom, right, left;
    findPOI(lipMask, top, bottom, right, left);
    
    cvRectangle(mouthRegionCopy, cvPoint(top.x-1, top.y-1), cvPoint(top.x+W+1, top.y+W+1), cvScalar(0, 0, 255, 0), 1, 0, 0);
    cvRectangle(mouthRegionCopy, cvPoint(bottom.x-1, bottom.y-1), cvPoint(bottom.x+W+1, bottom.y+W+1), cvScalar(0, 255, 0, 0), 1, 0, 0);
    cvRectangle(mouthRegionCopy, cvPoint(right.x-1, right.y-1), cvPoint(right.x+W+1, right.y+W+1), cvScalar(255, 0, 0, 0), 1, 0, 0);
    cvRectangle(mouthRegionCopy, cvPoint(left.x-1, left.y-1), cvPoint(left.x+W, left.y+W+1), cvScalar(255, 255, 0, 0), 1, 0, 0);
    
    cvCircle(mouthRegionCopy, cvPoint(top.x+W/2, top.y+W/2), 5, cvScalar(0, 0, 255, 0), 1, 8, 0);
    cvCircle(mouthRegionCopy, cvPoint(bottom.x+W/2, bottom.y+W/2), 5, cvScalar(0, 255, 0, 0), 1, 8, 0);
    cvCircle(mouthRegionCopy, cvPoint(right.x+W/2, right.y+W/2), 5, cvScalar(255, 0, 0, 0), 1, 8, 0);
    cvCircle(mouthRegionCopy, cvPoint(left.x+W/2, left.y+W/2), 5, cvScalar(255, 255, 0, 0), 1, 8, 0);
    
    cvLine(mouthRegionCopy, cvPoint(top.x+W/2, top.y+W/2), cvPoint(top.x+W/2, top.y+W/2+35), cvScalar(255, 255, 255, 255), 1, 8);
    cvLine(mouthRegionCopy, cvPoint(right.x+W/2, right.y+W/2), cvPoint(right.x+W/2+85, right.y+W/2), cvScalar(255, 255, 255, 255), 1, 8);
    
    cvNamedWindow("lipMask", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("mouthRegion", CV_WINDOW_AUTOSIZE);
    cvShowImage("lipMask", lipMask);
    cvShowImage("mouthRegion", mouthRegionCopy);
    
    char ok = cvWaitKey(0);
    if (ok==13)
    {
        Rect roiT( cvPoint(top.x, top.y), cvPoint(top.x+W, top.y+W) );
        Rect roiB( cvPoint(bottom.x, bottom.y), cvPoint(bottom.x+W, bottom.y+W) );
        Rect roiR( cvPoint(right.x, right.y), cvPoint(right.x+W, right.y+W) );
        Rect roiL( cvPoint(left.x, left.y), cvPoint(left.x+W, left.y+W) );
        
        cropImageSize(mouthRegion, tmpT, roiT);
        cropImageSize(mouthRegion, tmpB, roiB);
        cropImageSize(mouthRegion, tmpR, roiR);
        cropImageSize(mouthRegion, tmpL, roiL);
        
        cvShowImage("tmpT", tmpT);
        cvShowImage("tmpB", tmpB);
        cvShowImage("tmpR", tmpR);
        cvShowImage("tmpL", tmpL);
        
        //cvDestroyWindow("lipMask");
        //cvDestroyWindow("mouthRegion");
    }
    /*cvCircle(mouthRegion, top, 10, CV_RGB(255, 0, 0), 1, 8, 0);
    cvCircle(mouthRegion, bottom, 10, CV_RGB(0, 255, 0), 1, 8, 0);
    cvCircle(mouthRegion, right, 10, CV_RGB(0, 0, 255), 1, 8, 0);
    cvCircle(mouthRegion, left, 10, CV_RGB(255, 255, 0), 1, 8, 0);*/
    
    cvReleaseImage(&mouthRegionCopy);
    cvReleaseImage(&mouthRegionQSI);
    cvReleaseImage(&lipMask);
    
    return ok;
}


