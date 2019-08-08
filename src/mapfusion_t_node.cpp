#include "mapfusion_t/mapfusion_t_node.h"

/*
This node to subcribe the laser scan(/laser_scan), the Ultrasound scan(/us_scan) without the z axis and merge 2 scan together and publish (/scan)

mapping algorithm will subcibe to /scan for making a map without knowing that the scan comes from 2 different source. 
*/


 //Default Constructor
MapFusion::MapFusion()
{
 //ros::init (argc, argv, "scan_listener")
 ros::NodeHandle n; //ROS node handle
 ros::Subscriber sub_laser = n.subscribe ("laser_scan",1000, mergingScanner); // subcribe laserscan
 ros::Subscriber sub_pcl  = n.subscribe ("us_scan",1000) ;// subcribe pcl scan
 scan_pub  = n.advertise<sensor_msgs::LaserScan>("scan",50); // Laserscan publisher

}


/*Call back whenever a new message arrives*/
void MapFusion::publishingScan()
{
    int count = 0;
    num_readings = 0; //figure out what is this number
    laser_frequency = 40; //figure out what is this number
    double ranges[num_readings];
    double intensities[num_readings]; 
       //generate some fake data for our laser scan 
    //(this would be the result after the MergingScanner() 
    for(unsigned int i = 0; i < num_readings; ++i){
      ranges[i] = count;
      intensities[i] = 100 + count;
    }

    // initialize the laser data
    sensor_msgs::LaserScan scan;
    scan.header.stamp = ros::Time::now();
    scan.header.frame_id ="merged_laser_frame";
    scan.angle_min = -1.57;
    scan.angle_max = 1.57;
    scan.angle_increment = 3.14 / num_readings;
    scan.time_increment = (1 / laser_frequency) / (num_readings);
    scan.range_min = 0.0;
    scan.range_max = 100.0;
    scan.ranges.resize(num_readings);
    scan.intensities.resize(num_readings);
    for(unsigned int i = 0; i < num_readings; ++i){
      scan.ranges[i] = ranges[i];
      scan.intensities[i] = intensities[i];
    }

    scan_pub.publish(scan);
    ++count;
    

}
void MapFusion::mergingScanner()//onst sensor_msgs::LaserScan::ConstPtr& scan_in) 
{

  /*
  Algorithm goes here
  #TODO: read 2 scan and merge it together to the messege scan
  */

  num_readings+=1; // add the point to messege
  //ROS_INFO();

  //ROS_INFO("I heard: [%s]", msg->data.c_str());
}




int main(int argc, char **argv)
{
  MapFusion MapFusion;
/* initialize the node with mapmerge_listener name*/
  ros::Rate loop_rate(1.0);
  while (ros::ok()) {
    MapFusion.publishingScan();
    ros::spinOnce();
    loop_rate.sleep();
  }


  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */

  return 0;
}