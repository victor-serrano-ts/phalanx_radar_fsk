/*
 * math_util.h
 *
 *  Created on: Sep 4, 2023
 *      Author: victor
 */

#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

#include "main.h"

/**
 * @brief Compute the moving average.
 *
 * @param value New value to include in the moving average
 * @param index Pointer to index.
 * @param buffer Pointer to moving average buffer
 * @param size Size of the moving average buffer
 *
 * @return Moving average
 */
float moving_average(float value, int* index, float* buffer, uint32_t size);

#endif /* MATH_UTIL_H_ */
