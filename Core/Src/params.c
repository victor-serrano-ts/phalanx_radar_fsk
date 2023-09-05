/*
 * params.c
 *
 *  Created on: Sep 4, 2023
 *      Author: victor
 */

#include "params.h"

/** @brief Representation of the external parameters memory used to compute
 * offsets and sizes. */
typedef struct  __attribute__((__packed__))
{
    uint8_t status;
    /* Regular params */
    float32_t bin_slow_thr;
    float32_t bin_reverse_thr;
    float32_t bin_stop_thr;
    float32_t bin_fast_thr;
    float32_t speed_min;
    float32_t speed_max;
    int16_t acc_thr;
    int16_t count_slow_thr;
    int16_t count_reverse_thr;
    int16_t count_stop_thr;
    int16_t count_fast_thr;
    /* CFAR params */
    float32_t cfar_slow_thr;
    float32_t cfar_reverse_thr;
    float32_t cfar_stop_thr;
    float32_t cfar_fast_thr;
    /* Extra params */
    int16_t moving_avg_len;
    float32_t reverse_dynamic_low_th;
    float32_t reverse_dynamic_high_th;
    int16_t reverse_dynamic_min_val;
    float32_t high_speeding_th;
    int16_t count_fast_filter_thr;
    /* Power saving params */
//    uint8_t num_slots;
//    uint8_t slot_assigned;
//    uint16_t cycle_duration;
//    uint8_t min_battery_thr;
    /* Misc config */
    uint8_t event_version;
    uint8_t dtlg_mode;
} param_data_t;

/** External flash memeory address of the indicated parameter. */
#define PARAM_ADDRESS(x)                (MEM_PARAM_ADDRESS + \
                                         offsetof(param_data_t, x))

/** Given a main page parameter address, return its backup address. */
#define PARAM_BK_ADDRESS(x)             (x + 0x1000)

/** Size of the indicated parameter. */
#define PARAM_SIZE(x)                   (sizeof(((param_data_t*)0)->x))

/** Initial address of the main page. */
#define PARAM_TOTAL_ADDRESS             (PARAM_ADDRESS(status))

/** Initial address of the backup page. */
#define PARAM_TOTAL_BK_ADDRESS          (PARAM_BK_ADDRESS(PARAM_TOTAL_ADDRESS))

/** Size of all parameters plus status byte. */
#define PARAM_TOTAL_SIZE                (sizeof(param_data_t))

/** Indicates that all the parameters are OK. */
#define PARAM_STATUS_OK                 (0x95)


/**
 * @brief Get the status byte from the backup page.
 *
 * @retval true If status is OK.
 * @retval false If status is not OK or an error occurs.
 */
static bool param_get_status_bk(void)
{

	return true;

}

/**
 * @brief Get the status byte from the main page.
 *
 * @retval true If status is OK.
 * @retval false If status is not OK or an error occurs.
 */
static bool param_get_status(void)
{

	return true;
}

/**
 * @brief Erase the parameters from the main page.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
static bool param_erase(void)
{
	bool ret = true;


	return ret;
}

/**
 * @brief Erase the parameters from the main page.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
static bool param_erase_bk(void)
{
	bool ret = true;


	return ret;
}

/**
 * @brief Restore the main page with the data in the backup page.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
static bool param_restore(void)
{
	bool ret = true;


	return ret;
}

/**
 * @brief Write a parameter in the external flash. It first fills the backup
 * page with the parameters not modified, save the new parameter in the backup
 * page, and restores the main page with the data in the backup page.
 *
 * @param offset Initial external flash memory address.
 * @param length Number of bytes to write in the external flash memory.
 * @param buf Buffer with the bytes to write.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
static bool param_write(size_t offset, size_t length, const uint8_t *buf)
{
	bool ret = true;


	return ret;
}

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool param_init(void)
{
	bool ret = true;


	return ret;
}

bool param_set_default(void)
{
	bool ret = true;



	return ret;
}

bool param_get_bin_slow_thr(float32_t *bin_slow_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*bin_slow_thr = PARAM_INIT_BIN_SLOW_THR;
	}

	return ret;
}

bool param_set_bin_slow_thr(float32_t bin_slow_thr)
{
	bool ret;


	return ret;
}

bool param_get_bin_reverse_thr(float32_t *bin_reverse_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*bin_reverse_thr = PARAM_INIT_BIN_REVERSE_THR;
	}

	return ret;
}

bool param_set_bin_reverse_thr(float32_t bin_reverse_thr)
{
	bool ret;


	return ret;
}

bool param_get_bin_stop_thr(float32_t *bin_stop_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*bin_stop_thr = PARAM_INIT_BIN_STOP_THR;
	}

	return ret;
}

bool param_set_bin_stop_thr(float32_t bin_stop_thr)
{
	bool ret;


	return ret;
}

bool param_get_bin_fast_thr(float32_t *bin_fast_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*bin_fast_thr = PARAM_INIT_BIN_FAST_THR;
	}

	return ret;
}

bool param_set_bin_fast_thr(float32_t bin_fast_thr)
{
	bool ret;


	return ret;
}

bool param_get_speed_min(float32_t *speed_min)
{
  bool ret = false;


  if (ret == false)
  {
		*speed_min = PARAM_INIT_SPEED_MIN;
	}

	return ret;
}

bool param_set_speed_min(float32_t speed_min)
{
	bool ret;


	return ret;
}

bool param_get_speed_max(float32_t *speed_max)
{
  bool ret = false;


  if (ret == false)
  {
		*speed_max = PARAM_INIT_SPEED_MAX;
	}

	return ret;
}

bool param_set_speed_max(float32_t speed_max)
{
	bool ret;


	return ret;
}

bool param_get_acc_thr(int16_t *acc_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*acc_thr = PARAM_INIT_ACC_THR;
	}

	return ret;
}

bool param_set_acc_thr(int16_t acc_thr)
{
	bool ret;


	return ret;
}

bool param_get_count_slow_thr(int16_t *count_slow_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*count_slow_thr = PARAM_INIT_COUNT_SLOW_THR;
	}

	return ret;
}

bool param_set_count_slow_thr(int16_t count_slow_thr)
{
	bool ret;


	return ret;
}

bool param_get_count_reverse_thr(int16_t *count_reverse_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*count_reverse_thr = PARAM_INIT_COUNT_REVERSE_THR;
	}

	return ret;
}

bool param_set_count_reverse_thr(int16_t count_reverse_thr)
{
	bool ret;


	return ret;
}

bool param_get_count_stop_thr(int16_t *count_stop_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*count_stop_thr = PARAM_INIT_COUNT_STOP_THR;
	}

	return ret;
}

bool param_set_count_stop_thr(int16_t count_stop_thr)
{
	bool ret;


	return ret;
}

bool param_get_count_fast_thr(int16_t *count_fast_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*count_fast_thr = PARAM_INIT_COUNT_FAST_THR;
	}

	return ret;
}

bool param_set_count_fast_thr(int16_t count_fast_thr)
{
	bool ret;


	return ret;
}

bool param_get_cfar_slow_thr(float32_t *cfar_slow_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*cfar_slow_thr = PARAM_INIT_CFAR_SLOW_THR;
	}

	return ret;
}

bool param_set_cfar_slow_thr(float32_t cfar_slow_thr)
{
	bool ret;


	return ret;
}

bool param_get_cfar_reverse_thr(float32_t *cfar_reverse_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*cfar_reverse_thr = PARAM_INIT_CFAR_REVERSE_THR;
	}

	return ret;
}

bool param_set_cfar_reverse_thr(float32_t cfar_reverse_thr)
{
	bool ret;


	return ret;
}

bool param_get_cfar_stop_thr(float32_t *cfar_stop_thr)
{
  bool ret = false;


  if (ret == false)
  {
		*cfar_stop_thr = PARAM_INIT_CFAR_STOP_THR;
	}

	return ret;
}

bool param_set_cfar_stop_thr(float32_t cfar_stop_thr)
{
	bool ret;


	return ret;
}

bool param_get_cfar_fast_thr(float32_t *cfar_fast_thr)
{
  bool ret = false;


  if (ret == false)
  {
			*cfar_fast_thr = PARAM_INIT_CFAR_FAST_THR;
	}

	return ret;
}

bool param_set_cfar_fast_thr(float32_t cfar_fast_thr)
{
	bool ret;


	return ret;
}

bool param_get_moving_avg_len(int16_t *moving_avg_len)
{
  bool ret = false;


  if (ret == false)
  {
		*moving_avg_len = PARAM_INIT_MOVING_AVERAGE_LEN;
	}

	if (*moving_avg_len > PARAM_INIT_MOVING_AVERAGE_MAX_LEN) {
		*moving_avg_len = PARAM_INIT_MOVING_AVERAGE_MAX_LEN;
	}

	return ret;
}

bool param_set_moving_avg_len(int16_t moving_avg_len)
{
	bool ret;


	return ret;
}

bool param_get_reverse_dynamic_low_th(float32_t *reverse_dynamic_low_th)
{
  bool ret = false;


  if (ret == false)
  {
		*reverse_dynamic_low_th = PARAM_INIT_REVERSE_DYN_LOW_TH;
	}

	return ret;
}

bool param_set_reverse_dynamic_low_th(float32_t reverse_dynamic_low_th)
{
	bool ret;


	return ret;
}

bool param_get_reverse_dynamic_high_th(float32_t *reverse_dynamic_high_th)
{
  bool ret = false;


  if (ret == false)
  {
		*reverse_dynamic_high_th = PARAM_INIT_REVERSE_DYN_HIGH_TH;
	}

	return ret;
}

bool param_set_reverse_dynamic_high_th(float32_t reverse_dynamic_high_th)
{
	bool ret;


	return ret;
}

bool param_get_reverse_dynamic_min_val(int16_t *reverse_dynamic_min_val)
{
  bool ret = false;


  if (ret == false)
  {
		*reverse_dynamic_min_val = PARAM_INIT_REVERSE_DYN_MIN_VAL;
	}

	return ret;
}

bool param_set_reverse_dynamic_min_val(int16_t reverse_dynamic_min_val)
{
	bool ret;


	return ret;
}

bool param_get_high_speeding_th(float32_t *high_speeding_th)
{
  bool ret = false;


  if (ret == false)
  {
  	*high_speeding_th = PARAM_INIT_HIGH_SPEEDING_TH;
	}

	return ret;
}

bool param_set_high_speeding_th(float32_t high_speeding_th)
{
	bool ret;


	return ret;
}

bool param_get_count_fast_filter_thr(int16_t *count_fast_filter_thr)
{
  bool ret = false;


  if (ret == false)
  {
			*count_fast_filter_thr = PARAM_INIT_COUNT_FAST_FILTER_THR;
	}

	return ret;
}

bool param_set_count_fast_filter_thr(int16_t count_fast_filter_thr)
{
	bool ret;


	return ret;
}

bool param_get_num_slots(uint8_t *num_slots)
{
  bool ret = false;


  if (ret == false)
  {
  	*num_slots = PARAM_INIT_NUM_SLOTS;
	}

	return ret;
}

bool param_set_num_slots(uint8_t num_slots)
{
	bool ret;


	return ret;
}

bool param_get_slot_assigned(uint8_t *slot_assigned)
{
  bool ret = false;


  if (ret == false)
  {
  	*slot_assigned = PARAM_INIT_SLOT_ASSIGNED;
	}

	return ret;
}

bool param_set_slot_assigned(uint8_t slot_assigned)
{
	bool ret;


	return ret;
}

bool param_get_cycle_duration(uint16_t *cycle_duration)
{
  bool ret = false;


  if (ret == false)
  {
  	*cycle_duration = PARAM_INIT_CYCLE_DURATION;
	}

	return ret;
}

bool param_set_cycle_duration(uint16_t cycle_duration)
{
	bool ret;


	return ret;
}

bool param_get_min_batt_thr(uint8_t *min_battery_thr)
{
  bool ret = false;


  if (ret == false)
  {
  	*min_battery_thr = PARAM_INIT_MIN_BATT_THR;
	}

	return ret;
}

bool param_set_min_batt_thr(uint8_t min_battery_thr)
{
	bool ret;


	return ret;
}

bool param_get_event_version(uint8_t *event_version)
{
  bool ret = false;


  if (ret == false)
  {
  	*event_version = PARAM_INIT_EVENT_VERSION;
	}

	return ret;
}

bool param_set_event_version(uint8_t event_version)
{
	bool ret;


	return ret;
}

bool param_get_dtlg_mode(uint8_t *dtlg_mode)
{
  bool ret = false;


  if (ret == false)
  {
		*dtlg_mode = PARAM_INIT_DTLG_MODE;
	}

	return ret;
}

bool param_set_dtlg_mode(uint8_t dtlg_mode)
{
	bool ret;


	return ret;
}

bool param_get_general(param_general_t *params)
{
	bool ret = false;


	if (ret == false)
	{
		params->bin_slow_thr = PARAM_INIT_BIN_SLOW_THR;
		params->bin_reverse_thr = PARAM_INIT_BIN_REVERSE_THR;
		params->bin_stop_thr = PARAM_INIT_BIN_STOP_THR;
		params->bin_fast_thr = PARAM_INIT_BIN_FAST_THR;
		params->speed_min = PARAM_INIT_SPEED_MIN;
		params->speed_max = PARAM_INIT_SPEED_MAX;
		params->acc_thr = PARAM_INIT_ACC_THR;
		params->count_slow_thr = PARAM_INIT_COUNT_SLOW_THR;
		params->count_reverse_thr = PARAM_INIT_COUNT_REVERSE_THR;
		params->count_stop_thr = PARAM_INIT_CFAR_STOP_THR;
		params->count_fast_thr = PARAM_INIT_CFAR_FAST_THR;
	}

	return ret;
}

bool param_set_general(param_general_t *params)
{
	bool ret;


	return ret;
}

bool param_get_cfar(param_cfar_t *params)
{
  bool ret = false;


  if (ret == false)
  {
		params->cfar_slow_thr = PARAM_INIT_CFAR_SLOW_THR;
		params->cfar_reverse_thr = PARAM_INIT_CFAR_REVERSE_THR;
		params->cfar_stop_thr = PARAM_INIT_CFAR_STOP_THR;
		params->cfar_fast_thr = PARAM_INIT_CFAR_FAST_THR;
	}

	return ret;
}

bool param_set_cfar(param_cfar_t *params)
{
	bool ret;


	return ret;
}

bool param_get_ext(param_ext_t *params)
{
	bool ret = false;


	if (ret == false)
	{
		params->moving_avg_len = PARAM_INIT_MOVING_AVERAGE_LEN;
		params->reverse_dynamic_low_th = PARAM_INIT_REVERSE_DYN_LOW_TH;
		params->reverse_dynamic_high_th = PARAM_INIT_REVERSE_DYN_HIGH_TH;
		params->reverse_dynamic_min_val = PARAM_INIT_REVERSE_DYN_MIN_VAL;
		params->high_speeding_th = PARAM_INIT_HIGH_SPEEDING_TH;
		params->count_fast_filter_thr = PARAM_INIT_COUNT_FAST_FILTER_THR;
	}

	return ret;
}

bool param_set_ext(param_ext_t *params)
{
	bool ret;


	return ret;
}

bool param_get_power(param_power_t *params)
{
  bool ret = false;


  if (ret == false)
  {
		params->num_slots = PARAM_INIT_NUM_SLOTS;
		params->slot_assigned = PARAM_INIT_SLOT_ASSIGNED;
		params->cycle_duration = PARAM_INIT_CYCLE_DURATION;
		params->min_battery_thr = PARAM_INIT_MIN_BATT_THR;
	}

	return ret;
}

bool param_set_power(param_power_t *params)
{
	bool ret;


	return ret;
}



















