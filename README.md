# Magnetic-Core-Memory-Demo-Board

Magnetic Core Memory was used for random access storage in computers through the 70s.  I just discovered it last year and was intrigued by its operation.  There are many sites that go into detail on its operation so I will not reproduce that here.  

The core memory plane used on the demo board was purchased on Ebay.  From limited research on the internet, it appears to be a clone of the core memory used in the DEC PDP-8 computer.  The demo board consists of a single core memory plane containing 1024 cores or storage bits arranged in four 16 by 16 grids of 256 cores each.

<img width="767" height="1023" alt="image" src="https://github.com/user-attachments/assets/6a2d85c8-136d-480d-9838-b5187bb6f23f" />

The plane has two sense lines and one inhibit line.  The inhibit line enters/exits the plane on the bottom far left and far right.  The two sense lines enter/exit the plan in the middle – one on the bottom, one on the top.  Their paths are shown in the photo above.  Each sense line covers one-half of the plane, but they can be connected together in series to read the entire plane.

Originally, I had planned on using all of the memory cores in the demo.  However, this would have required quite a large board for the decoding and drive circuits.  Also, display of the information written and read would be difficult on the OLED display I had planned on using.  So, I opted to just use 64 memory cores.  The core values are easily displayed in hex format on one line of the OLED display.

The board uses eight x address lines and eight y address lines to address the memory cores in the lower left of the memory plane.  This requires eight SN754410 quadruple half-H drivers and four SN74HC595 8-bit shift registers to generate the drive currents.  Another SN754410 is used for the inhibit line drive current.

The sense line requires two LT1016 ultra-fast 10ns comparators with latch to capture the sense value read.  A 74LS123 one-shot device generates the latch signal and a 74LS32 OR gate combine the two outputs from the comparators.  The sense line generates a weak signal and also picks up quite a bit of noise, so using a latch to time the read appears to be essential for rock solid operation.  

The OLED display uses an i2c interface.  The key pad is connected via a PCF8574 i2c i/o expander.  The board is controlled by a Nano-ESP32.  Power to the board is provided by a 9 volt 1-2 amp DC wall wart.

I struggled getting the demo to work on the breadboard because of several power supply issues.  First, the power out of the LM317 was erratic at times and caused read and write errors.  I had thought the drive line resistor values were incorrect until I pushed on the LM317 and things straightened out.  Second, while the SN754410 datasheet stated that Vs could not be less than Vcc, I discovered that the demo board worked best when Vs was slightly higher than Vcc.  To accomplish this, I inserted a dropping resistor between the LM317 Vs output and Vcc.  The voltages that I found worked best with the 8.2 ohm drive limiting resistors were 5.2 volt Vs and 4.9 volt Vcc.  Vs is connected to the Nano-ESP32 Vin pin.  This slightly higher voltage also allows the Nano-ESP32 to work without issue when a USB cable is plugged into the Nano and the 9 volt power supply is also being used.

I added a relay to cut off Vs and Vcc to the line drivers and shift registers when the board was not being used.  This reduces power consumption and keeps the LM317 cooler given its minimal heatsink.  The ticking noise of the relay also provided some feedback on the board’s operation.  The relay and the board’s OLED display turn off after two minutes of no command entry or physical motion detection.

Below is a breadboard of the demo board.  

<img width="731" height="975" alt="image" src="https://github.com/user-attachments/assets/02677fd4-7f63-41b1-8ade-1c72156526ff" />

The small boards attached to the scope probes are differential amplifiers that I made using INA849 ultra-low-noise, high-bandwidth, instrumentation amplifiers.  They are powered by a HP triple output bench power supply.  This project is in another github repository at jpwolfe31.  These were very helpful in getting data on the snese line and drive currents.

I modified the motion detector to lower its profile by removing the three-lead sensor component from the end of its small board and resoldering it to one side of the small board.  

Final demo Board below:

<img width="762" height="1015" alt="image" src="https://github.com/user-attachments/assets/09cf16c5-22c0-410d-8e39-cd69305ecd31" />

The core plane is set on white plexi and covered by clear polycarbonate.  Sandwiched under the white plexi are the ribbon cables from the plane to the board.  To get the right side y lines to match up with the connector, I needed to fold them 90 degrees up and then fold them back down over themselves to the connector.  The other lines under the plexi just required one 90 degree fold.  The plane and the board are mounted on a thick abs plastic sheet.

The demo board allows one to individually set bits in the array, to write and read data and to do both of these things with the inhibit line active to demonstrate the inhibit function is working.  Extremely long tests can be made with random numbers being written and read (with and without the inhibit).  There are two switches at the top of the board that allow you to short the sense and inhibit lines to generate errors to help show the board is operating properly.  The memory works quite well.  Ten million consecutive writes and reads without error is no problem.

Oscilloscope screen captures show the various board signals in operation.  This is a separate document in the repository.

Also, check out the video at ___________________.

Happy to answer any questions.




