#include "mapfusion_t/mapfusion_t_node.h"

//TODO: use tf transform will only transform the ts_scan after this node merge tgogether, therefore, we  need to transform before merge together. (try to run this node 
//after the launch file)(launch file will start everything parralelly)

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

	//ROS_INFO("[lidar: %d]", size_lidar);

	  
	size_lidar = scan->ranges.size(); //361 data for lidar 0-360 = 
	// (*scan).size() : dereferance the pointer
	total_size += size_lidar;
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
/**
 *Callback function for Ultrasound messages
 *  
 */
// TODO: Inspect the marker message to see if it can use to directly merge to 
//laser message ( creat array, resize array, copy array (memcpy))
void MapFusion::scanCallback2(const sensor_msgs::LaserScan::ConstPtr& ts_scan)
{	  r = 0;
	  size_ts = ts_scan -> ranges.size();
	  total_size += size_ts;
	  //merged_ranges = boost::make_shared<sensor_msgs::LaserScan>();  
	  ROS_INFO("[ts: %d]", size_ts);
	  r+= size_ts;
	  merged_Laser.ranges.resize(total_size);
	  memcpy(&merged_Laser.ranges[size_lidar], &(ts_scan->ranges[0]), size_ts*sizeof(float)); // copy the pointer of tsscan to the last scan array
	  
}
/**
 *Publishing merged laser scanner
 *  
 */
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

	  ros::init(argc, argv, "mapFusion"); //name of the node 

	  MapFusion mf_object; // create fushion object
	  
	  ros::Rate r(5); // 10 hz // Problem: skip ts_scan and publish only lidar scan
	  while (ros::ok())
	  {	 
		    ros::spinOnce(); //execute all the callbacks
		    mf_object.mergelaser(); // publish the merge laser
		    r.sleep();
	  }
	
	  
	  return 0;
}