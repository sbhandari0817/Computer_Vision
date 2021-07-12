/*
    Santosh Bhandari
    1001387116
    Assignment 2 
*/

#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1


int main(int argc, char **argv)
{
    int no_penny = 0;
    int no_dime = 0;
    int no_nickle = 0;
    int no_quarter = 0;

    std::vector<double>area;
    cv::Mat imageIn;

    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <image_path> \n", argv[0]);
        return 0;
    }
    else
    {
        imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);

        // check for file error
        if(!imageIn.data)
        {
            std::cout << "Error while opening file " << argv[1] << std::endl;
            return 0;
        }
    }

    //Resizing image
    cv::Mat imageResized;
    cv::resize(imageIn, imageResized, cv::Size(imageIn.cols/4,imageIn.rows/5));
    imageIn = imageResized.clone();

    // convert the image to grayscale
    cv::Mat imageGray;
    cv::cvtColor(imageIn, imageGray, cv::COLOR_BGR2GRAY);

    // find the image edges
    cv::Mat imageEdges;
    const double cannyThreshold1 = 100;
    const double cannyThreshold2 = 200;
    const int cannyAperture = 3;
    cv::Canny(imageGray, imageEdges, cannyThreshold1, cannyThreshold2, cannyAperture);
    
    // erode and dilate the edges to remove noise
    int morphologySize = 1;
    cv::Mat edgesDilated;
    cv::dilate(imageEdges, edgesDilated, cv::Mat(), cv::Point(-1, -1), morphologySize);
    cv::Mat edgesEroded;
    cv::erode(edgesDilated, edgesEroded, cv::Mat(), cv::Point(-1, -1), morphologySize);
    
    // locate the image contours (after applying a threshold or canny)
    std::vector<std::vector<cv::Point> > contours;
    //std::vector<int> hierarchy;
    cv::findContours(edgesEroded, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // draw the contours
    cv::Mat imageContours = cv::Mat::zeros(imageEdges.size(), CV_8UC3);
    cv::RNG rand(12345);
    for(int i = 0; i < contours.size(); i++)
    {
        cv::Scalar color = cv::Scalar(rand.uniform(0, 256), rand.uniform(0,256), rand.uniform(0,256));
        cv::drawContours(imageContours, contours, i, color);
    }

    // fit ellipses to contours containing sufficient inliers
    std::vector<cv::RotatedRect> fittedEllipses(contours.size());
    for(int i = 0; i < contours.size(); i++)
    {
        // compute an ellipse only if the contour has more than 5 points (the minimum for ellipse fitting)
        if(contours.at(i).size() > 5)
        {
            fittedEllipses[i] = cv::fitEllipse(contours[i]);
            //Calculating area of the ellipse
            area.push_back(cv::contourArea(contours[i]));
        }
    }

    //Classifying coins according to size. 
    for (auto &value: area)
    {
        if (value > 19690)
        {
            no_quarter++;
        }
        else if(value > 15200)
        {
            no_nickle++;
        }
        else if (value > 12000)
        {
            no_penny++;
        }
        else if (value > 10500)
        {
            no_dime++;
        }
    }

    //Calculating total monetary value and printing
    double total;
    total = (0.01 *no_penny )+ (0.05*no_nickle)+(0.10*no_dime)+(0.25*no_quarter);
    //printing values
    std::cout<<"Penny - "<<no_penny<<std::endl;
    std::cout<<"Nickel - "<<no_nickle<<std::endl;
    std::cout<<"Dime - "<<no_dime<<std::endl;
    std::cout<<"Quarter - "<<no_quarter<<std::endl;
    std::cout<<"Total - $"<<total<<std::endl;

    // draw the ellipses around coin
    const int minEllipseInliers = 50;
    for(int i = 0; i < contours.size(); i++)
    {
        // draw any ellipse with sufficient inliers
        if(contours.at(i).size() > minEllipseInliers)
        {
            cv::Scalar color = cv::Scalar(rand.uniform(0, 256), rand.uniform(0,256), rand.uniform(0,256));
            cv::ellipse(imageIn, fittedEllipses[i], color, 2);
        }
    }

    // display the images
    cv::imshow("imageIn", imageIn);
    cv::waitKey();
}
