/*
 * events.c
 *
 *  Created on: Aug 29, 2023
 *      Author: victor
 */


#include "events.h"
#include "math_util.h"


/** Flag that indicates if a simulated event has been queried. */
static bool b_event_simulate = false;

/** Threshold of FFT magnitude for slow events. All magnitudes below this value
 * will be ignored. */
static float event_bin_slow_thr;

/** Threshold of FFT magnitude for reverse events. All magnitudes below this
 * value will be ignored. */
static float event_bin_reverse_thr;

/** Threshold of FFT magnitude for stopped events. All magnitudes below this
 * value will be ignored. */
static float event_bin_stop_thr;

/** Threshold of FFT magnitude for fast events. All magnitudes below this
 * value will be ignored. */
static float event_bin_fast_thr;

/** Minimum speed to detect motion (units in km/h). */
static float event_speed_min;

/** Maximum speed to detect motion (units in km/h). */
static float event_speed_max;

/** Acceleration threshold (units in cm/s<SUP>2</SUP>). */
static int16_t event_acc_thr;

/** Number of consecutive equal events needed to trigger a slow event
 * detection. */
static int16_t event_count_slow_thr;

/** Number of consecutive equal events needed to trigger a reverse event
 * detection. */
static int16_t event_count_reverse_thr;

/** Number of consecutive equal events needed to trigger a stop event
 * detection. */
static int16_t event_count_stop_thr;

/** Number of events to wait after a fast speed detection. */
static int16_t event_count_fast_thr;

/** Threshold of CFAR ratio for slow events. All ratios below this value will
 * be ignored. */
static float event_cfar_slow_thr;

/** Threshold of CFAR ratio for reverse events. All ratios below this value will
 * be ignored. */
static float event_cfar_reverse_thr;

/** Threshold of CFAR ratio for stopped events. All ratios below this value will
 * be ignored. */
static float event_cfar_stop_thr;

/** Threshold of CFAR ratio for fast events. All ratios below this value will
 * be ignored. */
static float event_cfar_fast_thr;

/** Scale to be applied for angle correction. */
static float event_angle_scale;

/** Latest event detected. */
static event_t event_latest;

/** Number of consecutive events. */
static int16_t event_count;

/** Current event speed. */
static float event_speed;

/** Current event bin_level. */
static float event_bin_level;

/** Current event reported. */
static bool event_reported;

/** Number of event cycles since the latest detected high speed object. */
static uint32_t event_count_fast;

/** Number of elements for the moving average */
static int16_t event_moving_avg_len;

/** Reverse dynamic low threshold */
static float event_reverse_dynamic_low_th;

/** Reverse dynamic high threshold */
static float event_reverse_dynamic_high_th;

/** Reverse dynamic min value */
static int16_t event_reverse_dynamic_min_val;

/** High speeding low threshold */
static float event_high_speeding_th;

/** Counter to ignore some events after a fast event */
static int16_t event_count_fast_filter_thr;

/** Buffer for moving average of bin_level */
static float last_bin_level[PARAM_INIT_MOVING_AVERAGE_MAX_LEN];

/** Current index for moving average of bin_level buffer */
static int last_bin_level_index;

/** Buffer for moving average of bin_level */
static float last_cfar_level[PARAM_INIT_MOVING_AVERAGE_MAX_LEN];

/** Current index for moving average of bin_level buffer */
static int last_cfar_level_index;

/** Current event speed. */
static float detected_event_speed;

/** Current event bin_level. */
static float detected_event_bin_level;

/** Latest event detected. */
static event_t detected_event;


void set_default_params(void)
{
  event_bin_slow_thr = PARAM_INIT_BIN_SLOW_THR;
  event_bin_reverse_thr = PARAM_INIT_BIN_REVERSE_THR;
  event_bin_stop_thr = PARAM_INIT_BIN_STOP_THR;
  event_bin_fast_thr = PARAM_INIT_BIN_FAST_THR;
  event_speed_min = PARAM_INIT_SPEED_MIN;
  event_speed_max = PARAM_INIT_SPEED_MAX;
  event_acc_thr = PARAM_INIT_ACC_THR;
  event_count_slow_thr = PARAM_INIT_COUNT_SLOW_THR;
  event_count_reverse_thr = PARAM_INIT_COUNT_REVERSE_THR;
  event_count_stop_thr = PARAM_INIT_COUNT_STOP_THR;
  event_count_fast_thr = PARAM_INIT_COUNT_FAST_THR;
  event_cfar_slow_thr = PARAM_INIT_CFAR_SLOW_THR;
  event_cfar_reverse_thr = PARAM_INIT_CFAR_REVERSE_THR;
  event_cfar_stop_thr = PARAM_INIT_CFAR_STOP_THR;
  event_cfar_fast_thr = PARAM_INIT_CFAR_FAST_THR;
  event_moving_avg_len = PARAM_INIT_MOVING_AVERAGE_LEN;
  event_reverse_dynamic_low_th = PARAM_INIT_REVERSE_DYN_LOW_TH;
  event_reverse_dynamic_high_th = PARAM_INIT_REVERSE_DYN_HIGH_TH;
  event_reverse_dynamic_min_val = PARAM_INIT_REVERSE_DYN_MIN_VAL;
  event_high_speeding_th = PARAM_INIT_HIGH_SPEEDING_TH;
  event_count_fast_filter_thr = PARAM_INIT_COUNT_FAST_FILTER_THR;

  // Apply angle scale
  event_angle_scale = 1.0 / cos(45.0 * M_PI / 180.0);
  event_speed_min /= event_angle_scale;
  event_speed_max /= event_angle_scale;
  event_reverse_dynamic_low_th /= event_angle_scale;
  event_reverse_dynamic_high_th /= event_angle_scale;
  event_high_speeding_th /= event_angle_scale;
}


static uint16_t event_dynamic_threshold(float32_t speed, int16_t threshold)
{
    // Sanitize inputs to avoid dividing by zero
    if (event_reverse_dynamic_min_val == 0 ||
            threshold == event_reverse_dynamic_min_val ||
        event_reverse_dynamic_high_th <= event_reverse_dynamic_low_th ||
        event_reverse_dynamic_min_val >= threshold) {
        return threshold;
    }

    if (speed < event_reverse_dynamic_low_th) {
        return threshold;
    } else if (speed > event_reverse_dynamic_high_th) {
        return event_reverse_dynamic_min_val;
    } else {
        return threshold - ((speed - event_reverse_dynamic_low_th)/
                                              ((event_reverse_dynamic_high_th-event_reverse_dynamic_low_th)/
                                               (threshold-event_reverse_dynamic_min_val))
                                         );
    }
}

static inline void evaluate_fast_event(float32_t speed, float32_t bin_level, float32_t cfar_level, fsk_motion_t motion)
{
	if ((bin_level > event_bin_fast_thr) &&
			(cfar_level > event_cfar_fast_thr) &&
			(speed > event_speed_max))
	{
		event_count_fast = 0;
	}
	else
	{
		event_count_fast++;
	}
}

static inline bool recent_fast_event()
{
	return event_count_fast <= event_count_fast_filter_thr;
}

static void event_clear()
{
	event_latest = EVENT_NONE;
	event_count = 0;
	event_speed = 0;
	event_bin_level = 0;
	event_reported = false;
}

event_result_t event_slow(float32_t speed, float32_t bin_level, float32_t cfar_level, fsk_motion_t motion)
{
	if (recent_fast_event()) {
		return EVENT_RESULT_NOT_THIS_EVENT;
	}

	if (speed < event_speed_max &&
			speed > event_speed_min &&
			motion == MOTION_APPROACHING) {
		if ((bin_level < event_bin_slow_thr) ||
				(cfar_level < event_cfar_slow_thr)) {
			return EVENT_RESULT_NEGATIVE;
		} else {
			return EVENT_RESULT_POSITIVE;
		}
	} else {
		return EVENT_RESULT_NOT_THIS_EVENT;
	}
}

event_result_t event_stop(float32_t speed, float32_t bin_level, float32_t cfar_level, fsk_motion_t motion)
{
	if (recent_fast_event()) {
		return EVENT_RESULT_NOT_THIS_EVENT;
	}

	if (speed < event_speed_min) {
		if ((bin_level < event_bin_stop_thr) ||
						(cfar_level < event_cfar_stop_thr)) {
			return EVENT_RESULT_NEGATIVE;
		} else {
			return EVENT_RESULT_POSITIVE;
		}
	} else {
		return EVENT_RESULT_NOT_THIS_EVENT;
	}
}

event_result_t event_reverse(float32_t speed, float32_t bin_level, float32_t cfar_level, fsk_motion_t motion)
{
	if (motion == MOTION_DEPARTING &&
			speed > event_speed_min) {
		if ((bin_level < event_bin_reverse_thr) ||
				(cfar_level < event_cfar_reverse_thr)) {
			return EVENT_RESULT_NEGATIVE;
		} else {
			return EVENT_RESULT_POSITIVE;
		}
	} else {
		return EVENT_RESULT_NOT_THIS_EVENT;
	}
}

event_result_t event_fast(float32_t speed, float32_t bin_level, float32_t cfar_level, fsk_motion_t motion)
{
	if (motion == MOTION_APPROACHING &&
			speed > event_speed_max &&
			speed < event_high_speeding_th) {
		if ((bin_level < event_bin_fast_thr) ||
						(cfar_level < event_cfar_fast_thr)) {
			return EVENT_RESULT_NEGATIVE;
		} else {
			return EVENT_RESULT_POSITIVE;
		}
	} else {
		return EVENT_RESULT_NOT_THIS_EVENT;
	}
}

event_result_t event_high_speeding(float32_t speed, float32_t bin_level, float32_t cfar_level, fsk_motion_t motion)
{
	if (motion == MOTION_APPROACHING &&
			speed > event_high_speeding_th) {
		if ((bin_level < event_bin_fast_thr) ||
				(cfar_level < event_cfar_fast_thr)) {
			return EVENT_RESULT_NEGATIVE;
		} else {
			return EVENT_RESULT_POSITIVE;
		}
	} else {
		return EVENT_RESULT_NOT_THIS_EVENT;
	}
}


event_result_t event_impact(int16_t acc_max) {
	//debug_printf("Impact: %d\r\n", acc_max);
	if (acc_max > event_acc_thr) {
		return EVENT_RESULT_POSITIVE;
	}
	return EVENT_RESULT_NOT_THIS_EVENT;
}

event_t event_detect(const fsk_result_t *radar_data, int16_t acc_max)
{
  event_t event = EVENT_NONE;
  int16_t count_thr = 1;
  event_result_t result = EVENT_RESULT_NOT_THIS_EVENT;

  float32_t cfar_level = 0.0;
  float32_t averaged_cfar_level = 0.0;

  float32_t averaged_bin_level = moving_average(radar_data->bin_level,
                                            &last_bin_level_index,
                                            last_bin_level,
                                            event_moving_avg_len);

  if (radar_data->noise_level > 1.0) {
		cfar_level = radar_data->bin_level / radar_data->noise_level;
  } else {
		cfar_level = radar_data->bin_level;
  }

  averaged_cfar_level = moving_average(cfar_level,
                                       &last_cfar_level_index,
                                       last_cfar_level,
                                       event_moving_avg_len);

  // When bin level is very low, use its value instead of the average
  // as the speed value may be very unreliable
  if (radar_data->bin_level <= 2.0) {
		averaged_bin_level = radar_data->bin_level;
  }

  // Fast events detection to filter slow or stop events which are the
  // last samples of a fast event
  evaluate_fast_event(radar_data->speed_kmh, averaged_bin_level, averaged_cfar_level, radar_data->motion);

  if (b_event_simulate)
  {
		b_event_simulate = false;
		event = EVENT_SIMULATED;
		result = EVENT_RESULT_POSITIVE;
  }
  else if (result = event_impact(acc_max))
  {
		event = EVENT_IMPACT;
  }
  else if (result = event_stop(radar_data->speed_kmh, averaged_bin_level, averaged_cfar_level, radar_data->motion))
  {
		// The speed direction should not change when detecting a stopped object
		// to avoid vibrating environment (traffic signals, vegetation)
		if (radar_data->motion == MOTION_DEPARTING)
		{
			event = EVENT_STOPPED_REV;
		}
		else
		{
			// A very slow object is considered a pedestrian or animal.
			// To determine if it's a vehicle, we check if we got a slow
			// vehicle first
			event = EVENT_PEDESTRIAN_ANIMAL;
		}

		count_thr = event_count_stop_thr;
  }
  else if (result = event_reverse(radar_data->speed_kmh, averaged_bin_level, averaged_cfar_level, radar_data->motion))
  {
		event = EVENT_REVERSE;
		count_thr = event_dynamic_threshold(radar_data->speed_kmh, event_count_reverse_thr);
  }
  else if (result = event_slow(radar_data->speed_kmh, averaged_bin_level, averaged_cfar_level, radar_data->motion))
  {
		event = EVENT_SLOW;
		count_thr = event_count_slow_thr;
  }
  else if (result = event_fast(radar_data->speed_kmh, averaged_bin_level, averaged_cfar_level, radar_data->motion))
  {
		event = EVENT_FAST;
		count_thr = event_dynamic_threshold(radar_data->speed_kmh, event_count_fast_thr);
  }
  else if (result = event_high_speeding(radar_data->speed_kmh, averaged_bin_level, averaged_cfar_level, radar_data->motion))
  {
		event = EVENT_HIGH_SPEEDING;
		count_thr = event_dynamic_threshold(radar_data->speed_kmh, event_count_fast_thr);
  }

  if (event_latest != event)
  {
		// Clear the event state
		event_clear();
		event_latest = event;
  }

  // Update the current event counter
  event_count += result;

  if (event_count <= 0)
  {
		event_clear();
  }

  if (result == EVENT_RESULT_POSITIVE)
  {
		// Update the current event speed
		if (radar_data->speed_kmh > event_speed)
		{
			event_speed = radar_data->speed_kmh;
		}

		// Update the current event bin_level
		if (averaged_bin_level > event_bin_level)
		{
			event_bin_level = averaged_bin_level;
		}
  }

  // Consecutive events count check
  if (event_count < count_thr || event_reported)
  {
    event = EVENT_NONE;
  }

  if (event == EVENT_FAST)
  {
		// As fast events don't stop sampling, store its data
		// But only when there is no previous detected event
		if (detected_event == EVENT_NONE) {
				detected_event_speed = event_speed;
				detected_event_bin_level = event_bin_level;
		}
  }

  // Only one type of stopped event is notified
  if (event == EVENT_STOPPED_REV)
  {
      event = EVENT_PEDESTRIAN_ANIMAL;
  }

  if (event != EVENT_NONE)
  {
      event_reported = true;
  }

  return event;
}

event_t wait_for_more_events(const fsk_result_t *radar_data, int16_t acc_max)
{
    event_t new_event;

    if (detected_event == EVENT_NONE) {
        // Only one type of stopped event is notified
        if (event_latest == EVENT_STOPPED_REV) {
            detected_event = EVENT_PEDESTRIAN_ANIMAL;
        } else {
            detected_event = event_latest;
        }
        detected_event_speed = event_speed;
        detected_event_bin_level = event_bin_level;
    }

    new_event = event_detect(radar_data, acc_max);

    // From slow to very slow in either direction -> Stop
    if (detected_event == EVENT_SLOW) {
        if (new_event == EVENT_PEDESTRIAN_ANIMAL) {
            detected_event = EVENT_STOPPED;
        }
    // From very slow to reverse -> Reverse
    } else if (detected_event == EVENT_PEDESTRIAN_ANIMAL) {
        if (new_event == EVENT_REVERSE) {
            detected_event = EVENT_REVERSE;
        }
    }
    // More special cases can be added here if needed

    return detected_event;
}

void event_reset(void)
{
    event_latest = EVENT_NONE;
    event_count = 0;
    event_speed = 0;
    event_bin_level = 0;
    event_count_fast = 0;
    b_event_simulate = false;
    memset(last_bin_level, 0, sizeof(last_bin_level));
    memset(last_cfar_level, 0, sizeof(last_cfar_level));
    detected_event = EVENT_NONE;
    detected_event_speed = 0;
    detected_event_bin_level = 0;
    event_reported = false;
}

float event_speed_adjust(float speed)
{
    return speed * event_angle_scale;
}
