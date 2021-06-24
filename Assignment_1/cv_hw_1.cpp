#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

//Functions Prototypes. 
static void clickCallback(int event, int x, int y, int flags, void* userdata);

//Function to track mouse movements. 
static void clickCallback(int event, int x, int y, int flags, void* userdata)
{
    if(event == (cv::EVENT_LBUTTONDOWN ))
    {
        
        std::cout << "LEFT CLICK (" << x << ", " << y << ")" << std::endl;
    }
    else if(event == cv::EVENT_RBUTTONDOWN)
    {
        std::cout << "RIGHT CLICK (" << x << ", " << y << ")" << std::endl;
    }
    else if(event == cv::EVENT_MBUTTONDOWN)
    {
        std::cout << "MIDDLE CLICK (" << x << ", " << y << ")" << std::endl;
    }
    else if(event == cv::EVENT_MOUSEMOVE)
    {
        std::cout << "MOUSE OVER (" << x << ", " << y << ")" << std::endl;
    }
    else if (event == cv::EVENT_LBUTTONUP)
    {
        std::cout<<"LEFT CLICK RELEASE (" << x << " ,"<< y << ")" <<std::endl;
    }
}



// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Assignment 1"

int main(int argc, char **argv)
{
    //intializing eye dropper to 255, 255, 255

    int EYEDROPER[3] ={255, 255, 255}; 
    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <image_path> \n", argv[0]);
        return 0;
    }
    else
    {
        cv::Mat imageIn;
        imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);
        
        // check for file error
        if(!imageIn.data)
        {
            std::cout << "Error while opening file " << argv[1] << std::endl;
            return 0;
        }
        else
        {
            cv::imshow(DISPLAY_WINDOW_NAME, imageIn);
           

            //this will track movement of mouse when the image is open
            cv::setMouseCallback(DISPLAY_WINDOW_NAME, clickCallback, &imageIn);

            cv::waitKey();

        }
    }
}


