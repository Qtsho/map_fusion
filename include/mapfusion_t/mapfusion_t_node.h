#include <tf/transform_listener.h>
#include <tf/message_filter.h>
#include <message_filters/subscriber.h>

#include "ros/ros.h"
#include <iterator>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"


class MapFusion
{
protected:
	ros::ServiceServer settings_srv;
	std::string dev;
	std::string frame;
	
public:
	MapFusion();
        void setScan_pub(ros::Publisher scan_pub);
        ros::Publisher getScan_pub() const;
        void mergelaser();
        sensor_msgs::LaserScan merged_Laser;
        int r=0;

private:
	unsigned int size_lidar = 0;
	unsigned int size_ts = 0;
        unsigned int total_size= 0;
	// **** ROS-related
	//sensor_msgs::LaserScan::Ptr merged_ranges;

	ros::NodeHandle nh;
  	ros::Subscriber sub;
  	ros::Subscriber sub_ts;
  	ros::Publisher scan_pub;
    // **** paramaters
    //std::vector<std::string> published_scan_topics_; // change this name

  	// **** member functions
 	void scanCallback (const sensor_msgs::LaserScanConstPtr& scan);
	void scanCallback2 (const sensor_msgs::LaserScanConstPtr& ts_scan);
	

};