#include "ros/ros.h"
#include "tf/transform_listener.h"
#include "sensor_msgs/PointCloud.h"
#include "tf/message_filter.h"
#include "message_filters/subscriber.h"
#include "laser_geometry/laser_geometry.h"

struct Point {
	float x;
	float y;
	float z;
	float v;
	ros::Time stamp;
};



class LaserScanToPointCloud{

public:

  ros::NodeHandle n_;
  laser_geometry::LaserProjection projector_;
  tf::TransformListener listener_;
  message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub_;
  tf::MessageFilter<sensor_msgs::LaserScan> laser_notifier_;

 

  ros::Publisher scan_pub_;
  //tf::message_filters::Subscriber<sensor_msgs::LaserScan> laser_sub_(n_, "scan", 10);
  //tf::message_filters::Subscriber<sensor_msgs::PointCloud2> us_sub(n_, "ts_alpha/pointcloud", 1);
  //pcl::concatenatePointCloud mergepcl;

  LaserScanToPointCloud(ros::NodeHandle n) : 


    n_(n),
    laser_sub_(n_, "scan", 10),
    laser_notifier_(laser_sub_,listener_, "base_link", 10)
  {
    laser_notifier_.registerCallback(boost::bind(&LaserScanToPointCloud::scanCallback, this, _1));
    laser_notifier_.setTolerance(ros::Duration(0.01));
    scan_pub_ = n_.advertise<sensor_msgs::PointCloud2>("/my_cloud",1);
  }

  void scanCallback (const sensor_msgs::LaserScan::ConstPtr& scan_in)
  {
    sensor_msgs::PointCloud2 cloud2;

    try
    {
        projector_.transformLaserScanToPointCloud(
          "base_link",*scan_in, cloud2,listener_);
    }
    catch (tf::TransformException& e)
    {
        std::cout << e.what();
        return;
    }
    
    // Do something with cloud.

    scan_pub_.publish(cloud2);

  }
}; //endclass


class MapHandler
{
protected:
	ros::NodeHandle nh;
	ros::Publisher pcl_pub;

public:
	MapHandler();
};