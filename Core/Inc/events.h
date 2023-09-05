/*
 * events.h
 *
 *  Created on: Aug 29, 2023
 *      Author: victor
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include "main.h"
#include "fsk_processing.h"
#include "params.h"

/** @brief Possible events that can be detected. */
typedef enum
{
    EVENT_NONE = 0,
    EVENT_STOPPED_REV = 1,
    EVENT_STOPPED = 2,
    EVENT_SLOW = 3,
    EVENT_FAST = 4,
    EVENT_REVERSE = 6,
    EVENT_PEDESTRIAN_ANIMAL = 7,
    EVENT_HIGH_SPEEDING = 8,
    EVENT_IMPACT = 9,
    EVENT_SIMULATED = 10,
    EVENT_ALARM = 11,
} event_t;

/** @brief Possible status for each event. */
typedef enum
{
    EVENT_RESULT_NEGATIVE = -1,
    EVENT_RESULT_NOT_THIS_EVENT = 0,
    EVENT_RESULT_POSITIVE = 1,
} event_result_t;

/* Functions */

/**
 * @brief Set default detection parameters.
 */
void set_default_params(void);

/**
 * Check the radar and accelerometer data to detect events.
 *
 * @param radar Radar data to analyze.
 * @param acc_max Maximum acceleration to be analyzed.
 *
 * @return Event detected through analyzed data (@ref event_t).
 */
event_t event_detect(const fsk_result_t *radar_data, int16_t acc_max);

/**
 * Check for an event change after the first detection.
 * If it changes (for example, from EVENT_SLOW to EVENT_STOPPED)
 * it will return the new one.
 *
 * @param radar Radar data to analyze.
 * @param acc_max Maximum acceleration to be analyzed.
 *
 * @return Event detected through analyzed data (@ref event_t).
 */
event_t wait_for_more_events(const fsk_result_t *radar_data, int16_t acc_max);

/**
 * @brief Reset the event state.
 */
void event_reset(void);

/**
 * @brief Adjust the speed according to the radar angle.
 *
 * @param[in] speed Event speed in km/h.
 *
 * @return Speed with angle correction.
 */
float32_t event_speed_adjust(float speed);

#endif /* EVENTS_H_ */
