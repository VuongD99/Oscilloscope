TINY SCOPE-----------------------------------------------------------------------------------------------------------------------------------------------
- Single-channel oscilloscope including a PsoC6 microcontroller and a NewHaven TFT LCD Display. 
- The user will be given a terminal window where they can enter commands to either start, stop, or change the settings of the oscilloscope. They will also have two potentiometers which they can use to scroll the waveforms.
- Included features:
  1. Sampling rate of 250,000 samples/second
  2. Configurable running modes (free running or trigger) 
  3. Configurable trigger channel, trigger level and slope
  4. Frequency display for both channels
  5. Waveform scrolling using potentiometers
  
- Analog data from an AD2 is parsed by the microcontroller into digital signal and sent to the Raspberry Pi over UART serial connection.
- Written in C++. Built as final project for CMPE121 Microprocessor Design Class at University of California, Santa Cruz.

