/*
    Santosh Bhandari
    1001387116
    Assignment 1
*/

#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"


// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1
#define DISPLAY_WINDOW_NAME "Assignment 1"

//Global variables 
int init_cropCord[2];//Inital coordiante to crop
int final_cropCord[2]; //Final coordinate to crop 

int tools = 0;          //Navigate between toos starts from 1-6

//Diffferent toools that will be used. 
std::string color[5] = {"EYEDROPER","CROP","PENCIL","PAINT BUCKET","RESET"};

cv::Vec3b EYEDROPER = cv::Vec3b (255, 255, 255); //intializing eye dropper to 255, 255, 255

//Helps to use pencil when mouse is over and left bottton is pressesd.
//This boolean value function as a switch to use pencil. 
bool Pencil = false;

//Store the color value of pixel when implementing
//paint box. 
cv::Vec3b paintColor;

cv::Mat imageIn;     //Image that we read and work on 
cv::Mat initilImage; //Copy of original image to do reset. 

//Functions Prototypes. 
static void clickCallback(int event, int x, int y, int flags, void* userdata); //Mouse movenment tracker
void changeColor(int x, int y);        //Pencil color changer
void paintBox(int x, int y);           // for Paintbox tools 

//Function to change pixel color when pencil is selected
void changeColor(int x, int y)
{
    //Initalizing the pencil color to pixel 
    imageIn.at<cv::Vec3b>(y,x) = EYEDROPER;

    //Displaying change in the image
    cv::imshow(DISPLAY_WINDOW_NAME, imageIn);
    cv::waitKey();
}
void paintBox(int x, int y)
{
    //Checking  if the pixel is same as that we want to change
    if (paintColor == (imageIn.at<cv::Vec3b>(y,x)))
    {
        imageIn.at<cv::Vec3b>(y,x) = EYEDROPER;  //assigning value of eyedroper

        //Checking if there is any pixel on left and on right of that pixel
        if (x < imageIn.cols && x > 0)
        {
            paintBox (x + 1, y);
            paintBox (x -1, y);
        }
        //Checking if there is any pixel on up and on down of that pixel
        if (y < imageIn.rows && y > 0)
        {
            paintBox (x, y+1);
            paintBox (x, y-1);
        }
    }
    return;
}

//Function to track mouse movements. 
static void clickCallback(int event, int x, int y, int flags, void* userdata)
{
    if (event ==cv::EVENT_LBUTTONDBLCLK)
    {
        if (tools == 5)
        {
            imageIn = initilImage.clone();
            cv::imshow(DISPLAY_WINDOW_NAME, initilImage);
            cv::waitKey();
        }

    }
    else if(event == (cv::EVENT_LBUTTONDOWN ))
    {
        if (tools == 1)
        {
            cv::Vec3b BGR_Value = imageIn.at<cv::Vec3b>(y,x);
            EYEDROPER = BGR_Value;
            int val[3];
            for (int i= 0; i < 3; i++)
            {
                val[i] = BGR_Value.val[i];
            }
            std::cout<<"EYEDROPER Value = [ "<< val[0]<<", "<<val[1]<<", "<<val[2]<<"]"<<std::endl;

        }
        if (tools == 2 )
        {
            init_cropCord[0] = x;
            init_cropCord[1]  = y; 
        }
        if (tools == 3)
        {
            Pencil = true;
            changeColor(x,y);
        }
        if (tools == 4)
        {
            paintColor = imageIn.at<cv::Vec3b>(y,x);

            //If selected pixel has same value as eyebropper
            //then changing color is not necessary. 
            if (EYEDROPER != paintColor)
            {
                paintBox(x, y);
                cv::imshow(DISPLAY_WINDOW_NAME, imageIn);
                cv::waitKey();
            }
        }

    }
    
    else if(event == cv::EVENT_RBUTTONDOWN)
    {
        
        if (tools < 5)
        {
            tools++;
            //In each right click it will select tools that will be used
            //in open CV
            std::cout << color[tools-1] <<" Selected" << std::endl;
        }
        else
        {
            tools = 0;
            std::cout<< "No tools Selected " <<std::endl;
        }
        
    }

    else if(event == cv::EVENT_MOUSEMOVE)
    {
        if (Pencil)
        {
            changeColor(x, y);
        }
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
                imageIn = imageROI.clone();
                cv::imshow(DISPLAY_WINDOW_NAME, imageROI);
                cv::waitKey();

            }   
                
        }
        if (tools == 3)
        {
            Pencil = false;
        }
       
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


