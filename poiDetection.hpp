/* 
 * File:   poiDetection.hpp
 * Author: Matteo
 *
 * Created on 12 maggio 2013, 11.42
 */

#include "imageProcess.hpp"
#include "param.hpp"

#ifndef POIDETECTION_HPP
#define	POIDETECTION_HPP

char poiDetection(IplImage* mouthRegion, IplImage* tmpT, IplImage* tmpB, IplImage* tmpR, IplImage* tmpL);
void convertImageRGBtoQSI(IplImage *imageRGB, IplImage* imageQSI);

#endif	/* POIDETECTION_HPP */

