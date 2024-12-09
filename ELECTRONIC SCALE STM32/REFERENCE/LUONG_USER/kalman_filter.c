#include "stdio.h"
#include "math.h"

#define PI 3.1415

//float32 dt = 0.01; // T Sampling
//float32 Q_angle = 0.005;
//float32 Q_bias = 0.003;
//float32 R_measure = 0.03;
//float32 bias = 0; // Reset bias
//float32 rate;
//float32 angle;
//float32 S; // estimate error
//float32 y; // different angle
//float32 P_00 = 0 , P_01 = 0 , P_10 =0 ,P_11 =0;
//float32 K_0 =0,K_1=0; // Kalman gain
//float32 Kalman(float32 newAngle, float32 newRate){ // Discrete Kalman filter time update equations - Time Update ("Predict") // Update xhat - Project the state ahead 
///* Step 1 */
////
//	angle = X_Raw_Gyro_Angle;
//	rate = newRate - bias;
//	angle += dt * rate; // Update estimation error covariance - Project the error covariance ahead
///* Step 2 */
//	P_00 += dt * ( dt*P_11 - P_10 - P_01 + Q_angle);
//	P_01 -= dt * P_11;
//	P_10 -= dt * P_11;
//	P_11 += Q_bias * dt;	// Discrete Kalman filter measurement update equations - Measurement Update ("Correct") // Calculate Kalman gain - Compute the Kalman gain 
///* Step 4 */
//	S = P_00 + R_measure;
///* Step 5 */
//	K_0 = P_00 / S;
//	K_1 = P_10 / S; // Calculate angle and bias - Update estimate with measurement zk (newAngle)
///* Step 3 */
//	y = newAngle - angle;
///* Step 6 */
//	angle += K_0 * y;
//	bias += K_1 * y; // Calculate estimation error covariance - Update the error covariance
///* Step 7 */
//	P_00 -= K_0 * P_00;
//	P_01 -= K_0;
//	
	