/**********************************************************************/
/* Freeware Demo SW from the www.dsp-weimich.com                      */
/* Version 1.0                                                        */
/* KalmanFilterExample.c                                              */
/* Support the Kalman Filter Example article                          */
/**********************************************************************/

#include "KalmanFilterExample.h"

/*
 Scalar Kalman Filter
 Input
 float input - input measured signal
 kalman_filter_data_s* kalman_data - Kalman filter data
 Return
 float x[0] estimate value
*/
float KalmanFilterExample(float input, kalman_filter_data_s* kalman_data)
{
	float P_minus[4]; /* matrix 2x2 */
	float x_minus[2]; /* vector 2x1 */
	float K_gain[2];  /* matrix 2x1 */
	float temp_help;
	
	/* Prediction Step */
	x_minus[0] = kalman_data->Phi_matrix[0]*kalman_data->x_plus[0] + kalman_data->Phi_matrix[1]*kalman_data->x_plus[1];
	x_minus[1] = kalman_data->Phi_matrix[2]*kalman_data->x_plus[0] + kalman_data->Phi_matrix[3]*kalman_data->x_plus[1];
	P_minus[0] = (kalman_data->Phi_matrix[0]*kalman_data->P_plus[0] + kalman_data->Phi_matrix[1]*kalman_data->P_plus[2])*kalman_data->Phi_matrix[0];
	P_minus[0] += (kalman_data->Phi_matrix[0]*kalman_data->P_plus[1] + kalman_data->Phi_matrix[1]*kalman_data->P_plus[3])*kalman_data->Phi_matrix[1];
	P_minus[0] += kalman_data->Q_matrix[0];
	P_minus[1] = (kalman_data->Phi_matrix[0]*kalman_data->P_plus[0] + kalman_data->Phi_matrix[1]*kalman_data->P_plus[2])*kalman_data->Phi_matrix[2];
	P_minus[1] += (kalman_data->Phi_matrix[0]*kalman_data->P_plus[1] + kalman_data->Phi_matrix[1]*kalman_data->P_plus[3])*kalman_data->Phi_matrix[3];
	P_minus[1] += kalman_data->Q_matrix[1];
	P_minus[2] = (kalman_data->Phi_matrix[2]*kalman_data->P_plus[0] + kalman_data->Phi_matrix[3]*kalman_data->P_plus[2])*kalman_data->Phi_matrix[0];
	P_minus[2] += (kalman_data->Phi_matrix[2]*kalman_data->P_plus[1] + kalman_data->Phi_matrix[3]*kalman_data->P_plus[3])*kalman_data->Phi_matrix[1];
	P_minus[2] += kalman_data->Q_matrix[2];
	P_minus[3] = (kalman_data->Phi_matrix[2]*kalman_data->P_plus[0] + kalman_data->Phi_matrix[3]*kalman_data->P_plus[2])*kalman_data->Phi_matrix[2];
	P_minus[3] += (kalman_data->Phi_matrix[2]*kalman_data->P_plus[1] + kalman_data->Phi_matrix[3]*kalman_data->P_plus[3])*kalman_data->Phi_matrix[3];
	P_minus[3] += kalman_data->Q_matrix[3];
	/* Kalman Gain */
	temp_help = (kalman_data->H_matrix[0]*P_minus[0] + kalman_data->H_matrix[1]*P_minus[2])*kalman_data->H_matrix[0];
	temp_help += (kalman_data->H_matrix[0]*P_minus[1] + kalman_data->H_matrix[1]*P_minus[3])*kalman_data->H_matrix[1];
	temp_help += kalman_data->R_matrix;
	K_gain[0] = (kalman_data->H_matrix[0]*P_minus[0] + kalman_data->H_matrix[1]*P_minus[1])/temp_help; /* temp_help shall be !=0 */
	K_gain[1] = (kalman_data->H_matrix[0]*P_minus[2] + kalman_data->H_matrix[1]*P_minus[3])/temp_help;
	/* Correction Step */
	kalman_data->P_plus[0] = (1.0 - K_gain[0]*kalman_data->H_matrix[0])*P_minus[0] - K_gain[0]*kalman_data->H_matrix[1]*P_minus[2];
	kalman_data->P_plus[1] = (1.0 - K_gain[0]*kalman_data->H_matrix[0])*P_minus[1] - K_gain[0]*kalman_data->H_matrix[1]*P_minus[3];
	kalman_data->P_plus[2] = -K_gain[1]*kalman_data->H_matrix[0]*P_minus[0] + (1.0 - K_gain[1]*kalman_data->H_matrix[1])*P_minus[2];
	kalman_data->P_plus[3] = -K_gain[1]*kalman_data->H_matrix[0]*P_minus[1] + (1.0 - K_gain[1]*kalman_data->H_matrix[1])*P_minus[3];
	kalman_data->x_plus[0] = x_minus[0] + K_gain[0]*(input - x_minus[0]);
	kalman_data->x_plus[1] = x_minus[1] + K_gain[1]*(input - x_minus[0]);
	
	return kalman_data->x_plus[0];
}

/* Only Test */
int main()
{
	uint16 index;

	/* Cosine + White Noise */
	float testSignal[120]=
	{
         1.044425,  0.732202,  1.107485,  0.832648,  0.997930, 
         0.743661,  0.928340,  0.546396,  0.510484,  0.114057, 
         0.181101, -0.103150, -0.561716, -0.307147, -0.921523, 
        -0.626582, -0.817900, -0.899243, -1.011869, -0.948660, 
        -1.024685, -1.078591, -0.854667, -0.905492, -0.548572, 
        -0.737286, -0.450227, -1.032790, -0.293712, -0.159926, 
        -0.117551,  0.363302,  0.637550,  0.284187,  0.737519, 
         0.521005,  0.893311,  1.054081,  0.525526,  0.782144, 
         1.009261,  0.967746,  1.229197,  1.179795,  0.998553, 
         0.806697,  0.615047,  0.003506, -0.023320,  0.510523, 
        -0.168222, -0.358753, -0.424160, -0.492052, -0.425293, 
        -0.460660, -0.712114, -1.311701, -0.815038, -0.827395, 
        -0.946493, -0.962119, -0.849309, -0.733573, -0.680960, 
        -0.742870, -0.529109, -0.567446, -0.136456, -0.199753, 
         0.169624,  0.231133,  0.686898,  0.284297,  0.491547, 
         0.766148,  0.808002,  1.106872,  1.134952,  0.806723, 
         1.119804,  0.931759,  1.326805,  0.890237,  0.653111, 
         0.973650,  0.903934,  0.350950, -0.088230, -0.208081, 
         0.084471, -0.272629, -0.352162, -0.748851, -0.887943, 
        -0.736057, -0.939653, -1.119212, -0.944213, -0.761944, 
        -0.988381, -0.681791, -0.900586, -1.064615, -0.843333, 
        -0.718142, -0.138929, -0.583088, -0.460869,  0.050059, 
         0.101613,  0.150226,  0.593740,  0.454790,  0.612092, 
         0.829426,  0.899205,  1.024959,  1.159378,  0.977459
	};
	
	/* Kalman Structure Initialization */
	kalman_filter_data_s kalman_data = 
	{
		/* Transition matrix: 2x2 */
		/* float Phi_matrix[4]; */
		{1.0, 0.25e-3, 0.0, 1.0},
		/* Q covariance plant noise matrix: 2x2 */
		/* float Q_matrix[4]; */
		{0.0, 0.0, 0.0, 1082.323},		
		/* Sensitivity matrix: 1X2 */
		/* float H_matrix[2]; */
		{1.0, 0.0},		
		/* Observation noise: R covariance matrix 1x1 */
		/* float R_matrix; */
		0.04,
		/* P plus current covariance matrix 2x2: estimate error */
		/* float P_plus[4]; */
		{0.04, 160.0, 160.0, 641082.323},
		/* x plus current state vector 2x1: value, speed */
		/* float x_plus[2]; */
		{0.0, 0.0},
	};

	/* Kalman Filter Test */
	for(index = 0; index < (sizeof(testSignal)/sizeof(float)); index++)
	{
		printf("%f\t%d\n",testSignal[index],index);
		testSignal[index] = KalmanFilterExample(testSignal[index], &kalman_data);
		
	}

    return 0;
}
