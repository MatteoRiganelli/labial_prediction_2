/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 */

//#define __TEST__

#include <iostream>

#include "HaarCascadeObjectDetector.h"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

HaarCascadeObjectDetector::HaarCascadeObjectDetector(string fname, double scale, double factor,
        unsigned int min_neighbors, unsigned int opts,
        Size min_size) : search_scale(scale),
scale_factor(factor),
min_neighbors(min_neighbors),
opts(opts), min_size(min_size) {
    if (!cascade.load(fname)) {
        /*Should eventually throw an exception*/
        cerr << "Could not load cascade file: " << fname << endl;
        exit(-1);
    }
}

HaarCascadeObjectDetector::HaarCascadeObjectDetector(CascadeClassifier cascade, double scale, double factor,
        unsigned int min_neighbors, unsigned int opts,
        Size min_size) : cascade(cascade),
search_scale(scale),
scale_factor(factor),
min_neighbors(min_neighbors),
opts(opts), min_size(min_size) {
    /*Nothing to do*/
}

HaarCascadeObjectDetector::~HaarCascadeObjectDetector() {
    /*Nothing to do*/
}

int
HaarCascadeObjectDetector::detect(Mat image, vector<Rect>& objects, Rect ROI) {
    vector<Rect> rects;
    Mat scaledImage;

    if (ROI != Rect()) {
        image = image(ROI);
    }

    if (this->search_scale != 1.0) {
        /*Generate a scaled image version for searching*/
        scaledImage = Mat(cvRound(image.rows / this->search_scale), cvRound(image.cols / this->search_scale), CV_8UC1);
        resize(image, scaledImage, scaledImage.size(), 0, 0, INTER_LINEAR);
    } else {
        scaledImage = Mat(image); //Create copy of input image
    }

    equalizeHist(scaledImage, scaledImage);

    /*Do the detection*/
    this->cascade.detectMultiScale(scaledImage,
            rects,
            this->scale_factor,
            this->min_neighbors,
            this->opts,
            this->min_size);

    vector<Rect>::const_iterator it;
    for (it = rects.begin(); it != rects.end(); it++) {
        //Determine original object rectangle 
        Rect object_ROI(cvRound(it->x * this->search_scale), cvRound(it->y * this->search_scale), cvRound(it->width * this->search_scale), cvRound(it->height * this->search_scale));

        if (ROI == Rect()) {
            objects.push_back(object_ROI);
        } else {
            objects.push_back(Rect(ROI.x + object_ROI.x, ROI.y + object_ROI.y, object_ROI.width, object_ROI.height));
        }
    }

    return rects.size();
}

void
HaarCascadeObjectDetector::setImageScale(double scale) {
    this->search_scale = scale;
}

void
HaarCascadeObjectDetector::setScaleFactor(double factor) {
    this->scale_factor = factor;
}

void
HaarCascadeObjectDetector::setMinNeighbors(unsigned int n) {
    this->min_neighbors = n;
}

void
HaarCascadeObjectDetector::setOptions(unsigned int opts) {
    this->opts = opts;
}

void
HaarCascadeObjectDetector::setMinSize(cv::Size min_size) {
    this->min_size = min_size;
}

#ifdef __TEST__
#endif /*__TEST__*/

