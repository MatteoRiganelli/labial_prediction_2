/* 
 * File:   param.hpp
 * Author: Matteo
 *
 * Created on 15 maggio 2013, 12.33
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include<opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

#include "HaarCascadeObjectDetector.h"

using namespace std;
using namespace cv;

#define FEAT_FACE_FILE "haarcascade_frontalface_alt.xml"
#define W 25            // template size
#define R 10            // search area size = W+R
#define TH 63

