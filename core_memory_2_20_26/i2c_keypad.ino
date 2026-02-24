// i2c PCF8574 matrix keypad with debounce
// uses Wire.h

int getKey()
{
key = scanKeypad();  // key is a global variable and gets picked up and used after getkey check
if ((key != 'x') && (readyFlag == 1)){
  readyFlag = 0;
  //Serial.println(key);  
  return 1;
  }
if (key == 'x'){ // no key clears repeatFlag
  readyFlag = 1;
  }
return 0;
}

char scanKeypad() {
  for (byte r = 0; r < ROWS; r++) {
    // Set all rows HIGH (input mode with pull-up)
    writePCF8574(0xff);
    // Drive current row LOW
    byte rowMask = ~(1 << rowPins[r]);  // bitwise not - changes one bit to its opposite - 0 to 1, and 1 to 0.
    writePCF8574(rowMask);
    delayMicroseconds(50); // debounce settle time
    // Read columns
    byte data = readPCF8574();
    for (byte c = 0; c < COLS; c++) {
      if (!(data & (1 << colPins[c]))) {
        return keys[r][c];
      }
    }
  }
  return 'x'; // No key pressed
}

void writePCF8574(byte data)
{
Wire.beginTransmission(PCF8574_ADDR);
Wire.write(data);
Wire.endTransmission();
}

byte readPCF8574() 
{
Wire.requestFrom(PCF8574_ADDR, 1);
return(Wire.read());
}
