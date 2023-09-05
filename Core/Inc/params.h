/*
 * params.h
 *
 *  Created on: Sep 4, 2023
 *      Author: victor
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#include "main.h"

/**
 * @defgroup Parameter_initial_value
 * @{
 */
#define PARAM_INIT_BIN_SLOW_THR         (0.0f)
#define PARAM_INIT_BIN_REVERSE_THR      (0.0f)
#define PARAM_INIT_BIN_STOP_THR         (0.0f)
#define PARAM_INIT_BIN_FAST_THR         (0.0f)
#define PARAM_INIT_SPEED_MIN            (4.5f)
#define PARAM_INIT_SPEED_MAX            (50.0f)
#define PARAM_INIT_ACC_THR              (4000)
#define PARAM_INIT_COUNT_SLOW_THR       (8)
#define PARAM_INIT_COUNT_REVERSE_THR    (8)
#define PARAM_INIT_COUNT_STOP_THR       (32)
#define PARAM_INIT_COUNT_FAST_THR       (8)
#define PARAM_INIT_CFAR_SLOW_THR        (5.0f)
#define PARAM_INIT_CFAR_REVERSE_THR     (5.0f)
#define PARAM_INIT_CFAR_STOP_THR        (5.0f)
#define PARAM_INIT_CFAR_FAST_THR        (5.0f)
#define PARAM_INIT_MOVING_AVERAGE_LEN   (4)
#define PARAM_INIT_MOVING_AVERAGE_MAX_LEN   (16)
#define PARAM_INIT_REVERSE_DYN_LOW_TH   (50.0f)
#define PARAM_INIT_REVERSE_DYN_HIGH_TH  (100.0f)
#define PARAM_INIT_REVERSE_DYN_MIN_VAL  (3)
#define PARAM_INIT_HIGH_SPEEDING_TH     (120.0f)
#define PARAM_INIT_COUNT_FAST_FILTER_THR (16)
#define PARAM_INIT_NUM_SLOTS            (2)
#define PARAM_INIT_SLOT_ASSIGNED        (0)
#define PARAM_INIT_CYCLE_DURATION       (3600)
#define PARAM_INIT_MIN_BATT_THR         (0)
#define PARAM_INIT_EVENT_VERSION        (0xFF)
#define PARAM_INIT_DTLG_MODE            (3/*DTLG_MODE_IDLE*/)
/** @} */

/** @brief Struct including the general parameters */
typedef struct __attribute__((__packed__))
{
    float bin_slow_thr;
    float bin_reverse_thr;
    float bin_stop_thr;
    float bin_fast_thr;
    float speed_min;
    float speed_max;
    int16_t acc_thr;
    int16_t count_slow_thr;
    int16_t count_reverse_thr;
    int16_t count_stop_thr;
    int16_t count_fast_thr;
} param_general_t;

/** @brief Struct including the CFAR specific parameters */
typedef struct __attribute__((__packed__))
{
    float cfar_slow_thr;
    float cfar_reverse_thr;
    float cfar_stop_thr;
    float cfar_fast_thr;
} param_cfar_t;

/** @brief Struct including the extended parameters */
typedef struct __attribute__((__packed__))
{
    int16_t moving_avg_len;
    float reverse_dynamic_low_th;
    float reverse_dynamic_high_th;
    int16_t reverse_dynamic_min_val;
    float high_speeding_th;
    int16_t count_fast_filter_thr;
} param_ext_t;

/** @brief Struct including the power saving parameters */
typedef struct __attribute__((__packed__))
{
    uint8_t num_slots;
    uint8_t slot_assigned;
    uint16_t cycle_duration;
    uint8_t min_battery_thr;
} param_power_t;

/**
 * @brief Initializes the external parameters memory and checks for its
 * correctness.
 *
 * @retval true If initialization is correct.
 * @retval false If an error occurs.
 */
bool param_init(void);

/**
 * @brief Set the default parameter values.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_default(void);

/**
 * @brief Get bin_slow_thr parameter value.
 *
 * @param bin_slow_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_bin_slow_thr(float32_t *bin_slow_thr);

/**
 * @brief Set bin_slow_thr parameter value.
 *
 * @param bin_slow_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_bin_slow_thr(float32_t bin_slow_thr);

/**
 * @brief Get bin_reverse_thr parameter value.
 *
 * @param bin_reverse_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_bin_reverse_thr(float32_t *bin_reverse_thr);

/**
 * @brief Set bin_reverse_thr parameter value.
 *
 * @param bin_reverse_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_bin_reverse_thr(float32_t bin_reverse_thr);

/**
 * @brief Get bin_stop_thr parameter value.
 *
 * @param bin_stop_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_bin_stop_thr(float32_t *bin_stop_thr);

/**
 * @brief Set bin_stop_thr parameter value.
 *
 * @param bin_stop_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_bin_stop_thr(float32_t bin_stop_thr);

/**
 * @brief Get bin_fast_thr parameter value.
 *
 * @param bin_fast_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_bin_fast_thr(float32_t *bin_fast_thr);

/**
 * @brief Set bin_fast_thr parameter value.
 *
 * @param bin_fast_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_bin_fast_thr(float32_t bin_fast_thr);

/**
 * @brief Get speed_min parameter value.
 *
 * @param speed_min Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_speed_min(float32_t *speed_min);

/**
 * @brief Set speed_min parameter value.
 *
 * @param speed_min Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_speed_min(float32_t speed_min);

/**
 * @brief Get speed_max parameter value.
 *
 * @param speed_max Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_speed_max(float32_t *speed_max);

/**
 * @brief Set speed_max parameter value.
 *
 * @param speed_max Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_speed_max(float32_t speed_max);

/**
 * @brief Get acc_thr parameter value.
 *
 * @param acc_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_acc_thr(int16_t *acc_thr);

/**
 * @brief Set acc_thr parameter value.
 *
 * @param acc_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_acc_thr(int16_t acc_thr);

/**
 * @brief Get count_slow_thr parameter value.
 *
 * @param count_slow_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_count_slow_thr(int16_t *count_slow_thr);

/**
 * @brief Set count_slow_thr parameter value.
 *
 * @param count_slow_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_count_slow_thr(int16_t count_slow_thr);

/**
 * @brief Get count_reverse_thr parameter value.
 *
 * @param count_reverse_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_count_reverse_thr(int16_t *count_reverse_thr);

/**
 * @brief Set count_reverse_thr parameter value.
 *
 * @param count_reverse_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_count_reverse_thr(int16_t count_reverse_thr);

/**
 * @brief Get count_stop_thr parameter value.
 *
 * @param count_stop_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_count_stop_thr(int16_t *count_stop_thr);

/**
 * @brief Set count_stop_thr parameter value.
 *
 * @param count_stop_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_count_stop_thr(int16_t count_stop_thr);

/**
 * @brief Get count_fast_thr parameter value.
 *
 * @param count_fast_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_count_fast_thr(int16_t *count_fast_thr);

/**
 * @brief Set count_fast_thr parameter value.
 *
 * @param count_fast_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_count_fast_thr(int16_t count_fast_thr);

/**
 * @brief Get cfar_slow_thr parameter value.
 *
 * @param cfar_slow_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_cfar_slow_thr(float32_t *cfar_slow_thr);

/**
 * @brief Set cfar_slow_thr parameter value.
 *
 * @param cfar_slow_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_cfar_slow_thr(float32_t cfar_slow_thr);

/**
 * @brief Get cfar_reverse_thr parameter value.
 *
 * @param cfar_reverse_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_cfar_reverse_thr(float32_t *cfar_reverse_thr);

/**
 * @brief Set cfar_reverse_thr parameter value.
 *
 * @param cfar_reverse_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_cfar_reverse_thr(float32_t cfar_reverse_thr);

/**
 * @brief Get cfar_stop_thr parameter value.
 *
 * @param cfar_stop_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_cfar_stop_thr(float32_t *cfar_stop_thr);

/**
 * @brief Set cfar_stop_thr parameter value.
 *
 * @param cfar_stop_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_cfar_stop_thr(float32_t cfar_stop_thr);

/**
 * @brief Get cfar_fast_thr parameter value.
 *
 * @param cfar_fast_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_cfar_fast_thr(float32_t *cfar_fast_thr);

/**
 * @brief Set cfar_fast_thr parameter value.
 *
 * @param cfar_fast_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_cfar_fast_thr(float32_t cfar_fast_thr);

/**
 * @brief Get moving_avg_len parameter value.
 *
 * @param moving_avg_len Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_moving_avg_len(int16_t *moving_avg_len);

/**
 * @brief Set moving_avg_len parameter value.
 *
 * @param moving_avg_len Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_moving_avg_len(int16_t moving_avg_len);
/**
 * @brief Get reverse_dynamic_low_th parameter value.
 *
 * @param reverse_dynamic_low_th Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_reverse_dynamic_low_th(float32_t *reverse_dynamic_low_th);

/**
 * @brief Set reverse_dynamic_low_th parameter value.
 *
 * @param reverse_dynamic_low_th Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_reverse_dynamic_low_th(float32_t reverse_dynamic_low_th);
/**
 * @brief Get reverse_dynamic_high_th parameter value.
 *
 * @param reverse_dynamic_high_th Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_reverse_dynamic_high_th(float32_t *reverse_dynamic_high_th);

/**
 * @brief Set reverse_dynamic_high_th parameter value.
 *
 * @param reverse_dynamic_high_th Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_reverse_dynamic_high_th(float32_t reverse_dynamic_high_th);
/**
 * @brief Get reverse_dynamic_min_val parameter value.
 *
 * @param reverse_dynamic_min_val Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_reverse_dynamic_min_val(int16_t *reverse_dynamic_min_val);

/**
 * @brief Set reverse_dynamic_min_val parameter value.
 *
 * @param reverse_dynamic_min_val Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_reverse_dynamic_min_val(int16_t reverse_dynamic_min_val);

/**
 * @brief Get high_speeding_th parameter value.
 *
 * @param high_speeding_th Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_high_speeding_th(float32_t *high_speeding_th);

/**
 * @brief Set high_speeding_th parameter value.
 *
 * @param high_speeding_th Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_high_speeding_th(float32_t high_speeding_th);

/**
 * @brief Get count_fast_filter_thr parameter value.
 *
 * @param count_fast_filter_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_count_fast_filter_thr(int16_t *count_fast_filter_thr);

/**
 * @brief Set count_fast_filter_thr parameter value.
 *
 * @param count_fast_filter_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_count_fast_filter_thr(int16_t count_fast_filter_thr);

/**
 * @brief Get num_slots parameter value.
 *
 * @param num_slots Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_num_slots(uint8_t *num_slots);

/**
 * @brief Set num_slots parameter value.
 *
 * @param num_slots Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_num_slots(uint8_t num_slots);

/**
 * @brief Get slot_assigned parameter value.
 *
 * @param slot_assigned Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_slot_assigned(uint8_t *slot_assigned);

/**
 * @brief Set slot_assigned parameter value.
 *
 * @param slot_assigned Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_slot_assigned(uint8_t slot_assigned);

/**
 * @brief Get cycle_duration parameter value.
 *
 * @param cycle_duration Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_cycle_duration(uint16_t *cycle_duration);

/**
 * @brief Set cycle_duration parameter value.
 *
 * @param cycle_duration Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_cycle_duration(uint16_t cycle_duration);

/**
 * @brief Get min_battery_thr parameter value.
 *
 * @param min_battery_thr Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_min_batt_thr(uint8_t *min_battery_thr);

/**
 * @brief Set min_battery_thr parameter value.
 *
 * @param min_battery_thr Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_min_batt_thr(uint8_t min_battery_thr);

/**
 * @brief Get event_version parameter value.
 *
 * @param event_version Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_event_version(uint8_t *event_version);

/**
 * @brief Set event_version parameter value.
 *
 * @param event_version Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_event_version(uint8_t event_version);

/**
 * @brief Get dtlg_mode parameter value.
 *
 * @param dtlg_mode Returned value.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_dtlg_mode(uint8_t *dtlg_mode);

/**
 * @brief Set dtlg_mode parameter value.
 *
 * @param dtlg_mode Parameter value to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_dtlg_mode(uint8_t dtlg_mode);

/**
 * @brief Get a complete set of general params
 *
 * @param params Parameter values struct (output).
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_general(param_general_t *params);

/**
 * @brief Set a complete set of general params
 *
 * @param params Parameter values to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_general(param_general_t *params);

/**
 * @brief Get a complete set of cfar params
 *
 * @param params Parameter values struct (output).
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_cfar(param_cfar_t *params);

/**
 * @brief Set a complete set of cfar params
 *
 * @param params Parameter values to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_cfar(param_cfar_t *params);

/**
 * @brief Get a complete set of ext params
 *
 * @param params Parameter values struct (output).
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_ext(param_ext_t *params);

/**
 * @brief Set a complete set of ext params
 *
 * @param params Parameter values to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_ext(param_ext_t *params);

/**
 * @brief Get a complete set of power saving params
 *
 * @param params Parameter values struct (output).
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_get_power(param_power_t *params);

/**
 * @brief Set a complete set of power saving params
 *
 * @param params Parameter values to set.
 *
 * @retval true If transaction is correct.
 * @retval false If an error occurs.
 */
bool param_set_power(param_power_t *params);


#endif /* PARAMS_H_ */
