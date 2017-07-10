#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/rgb/image_raw", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  std::vector<cv::Point> points; //declare vector containing points
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat frame = cv::imread(cv_ptr->image);
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
   }

    void onMouse(int evt, int x, int y, int flags, void* param)
    {
		if(evt == CV_EVENT_LBUTTONDOWN) 
		{
		    std::vector<cv::Point>* ptPtr = (std::vector<cv::Point>*)param;
		    ptPtr->push_back(cv::Point(x,y));
		}
     }




	if (points.size() =1 ) //we have 2 points
	{
	    for (auto it = points.begin(); it != points.end(); ++it)
	    {

	        cout<<"X and Y coordinates are given below"<<endl;
	        cout<<(*it).x<<'\t'<<(*it).y<<endl; 
	    }
	    //draw points
	}
 

    // Update GUI Window
    cv::setMouseCallback(OPENCV_WINDOW,onMouse,(void*)&points);
    cv::imshow(OPENCV_WINDOW, frame);
    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};






int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}


