
#include <ros.h>
#include <std_msgs/Float32.h>
#include <robot_msgs/Motor.h>
#include "PID.h"
#include <TimerOne.h>
#define LOOP_TIME        200000 


void messageCb( const robot_msgs::Motor& msg);

ros::NodeHandle  nh;
robot_msgs::Motor sp_msg;

ros::Publisher pub("/Motor/speed", &sp_msg);
ros::Subscriber<robot_msgs::Motor> sub("/Motor/speed_set", &messageCb );

float speed_1 = 0;
float speed_2 = 0;

void messageCb(const robot_msgs::Motor& msg){
  speed_1 = msg.left_motor.data;
  speed_2 = msg.right_motor.data;
}




void timerIsr(){
  Timer1.detachInterrupt();  //stop the timer
  M1RPM();
  M2RPM();  
  sp_msg.right_motor.data = m1_speed;
  sp_msg.left_motor.data = m2_speed;
  pub.publish(&sp_msg);
  Timer1.attachInterrupt(timerIsr);  //enable the timer
}




void setup() {
  
  Timer1.initialize(LOOP_TIME);
    
  Timer1.attachInterrupt(timerIsr); // enable the timer
  setting();
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub);
  
}



void loop() {


  
  nh.spinOnce();

  M1vel_PID(speed_1);
  M2vel_PID(speed_2);

}
