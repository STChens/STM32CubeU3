## <b>CFFT_ACC Application Description</b>

This application exploits the HSP Engine Middleware to demonstrate how to process a Complex FFT in Accelerator Mode.

Several topics are covered :

  - Allocate buffers in BRAM
  - Transfer Q1.31 data of complex signal in HSP BRAM
  - Convert the Q1.31 data to Float with HSP Direct Command: HSP_ACC_VectQ312F
  - Process the FFT with HSP Direct Command: HSP_ACC_Fft_f32
  - Compute the magnitude with HSP Direct Command: HSP_ACC_CmplxMag_f32
  - Check the Dominant Frequencies

The input signal combines 2 complex signal:

  - one at 13Hz with a phase at -PI/3
  - one at 34Hz with a phase at PI/4
	
The CFFT identifies the 2 dominant frequencies 13Hz & 34Hz.

#### <b>Expected success behavior</b>

On success operation, the LED_GREEN light is switched on.

#### <b>Error behaviors</b>

On failure operation, the LED_GREEN light is toggling at 1Hz.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

### <b>Notes</b>

### <b>Keywords</b>

HSP Engine, HSP, Complex, FFT, Magnitude, Phase, Q1.31, Accelerator

### <b>Hardware and Software environment</b>

  - This application runs on STM32U3C5xx devices.
  - This application has been tested with STMicroelectronics STM32U3C5ZI-Q boards revision MB2222-U3C5ZIQ-B01
    and can be easily tailored to any other supported device and development board.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
