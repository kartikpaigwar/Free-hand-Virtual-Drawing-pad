#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
    VideoCapture cap(0); //capture the video from webcam
    
   
    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("selectColour", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 int iLowH = 90;
 int iHighH = 138;

 int iLowS = 126; 
 int iHighS = 255;

 int iLowV = 60;
 int iHighV = 255;
 int m=0;
 int n=0;
 int o=255;
 
 int p=2;
 int q=0;

 //Create trackbars in "selectColour" window
 
 createTrackbar("draw", "selectColour", &p, 50); //Hue (0 - 179)
 
 createTrackbar("erase", "selectColour", &q, 100); //Hue (0 - 179)
 
 
 
 createTrackbar("Blue", "selectColour", &m, 255); //Hue (0 - 179)


 createTrackbar("Green", "selectColour", &n, 255); //Saturation (0 - 255)

 createTrackbar("Red", "selectColour", &o, 255);//Value (0 - 255)


 int iLastX = -1; 
 int iLastY = -1;

 //Capture a temporary image from the camera
 Mat imgTmp;
 
 cap.read(imgTmp);
// resize(imgTmp, imgTmp, Size(1800, 900), 0, 0, INTER_CUBIC);


 //Create a black image with the size as the camera output
 Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
 //resize(imgLines, imgLines, Size(1800, 900), 0, 0, INTER_CUBIC);

    while (true)
    {   
        Mat im = Mat::zeros( imgTmp.size(), CV_8UC3 );;
        //resize(im, im, Size(1800,900 ), 0, 0, INTER_CUBIC);
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
        //resize(imgOriginal, imgOriginal, Size(1800, 900), 0, 0, INTER_CUBIC);




         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

  Mat imgHSV;

  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
  Mat imgThresholded;

  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (removes small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  //morphological closing (removes small holes from the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  //Calculate the moments of the thresholded image
  Moments oMoments = moments(imgThresholded);

  double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;
   
   int posX = dM10 / dArea;
   int posY = dM01 / dArea;
  
  
  
  
  circle( im, Point(posX, posY), p, Scalar( 255, 255, 255 ), p, 8 );
       

  // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  if (dArea > 4000000 )
  {
   //calculate the position of the ball
   
   if(p==0)
  { circle( im, Point(posX, posY), 8, Scalar( 255, 255, 255 ),8, 8 );
  }
  else
  { circle( im, Point(posX, posY), p, Scalar( m,n,o), p, 8 );
  }
 
   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
   {
    //Draw a red line from the previous point to the current point
    
    if(p==0)
    {
     line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,0),q);
    }
    else
    {
    line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(m,n,o), p);
    }
   }

  }
   iLastX = posX;
   iLastY = posY;
  Mat thre;
  //flip(imgThresholded,thre,1);
  //imshow("Thresholded Image", thre); //show the thresholded image
  Mat pic=imgOriginal;
   Mat pics;
   flip(pic,pics,1);
  imshow("cam",pics); //show the original image
  
   
  imgOriginal = imgOriginal + imgLines;
  im = im + imgLines;
  Mat Lines;
  flip(im,Lines,1);
  imshow("Original",Lines); //show the original image
  
  
  
  if(waitKey(50)=='e')
  {
   cout << "esc key is pressed by user" << endl;
  
   imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;

   continue;
  
  }
  
  

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }

   return 0;
}
