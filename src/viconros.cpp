#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "viconros/viconmocap.h"
#include <sstream>
#include <string.h>
#include "CFetchViconData.h"
int main(int argc, char **argv) {
	ros::init(argc, argv, "viconros");
	ros::NodeHandle n("~");
	std::string ip;
	std::string model;
	std::string segment;
	n.getParam("host",ip);
	n.getParam("model",model);
	n.getParam("segment",segment);
	ROS_INFO("HOST:%s",ip.c_str());
	ROS_INFO("MODEL:%s; SEGMENT:%s",model.c_str(),segment.c_str());
	ros::Publisher vicon_pub = n.advertise<viconros::viconmocap>("vicon", 1000);
	ros::Rate loop_rate(100);
	int count = 0;
	CFetchViconData * vicon=new CFetchViconData();
	const char * host=ip.c_str();
	ObjStatus objs;
	
	if(!(vicon->IsConnected))
    { 
            ROS_INFO("Connecting to %s",host);
            bool res=vicon->Connect(host);
            if(res==false)
            {
                ROS_INFO("Failed to connect!\r\n");
                    return 0;
            }
            else
            {
                ROS_INFO("Successfully connected!\r\n");
            }

    }
	while (ros::ok()) {
		viconros::viconmocap msg;
		objs=vicon->GetStatus(model.c_str(),segment.c_str());
		msg.time=objs.tm;
		msg.position.x =objs.pos[0];
		msg.position.y =objs.pos[1];
		msg.position.z =objs.pos[2];
		msg.velocity.x =objs.vel[0];
		msg.velocity.y =objs.vel[1];
		msg.velocity.z =objs.vel[2];
		ROS_INFO("position:%f-%f-%f; velocity: %f-%f-%f", msg.position.x,msg.position.y,msg.position.z,msg.velocity.x,msg.velocity.y,msg.velocity.z);
		vicon_pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
		++count;
	}
	return 0;
}