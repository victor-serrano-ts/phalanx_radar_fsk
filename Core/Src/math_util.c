/*
 * math_util.c
 *
 *  Created on: Sep 4, 2023
 *      Author: victor
 */

#include "math_util.h"


float32_t moving_average(float32_t value, int* index, float32_t* buffer, uint32_t size)
{
	float32_t result;

	if (size <= 1) {
			return value;
	}

	buffer[*index] = value;
	*index = (*index + 1) % size;

	arm_mean_f32(buffer, size, &result);

	return result;
}
