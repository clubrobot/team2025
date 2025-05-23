//
// Created by romain on 5/17/25.
//

#include "include/SensorArray.h"

#include "../../Include/My_Clock.h"
#include "uld/include/VL53L5CX.h"

SensorArray::SensorArray(i2c_t *i2c, uint32_t latch, uint32_t data, uint32_t clock)
{
    this->power_config = 0b00000000;

    //Define pins
    this->ds_pin = data;
    this->shcp_pin = clock;
    this->stcp_pin = latch;

    this->i2c_bus = i2c;

    this->nb_sensors = 0;

    memset(this->raw_data, 0 , sizeof this->raw_data);
}

uint8_t SensorArray::addSensor(const SensorConfig sensor_cfg)
{
    //Check if the i2c address is not already used
    for (int i = 0; i < this->nb_sensors; ++i)
    {
        if (sensor_cfg.addr == sensors[i].cfg.addr)
        {
            logs.log(ERROR_LEVEL, "Sensor %d and sensor %d have the same address : 0x%02x\n", sensor_cfg.pin, sensors[i].cfg.pin, sensor_cfg.addr);
            return 1;
        }
    }
    if (this->nb_sensors < SENSORARRAY_MAX)
    {
        this->sensors[nb_sensors] =  {.sensor = new VL53L5CX(this->i2c_bus), .cfg = sensor_cfg};
        ++nb_sensors;
        return 0;
    }
    logs.log(ERROR_LEVEL, "The sensor array as already 8 sensors\n");
    return 1;
}

/*
 * Waring : The sensors must be shut down before initialization with a new address
 * A misfunctioning sensor can cause others to fail / not respond
 */

uint8_t SensorArray::Init()
{
    int started_sensors = 0;

    //Init the pins
    pinMode(stcp_pin, OUTPUT);
    pinMode(shcp_pin, OUTPUT);
    pinMode(ds_pin, OUTPUT);

    this->power_config = 0;
    this->ApplyPowerConfig();

    for (int i = 0; i < this->nb_sensors; ++i)
    {
        SensorHandle *handle = &sensors[i];
        logs.log(INFO_LEVEL, "Starting VL53L5CX %d for address : 0x%02x \n", handle->cfg.pin, handle->cfg.addr);

        //Power this sensor to change its address
        this->power_config += 1 << handle->cfg.pin - 1;
        this->ApplyPowerConfig();

        poly_delay(200);

        //Check if the sensor is alive
        uint8_t status = handle->sensor->vl53l5cx_is_alive(&handle->is_alive);

        if (status != VL53L5CX_STATUS_OK || handle->is_alive != 1)
        {
            logs.log(WARNING_LEVEL, "VL53L5CX not detected at default address 0x%X, status : %d\n", handle->sensor->_dev.platform.address, status);

            //Try at the "futur" address if already initilised
            handle->sensor->p_dev->platform.address = handle->cfg.addr;

            //Check if the sensor is alive
            status = handle->sensor->vl53l5cx_is_alive(&handle->is_alive);

            if (status != VL53L5CX_STATUS_OK || handle->is_alive != 1)
            {
                this->power_config = this->power_config & ~(1 << (handle->cfg.pin - 1));
                logs.log(ERROR_LEVEL, "VL53L5CX not detected at 0x%X, status : %d\n", handle->sensor->_dev.platform.address, status);
                continue;
            }
        }

        //Flash the firmware and init the sensor
        status = handle->sensor->vl53l5cx_init();

        if (status != VL53L5CX_STATUS_OK)
        {
            handle->is_alive = 0;
            this->power_config = this->power_config & ~(1 << (handle->cfg.pin - 1));
            logs.log(ERROR_LEVEL, "Failed to initialize VL53L5CX %d, status : %d\n", handle->cfg.pin, status);
            continue;
        }

        ++started_sensors;
        handle->is_alive = 1;

        if (status == VL53L5CX_STATUS_OK)
        {
            logs.log(GOOD_LEVEL, "VL53L5CX %d initialized successfully !\n", handle->cfg.pin);
        }

        //Apply the sensor configuration
        handle->sensor->vl53l5cx_set_resolution(VL53L5CX_RESOLUTION_8X8);
        handle->sensor->vl53l5cx_set_ranging_mode(VL53L5CX_RANGING_MODE_CONTINUOUS);
        handle->sensor->vl53l5cx_set_ranging_frequency_hz(15);
        handle->sensor->vl53l5cx_set_i2c_address(handle->cfg.addr);
        handle->sensor->vl53l5cx_start_ranging();

    }

    this->ApplyPowerConfig();

    logs.log(started_sensors != this->nb_sensors ? ERROR_LEVEL : GOOD_LEVEL, "Started %d/%d sensors\n", started_sensors, this->nb_sensors);

    return started_sensors != this->nb_sensors;
}

void SensorArray::ApplyPowerConfig() const
{
    digitalWrite(ds_pin, 0);
    digitalWrite(stcp_pin, 0);
    digitalWrite(shcp_pin, 0);

    for (int i = SENSORARRAY_MAX - 1; i >= 0; i--)
    {
        delay(1);
        digitalWrite(shcp_pin, 0);

        //Sets the pin to HIGH or LOW depending on bits of the power config
        digitalWrite(ds_pin, power_config & (1<<i));

        //Register shifts bits on upstroke of clock pin
        digitalWrite(shcp_pin, 1);

        //Clear the data pin after shift to prevent bleed through
        digitalWrite(ds_pin, 0);

    }

    //Stop shifting
    digitalWrite(shcp_pin, 0);
    digitalWrite(stcp_pin, 1);
}

uint8_t SensorArray::Start()
{
    uint8_t status = 0;

    for (int i = 0; i < this->nb_sensors; ++i)
    {
        SensorHandle *handle = &sensors[i];

        if (handle->is_alive != 1)
        {
            continue;
        }

        status |= handle->sensor->vl53l5cx_start_ranging();
    }

    return status;
}


uint8_t SensorArray::AquireRawData()
{
    uint8_t status = 0;
    for (int i = 0; i < this->nb_sensors; ++i)
    {
        SensorHandle *handle = &sensors[i];

        if (handle->is_alive != 1)
        {
            continue;
        }

        //handle->sensor->vl53l5cx_start_ranging();

        //Wait until the data is ready, TODO under the timeout
        uint8_t ready = 0;
        while (!ready)
        {
            handle->sensor->vl53l5cx_check_data_ready(&ready);
            poly_delay(5);
        }

        VL53L5CX_ResultsData data;
        uint8_t r_status = handle->sensor->vl53l5cx_get_ranging_data(&data);
        status |= r_status;

        //Don't copy the data if the sensor has returned an error
        if (r_status != 0)
            continue;


        for (int j = 0; j < 64; ++j)
        {
            //According to uld's guide, um2884 5, 9 and 10 status code doesn't impact mesurments
            if (data.target_status[j] == 5 || data.target_status[j] == 9 || data.target_status[j] == 10)
            {
                this->raw_data[i][j] = data.distance_mm[j];
                continue;
            }

            //When the mesure is not valid
            this->raw_data[i][j] = -1;
        }

        memcpy(this->raw_data[i], data.distance_mm, sizeof data.distance_mm);
    }
    return status;
}



void SensorArray::Stop()
{
    for (int i = 0; i < this->nb_sensors; ++i)
    {
        sensors[i].sensor->vl53l5cx_stop_ranging();
        delete sensors[i].sensor;
    }
}

