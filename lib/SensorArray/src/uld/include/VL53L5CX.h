//
// Created by romain on 5/18/25.
//

#ifndef VL53L5CX_H
#define VL53L5CX_H

#include <Wire.h>

#include "Arduino.h"
#include "platform.h"
#include "vl53l5cx_api.h"


class VL53L5CX {
  public:
    /** Constructor
     * @param[in] i2c device I2C to be used for communication
     * @param[in] lpn_pin pin to be used as component LPn
     * @param[in] i2c_rst_pin pin to be used as component I2C_RST
     */
    VL53L5CX(TwoWire *i2c)
    {
      memset((void *)&_dev, 0x0, sizeof(VL53L5CX_Configuration));
      _dev.platform.address = VL53L5CX_DEFAULT_I2C_ADDRESS;
      _dev.platform.dev_i2c = i2c;
      p_dev = &_dev;
    }


    /** Destructor
     */
    virtual ~VL53L5CX() {}
    /* warning: VL53LX class inherits from GenericSensor, RangeSensor and LightSensor, that haven`t a destructor.
       The warning should request to introduce a virtual destructor to make sure to delete the object */

    /**
     * @brief  Initialize the sensor
     * @param (uint8_t) addr : New I2C address.
     * @return (uint8_t) status : 0 if init_sensor is OK.
     */
    int init_sensor(uint8_t addr = VL53L5CX_DEFAULT_I2C_ADDRESS)
    {
      uint8_t status = VL53L5CX_STATUS_OK;
      uint8_t isAlive = 0;

      if (addr != _dev.platform.address) {
        status = vl53l5cx_set_i2c_address(addr);
        if (status != VL53L5CX_STATUS_OK) {
          return VL53L5CX_STATUS_ERROR;
        }
      }

      status = vl53l5cx_is_alive(&isAlive);
      if (!isAlive || status != VL53L5CX_STATUS_OK) {
        return VL53L5CX_STATUS_ERROR;
      }

      // Init VL53L5CX sensor
      status = vl53l5cx_init();
      if (status != VL53L5CX_STATUS_OK) {
        return VL53L5CX_STATUS_ERROR;
      }

      return (int)status;
    }


    /* vl53l5cx_api.h */
    /**
     * @brief This function is used to check if the sensor is alive.
     * @param (uint8_t) *p_is_alive : 1 if the sensor is alive, 0 in case of error.
     * @return (uint8_t) status : 0 if is_alive is OK.
     */
    uint8_t vl53l5cx_is_alive(
      uint8_t       *p_is_alive);

    /**
     * @brief Mandatory function used to initialize the sensor. This function must
     * be called after a power on, to load the firmware into the VL53L5CX. It takes
     * a few hundred milliseconds.
     * @return (uint8_t) status : 0 if initialization is OK.
     */

    uint8_t vl53l5cx_init();

    /**
     * @brief This function is used to change the I2C address of the sensor. If
     * multiple VL53L5 sensors are connected to the same I2C line, all other LPn
     * pins needs to be set to Low. The default sensor address is 0x52.
     * @param (uint16_t) i2c_address : New I2C address.
     * @return (uint8_t) status : 0 if new address is OK
     */

    uint8_t vl53l5cx_set_i2c_address(
      uint16_t      i2c_address);

    /**
     * @brief This function is used to get the current sensor power mode.
     * @param (uint8_t) *p_power_mode : Current power mode. The value of this
     * pointer is equal to 0 if the sensor is in low power,
     * (VL53L5CX_POWER_MODE_SLEEP), or 1 if sensor is in standard mode
     * (VL53L5CX_POWER_MODE_WAKEUP).
     * @return (uint8_t) status : 0 if power mode is OK
     */

    uint8_t vl53l5cx_get_power_mode(
      uint8_t       *p_power_mode);

    /**
     * @brief This function is used to set the sensor in Low Power mode, for
     * example if the sensor is not used during a long time. The macro
     * VL53L5CX_POWER_MODE_SLEEP can be used to enable the low power mode. When user
     * want to restart the sensor, he can use macro VL53L5CX_POWER_MODE_WAKEUP.
     * Please ensure that the device is not streaming before calling the function.
     * @param (uint8_t) power_mode : Selected power mode (VL53L5CX_POWER_MODE_SLEEP
     * or VL53L5CX_POWER_MODE_WAKEUP)
     * @return (uint8_t) status : 0 if power mode is OK, or 127 if power mode
     * requested by user is not valid.
     */

    uint8_t vl53l5cx_set_power_mode(
      uint8_t       power_mode);

    /**
     * @brief This function starts a ranging session. When the sensor streams, host
     * cannot change settings 'on-the-fly'.
     * @return (uint8_t) status : 0 if start is OK.
     */

    uint8_t vl53l5cx_start_ranging();

    /**
     * @brief This function stops the ranging session. It must be used when the
     * sensor streams, after calling vl53l5cx_start_ranging().
     * @return (uint8_t) status : 0 if stop is OK
     */

    uint8_t vl53l5cx_stop_ranging();

    /**
     * @brief This function checks if a new data is ready by polling I2C. If a new
     * data is ready, a flag will be raised.
     * @param (uint8_t) *p_isReady : Value of this pointer be updated to 0 if data
     * is not ready, or 1 if a new data is ready.
     * @return (uint8_t) status : 0 if I2C reading is OK
     */

    uint8_t vl53l5cx_check_data_ready(
      uint8_t       *p_isReady);

    /**
     * @brief This function gets the ranging data, using the selected output and the
     * resolution.
     * @param (VL53L5CX_ResultsData) *p_results : VL53L5 results structure.
     * @return (uint8_t) status : 0 data are successfully get.
     */

    uint8_t vl53l5cx_get_ranging_data(
      VL53L5CX_ResultsData    *p_results);

    /**
     * @brief This function gets the current resolution (4x4 or 8x8).
     * @param (uint8_t) *p_resolution : Value of this pointer will be equal to 16
     * for 4x4 mode, and 64 for 8x8 mode.
     * @return (uint8_t) status : 0 if resolution is OK.
     */

    uint8_t vl53l5cx_get_resolution(
      uint8_t       *p_resolution);

    /**
     * @brief This function sets a new resolution (4x4 or 8x8).
     * @param (uint8_t) resolution : Use macro VL53L5CX_RESOLUTION_4X4 or
     * VL53L5CX_RESOLUTION_8X8 to set the resolution.
     * @return (uint8_t) status : 0 if set resolution is OK.
     */

    uint8_t vl53l5cx_set_resolution(
      uint8_t                         resolution);

    /**
     * @brief This function gets the current ranging frequency in Hz. Ranging
     * frequency corresponds to the time between each measurement.
     * @param (uint8_t) *p_frequency_hz: Contains the ranging frequency in Hz.
     * @return (uint8_t) status : 0 if ranging frequency is OK.
     */

    uint8_t vl53l5cx_get_ranging_frequency_hz(
      uint8_t       *p_frequency_hz);

    /**
     * @brief This function sets a new ranging frequency in Hz. Ranging frequency
     * corresponds to the measurements frequency. This setting depends of
     * the resolution, so please select your resolution before using this function.
     * @param (uint8_t) frequency_hz : Contains the ranging frequency in Hz.
     * - For 4x4, min and max allowed values are : [1;60]
     * - For 8x8, min and max allowed values are : [1;15]
     * @return (uint8_t) status : 0 if ranging frequency is OK, or 127 if the value
     * is not correct.
     */

    uint8_t vl53l5cx_set_ranging_frequency_hz(
      uint8_t       frequency_hz);

    /**
     * @brief This function gets the current integration time in ms.
     * @param (uint32_t) *p_time_ms: Contains integration time in ms.
     * @return (uint8_t) status : 0 if integration time is OK.
     */

    uint8_t vl53l5cx_get_integration_time_ms(
      uint32_t      *p_time_ms);

    /**
     * @brief This function sets a new integration time in ms. Integration time must
     * be computed to be lower than the ranging period, for a selected resolution.
     * Please note that this function has no impact on ranging mode continuous.
     * @param (uint32_t) time_ms : Contains the integration time in ms. For all
     * resolutions and frequency, the minimum value is 2ms, and the maximum is
     * 1000ms.
     * @return (uint8_t) status : 0 if set integration time is OK.
     */

    uint8_t vl53l5cx_set_integration_time_ms(
      uint32_t      integration_time_ms);

    /**
     * @brief This function gets the current sharpener in percent. Sharpener can be
     * changed to blur more or less zones depending of the application.
     * @param (uint32_t) *p_sharpener_percent: Contains the sharpener in percent.
     * @return (uint8_t) status : 0 if get sharpener is OK.
     */

    uint8_t vl53l5cx_get_sharpener_percent(
      uint8_t       *p_sharpener_percent);

    /**
     * @brief This function sets a new sharpener value in percent. Sharpener can be
     * changed to blur more or less zones depending of the application. Min value is
     * 0 (disabled), and max is 99.
     * @param (uint32_t) sharpener_percent : Value between 0 (disabled) and 99%.
     * @return (uint8_t) status : 0 if set sharpener is OK.
     */

    uint8_t vl53l5cx_set_sharpener_percent(
      uint8_t       sharpener_percent);

    /**
     * @brief This function gets the current target order (closest or strongest).
     * @param (uint8_t) *p_target_order: Contains the target order.
     * @return (uint8_t) status : 0 if get target order is OK.
     */

    uint8_t vl53l5cx_get_target_order(
      uint8_t       *p_target_order);

    /**
     * @brief This function sets a new target order. Please use macros
     * VL53L5CX_TARGET_ORDER_STRONGEST and VL53L5CX_TARGET_ORDER_CLOSEST to define
     * the new output order. By default, the sensor is configured with the strongest
     * output.
     * @param (uint8_t) target_order : Required target order.
     * @return (uint8_t) status : 0 if set target order is OK, or 127 if target
     * order is unknown.
     */

    uint8_t vl53l5cx_set_target_order(
      uint8_t       target_order);

    /**
     * @brief This function is used to get the ranging mode. Two modes are
     * available using ULD : Continuous and autonomous. The default
     * mode is Autonomous.
     * @param (uint8_t) *p_ranging_mode : current ranging mode
     * @return (uint8_t) status : 0 if get ranging mode is OK.
     */

    uint8_t vl53l5cx_get_ranging_mode(
      uint8_t       *p_ranging_mode);

    /**
     * @brief This function is used to set the ranging mode. Two modes are
     * available using ULD : Continuous and autonomous. The default
     * mode is Autonomous.
     * @param (uint8_t) ranging_mode : Use macros VL53L5CX_RANGING_MODE_CONTINUOUS,
     * VL53L5CX_RANGING_MODE_CONTINUOUS.
     * @return (uint8_t) status : 0 if set ranging mode is OK.
     */

    uint8_t vl53l5cx_set_ranging_mode(
      uint8_t       ranging_mode);

    /**
     * @brief This function can be used to read 'extra data' from DCI. Using a known
     * index, the function fills the casted structure passed in argument.
     * @param (uint8_t) *data : This field can be a casted structure, or a simple
     * array. Please note that the FW only accept data of 32 bits. So field data can
     * only have a size of 32, 64, 96, 128, bits ....
     * @param (uint32_t) index : Index of required value.
     * @param (uint16_t)*data_size : This field must be the structure or array size
     * (using sizeof() function).
     * @return (uint8_t) status : 0 if OK
     */

    uint8_t vl53l5cx_dci_read_data(
      uint8_t       *data,
      uint32_t      index,
      uint16_t      data_size);

    /**
     * @brief This function can be used to write 'extra data' to DCI. The data can
     * be simple data, or casted structure.
     * @param (uint8_t) *data : This field can be a casted structure, or a simple
     * array. Please note that the FW only accept data of 32 bits. So field data can
     * only have a size of 32, 64, 96, 128, bits ..
     * @param (uint32_t) index : Index of required value.
     * @param (uint16_t)*data_size : This field must be the structure or array size
     * (using sizeof() function).
     * @return (uint8_t) status : 0 if OK
     */

    uint8_t vl53l5cx_dci_write_data(
      uint8_t       *data,
      uint32_t      index,
      uint16_t      data_size);

    /**
     * @brief This function can be used to replace 'extra data' in DCI. The data can
     * be simple data, or casted structure.
     * @param (uint8_t) *data : This field can be a casted structure, or a simple
     * array. Please note that the FW only accept data of 32 bits. So field data can
     * only have a size of 32, 64, 96, 128, bits ..
     * @param (uint32_t) index : Index of required value.
     * @param (uint16_t)*data_size : This field must be the structure or array size
     * (using sizeof() function).
     * @param (uint8_t) *new_data : Contains the new fields.
     * @param (uint16_t) new is
_po
r_m
ion
tin
 st
rt__data_size : New data size.
     * @param (uint16_t) new_data_pos : New data position into the buffer.
     * @return (uint8_t) status : 0 if OK
     */

    uint8_t vl53l5cx_dci_replace_data(
      uint8_t       *data,
      uint32_t      index,
      uint16_t      data_size,
      uint8_t       *new_data,
      uint16_t      new_data_size,
      uint16_t      new_data_pos);

    void SwapBuffer(
      uint8_t     *buffer,
      uint16_t     size);

    /* Helpful APIs */
    uint8_t get_stream_count(void)
    {
      return _dev.streamcount;
    };

  protected:

    /**
     * @brief Inner function, not available outside this file. This function is used
     * to wait for an answer from VL53L5CX sensor.
     */
    uint8_t _vl53l5cx_poll_for_answer(
      uint8_t       size,
      uint8_t       pos,
      uint16_t      address,
      uint8_t       mask,
      uint8_t       expected_value);

  uint8_t _vl53l5cx_poll_for_mcu_boot();

    /**
     * @brief Inner function, not available outside this file. This function is used
     * to set the offset data gathered from NVM.
     */
    uint8_t _vl53l5cx_send_offset_data(
      uint8_t       resolution);


    /**
     * @brief Inner function, not available outside this file. This function is used
     * to set the Xtalk data from generic configuration, or user's calibration.
     */
    uint8_t _vl53l5cx_send_xtalk_data(
      uint8_t       resolution);

    /**
     * @brief Inner function, not available outside this file. This function is used to
     * wait for an answer from VL53L5 sensor.
     */
    uint8_t _vl53l5cx_poll_for_answer_xtalk(
      uint16_t      address,
      uint8_t       expected_value);

    /**
     * @brief Inner function, not available outside this file. This function is used to
     * program the output using the macro defined into the 'platform.h' file.
     */
    uint8_t _vl53l5cx_program_output_config();

    /* Platform APIs */

    /**
     * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
     * structure.
     * @param (uint16_t) Address : I2C location of value to read.
     * @param (uint8_t) *p_values : Pointer of value to read.
     * @return (uint8_t) status : 0 if OK
     */

    uint8_t RdByte(
      VL53L5CX_Platform *p_platform,
      uint16_t RegisterAddress,
      uint8_t *p_value);

    /**
     * @brief Mandatory function used to write one single byte.
     * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
     * structure.
     * @param (uint16_t) Address : I2C location of value to read.
     * @param (uint8_t) value : Pointer of value to write.
     * @return (uint8_t) status : 0 if OK
     */

    uint8_t WrByte(
      VL53L5CX_Platform *p_platform,
      uint16_t RegisterAddress,
      uint8_t value);

    /**
     * @brief Mandatory function used to read multiples bytes.
     * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
     * structure.
     * @param (uint16_t) Address : I2C location of values to read.
     * @param (uint8_t) *p_values : Buffer of bytes to read.
     * @param (uint32_t) size : Size of *p_values buffer.
     * @return (uint8_t) status : 0 if OK
     */

    uint8_t RdMulti(
      VL53L5CX_Platform *p_platform,
      uint16_t RegisterAddress,
      uint8_t *p_values,
      uint32_t size);

    /**
     * @brief Mandatory function used to write multiples bytes.
     * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
     * structure.
     * @param (uint16_t) Address : I2C location of values to write.
     * @param (uint8_t) *p_values : Buffer of bytes to write.
     * @param (uint32_t) size : Size of *p_values buffer.
     * @return (uint8_t) status : 0 if OK
     */

    uint8_t WrMulti(
      VL53L5CX_Platform *p_platform,
      uint16_t RegisterAddress,
      uint8_t *p_values,
      uint32_t size);

    /**
     * @brief Mandatory function, used to wait during an amount of time. It must be
     * filled as it's used into the API.
     * @param (VL53L5CX_Platform*) p_platform : Pointer of VL53L5CX platform
     * structure.
     * @param (uint32_t) TimeMs : Time to wait in ms.
     * @return (uint8_t) status : 0 if wait is finished.
     */

    uint8_t WaitMs(
      VL53L5CX_Platform *p_platform,
      uint32_t TimeMs);

  uint8_t vl53l5cx_set_VHV_repeat_count(
    uint32_t repeat_count);

  uint8_t vl53l5cx_get_VHV_repeat_count(
    uint32_t *p_repeat_count);

  uint8_t vl53l5cx_disable_internal_cp();

  uint8_t vl53l5cx_enable_internal_cp();

protected:

  /* VL53L5CX Device */
  VL53L5CX_Configuration _dev;
  VL53L5CX_Configuration *p_dev;

};



#endif //VL53L5CX_H
