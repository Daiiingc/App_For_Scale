/*
	**********************************************
	*@file				kalman_filter_lib.h					*
	*@author			Vu Quang Luong				*
	*@date				15/07/2023						*
	SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
	Created by Denys Sene, January, 1, 2017.
	Released under MIT License - see LICENSE file for details.
	Class SimpleKalmanFilter
	
	**********************************************
*/

#ifndef	_KALMAN_
#define	_KALMAN_

#ifdef __cplusplus
	extern "C"{
#endif

#include"stdio.h"

void SimpleKalmanFilter(float mea_e, float est_e, float q);
float updateEstimate(float mea);
void setMeasurementError(float mea_e);
void setEstimateError(float est_e);
void setProcessNoise(float q);
float getKalmanGain(void);
float getEstimateError(void);

#ifdef __cplusplus
}
#endif

#endif



/***********************************************/

 
 
 
 

