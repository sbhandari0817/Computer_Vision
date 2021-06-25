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
int init_cropCord[2];
int final_cropCord[2];

cv::Mat imageIn;
cv::Mat initilImage;

//Functions Prototypes. 
static void clickCallback(int event, int x, int y, int flags, void* userdata);

//Function to track mouse movements. 
static void clickCallback(int event, int x, int y, int flags, void* userdata)
{
    if (event ==cv::EVENT_LBUTTONDBLCLK)
    {
        std::cout<<"LEFT DOUBLE CLICK "<<x <<" ," << y <<std::endl;
        cv::imshow(DISPLAY_WINDOW_NAME, initilImage);
        cv::waitKey();

    }
    else if(event == (cv::EVENT_LBUTTONDOWN ))
    {
        if (tools == 1)
        {
            cv::Vec3b BGR_Value = imageIn.at<cv::Vec3b>(y,x);
            for (int i = 0; i < 3; i++)
            {
                 EYEDROPER[i] = BGR_Value.val[i];
                 
            }
            std::cout<<"EYEDROPER Value = [ "<< EYEDROPER[0]<<", "<<EYEDROPER[1]<<", "<<EYEDROPER[2]<<"]"<<std::endl;

        }
        if (tools == 2 )
        {
            init_cropCord[0] = x;
            init_cropCord[1]  = y; 
        }

        
        //std::cout << "LEFT CLICK (" << x << ", " << y << ")" << std::endl;
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
        if (tools == 2)
        {
            final_cropCord[0] = x;
            final_cropCord[1] = y;

            cv::Point p1(init_cropCord[0],init_cropCord[1]);
            cv::Point p2(final_cropCord[0],final_cropCord[1]);
            cv::Rect region(p1, p2); 
            cv::Mat imageROI = imageIn(region);

            //if reactangle is not formed then imshow throw an error
            //To avoid that using if statment . 
            if (!region.empty())
            {
                cv::imshow(DISPLAY_WINDOW_NAME, imageROI);
                cv::waitKey();
            }   
                
        }
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


