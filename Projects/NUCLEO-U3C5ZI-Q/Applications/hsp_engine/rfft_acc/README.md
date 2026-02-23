## <b>RFFT_ACC Application Description</b>

This application exploits the HSP Engine Middleware to demonstrate how to process a RFFT in Accelerator Mode.

Several topics are covered :

  - Allocate buffers in BRAM
  - Transfer data in BRAM
  - Process the RFFT with the HSP Direct Command: HSP_ACC_Rfft_f32
  - Compute the magnitude of RFFT result with the HSP Direct Command: HSP_ACC_VectAbs_f32
  - Extract the dominant frequencies.

The input signal combines 2 sinusoids at 50Hz and 200Hz.

The RFFT identifies the 2 dominant frequencies 50Hz & 200Hz.

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

HSP Engine, HSP, RFFT, Accelerator

### <b>Hardware and Software environment</b>

  - This application runs on STM32U3C5xx devices.
  - This application has been tested with STMicroelectronics STM32U3C5ZI-Q boards revision MB2222-U3C5ZIQ-B01
    and can be easily tailored to any other supported device and development board.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
