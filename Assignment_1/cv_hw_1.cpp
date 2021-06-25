#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"


// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Assignment 1"

//Global variables 
int No_LB_Clicked = 0;        
int No_RB_Clicked = 0;
int Coordnate_LB_Down[2];
int Coordinate_LB_Up[2];
int tools = 0;
std::string color[4] = {"EYEDROPER","CROP","PENCIL","PAINT BUCKET"};

int EYEDROPER[3] ={255, 255, 255}; //intializing eye dropper to 255, 255, 255

cv::Mat imageIn;
cv::Mat initilImage;

//Functions Prototypes. 
static void clickCallback(int event, int x, int y, int flags, void* userdata);

//Function to track mouse movements. 
static void clickCallback(int event, int x, int y, int flags, void* userdata)
{
    if(event == (cv::EVENT_LBUTTONDOWN ))
    {
        if (tools == 1)
        {
            
        }
        
        //std::cout << "LEFT CLICK (" << x << ", " << y << ")" << std::endl;
    }
    else if (event ==cv::EVENT_LBUTTONDBLCLK)
    {
        std::cout<<"LEFT DOUBLE CLICK "<<x <<" ," << y <<std::endl;
        imageIn = initilImage.clone();
    }
    else if(event == cv::EVENT_RBUTTONDOWN)
    {
        //In each right click it will select tools that will be used
        //in open CV
        std::cout << color[tools] <<" Selected" << std::endl;
        if (tools < 3)
        {
            tools++;
        }
        else
        {
            tools = 0;
        }
    }

    else if(event == cv::EVENT_MBUTTONDOWN)
    {
       // std::cout << "MIDDLE CLICK (" << x << ", " << y << ")" << std::endl;
    }
    else if(event == cv::EVENT_MOUSEMOVE)
    {
       // std::cout << "MOUSE OVER (" << x << ", " << y << ")" << std::endl;
    }
    else if (event == cv::EVENT_LBUTTONUP)
    {
       // std::cout<<"LEFT CLICK RELEASE (" << x << " ,"<< y << ")" <<std::endl;
    }
}

int main(int argc, char **argv)
{
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
        else
        {
            //making clone of original image to reset the changes that we make 
            initilImage = imageIn.clone();

            cv::imshow(DISPLAY_WINDOW_NAME, imageIn);
           

            //this will track movement of mouse when the image is open
            cv::setMouseCallback(DISPLAY_WINDOW_NAME, clickCallback, &imageIn);

            cv::waitKey();

        }
    }
}


