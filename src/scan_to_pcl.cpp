#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>


class Laser2Pcl{
  public:
      Laser2Pcl(); // Constructor
      void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
  private:
        ros::NodeHandle node_;
        laser_geometry::LaserProjection projector_;
        tf::TransformListener tfListener_;

        ros::Publisher point_cloud_publisher_;
        ros::Subscriber scan_sub_;


};
//Constructor of my filter
Laser2Pcl::Laser2Pcl(){
        scan_sub_ = node_.subscribe<sensor_msgs::LaserScan> ("/scan", 100, &Laser2Pcl::scanCallback, this); //subcribe to a scan and go to call back
        point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("pointcloud", 100, false);          // create a point cloud publisher with /cloud topic
        tfListener_.setExtrapolationLimit(ros::Duration(0.1));                                              // setup tf
}     

void Laser2Pcl::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
    sensor_msgs::PointCloud2 cloud;                                                                         // create a pcl2 msg
    projector_.transformLaserScanToPointCloud("laser", *scan, cloud, tfListener_);                      //transform to pcl using laser geometry class, 'base_link' fix where see all the scan in
    point_cloud_publisher_.publish(cloud);                                                                  //publish pcl
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "Laser2Pcl");

    Laser2Pcl filter;

    ros::spin();

    return 0;
}