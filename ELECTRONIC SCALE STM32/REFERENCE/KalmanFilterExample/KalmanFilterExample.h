/**********************************************************************/
/* Freeware Demo SW from the www.dsp-weimich.com                      */
/* Version 1.0                                                        */
/* KalmanFilterExample.h                                              */
/* Support the Kalman Filter Example article                          */
/**********************************************************************/

#include<stdio.h>

/* Export defines */
#define uint8    unsigned char 
#define sint8    signed char
#define uint16   unsigned int
#define sint16   signed int
#define uint32   unsigned long
#define sint32   signed long

/* Export structures */
typedef struct kalman_filter_data
{
	/* Transition matrix: 2x2 */
    float Phi_matrix[4];
	/* Q covariance plant noise matrix: 2x2 */
    float Q_matrix[4];
	/* Sensitivity matrix: 1X2 */
    float H_matrix[2];
	/* Observation noise: R covariance matrix 1x1 */
    float R_matrix;
	/* P plus current covariance matrix 2x2: estimate error */
    float P_plus[4];
	/* x plus current state vector 2x1: value, speed */
    float x_plus[2];
} kalman_filter_data_s;


/* Export function */
extern float KalmanFilterExample(float, kalman_filter_data_s*);
