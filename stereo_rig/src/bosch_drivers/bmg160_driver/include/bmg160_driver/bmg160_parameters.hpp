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

#ifndef BMG160_PARAMETERS_H_
#define BMG160_PARAMETERS_H_

#include <ros/console.h>

#include <bosch_drivers_parameters.hpp>
#include <bosch_drivers_hardware_interface.hpp>


using namespace bosch_drivers_common;

class BMG160Parameters: public Parameters
{
public:

  /**
   * \brief  Configurable range values that the user can adjust on the sensor.
   * See datasheet page 38
   */
  enum range
  {
    /** 
     * max: ±2000 [°/s], sensitivity: 61.0  [milli°/(s * LSB)]
     */ 
    RANGE_2000 = 0x00,
    /**
     * max: ±1000 [°/s], sensitivity: 30.5  [milli°/(s * LSB)
     */
    RANGE_1000 = 0x01,
    /** 
     * max: ±500 [°/s], sensitivity: 15.3  [milli°/(s * LSB)]
     */
    RANGE_500 = 0x02,
    /** 
     * max: ±250 [°/s], sensitivity: 7.6  [milli°/(s * LSB)]
     */
    RANGE_250 = 0x03,
    /** 
     * max: ±125 [°/s], sensitivity: 3.8  [milli°/(s * LSB)]
     */
    RANGE_125 = 0x04 
  };

  /**
   * \brief  Configurable bandwidths that the user can set on the sensor
   */  
  enum bandwidth
  {
    BW_32HZ  = 0x07,  
    BW_64HZ  = 0x06,  
    BW_12HZ  = 0x05, 
    BW_23HZ  = 0x04,
    BW_47HZ  = 0x03,  
    BW_116HZ = 0x02, 
    BW_230HZ = 0x01,
    BW_UNFILTERED  = 0x00, 
  };

  /**
   * \brief  constructor
   */
  BMG160Parameters();
  
  /**
   * \brief  destructor
   */
  ~BMG160Parameters();
  
 /**
  * \brief  Change gyro protocol parameter to a different protocol.
  *
  * The protocol depends on the physical hardware configuration 
  * of the sensor.  
  */
  bool setProtocol( interface_protocol protocol );

  /** 
   * \brief  Change interface protocol frequency to a different frequency.
   *
   * User should not need to change this value.  The frequency
   * set upon instantiation is functional.
   */
  bool setFrequency( int frequency );

  /**
   * \brief  Alert software driver of the physical pin of the hardware 
   *      interface that the sensor's chip-select is connected to.
   *  
   *      This is an SPI-relevant parameter only.
   */  
  bool setPin( uint8_t pin );

  /**
   * \brief  Alerts the software driver that the chip's SDO pin is 
   *      connected to either VDD or GND, which allows it to deduce
   *      the I2C device address.
   *
   * \param  high_or_low  \a true indicates SDO is connected to VDD.
   *      \a false indicates that SDO is connected to GND.
   */  
  void setSlaveAddressBit( bool high_or_low );
        
  /** 
   * \brief  Configure the hardware-interface's SPI mode by which it 
   *      communicates with the sensor.
   *
   * \note  see the wikipedia article on SPI for more information on the
   *      corresponding CPOL and CPHA settings.
   * Valid modes are defined in the bosch_drivers_common.
   * The user should NOT have to change this value from a value
   *      different from the value set upon instantiation.
   * In detail, this method directly manipulates the class flags_
   *      value, which is passed to the hardware interface class.
   * \param mode  the SPI mode of the sensor. Valid modes: SPI_MODE_0,
   *         SPI_MODE_1, SPI_MODE_2, SPI_MODE_3.
   * \return boolean indicating successful input.
   */
  bool setSpiMode( uint8_t mode );

  /** 
   * \brief Configure the hardware-interface's SPI byte-order by which it 
   *     will communicate with the sensor.
   *
   * Byte-order arguments are either MSB_FIRST or LSB_FIRST, 
   *     both of which are defined in the bosch_drivers_common.
   * \note  The user should NOT have to change this value from a value
   *      different from the value set upon instantiation.
   * In detail, this method directly manipulates the class flags_
   *      value, which is passed to the hardware interface class.
   */
  bool setByteOrder( uint8_t value );

  /**
   * \brief  Change gyro range parameter to a different range.
   *
   * To apply this range on the sensor, the user must 
   * subsequently call one of two methods in the bmg160 driver: 
   * initialize() or changeRange().
   */
  void setRange( range new_range );

  /**
   * \brief  Change filter parameter to filtered or unfiltered, depending
   *      on input value.
   *
   * To apply this filter parameter on the sensor, the user must 
   * subsequently call one of two methods in the bmg160 driver: 
   * initialize() or filterData(gyro_is_filtered_) , where
   * gyro_is_filtered is a class member of this class.
   */
  void setFilter( bool input ); 

  /**
   * \brief  Change bandwidth parameter to input bandwidth.
   *
   * To apply this parameter on the sensor, the user must 
   * subsequently call one of two methods in the bmg160 driver: 
   * initialize() or changeBandwidth().
   */
  void setBandwidth( bandwidth bw );

  /** 
   * \brief Retreive the sensitivity value to the corresponding gyro
   *      range such that the raw data can be correctly calculated.
   *
   * \return  class sensitivity_ value, who's current value should be 
   *      the sensitivity that corresponds with the correct range 
   *      setting.
   */
  double getSensitivity();
  
  /**
   * \brief  retreive the interface protocol for which the device is
   *      configured.
   * \return class protocol_ value.
   */ 
  interface_protocol getProtocol();

  /**
   * \return frequency that the hardware interface is set to 
   *      commmunicate.
   */ 
  int getFrequency();

  /**
   * \brief  Retreive the SPI-relevant settings encoded in the flags_ 
   *      member.
   *
   * This is an SPI-relevant parameter only.  The flags_
   * value is ignored when the device is read from a protocol 
   * other than SPI.
   * \return class flags_ member.
   */ 
  int* getFlags();

  /**
   * \brief  retreive the physical pin of the hardware 
   * interface that the sensor's chip-select is connected to.
   *
   * This is an SPI-relevant parameter only.
   * \return hardware interface pin.
   */ 
  int getPin();
 
  /**
   * \return  slave_address_bit_
   */
  bool getSlaveAddressBit();

  /**
   * \brief  The user-specified range value of the gyroscope.
   */
  range range_;

  /**
   *  \brief A boolean indicating whether or not the gyro is outputting
   *      filtered data.
   */
  bool gyro_is_filtered_;

  /**
   *  \brief the user-specified bandwidth register of the gyroscope.
   */
  bandwidth bw_reg_;

  /**
   * \brief  the sensitivity of the current gyro range.
   */
  double sensitivity_;                        

 /**
  * \brief  Indicates whether the SDO pin is connected to VDD or GND.
  */ 
  bool slave_address_bit_;
};

#endif // BMG160_PARAMETERS_H_
