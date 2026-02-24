// lookup_data [x pos][y pos][bit setting][byte data for x and y drivers]
// on SN754410 driver ic, input 1 is 01 (x up, y right), input 2 is 10 (x down, y left),
//   input 3 is 0100 (x up, y right), input 4 is 1000 (x down, y left) - 12 34 always opposite
//
uint8_t lookup_data[8][4][2][2] = {  
// bit settings for applicalbe x drive and y drive - 74HC595 data bytes 
// for x 01 up, 10 down
// for y 01 right, 10 left
//
//        0 value                   1 value
//     xd           yd           xd          yd
//
{  //x = 0 y = 0-3 pos
{ {0b00000010, 0b00000010}, {0b00000001, 0b00000001} }, // 00 00   
// above - x down, y left          x up, y right
{ {0b00000010, 0b00000100}, {0b00000001, 0b00001000} },
// above - x down, y right          x up, y left
{ {0b00000010, 0b00100000}, {0b00000001, 0b00010000} },
// above - x down, y left          x up, y right
{ {0b00000010, 0b01000000}, {0b00000001, 0b10000000} }  // 00 03 etc
// above - x down, y right         x up, y left
},
{  //x = 1 y = 0-3
// x follows inhibit line, so x needs to be reversed every odd column
{ {0b00000100, 0b00000010}, {0b00001000, 0b00000001} }, // 01 00
// above - x up, y left        x down, y right
{ {0b00000100, 0b00000100}, {0b00001000, 0b00001000} }, 
// above - x up, y right       x down, y left
{ {0b00000100, 0b00100000}, {0b00001000, 0b00010000} }, 
// above - x up, y left        x down, y right
{ {0b00000100, 0b01000000}, {0b00001000, 0b10000000} }
// above - x up, y right       x down, y left
},
{  //x = 2 y = 0-3
{ {0b00100000, 0b00000010}, {0b00010000, 0b00000001} }, // 02 00
// above - x down, y left          x up, y right
{ {0b00100000, 0b00000100}, {0b00010000, 0b00001000} },
// above - x down, y right          x up, y left
{ {0b00100000, 0b00100000}, {0b00010000, 0b00010000} },
// above - x down, y left          x up, y right
{ {0b00100000, 0b01000000}, {0b00010000, 0b10000000} }
// above - x down, y right         x up, y left
},
{  //x = 3 y = 0-3
// x follows inhibit line, so x needs to be reversed every odd column
{ {0b01000000, 0b00000010}, {0b10000000, 0b00000001} }, // 03 00
// above - x up, y left        x down, y right
{ {0b01000000, 0b00000100}, {0b10000000, 0b00001000} },
// above - x up, y right       x down, y left
{ {0b01000000, 0b00100000}, {0b10000000, 0b00010000} },
// above - x up, y left        x down, y right
{ {0b01000000, 0b01000000}, {0b10000000, 0b10000000} }
// above - x up, y right       x down, y left
},
};

void writeMem(int x, int y, int value)
{
decodeStartus = micros();  // for timing measurement
// output[0] y4-7  output[0] goes out first
// output[1] y0-3
// output[2] x4-7
// output[3] x0-3
if ((y < 4) && (x < 4)){
  output[0] = 0x00; 
  output[1] = lookup_data[x][y][value][1]; // y byte is 1
  output[2] = 0x00;
  output[3] = lookup_data[x][y][value][0]; // x byte is 0
  } 
if ((y >= 4) && (x < 4)){
  output[0] = lookup_data[x][y-4][value][1]; // y byte is 1 
  output[1] = 0x00;
  output[2] = 0x00;
  output[3] = lookup_data[x][y-4][value][0]; // x byte is 0
  } 
if ((y < 4) && (x >= 4)){
  output[0] = 0x00; 
  output[1] = lookup_data[x-4][y][value][1]; // y byte is 1
  output[2] = lookup_data[x-4][y][value][0]; // x byte is 0
  output[3] = 0x00;
  } 
if ((y >= 4) && (x >= 4)){
  output[0] = lookup_data[x-4][y-4][value][1]; // y byte is 10x00; 
  output[1] = 0x00;
  output[2] = lookup_data[x-4][y-4][value][0]; // x byte is 0
  output[3] = 0x00;
  } 
// write and latch address and driver output
SPIWrite(); 
// set inhibit if required
if (inhibitFlag == 1){
  // determine direction of current based on value
  // inhibit current equal to the opposite of x current works best
  if (value == 0){ 
    inhibitOut0 = 1;  // A5 (in1) if value 0 (down), sets current up for x0 - x0 up is right to left
    inhibitOut1 = 0;  // A6 (in2)
    }
  if (value == 1) {
    inhibitOut0 = 0; // A5 if value 1 (up), sets current down for x0 - x0 down is left to right
    inhibitOut1 = 1; // A6
    }
  digitalWrite(inhibitPin0, inhibitOut0); // A5 In1 active high
  digitalWrite(inhibitPin1, inhibitOut1); // A6 In2 active high
  // no need to clear outputs if inhibitFlag remains set
  // outputs set to 0 when inhibitFlag is cleared
  // use seperate inhibit enable to raise high prior to x drive output
  digitalWrite(inhibitEnablePin, HIGH);  
  // Final drive values 
  // - 8.2 ohm with inhibit at 8.2 ohm
  // - drive voltage VS at 5.2v
  // - voltage accross current limiting resistors is 3.0v.
  // - current on eachy drive line and the inhibit line is 365mA
  }
// pulse outputs (not including inhibit) using enable
writeStartus = micros(); // for timing measurement
digitalWrite(enPin, HIGH); // latch goes high 650ns later - then need 1us to read sense
delayMicroseconds(1); // delay needed to prevent errors 
// sense reads after delay from enPin HIGH
//SPIWriteClear(); //  not needed with enable low and next output rewrite of entire buffer
digitalWrite(enPin, LOW); 
senseIn = digitalRead(sensePin); // read latched sense pin
writeEndus = micros(); // for timing measurement
if (inhibitFlag == 1){
  digitalWrite(inhibitPin0, LOW); // active high 
  digitalWrite(inhibitPin1, LOW); // active high
  // putting above before enable low greatly reduces back emf
    digitalWrite(inhibitEnablePin, LOW);  
  }
// update memory - no change if inhibit active
if (inhibitFlag != 1) {bitWrite64(memContents, ((x * 8) + y), value);} 
/* for testing
Serial.print(x); 
Serial.print("\t");
Serial.print(y);
Serial.print("\t");
Serial.print(value); 
Serial.print("\t");
Serial.print(senseIn);
Serial.print("\t\t");
Serial.println(inhibitFlag);
*/
}

void SPIWrite()
{
// copy output to buffer
for (int i = 0; i < 4; i++) {
  buffer[i] = output[i];
  }
// SPI transfer sends the msbit first of each byte.  
// Also with the buffer array, buffer[0] goes out first.
SPI.transfer(buffer, 4); // __us
// latch output with rclk
digitalWrite(rclkPin, HIGH); // 1us
digitalWrite(rclkPin, LOW); // 1us
// SPI.endTransaction(); // only if another SPI device
}

void SPIWriteClear()
{ // writes 0s to output latches
// copy output to buffer
for (int i = 0; i < 4; i++) {
  buffer[i] = 0;
  }
SPI.transfer(buffer, 4); 
// latch output with rclk
digitalWrite(rclkPin, HIGH);
digitalWrite(rclkPin, LOW);
// SPI.endTransaction();  // only if another SPI device
}