#include "VL53L0X.h"

// Unless stated otherwise, this implementation is pulled from 
// ArtfulBytes' open source VL53L0X driver.
// here is the specific webpage: www.artfulbytes.com/vl53l0x-post

#define REG_IDENTIFICATION_MODEL_ID (0xC0)
#define REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV (0x89)
#define REG_MSRC_CONFIG_CONTROL (0x60)
#define REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT (0x44)
#define REG_SYSTEM_SEQUENCE_CONFIG (0x01)
#define REG_DYNAMIC_SPAD_REF_EN_START_OFFSET (0x4F)
#define REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD (0x4E)
#define REG_GLOBAL_CONFIG_REF_EN_START_SELECT (0xB6)
#define REG_SYSTEM_INTERRUPT_CONFIG_GPIO (0x0A)
#define REG_GPIO_HV_MUX_ACTIVE_HIGH (0x84)
#define REG_SYSTEM_INTERRUPT_CLEAR (0x0B)
#define REG_RESULT_INTERRUPT_STATUS (0x13)
#define REG_SYSRANGE_START (0x00)
#define REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0 (0xB0)
#define REG_RESULT_RANGE_STATUS (0x14)
#define REG_SLAVE_DEVICE_ADDRESS (0x8A)
#define RANGE_SEQUENCE_STEP_TCC (0x10) /* Target CentreCheck */
#define RANGE_SEQUENCE_STEP_MSRC (0x04) /* Minimum Signal Rate Check */
#define RANGE_SEQUENCE_STEP_DSS (0x28) /* Dynamic SPAD selection */
#define RANGE_SEQUENCE_STEP_PRE_RANGE (0x40)
#define RANGE_SEQUENCE_STEP_FINAL_RANGE (0x80)
#define VL53L0X_EXPECTED_DEVICE_ID (0xEE)
#define VL53L0X_DEFAULT_ADDRESS (0x29)
#define SPAD_TYPE_APERTURE (0x01)
#define SPAD_START_SELECT (0xB4)
#define SPAD_MAX_COUNT (44)
#define SPAD_MAP_ROW_COUNT (6)
#define SPAD_ROW_SIZE (8)
#define SPAD_APERTURE_START_INDEX (12)
static uint8_t stop_variable = 0;

// Written by us
bool TOF_write(char index, char data) {
  return write_check(0x29, index, data);
}

// Written by us
bool TOF_read(char index, char * data) {
  return read_check(0x29, index, data);
}

bool device_is_booted() {
  uint8_t dev_id = 0;
  if(!TOF_read(REG_IDENTIFICATION_MODEL_ID, &dev_id)) {
    return false;
  }
  return dev_id == VL53L0X_EXPECTED_DEVICE_ID;
}

bool data_init() {

  bool success = false;

  uint8_t vhv_config_scl_sda = 0;
  if(!TOF_read(REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, &vhv_config_scl_sda)) {
    return false;
  }

  vhv_config_scl_sda |= 0x01; //set last bit to 1
  if(!TOF_write(REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, vhv_config_scl_sda)) {
    return false;
  }

  
  // DO NOT ACTIVATE!!
  // This is from ArtfulBytes' code, but makes our setup nonfunctional.
  //    /* Set I2C standard mode */
  //success = TOF_write(0x88, 0x00);

  success = TOF_write(0x80, 0x01);
  success &= TOF_write(0xFF, 0x01);
  success &= TOF_write(0x00, 0x00);
  success &= TOF_read(0x91, &stop_variable);
  success &= TOF_write(0x00, 0x01);
  success &= TOF_write(0xFF, 0x00);
  success &= TOF_write(0x80, 0x00);

  return success;
}
bool load_default_tuning_settings()
{
    bool success = TOF_write(0xFF, 0x01);
    success &= TOF_write(0x00, 0x00);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x09, 0x00);
    success &= TOF_write(0x10, 0x00);
    success &= TOF_write(0x11, 0x00);
    success &= TOF_write(0x24, 0x01);
    success &= TOF_write(0x25, 0xFF);
    success &= TOF_write(0x75, 0x00);
    success &= TOF_write(0xFF, 0x01);
    success &= TOF_write(0x4E, 0x2C);
    success &= TOF_write(0x48, 0x00);
    success &= TOF_write(0x30, 0x20);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x30, 0x09);
    success &= TOF_write(0x54, 0x00);
    success &= TOF_write(0x31, 0x04);
    success &= TOF_write(0x32, 0x03);
    success &= TOF_write(0x40, 0x83);
    success &= TOF_write(0x46, 0x25);
    success &= TOF_write(0x60, 0x00);
    success &= TOF_write(0x27, 0x00);
    success &= TOF_write(0x50, 0x06);
    success &= TOF_write(0x51, 0x00);
    success &= TOF_write(0x52, 0x96);
    success &= TOF_write(0x56, 0x08);
    success &= TOF_write(0x57, 0x30);
    success &= TOF_write(0x61, 0x00);
    success &= TOF_write(0x62, 0x00);
    success &= TOF_write(0x64, 0x00);
    success &= TOF_write(0x65, 0x00);
    success &= TOF_write(0x66, 0xA0);
    success &= TOF_write(0xFF, 0x01);
    success &= TOF_write(0x22, 0x32);
    success &= TOF_write(0x47, 0x14);
    success &= TOF_write(0x49, 0xFF);
    success &= TOF_write(0x4A, 0x00);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x7A, 0x0A);
    success &= TOF_write(0x7B, 0x00);
    success &= TOF_write(0x78, 0x21);
    success &= TOF_write(0xFF, 0x01);
    success &= TOF_write(0x23, 0x34);
    success &= TOF_write(0x42, 0x00);
    success &= TOF_write(0x44, 0xFF);
    success &= TOF_write(0x45, 0x26);
    success &= TOF_write(0x46, 0x05);
    success &= TOF_write(0x40, 0x40);
    success &= TOF_write(0x0E, 0x06);
    success &= TOF_write(0x20, 0x1A);
    success &= TOF_write(0x43, 0x40);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x34, 0x03);
    success &= TOF_write(0x35, 0x44);
    success &= TOF_write(0xFF, 0x01);
    success &= TOF_write(0x31, 0x04);
    success &= TOF_write(0x4B, 0x09);
    success &= TOF_write(0x4C, 0x05);
    success &= TOF_write(0x4D, 0x04);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x44, 0x00);
    success &= TOF_write(0x45, 0x20);
    success &= TOF_write(0x47, 0x08);
    success &= TOF_write(0x48, 0x28);
    success &= TOF_write(0x67, 0x00);
    success &= TOF_write(0x70, 0x04);
    success &= TOF_write(0x71, 0x01);
    success &= TOF_write(0x72, 0xFE);
    success &= TOF_write(0x76, 0x00);
    success &= TOF_write(0x77, 0x00);
    success &= TOF_write(0xFF, 0x01);
    success &= TOF_write(0x0D, 0x01);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x80, 0x01);
    success &= TOF_write(0x01, 0xF8);
    success &= TOF_write(0xFF, 0x01);
    success &= TOF_write(0x8E, 0x01);
    success &= TOF_write(0x00, 0x01);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x80, 0x00);
    return success;
}

static bool configure_interrupt()
{
    /* Interrupt on new sample ready */
    if (!TOF_write(REG_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04)) {
        return false;
    }

    /* Configure active low since the pin is pulled-up on most breakout boards */
    uint8_t gpio_hv_mux_active_high = 0;
    if (!TOF_read(REG_GPIO_HV_MUX_ACTIVE_HIGH, &gpio_hv_mux_active_high)) {
        return false;
    }
    gpio_hv_mux_active_high &= ~0x10;
    if (!TOF_write(REG_GPIO_HV_MUX_ACTIVE_HIGH, gpio_hv_mux_active_high)) {
        return false;
    }

    if (!TOF_write(REG_SYSTEM_INTERRUPT_CLEAR, 0x01)) {
        return false;
    }
    return true;
}

bool set_sequence_steps_enabled(uint8_t sequence_step)
{
    return TOF_write(REG_SYSTEM_SEQUENCE_CONFIG, sequence_step);
}

bool static_init() {
  // We're skipping the spad setup

  if(!load_default_tuning_settings()) {
    return false;
  }

  if(!configure_interrupt()) {
    return false;
  }

  if (!set_sequence_steps_enabled(RANGE_SEQUENCE_STEP_DSS +
                                    RANGE_SEQUENCE_STEP_PRE_RANGE +
                                    RANGE_SEQUENCE_STEP_FINAL_RANGE)) {
    return false;
  }

  return true;
}

typedef enum
{
    CALIBRATION_TYPE_VHV,
    CALIBRATION_TYPE_PHASE
} calibration_type_t;

static bool perform_single_ref_calibration(calibration_type_t calib_type)
{
    uint8_t sysrange_start = 0;
    uint8_t sequence_config = 0;
    switch (calib_type)
    {
    case CALIBRATION_TYPE_VHV:
        sequence_config = 0x01;
        sysrange_start = 0x01 | 0x40;
        break;
    case CALIBRATION_TYPE_PHASE:
        sequence_config = 0x02;
        sysrange_start = 0x01 | 0x00;
        break;
    }
    if (!TOF_write(REG_SYSTEM_SEQUENCE_CONFIG, sequence_config)) {
        return false;
    }
    if (!TOF_write(REG_SYSRANGE_START, sysrange_start)) {
        return false;
    }
    /* Wait for interrupt */
    uint8_t interrupt_status = 0;
    bool success = false;
    do {
        success = TOF_read(REG_RESULT_INTERRUPT_STATUS, &interrupt_status);
    } while (success && ((interrupt_status & 0x07) == 0));
    if (!success) {
        return false;
    }
    if (!TOF_write(REG_SYSTEM_INTERRUPT_CLEAR, 0x01)) {
        return false;
    }

    if (!TOF_write(REG_SYSRANGE_START, 0x00)) {
        return false;
    }
    return true;
}

/**
 * Temperature calibration needs to be run again if the temperature changes by
 * more than 8 degrees according to the datasheet.
 */
static bool perform_ref_calibration()
{
    if (!perform_single_ref_calibration(CALIBRATION_TYPE_VHV)) {
        return false;
    }
    if (!perform_single_ref_calibration(CALIBRATION_TYPE_PHASE)) {
        return false;
    }
    /* Restore sequence steps enabled */
    if (!set_sequence_steps_enabled(RANGE_SEQUENCE_STEP_DSS +
                                    RANGE_SEQUENCE_STEP_PRE_RANGE +
                                    RANGE_SEQUENCE_STEP_FINAL_RANGE)) {
        return false;
    }
    return true;
}


bool vl53l0x_init() {
  
  if(!data_init()) {
    return false;
  }

  if(!static_init()) {
    return false;
  }
  
  if(!perform_ref_calibration()) {
    return false;
  }

  return true;
}

bool vl53l0x_read_range_single(uint16_t *range)
{
    bool success = TOF_write(0x80, 0x01);
    success &= TOF_write(0xFF, 0x01);
    success &= TOF_write(0x00, 0x00);
    success &= TOF_write(0x91, stop_variable);
    success &= TOF_write(0x00, 0x01);
    success &= TOF_write(0xFF, 0x00);
    success &= TOF_write(0x80, 0x00);
    if (!success) {
        return false;
    }

    if (!TOF_write(REG_SYSRANGE_START, 0x01)) {
        return false;
    }

    uint8_t sysrange_start = 0;
    do {
        success = TOF_read(REG_SYSRANGE_START, &sysrange_start);
    } while (success && (sysrange_start & 0x01));
    if (!success) {
        return false;
    }

    uint8_t interrupt_status = 0;
    do {
        success = TOF_read(REG_RESULT_INTERRUPT_STATUS, &interrupt_status);
    } while (success && ((interrupt_status & 0x07) == 0));
    if (!success) {
        return false;
    }
    
    // Our I2C functions only support 8 bit reads, 
    // so we've altered this block of code
    // to support our functions. 
    uint8_t lo_range;
    uint8_t hi_range;
    if( !TOF_read(REG_RESULT_RANGE_STATUS + 10, &hi_range) |
        !TOF_read(REG_RESULT_RANGE_STATUS + 11, &lo_range)) {
      return false;
    }
    *range = (hi_range << 8) + lo_range;

    if (!TOF_write(REG_SYSTEM_INTERRUPT_CLEAR, 0x01)) {
        return false;
    }

    /* 8190 or 8191 may be returned when obstacle is out of range. */
    if (*range == 8190 || *range == 8191) {
        *range = VL53L0X_OUT_OF_RANGE;
    }

    return true;
}