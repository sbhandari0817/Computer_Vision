/*
	Santosh Bhandari
	1001387116
	Assignment 3
*/



#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/common/time.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/segmentation/sac_segmentation.h>


#define NUM_COMMAND_ARGS 2

//Opens points cloud file 
// cloud out pointer to point cloud 
// takes file input and path to input file 
// Throw error if files are not supported

bool openCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloudOut, std::string fileName)
{
    // handle various file types
    std::string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);
    if(fileExtension.compare("pcd") == 0)
    {
        // attempt to open the file
        if(pcl::io::loadPCDFile<pcl::PointXYZRGBA>(fileName, *cloudOut) == -1)
        {
            PCL_ERROR("error while attempting to read pcd file: %s \n", fileName.c_str());
            return false;
        }
        else
        {
            return true;
        }
    }
    else if(fileExtension.compare("ply") == 0)
    {
        // attempt to open the file
        if(pcl::io::loadPLYFile<pcl::PointXYZRGBA>(fileName, *cloudOut) == -1)
        {
            PCL_ERROR("error while attempting to read pcl file: %s \n", fileName.c_str());
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        PCL_ERROR("error while attempting to read unsupported file: %s \n", fileName.c_str());
        return false;
    }
}



//  This functionn save the given point cloud
//  in PCD format
bool saveCloud(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloudIn, std::string fileName, bool binaryMode=true)
{
    // if the input cloud is empty, return
    if(cloudIn->points.size() == 0)
    {
        return false;
    }

    // attempt to save the file
    if(pcl::io::savePCDFile<pcl::PointXYZRGBA>(fileName, *cloudIn, binaryMode) == -1)
    {
        PCL_ERROR("error while attempting to save pcd file: %s \n", fileName);
        return false;
    }
    else
    {
        return true;
    }
}


//Finding the plane 
void segmentPlane(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloudIn, pcl::PointIndices::Ptr &inliers, double distanceThreshold, int maxIterations)
{
    // store the model coefficients
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);

    // Create the segmentation object for the planar model and set the parameters
    pcl::SACSegmentation<pcl::PointXYZRGBA> seg;
    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(maxIterations);
    seg.setDistanceThreshold(distanceThreshold);

    // Segment the largest planar component from the remaining cloud
    seg.setInputCloud(cloudIn);
    seg.segment(*inliers, *coefficients);
}


// This is the main function 
int main(int argc, char** argv)
{
    // validate and parse the command line arguments
    if(argc != NUM_COMMAND_ARGS + 1)
    {
        std::printf("USAGE: %s <file_name>\n", argv[0]);
        return 0;
    }
    std::string inputFilePath(argv[1]);
    std::string outputFilePath(argv[2]);

    // create a stop watch for measuring time
    pcl::StopWatch watch;

    // open the point cloud
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
    openCloud(cloud, inputFilePath);

    // start timing the processing step
    watch.reset();

    // segment a plane
    const float distanceThreshold = 0.0254;
    const int maxIterations = 5000;
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
    segmentPlane(cloud, inliers, distanceThreshold, maxIterations);
    std::cout << "Segmentation result: " << inliers->indices.size() << " points" << std::endl;
    
    // color the plane inliers green
    for(int i = 0; i < inliers->indices.size(); i++)
    {
        int index = inliers->indices.at(i);
        cloud->points.at(index).r = 0;
        cloud->points.at(index).g = 0;
        cloud->points.at(index).b = 255;
    }
 
    // get the elapsed time
    double elapsedTime = watch.getTimeSeconds();
    std::cout << elapsedTime << " seconds passed " << std::endl;

    // save the point cloud
    saveCloud(cloud, outputFilePath);

    // exit program
    return 0;
}

