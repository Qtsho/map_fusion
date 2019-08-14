#include "mapfusion_t/mapfusion_t_node.h"
// lidar_pointer = boost::make_shared<sensor_msgs::LaserScan>();
// ts_pointer = boost::make_shared<sensor_msgs::LaserScan>();

MapFusion::MapFusion ()
{
	  sub = nh.subscribe<sensor_msgs::LaserScan>("/scan", 1,
		  &MapFusion::scanCallback, this);
	  
	  sub_ts = nh.subscribe<sensor_msgs::LaserScan>("/scan_ts", 1, 
		  &MapFusion::scanCallback2, this);
	  scan_pub = nh.advertise<sensor_msgs::LaserScan>("/merged_scan", 10);
}

void MapFusion::setScan_pub(ros::Publisher scan_pub)
{
	  this->scan_pub = scan_pub;
}

ros::Publisher MapFusion::getScan_pub() const
{
	  return scan_pub;
}



void MapFusion::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	//merged_ranges = boost::make_shared<sensor_msgs::LaserScan>();  
	//scan is a pointer to  object sensor messege
	size_lidar = scan->ranges.size(); //361 data for lidar 0-360
	total_size += size_lidar;
	//ROS_INFO("[lidar: %d]", size_lidar);

//	merged_Laser.header = scan->header;
//	merged_ranges->range_min = scan->range_min;
//	merged_ranges->range_max = scan->range_max;
//	merged_ranges->angle_increment = scan->angle_increment;
//	merged_ranges->time_increment = scan->time_increment;
//	merged_ranges->scan_time = scan->scan_time;
//	merged_ranges->header.frame_id = "merged_ranges";
//	merged_ranges->angle_min = scan-> angle_min;
//	merged_ranges-> angle_max = scan-> angle_max;
	
	merged_Laser.header = scan->header;
	merged_Laser.range_min = scan->range_min;
	merged_Laser.range_max = scan->range_max;
	merged_Laser.angle_increment = scan->angle_increment;
	merged_Laser.time_increment = scan->time_increment;
	merged_Laser.scan_time = scan->scan_time;
	merged_Laser.header.frame_id = "merged_ranges";
	merged_Laser.angle_min = scan-> angle_min;
	merged_Laser. angle_max = scan-> angle_max;
	/*TODO: Use vector for dynamic array in c++ or use resize() array*/
	merged_Laser.ranges = scan-> ranges; 

	
}

void MapFusion::scanCallback2(const sensor_msgs::LaserScan::ConstPtr& ts_scan)
{
	  r = 0;
	  size_ts = ts_scan -> ranges.size();
	  total_size += size_ts;
	  //merged_ranges = boost::make_shared<sensor_msgs::LaserScan>();  
	  ROS_INFO("[ts: %d]", size_ts);
	  r+= size_ts;
	  merged_Laser.ranges.resize(total_size);
	  memcpy(&merged_Laser.ranges[size_lidar], &ts_scan->ranges[r], size_ts*sizeof(float)); 
	  
}

void MapFusion::mergelaser()
{
	 ROS_INFO("[lidar:%d ts: %d total: %d]", this->size_lidar,size_ts,total_size);
	 scan_pub.publish(merged_Laser);
	 total_size = 0;
}
int main(int argc, char **argv)
{  /**
	   * The ros::init() function needs to see argc and argv so that it can perform
	   * any ROS arguments and name remapping that were provided at the command line.
	   * For programmatic remappings you can use a different version of init() which takes
	   * remappings directly, but for most command-line programs, passing argc and argv is
	   * the easiest way to do it.  The third argument to init() is the name of the node.
	   *
	   * You must call one of the versions of ros::init() before using any other
	   * part of the ROS system.
	   */

	  ros::init(argc, argv, "mapFusion");

	  MapFusion mf_object;
	  
	  ros::Rate r(10); // 10 hz
	  while (ros::ok())
	  {	 
		    ros::spinOnce(); //execute all the callbacks
		    mf_object.mergelaser();
		    r.sleep();
	  }
	
	  
	  return 0;
}