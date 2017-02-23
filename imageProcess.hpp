/* 
 * File:   imageProcess.hpp
 * Author: Matteo
 *
 * Created on 12 maggio 2013, 17.07
 */

#include "param.hpp"

#ifndef IMAGEPROCESS_HPP
#define	IMAGEPROCESS_HPP

void cropImageSize(IplImage* src, IplImage* result, Rect roi);
void cropImage2(IplImage* src, IplImage* result, Rect roi);
void cropImageEllipse(IplImage* src, IplImage* result, CvPoint t, CvPoint l);

Rect findMouthRect1(Rect faceRect);
void findLip(IplImage* src, ObjectDetector* faceDetector, IplImage* result, bool &mouthFound);
float findDarkArea( IplImage* mouthRegion, CvPoint t, CvPoint l);
float findDarkArea2( IplImage* mouthRegion, CvPoint t, CvPoint r, CvPoint b, CvPoint l);

#endif	/* IMAGEPROCESS_HPP */

