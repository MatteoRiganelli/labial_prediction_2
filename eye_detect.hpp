/* 
 * File:   eye_detect.hpp
 * Author: Matteo
 *
 * Created on 11 luglio 2013, 10.49
 */

#ifndef EYE_DETECT_HPP
#define	EYE_DETECT_HPP

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>  

using namespace std;
using namespace cv;

#define CLOSE_TIME 5
#define OPEN_TIME 15

class EyeDetector
{
public:
    EyeDetector();
    int detect();
    
private:
    int detectAndDisplay(Mat frame);
    
    string face_cascade_name;
    string eyes_cascade_name;
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;
    string window_name;
    //RNG rng(12345);
};

#endif	/* EYE_DETECT_HPP */

