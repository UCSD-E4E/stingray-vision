/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2012, Robert Bosch LLC.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Robert Bosch nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************/

//\Author Joshua Vasquez and Philip Roan, Robert Bosch LLC

// ROS headers
#include <ros/ros.h>
#include <ros/time.h>

#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>

#include <bmg160.hpp>
#include <sub20_interface.hpp>

// ROS message
#include <bmg160_driver/bmg160meas.h>

using namespace bosch_drivers_common;

int main( int argc, char **argv )
{  
  //ROS initialization and publisher/subscriber setup
  ros::init( argc, argv, "BMG160_node" );
  ros::NodeHandle nh;

  std::string hw_id;
  int number_of_bmg160_sensors;
  int publish_rate_Hz;

 
  // Get parameters from .launch file or parameter server, or take defaults
  nh.param<int>( "/bmg160_node/number_of_bmg160_sensors", number_of_bmg160_sensors, 1 );
  nh.param<int>( "/bmg160_node/publish_rate_Hz", publish_rate_Hz, 50 );
  nh.param<std::string>( "/bmg160_node/hardware_id", hw_id, "0208" );

  Sub20Interface sub20( hw_id ); // (Each sub20 is identified by its serial number.)
  std::vector<BMG160*> BMG160_Chain;

  // Intantiate the array of BMG160s
  for( int i = 0; i < number_of_bmg160_sensors; i++ )
  { 
    // instantiate a sensor and pass in a pointer to it's hw interface:
    BMG160* gyro_sensor = new BMG160( &sub20 ); 
  
    // Adjust sensor settings:
    gyro_sensor->setProtocol( SPI );
    gyro_sensor->setFrequency( 125000 );
    gyro_sensor->setPin( i );
  
    //gyro_sensor->setRange( BMG160Parameters::RANGE_125 );
    //gyro_sensor->setBandwidth( BMG160Parameters::BW_32HZ );
    //gyro_sensor->setFilter( false );
    // Initialize each gyro_sensor
    if( gyro_sensor->initialize() == false )
    {
      ROS_ERROR( "gyro_sensor initialization %d failed.", i );
      return 1;
    }
  
    // Calibrate each bmg160 in a static position:
    gyro_sensor->SimpleCalibration();      

    // Add the sensor to the sensor chain
    BMG160_Chain.push_back( gyro_sensor );
  }
 
  // Set up ROS message and publisher
  bmg160_driver::bmg160meas msg;
  ros::Publisher bmg160_pub = nh.advertise<bmg160_driver::bmg160meas>( "BMG160_data", 20 );   

  //set loop rate for measurement polling
  ros::Rate loop_rate_Hz( publish_rate_Hz ); 
    
  // Run sensor node:
  while( nh.ok() )
  {
    // remove old data on all vectors, so data doesn't accumulate:
    msg.GyroX.clear();
    msg.GyroY.clear();
    msg.GyroZ.clear();
    msg.GyroTemperature.clear();  

    for( int j = 0; j < number_of_bmg160_sensors; j++ )
    {
      // Read data from IMU and pass on Chip Select:

      if( BMG160_Chain[j]->takeMeasurement() == false )
      {
        ROS_ERROR( "Failure: gyro_sensor device %d", j );
        return 1;
      }
 
      msg.GyroX.push_back( BMG160_Chain[j]->GyroX_ );
      msg.GyroY.push_back( BMG160_Chain[j]->GyroY_ );
      msg.GyroZ.push_back( BMG160_Chain[j]->GyroZ_ );
  
      msg.GyroTemperature.push_back( BMG160_Chain[j]->Temperature_ );
    }
    msg.header.stamp = ros::Time::now();

    bmg160_pub.publish( msg ); 

    ros::spinOnce();
    loop_rate_Hz.sleep();
  }
  
  ROS_WARN( "Closing BMG160 driver." );
  return 0;
}
