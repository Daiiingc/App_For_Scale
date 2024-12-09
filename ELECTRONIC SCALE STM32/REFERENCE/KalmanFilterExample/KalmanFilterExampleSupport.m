%*********************************************************************/
% Freeware Demo SW from the www.dsp-weimich.com                      */
% Version 1.0                                                        */
% KalmanFilterExampleSupport.m                                       */
% Octave Script                                                      */
% High-level interactive language for numerical computations         */
% Support the Kalman Filter with Example article                     */
%*********************************************************************/
close all
clear all
clc all

pkg load control
pkg load signal
pkg load communications

%--------------------------
% Kalman Filter Parameters
%--------------------------
Tsample = 0.25e-3;        % 0.25ms
Fband_in_Hz = 100;        % signal model: 0...100Hz
Um_ampl = 1;              % 1V
Variance_m = (Um_ampl*Tsample*(2*pi*Fband_in_Hz)^2/3)^2; % maneuvering^2
Variance_o = 0.04;        % measurement noise in V^2
Phi_matrix = [1 Tsample; 0 1]; % transition matrix
H_matrix = [1 0];         % sensitivity matrix
Q_matrix = [0 0; 0 Variance_m]; % plant covariance matrix
R_matrix = Variance_o;    % observation covariance matrix
P_start = [Variance_o Variance_o/Tsample; Variance_o/Tsample (Variance_o/Tsample^2)+Variance_m]; % P_0
x_state_start = [0;0];    % state_0

% Float Test Signal Table to File
% Support C-Code
% Input parameters:
% Amplitude in Volt
% Variance_o - observation noise power
% Frequency - 2*pi*f*Ts
% FileNameString - output file name
% Output:
% txt file with the signal data
% testSignal = Amplitude*cos(Frequency*n) + observationWhiteNoise
function testSignal = FloatTestSignal2file(Amplitude, Variance_o, Frequency, FileNameString)
   periodNumber = 3; % cosine period number
   signalLength = int64(2*pi*periodNumber/Frequency);
   testSignal = zeros(1, signalLength);
   nmb_in_line = 5; % output file: numbers in line
   fileDescriptor = fopen(FileNameString, 'wt');
   % Cosine signal
   signal_orig = Amplitude*cos(Frequency*[0:signalLength-1]);
   % Cosine plot
   figure();
   plot(signal_orig);
   grid();
   title ("Original Signal");
   xlabel ("Samples");
   ylabel ("Original Signal");
   xlim("manual");
   xlim([0 signalLength-1]);
   % Disturb noise
   whiteNoise = wgn(1,signalLength,10*log10(Variance_o));
   % Noise plot
   figure();
   plot(whiteNoise);
   grid();
   title ("Observation - white noise");
   xlabel ("Samples");
   ylabel ("o - noise");
   xlim("manual");
   xlim([0 signalLength-1]);
%   var(whiteNoise) % only debugging: noise power
   % Input signal: cosine + white noise
   testSignal = signal_orig + whiteNoise;
   % Input signal plot 
   figure();
   plot(testSignal);
   grid();
   title ("Signal + Noise");
   xlabel ("Samples");
   ylabel ("Signal + Noise");
   xlim("manual");
   xlim([0 signalLength-1]);
   % Save the input signal in the file. C-code support   
   for index = 0:signalLength-2
     if(mod(index,nmb_in_line) == 0)
        fprintf(fileDescriptor, "\n        ");
     end
     fprintf(fileDescriptor, "%9f, ", testSignal(index+1));
   end
   % Last number without comma
   if(mod(signalLength-1,nmb_in_line) == 0)
      fprintf(fileDescriptor, "\n        ");
   end
   fprintf(fileDescriptor, "%9f\n", testSignal(end));
   fclose(fileDescriptor);
endfunction

% Kalman Filter Example
% Input parameters:
% signalIn   - Input Signal
% Phi_matrix - transition matrix
% Q_matrix   - covariance matrix of the plant noise 
% H_matrix   - measurement sensitivity matrix
% R_matrix   - covariance matrix of the observation noise
% P0         - start value: covariance matrix of the estimate error
% x0         - start value of the x state vector
% Output:
% signalOut  - output of the Kalman Filter
% KalmanGain - Kalman Gain
% P_Covariance - P covariance: Estimate Error 
function [signalOut, KalmanGain, P_Covariance] = KalmanFilterExample(signalIn, Phi_matrix, Q_matrix, H_matrix, R_matrix, P0, x0)
   % Initialization
   I_matrix = eye(2);
   signalLength = length(signalIn);
   signalOut = zeros(2, signalLength);
   KalmanGain = zeros(2, signalLength);
   P_Covariance = zeros(2, 2, signalLength);
   x_plus = x0; 
   P_plus = P0;
   % Kalman Filter
   for index = 1:signalLength
     x_minus = Phi_matrix * x_plus;
     P_minus = Phi_matrix * P_plus * Phi_matrix.' + Q_matrix;
     K_gain = P_minus * H_matrix.' * (H_matrix * P_minus * H_matrix.' + R_matrix)^-1;
     P_plus = (I_matrix - K_gain * H_matrix) * P_minus;
     x_plus = x_minus + K_gain * (signalIn(index) - H_matrix * x_minus);
     signalOut(:,index) = x_plus;
     KalmanGain(:,index) = K_gain;
     P_Covariance(:,:,index) = P_plus;
   end
   % Results Plot
   % Kalman Filter Output of the Signal Value
   figure();
   plot(signalOut(1,:));
   grid();
   title ("Kalman Filter: Estimate of the Signal Value");
   xlabel ("Samples");
   ylabel ("Value");
   xlim("manual");
   xlim([0 signalLength-1]);
##   % Kalman Filter Output of the Signal Speed
##   figure();
##   plot(signalOut(2,:));
##   grid();
##   title ("Kalman Filter: Estimate of the Signal Speed");
##   xlabel ("Samples");
##   ylabel ("Speed");
##   xlim("manual");
##   xlim([0 signalLength-1]);
   % Kalman Gain of the Signal Value
   figure();
   plot(KalmanGain(1,:));
   grid();
   title ("Kalman Filter: Kalman Gain of the Signal Value");
   xlabel ("Samples");
   ylabel ("K Gain of the Value");
   xlim("manual");
   xlim([0 signalLength-1]);
##   % Kalman Gain of the Signal Speed
##   figure();
##   plot(KalmanGain(2,:));
##   grid();
##   title ("Kalman Filter: Kalman Gain of the Signal Speed");
##   xlabel ("Samples");
##   ylabel ("K Gain of the Speed");
##   xlim("manual");
##   xlim([0 signalLength-1]);
   % P Estimate Covariance Error: Value 
   figure();
   plot(P_Covariance(1,1,:));
   grid();
   title ("Kalman Filter: P Covariance: Estimate Value Error");
   xlabel ("Samples");
   ylabel ("Estimate Value Error Variance");
   xlim("manual");
   xlim([0 signalLength-1]);
##   % P Estimate Covariance Error: Speed 
##   figure();
##   plot(P_Covariance(2,2,:));
##   grid();
##   title ("Kalman Filter: P Covariance: Estimate Speed Error");
##   xlabel ("Samples");
##   ylabel ("Estimate Speed Error Variance");
##   xlim("manual");
##   xlim([0 signalLength-1]);
endfunction

% Generate the Test Signal
testSignal = FloatTestSignal2file(Um_ampl, Variance_o, 2*pi*Fband_in_Hz*Tsample, "testSignal.txt");

% Scalar Kalman Filter
[KalmanOut, K_Gain, P_Out] = KalmanFilterExample(testSignal, Phi_matrix, Q_matrix, H_matrix, R_matrix, P_start, x_state_start);
