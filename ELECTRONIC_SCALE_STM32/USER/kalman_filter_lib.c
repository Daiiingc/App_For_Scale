#include "kalman_filter_lib.h"
#include <stdint.h>
#include <math.h>

static float _err_measure;
static float _err_estimate;
static float _q;
static float _current_estimate;
static float _last_estimate;
static float _kalman_gain;

/*
 SimpleKalmanFilter(e_mea, e_est, q);
 e_mea: Muc do khong chac chan cua phep do (mong doi vào phép do) 
 e_est: Uoc tính không chac chan (setup giong e_mea, thu vien se tu hieu chinh) 
 q: Phuong sai. Recommend 0,001 - 1 (thuong là 0,001), tùy vào du án hieu chinh cho phù hop
 */
void SimpleKalmanFilter(float mea_e, float est_e, float q){
	_err_measure = mea_e;
	_err_estimate = est_e;
	_q = q;
}


float updateEstimate(float mea){
	_kalman_gain = _err_estimate/(_err_estimate + _err_measure);
	_current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
	_err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
	_last_estimate=_current_estimate;
	return _current_estimate;
}

void setMeasurementError(float mea_e){
 _err_measure = mea_e;
}
void setEstimateError(float est_e){
 _err_estimate = est_e;
}

void setProcessNoise(float q){
 _q = q;
}

float getKalmanGain(void) {
 return _kalman_gain;
}

float getEstimateError(void){
	return _err_estimate;
} 

//SimpleKalmanFilter(100, 100, 0.05);	
//updateEstimate(testSignal[i]);







