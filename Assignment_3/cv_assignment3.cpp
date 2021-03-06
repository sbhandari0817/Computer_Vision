// include necessary dependencies
#include <iostream>
/*
    Santosh Bhandari
    1001387116
    Assignment 3
*/


#include <cstdio>
#include "opencv2/opencv.hpp"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1


int main(int argc, char **argv)
{
    // store video capture parameters
    std::string fileName;

    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <file_path> \n", argv[0]);
        return 0;
    }
    else
    {
        fileName = argv[1];
    }

    // open the video file
    cv::VideoCapture capture(fileName);
    if(!capture.isOpened())
    {
        std::printf("Unable to open video source, terminating program! \n");
        return 0;
    }

    // get the video source parameters
    int captureWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    int captureHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    int captureFPS = static_cast<int>(capture.get(cv::CAP_PROP_FPS));
    std::cout << "Video source opened successfully (width=" << captureWidth << " height=" << captureHeight << " fps=" << captureFPS << ")!" << std::endl;

    // create image window
    cv::namedWindow("captureFrame", cv::WINDOW_AUTOSIZE);
	
    //Traffic Counter;
    int east = 0;
    int west = 0;

	// set background filtering parameters
    const int bgHistory = 650;
    const float bgThreshold = 80;
    const bool bgShadowDetection = false;
    cv::Mat fgMask; //fg mask generated by MOG2 method
    cv::Ptr<cv::BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
    pMOG2 = cv::createBackgroundSubtractorMOG2(bgHistory, bgThreshold, bgShadowDetection);

    // process data until program termination
    bool doCapture = true;
    int frameCount = 0;
    while(doCapture)
    {
        // get the start time
        double startTicks = static_cast<double>(cv::getTickCount());

        std::cout<<"WESTBOUND COUNT: "<<west<<std::endl;
        std::cout<<"EASTBOUND COUNT: "<<east<<std::endl;
        // attempt to acquire and process an image frame
        cv::Mat captureFrame;
        cv::Mat grayFrame;
        //cv::Mat processedFrame;
        bool captureSuccess = capture.read(captureFrame);
        if(captureSuccess)
        {
			// pre-process the raw image frame
            const int rangeMin = 0;
            const int rangeMax = 255;
            cv::cvtColor(captureFrame, grayFrame, cv::COLOR_BGR2GRAY);
            cv::normalize(grayFrame, grayFrame, rangeMin, rangeMax, cv::NORM_MINMAX, CV_8UC1);

			// extract the foreground mask from image
			pMOG2->apply(grayFrame, fgMask);

            // erode and dilate the edges to remove noise
            int morphologySize = 4;
            cv::Mat edgesDilated;
            cv::dilate(fgMask, edgesDilated, cv::Mat(), cv::Point(-1, -1), morphologySize);
            cv::Mat edgesEroded;
            cv::erode(edgesDilated, edgesEroded, cv::Mat(), cv::Point(-1, -1), morphologySize);
            
            // locate the image contours (after applying a threshold or canny)
            std::vector<std::vector<cv::Point> > contours;
            //std::vector<int> hierarchy;
            cv::findContours(edgesEroded, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

            // draw the contours
            cv::Mat imageContours = cv::Mat::zeros(fgMask.size(), CV_8UC3);
            cv::RNG rand(12345);
            for(int i = 0; i < contours.size(); i++)
            {
                cv::Scalar color = cv::Scalar(rand.uniform(0, 256), rand.uniform(0,256), rand.uniform(0,256));
                cv::drawContours(imageContours, contours, i, color);
            }

            //cv::imshow ("Image Contours", imageContours);

            // compute minimum area bounding rectangles
            std::vector<cv::RotatedRect> minAreaRectangles(contours.size());
            for(int i = 0; i < contours.size(); i++)
            {
                // compute a minimum area bounding rectangle for the contour
                minAreaRectangles[i] = cv::minAreaRect(contours[i]);
            }

            // draw the rectangles
            cv::Mat imageRectangles = cv::Mat::zeros(fgMask.size(), CV_8UC3);
            for(int i = 0; i < contours.size(); i++)
            {
                cv::Scalar color = cv::Scalar(25, 240, 80);
                cv::Scalar color1 = cv::Scalar(25, 40, 240);
                cv::Point2f rectanglePoints[4];
                if (cv::contourArea(contours[i])> 8000)
                {
                    cv::Rect rectangle = minAreaRectangles[i].boundingRect();
                    
                    if (rectangle.y > 300)
                    {
                        if (rectangle.x > 1280 && rectangle.x < 1310)
                        {
                            east++;   
                        }
                        cv::rectangle(captureFrame,rectangle,color1,2);
                        
                    }
                    else
                    {
                        cv::rectangle(captureFrame,rectangle,color,2); 
                        if (rectangle.x > 1280 && rectangle.x < 1320)
                        {
                            west++;   
                        }
                    }
                    
                }
            }

            //cv::imshow("imageRectangles", imageRectangles);
            // increment the frame counter
            frameCount++;
        }
        else
        {
            std::printf("Unable to acquire image frame! \n");
            //Terminating program if no more frame is there
            doCapture = false;
        }

        // update the GUI window if necessary
        if(captureSuccess)
        {

            //Showng the image "Playing video by playing each frame"
            cv::imshow("captureFrame", captureFrame);
            // get the number of milliseconds per frame
            int delayMs = (1.0 / captureFPS) * 1000;

            // check for program termination
            if(((char) cv::waitKey(delayMs)) == 'q')
            {
                doCapture = false;
            }
        }

        // compute the frame processing time
        double endTicks = static_cast<double>(cv::getTickCount());
        double elapsedTime = (endTicks - startTicks) / cv::getTickFrequency();
       // std::cout << "Frame processing time: " << elapsedTime << std::endl;
    }

    // release program resources before returning
    capture.release();
    cv::destroyAllWindows();
}