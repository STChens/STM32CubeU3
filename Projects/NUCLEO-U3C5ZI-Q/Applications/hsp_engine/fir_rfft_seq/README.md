## <b>FIR_RFFT_SEQ Application Description</b>

This application exploits the HSP Engine Middleware to demonstrate with Sequencer Mode with Ping/Pong buffer:

   - How to process a FIR + FFT with processing list.
   - How to manage a Ping/Pong buffers in input and output.
   - How to trig a Processing through a DMA Transfer Complete event
   - How to trig a DMA Transfer by the Processing List

Several topics are covered :

  - Allocate buffers in BRAM
  - Transfer data from/to BRAMAB
  - Record a Processing list
  - Configure the HSP Event 2 to trig the Processing List on DMA Transfer Complete (rising edge)

The input signal (sampling frequency at 48000kHz) combines several sinusoids at 750, 3000, 6000, 8000Hz, split in 3 buffers of 256 samples.

The processing list performs these actions:

  - low-pass filter with a 31th order FIR filter (32 coefficients) to remove high frequency >= 6000Hz.
  - RFFT process
  - Compute the magnitude of RFFT results.
  - Trig a DMA Transfer to output the RFFT results from HSP BRAMAB to Flash memory

2 DMAs are configured to manage the data transfer between the Flash and HSP BRAMAB memory regions.

At the initialization step of the MW HSP Engine (call of MX_HSP_Engine_Init), some HSP buffers are allocated in HSP BRAMAB and the Processing List is recorded and configured.

The MX_HSP_Engine_Process() called in the while(1) performs:

  - the management of the Ping/Pong buffers in Input & Output,
  - the transfer of the 3 samples series in HSP BRAMAB,
  - the extraction of dominant frequencies to check the "FIR + FFT" processing results.

The processing is ended after nb_loop iterations  

The dominant frequencies should be 750 & 3000 Hz at each processing.

#### <b>Expected success behavior</b>

On success operation, the LED_GREEN light is switched on.

#### <b>Error behaviors</b>

On failure operation, the LED_GREEN light is toggling at 1Hz.

#### <b>Assumptions if any</b>
None

#### <b>Known limitations</b>
None

### <b>Notes</b>

In order to support Terminal I/O user has to set USE_TRACE define to 1

Depending of IDE, to watch content of Terminal I/O note that:

 - When resorting to EWARM IAR IDE:
   Command Code is displayed on debugger as follows: View --> Terminal I/O

 - When resorting to MDK-ARM KEIL IDE:
   Command Code is displayed on debugger as follows: View --> Serial Viewer --> Debug (printf) Viewer

 - When resorting to STM32CubeIDE:
   Command Code is displayed on debugger as follows: Window--> Show View--> Console.
   In Debug configuration :
   - Window\Debugger, select the Debug probe : ST-LINK(OpenOCD)
   - Window\Startup,add the command "monitor arm semihosting enable"

### <b>Keywords</b>

HSP Engine, HSP, Filter, FIR, RFFT, Magnitude, Processing List, DMA, Sequencer

### <b>Hardware and Software environment</b>

  - This application runs on STM32U3C5xx devices.
  - This application has been tested with STMicroelectronics STM32U3C5ZI-Q boards revision MB2222-U3C5ZIQ-B01
    and can be easily tailored to any other supported device and development board.

###  <b>How to use it ?</b>

In order to make the program work, you must do the following:

 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the application
