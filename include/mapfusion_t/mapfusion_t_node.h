#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sstream>
#include <stdlib.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/point_cloud_conversion.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include <tf/transform_listener.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/message_filter.h>
#include <message_filters/subscriber.h>




class MapFusion
{
protected:
	ros::NodeHandle nh;
	ros::ServiceServer settings_srv;
	std::string dev;
	std::string frame;
	int num_readings ;
  	double laser_frequency;
  	ros::Publisher scan_pub;
public:
	MapFusion();
	void mergingScanner();
	void publishingScan();	

private:
};