#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include "drawLandmarks.hpp"

//https://curl.haxx.se/docs/install.html
#include <stdio.h>
 
 
using namespace std;
using namespace cv;
using namespace cv::face;

// Compile command
// g++ $(pkg-config --cflags --libs opencv) -std=c++11 face.cpp -o hellpCpp -lcurl
 
 
int main(int argc,char** argv)
{



    // Load Face Detector
    CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");
 
    // Create an instance of Facemark
    Ptr<Facemark> facemark = FacemarkLBF::create();
 
    // Load landmark detector
    facemark->loadModel("lbfmodel.yaml");
 
    // Set up webcam for video capture
    VideoCapture cam(0);
     
    // Variable to store a video frame and its grayscale 
    Mat frame, gray;
     
    // Read a frame
    while(cam.read(frame))
    {
       
      // Find face
      vector<Rect> faces;
      // Convert frame to grayscale because
      // faceDetector requires grayscale image.
      cvtColor(frame, gray, COLOR_BGR2GRAY);
 
      // Detect faces
      faceDetector.detectMultiScale(gray, faces);
       
      // Variable for landmarks. 
      // Landmarks for one face is a vector of points
      // There can be more than one face in the image. Hence, we 
      // use a vector of vector of points. 
      vector< vector<Point2f> > landmarks;
       
      // Run landmark detector
      bool success = facemark->fit(frame,faces,landmarks);
       
      if(success)
      {
        int biggestFace = 0;
        float biggestFaceArea = 0.0;
        // If successful, render the landmarks on the face
        for(int i = 0; i < landmarks.size(); i++)
        {
          float faceArea = getFaceArea(landmarks[i]);
          if(faceArea > biggestFaceArea) {
            biggestFaceArea = faceArea;
            biggestFace = i;
          }
        }

        //std::cout<<"face area: "<<biggestFaceArea;

        if(biggestFaceArea > 100000) {
          drawLandmarks(frame, landmarks[biggestFace]);
        }

      }
 
      // Display results 
      imshow("Facial Landmark Detection", frame);
      // Exit loop if ESC is pressed
      if (waitKey(1) == 27) break;
       
    }
    return 0;
}