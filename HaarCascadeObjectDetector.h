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


/**
 * @brief Object Detector Virtual Class
 * @author Robert Kirchgessner
 */

#ifndef HCOBJECTDETECTOR_H
#define HCOBJECTDETECTOR_H

#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "ObjectDetector.h"

#define DEFAULT_SEARCH_SCALE  1.3
#define DEFAULT_SCALE_FACTOR  1.1
#define DEFAULT_MIN_NEIGHBORS 2
#define DEFAULT_OPTS          CV_HAAR_SCALE_IMAGE
#define DEFAULT_MIN_SIZE      cv::Size(30, 30)

class HaarCascadeObjectDetector : public ObjectDetector {
public:
    HaarCascadeObjectDetector(std::string fname,
            double scale = DEFAULT_SEARCH_SCALE,
            double factor = DEFAULT_SCALE_FACTOR,
            unsigned int min_neighbors = DEFAULT_MIN_NEIGHBORS,
            unsigned int opts = DEFAULT_OPTS,
            cv::Size min_size = DEFAULT_MIN_SIZE);

    HaarCascadeObjectDetector(cv::CascadeClassifier cascade,
            double scale = DEFAULT_SEARCH_SCALE,
            double factor = DEFAULT_SCALE_FACTOR,
            unsigned int min_neighbors = DEFAULT_MIN_NEIGHBORS,
            unsigned int opts = DEFAULT_OPTS,
            cv::Size min_size = DEFAULT_MIN_SIZE);

    virtual ~HaarCascadeObjectDetector();

    int detect(cv::Mat image, std::vector<cv::Rect>& objects, cv::Rect ROI = cv::Rect());

    void setImageScale(double scale);
    void setScaleFactor(double factor);
    void setMinNeighbors(unsigned int n);
    void setOptions(unsigned int opts);
    void setMinSize(cv::Size min_size);

private:
    cv::CascadeClassifier cascade;

    /*OpenCV Cascade search params*/
    double search_scale;
    double scale_factor;
    unsigned int min_neighbors;
    unsigned int opts;
    cv::Size min_size;
};

#endif /*HCOBJECTDETECTOR_H*/