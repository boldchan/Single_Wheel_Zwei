/*
 * filter.h
 *
 *  Created on: Aug 12, 2015
 *      Author: cp
 */

#ifndef FILTER_H_
#define FILTER_H_
extern float SEq_1, SEq_2, SEq_3, SEq_4; 		// estimated orientation quaternion elements with initial conditions

extern void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z) ;

#endif /* FILTER_H_ */
