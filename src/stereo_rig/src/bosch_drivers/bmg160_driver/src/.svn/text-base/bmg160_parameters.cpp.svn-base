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

/*********************************************************************
 *
 * Disclaimer
 *
 * This source code is not officially released or supported by Bosch Sensortec.
 * Please contact the listed authors with bugs or other modifications.
 * If you would like the official Bosch Sensortec drivers, please contact:
 * contact@bosch-sensortec.com
 *
 *********************************************************************/

//\Author Joshua Vasquez and Philip Roan, Robert Bosch LLC

#include "bmg160_driver/bmg160_parameters.hpp"


/**********************************************************************/
/**********************************************************************/
BMG160Parameters::BMG160Parameters()
{
  /** 
   * \brief I2C Settings: (the default configuration) 
   */
  this->setFrequency( 400000 );
  this->setProtocol( I2C );
  this->setSlaveAddressBit( 0 );
 
  /**
   * \brief SPI Settings: (relevant only if the user selects SPI)
   *
   * \a flags_ must be first cleared to zero, because the next
   *     two settings directly manipulate bits in flags_.
   * \note The user should not have to change these next two settings: 
   */
  flags_ = 0;
  this->setSpiMode( SPI_MODE_0 );
  this->setByteOrder( MSB_FIRST ); 
  this->setPin( 10 );

  /** 
   * \brief Default sensor settings:
   *
   * These settings correspond to the sensor's settings after every
   *     power-on or softReset().
   * These settings have been deduced from the datasheet default
   *     register values.
   */
  this->setRange( RANGE_2000 );
  this->setBandwidth( BW_47HZ );
  /** units: [g/LSB] // corresponds to RANGE_2 */
 
  /**
   * \brief These next settings will write values to the sensor's 
   *     registers upon calling initialize() from the bmg160 driver:
   */
  this->setFilter( true ); 
  this->setRange( RANGE_2000 );
  this->setBandwidth( BW_UNFILTERED );
}


/**********************************************************************/
BMG160Parameters::~BMG160Parameters() 
{
}


/**********************************************************************/
double BMG160Parameters::getSensitivity()
{
  return sensitivity_;
}



/**********************************************************************/
bool BMG160Parameters::setProtocol( interface_protocol protocol )
{
  switch( protocol )
  {
  case I2C:
    protocol_ = protocol;
    break;
  case SPI:
    protocol_ = protocol;
    break;
  default:
    ROS_ERROR("BMG160Parameters:Unsupported protocol.");
    return false;
  }
  return true;
}


/**********************************************************************/
bool BMG160Parameters::setFrequency( int frequency )
{
  frequency_ = frequency;
  return true;
}



/**********************************************************************/
interface_protocol BMG160Parameters::getProtocol()
{
  return protocol_;
}


/**********************************************************************/
int BMG160Parameters::getFrequency()
{
  return frequency_;
}


/**********************************************************************/
bool BMG160Parameters::setPin( uint8_t pin )
{
  // adjust pin to desired value:
  pin_ = pin;
  // adjust the current flags to include the pin value:
  flags_ = ((0x0F)&flags_) | (pin_ << 4); 

  return true;
}


/**********************************************************************/
void BMG160Parameters::setSlaveAddressBit( bool high_or_low )
{
  slave_address_bit_ = high_or_low;
}


/**********************************************************************/
int BMG160Parameters::getPin()  
{
  return pin_;
}


/**********************************************************************/
bool BMG160Parameters::getSlaveAddressBit()
{
  return slave_address_bit_;
}



/**********************************************************************/
int* BMG160Parameters::getFlags()
{
  return &flags_;
}


/**********************************************************************/
bool BMG160Parameters::setSpiMode( uint8_t mode )
{
  switch( mode )
  {
  case SPI_MODE_3:
    break;
  case SPI_MODE_0:
    break;
  case SPI_MODE_1: {}
  case SPI_MODE_2: {}
  default:
    ROS_ERROR("BMG160Parameters: SPI_MODE can only be one of four choices");
    return false;
  }

  // adjust the flags
  flags_ = ((0xFC & flags_) | (mode) );
  return true;
}


/**********************************************************************/
bool BMG160Parameters::setByteOrder( uint8_t value )
{
  if( value > 1 )
  {
    ROS_ERROR("BMG160Parameters: Byte order must be either LSBFIRST or MSBFIRST");
    return false;
  }
  // adjust the flags
  flags_ = ((0xFB & flags_) | (value << 2));
  return true;
}


/**********************************************************************/
void BMG160Parameters::setRange( range new_range )
{
  // adjust the sensitivity accordingly:
  switch( new_range )
  {
  case RANGE_2000:
    sensitivity_ = 0.06097561; // approx: (1/16.4)  [(°/s) / LSB ]
    break;
  case RANGE_1000:
    sensitivity_ = 0.030487805; // approx: (1/32.8)  [(°/s) / LSB ]
    break;
  case RANGE_500:
    sensitivity_ = 0.015243902; // approx: (1/65.6)  [(°/s) / LSB ]
    break;
  case RANGE_250:
    sensitivity_ = 0.007621951; // approx: (1/131.2)  [(°/s) / LSB ]
    break;
  case RANGE_125:
    sensitivity_ = 0.003810976; // approx: (1/262.4)  [(°/s) / LSB ]
    break;
  default: // shouldn't happen because input argument is only an accel_range data type.
    ROS_ERROR("BMG160Parameters: invalid range setting.");
    return;
  }
  // reassign the class's new range:
  range_ = new_range;

  /**
   *  \note  \a bmg160::changeRange() must be called after this method 
   * so that the sensor's range is adjusted as well!
   */
}


/**********************************************************************/
void BMG160Parameters::setFilter( bool request )
{
  gyro_is_filtered_ = request;
}


/**********************************************************************/
void BMG160Parameters::setBandwidth( bandwidth bw )
{ 
  bw_reg_ = bw;
}
