#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#ifndef _renderFace_H_
#define _renderFace_H_

using namespace cv; 
using namespace std; 

#define COLOR Scalar(255, 200,0)

// drawPolyLine draws a poly line by joining 
// successive points between the start and end indices. 
void drawPolyline
(
  Mat &im,
  const vector<Point2f> &landmarks,
  const int start,
  const int end,
  bool isClosed = false
)
{
    // Gather all points between the start and end indices
    vector <Point> points;
    for (int i = start; i <= end; i++)
    {
        points.push_back(cv::Point(landmarks[i].x, landmarks[i].y));
    }
    // Draw polylines. 
    polylines(im, points, isClosed, COLOR, 2, 16);
    
}

//detect headtilt
void tiltRatio
(
  const vector<Point2f> &landmarks,
  const int jawStart,
  const int jawEnd,
  const int lipStart,
  const int lipEnd
)
{
  // float jawX = 0.0;

  // for(int i = jawStart; i <= jawEnd; i++)
  // {
  //   jawX = jawX + landmarks[i].x;
  // }

  // jawX = jawX / (jawEnd - jawStart);

  // float lipX = 0.0;

  // for(int i = lipStart; i <= lipEnd; i++)
  // {
  //   lipX = lipX + landmarks[i].x;
  // }

  // lipX = lipX / (lipEnd - lipStart);

  // if(lipX < jawX)
  // {
  //   std::cout<< "left" << endl;
  // } else  {
  //   std::cout<< "right" << endl;
  // }

  float minJ, maxJ, minL, maxL;
  float midJ, midL;
  float angle;

  minJ = landmarks[jawStart].x;
  maxJ = landmarks[jawStart].x;

  for(int i = jawStart; i <= jawEnd; i++) {
    if(landmarks[i].x < minJ) {
      minJ = landmarks[i].x;
    }

    if(landmarks[i].x > maxJ) {
      maxJ = landmarks[i].x;
    }
  }

  midJ = (minJ + maxJ)/2.0;

  minL = landmarks[lipStart].x;
  maxL = landmarks[lipStart].x;

  for(int i = lipStart; i <= lipEnd; i++) {
    if(landmarks[i].x < minL) {
      minL = landmarks[i].x;
    }

    if(landmarks[i].x > maxL) {
      maxL = landmarks[i].x;
    }
  }

  midL = (minL + maxL)/2.0;



  angle = (midJ - midL) / ((minJ - maxJ)/2);
  angle = std::acos(angle);
  angle = angle * (360/M_PI);

  std::cout << "angle: " + to_string(angle) << endl;


}


void drawLandmarks(Mat &im, vector<Point2f> &landmarks)
{
    // Draw face for the 68-point model.
    if (landmarks.size() == 68)
    {
      drawPolyline(im, landmarks, 0, 16);           // Jaw line
      drawPolyline(im, landmarks, 17, 21);          // Left eyebrow
      drawPolyline(im, landmarks, 22, 26);          // Right eyebrow
      drawPolyline(im, landmarks, 27, 30);          // Nose bridge
      drawPolyline(im, landmarks, 30, 35, true);    // Lower nose
      drawPolyline(im, landmarks, 36, 41, true);    // Left eye
      drawPolyline(im, landmarks, 42, 47, true);    // Right Eye
      drawPolyline(im, landmarks, 48, 59, true);    // Outer lip
      drawPolyline(im, landmarks, 60, 67, true);    // Inner lip

      tiltRatio(landmarks, 0, 16, 48, 59);
    }
    else 
    { // If the number of points is not 68, we do not know which 
      // points correspond to which facial features. So, we draw 
      // one dot per landamrk. 
      for(int i = 0; i < landmarks.size(); i++)
      {
        circle(im,landmarks[i],3, COLOR, FILLED);
      }
    }
    
}

#endif // _renderFace_H_