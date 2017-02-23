#include "eye_detect.hpp"

EyeDetector::EyeDetector()
{
    face_cascade_name = "haarcascade_frontalface_alt.xml";
    eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
    //window_name = "Capture - Face detection";
    //-- 1. Load the cascades
    if (!face_cascade.load(face_cascade_name)) {
        printf("--(!)Error loading\n");
    }
    if (!eyes_cascade.load(eyes_cascade_name)) {
        printf("--(!)Error loading\n");
    }
}

/** @function main */
int EyeDetector::detect() 
// return 1 or 0: 1=accepts word, 0=deletes word
{
    CvCapture* capture;
    Mat frame;    

    /// Read the video stream ///
    capture = cvCaptureFromCAM(-1);
    bool completed = false;
    int eye_status = -1;
    int open=0, close=0;
    int fontFace = CV_FONT_HERSHEY_DUPLEX;
    double fontScale = 1.5;
    int thickness = 2;  
    string eo;
    string ec;
    ostringstream oss;
    string tmp;
    if (capture)
    {
        while (!completed)
        {
            frame = cvQueryFrame(capture);;
            /// Apply the classifier to the frame ///
            if (!frame.empty())
            {
                eye_status = detectAndDisplay(frame);
                if (eye_status==1) // eyes close
                {
                    open-=2;
                    close++;
                    oss.str("");    oss.clear();
                    oss << open;    //tmp = oss.str();
                    eo = oss.str();
                    oss.str("");    oss.clear();
                    oss << close;
                    ec = oss.str();
                    putText(frame, eo, Point(50, 50), fontFace, 1, cvScalar(255, 0, 0, 0), 2,8);
                    putText(frame, ec, Point(50, 100), fontFace, 1, cvScalar(255, 0, 0, 0), 2,8);
                    //imshow(window_name, frame);
                    if (close>=CLOSE_TIME)
                    {
                        //destroyWindow(window_name);
                        return 1;
                    }
                }
                else    // eyes open
                {
                    open++;
                    close=0;
                    oss.str("");  oss.clear();
                    oss << "open = " << open;
                    eo = oss.str();
                    oss.str("");  oss.clear();
                    oss << "close = " << close;
                    ec = oss.str();
                    putText(frame, eo, Point(50, 50), fontFace, 1, cvScalar(255, 0, 0, 0), 2,8);
                    putText(frame, ec, Point(50, 100), fontFace, 1, cvScalar(255, 0, 0, 0), 2,8);
                    //imshow(window_name, frame);
                    if (open>=OPEN_TIME)
                    {
                        //destroyWindow(window_name);
                        return 0;
                    }
                }
            } 
            /*else {
                printf(" --(!) No captured frame -- Break!");
                break;
            }*/

            int c = waitKey(10);
            if ((char) c == 'c') {
                break;
            }
        }
    }
    return 0;
}

int EyeDetector::detectAndDisplay(Mat frame) 
// retun 0 or 1: 0=open eyes, 1=closed eyes
{
    vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //-- Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
    
    for (int i = 0; i < faces.size(); i++) 
    {
        Point center(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
        ellipse(frame, center, Size(faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
        
        Rect eyesROI = faces[i];
        eyesROI.height /= 2;        
        Mat faceROI = frame_gray(eyesROI);
        //Mat faceROI = frame_gray(faces[i]);
        vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
        
        int fontFace = CV_FONT_HERSHEY_DUPLEX;
        double fontScale = 1.5;
        int thickness = 3;  
        if (eyes.size()==0)
        {
            putText(frame, "EYES CLOSE", Point(225, 40), fontFace, fontScale, cvScalar(255, 0, 0, 0), thickness,8);
            //imshow(window_name, frame);
            return 1;
        }
        else
        {
            putText(frame, "EYES OPEN", Point(225, 40), fontFace, fontScale, cvScalar(255, 0, 0, 0), thickness,8);
            for (int j = 0; j < eyes.size(); j++) {
                Point center(faces[i].x + eyes[j].x + eyes[j].width * 0.5, faces[i].y + eyes[j].y + eyes[j].height * 0.5);
                int radius = cvRound((eyes[j].width + eyes[j].height)*0.4);
                circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
            }
            //imshow(window_name, frame);
            return 0;
        }
    }
}
