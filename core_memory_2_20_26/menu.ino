/* main waterfall command menu
keys 0 to 7
0 is scroll menu
1 is select menu
2-7 select action
*/
void menu() 
{
// Serial.println(key);  
if (key == '0') { // scroll
  menuState++;
  executeState = 99; // clear execute state as new menu
  if (menuState > 5) {menuState = 1;} // skip splash screen menuState = 0
  updateMenu();
  }
if (executeState != 99) {executeAction();}  // perform action
if (key == '1') { // select instructions  
  showInstructions();
  executeState = menuState;
  }
}

void updateMenu() 
{
switch (menuState) {
case 0: // splash screen
  splashScreen();
  break;
case 1: // write / read
  strcpy(oled_line1_str, "    write/read     ");
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  strcpy(oled_line6_str, "                   ");
  updateMenuDisplay();
  break;
case 2: // write inhibit
  strcpy(oled_line1_str, "    write/inh/read ");
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  strcpy(oled_line6_str, "                   ");
  updateMenuDisplay();
  break;
case 3: // read / write
  strcpy(oled_line1_str, "    read/write     ");
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  strcpy(oled_line6_str, "                   ");
  updateMenuDisplay();
  break;
case 4: // x y set
  strcpy(oled_line1_str, "    x-y set bit    ");
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  strcpy(oled_line6_str, "                   ");
  updateMenuDisplay();
  break;
case 5: // misc
  strcpy(oled_line1_str, "    misc           ");
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  strcpy(oled_line6_str, "                   ");
  updateMenuDisplay();
  break;
default:
  break;
}
}

void splashScreen()
{
strcpy(oled_line1_str, "magnetic core     "); // 19 char max
strcpy(oled_line2_str, "  memory board    ");
strcpy(oled_line3_str, "                  ");
strcpy(oled_line4_str, "                  ");  
strcpy(oled_line5_str, " 0 to scroll      ");
strcpy(oled_line6_str, " 1 to select      ");
updateMenuDisplay();
}

void updateMenuDisplay()
{
display.clearDisplay();
oledText(oled_line1_str, 8, 3, 1, false);
oledText(oled_line2_str, 8, 13, 1, false);
oledText(oled_line3_str, 8, 23, 1, false);
oledText(oled_line4_str, 8, 33, 1, false);
oledText(oled_line5_str, 8, 43, 1, false);
oledText(oled_line6_str, 8, 53, 1, false);
oledRect(); // also displays display
}

void showInstructions()
{
switch (menuState) {
case 1: // write/read
  strcpy(oled_line1_str, "    write/read     ");
  strcpy(oled_line2_str, " 2 - alpha         ");
  strcpy(oled_line3_str, " 3 - 00-ff         ");
  strcpy(oled_line4_str, " 4 - random once   ");
  strcpy(oled_line5_str, " 5 - random 6400   ");
  strcpy(oled_line6_str, " 6 7 - 100K ");
  strcat(oled_line6_str, strMaxCnt);
  updateMenuDisplay();
  break;
case 2: // write/inhw/inhibit
  strcpy(oled_line1_str, "   write/inhw/read");
  strcpy(oled_line2_str, " 2 - alpha        ");
  strcpy(oled_line3_str, " 3 - 00-ff        ");
  strcpy(oled_line4_str, " 4 - random once  ");
  strcpy(oled_line5_str, " 5 - random 6400  ");
  strcpy(oled_line6_str, " 6 7 - 100K ");
  strcat(oled_line6_str, strMaxCnt);
  updateMenuDisplay();
  break;
case 3: // read/write
  strcpy(oled_line1_str, "    read/write     ");
  strcpy(oled_line2_str, " 2 - write alpha   ");
  strcpy(oled_line3_str, " 3 - write names   ");
  strcpy(oled_line4_str, " 4 - read hex      ");
  strcpy(oled_line5_str, " 5 - read ascii    ");
  strcpy(oled_line6_str, " 6 7 map, 6400     ");
  updateMenuDisplay();
  break;
case 4: // x y set
  strcpy(oled_line1_str, "    x-y set bit    ");
  strcpy(oled_line2_str, " 2 - increment x   ");
  strcpy(oled_line3_str, " 3 - increment y   ");
  strcpy(oled_line4_str, " 4 - inhibit on/off");
  strcpy(oled_line5_str, " 5 - map           ");
  strcpy(oled_line6_str, " 6 7 - write 0 1   ");
  updateMenuDisplay();
  break;
case 5: // misc
  strcpy(oled_line1_str, "    misc           ");
  strcpy(oled_line2_str, "  2 - values       ");
  strcpy(oled_line3_str, "  3 - timing       ");
  strcpy(oled_line4_str, "  4 - set high cnt ");
  strcpy(oled_line5_str, "  5 - about        ");
  strcpy(oled_line6_str, "                   ");
  updateMenuDisplay();
  break;
}
}

void executeAction() 
{
switch (menuState) {
case 1: // write/read
  writeReadExecute();
  break;
case 2: // write/inhibit write/read
  writeInhibitExecute();
  break;
case 3: // read/write
  readWriteExecute();
  break;
case 4: // x-y set bit
  xyExecute();
  break;
case 5: // misc
  miscExecute();
  break;
default:
  break;
  }
}


// 111111111111111111111111111
// 111111111111111111111111111
void writeReadExecute()
{
if (key == '2'){ // write alpha once
  errorCnt = 0;
  // write core memory 
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(stringWrite0[stringNum0], memPos);
      writeMem(selBitx, selBity, valBit); 
    }
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-8
      writeMem(selBitx, selBity, 0); 
      bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = stringWrite0[stringNum0] ^ memRead;
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  strcpy(oled_line1_str, "    write/read    ");
  // sprintf format
  // the mandatory % character
  // a 0 flag for padding
  // the 16 as "minimum field width"
  // the ll as "length modifiers"  (lomg lomg)
  // the x conversion specifier (X for upper case)
  sprintf(oled_line2_str, "%016llx", stringWrite0[stringNum0]);
  addSpacesL2(); // adds spaces to oled_line2_str
  sprintf(oled_line3_str, "%016llx", memRead);
  addSpacesL3(); // adds spaces to oled_line3_str
  sprintf(oled_line6_str, " error count %d", errorCnt);
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  stringNum0++; // roll through stings
  stringNum0 = stringNum0 % 4;
  memPos = 0; // return to 0 for other functions
  selBitx = 0;
  selBity = 0;
  }
if (key == '3'){ // write through range once 0000000000000000-ffffffffffffffff 
  errorCnt = 0;
  testM = 0;
  // while loop until done
  while(1){  
    memRead = 0; 
    // clear memory
    for (memPos = 0; memPos < 64; memPos++){
        selBitx = memPos / 8; // retruns 0-7
        selBity = memPos - (selBitx * 8); // returns 0-7
        //int valBit = bitRead(stringWrite[stringNum], memPos);
        //writeMem(selBitx, selBity, 1); 
        // int valBit = bitRead(test0, memPos);
        writeMem(selBitx, selBity, 0); 
        }
    for (memPos = 0; memPos < 64; memPos++) { 
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      //int valBit = bitRead(stringWrite[stringNum], memPos);
      //writeMem(selBitx, selBity, 1); 
      int valBit = bitRead64(testM, memPos);
      writeMem(selBitx, selBity, valBit); 
      }
    for (memPos = 0; memPos < 64; memPos++){
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      writeMem(selBitx, selBity, 0); 
      bitWrite64(memRead, memPos, senseIn);
      }
    // check and update error count
    comparedResult = testM ^ memRead;
    // comparedResult++; // add error for testing
    errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
    strcpy(oled_line1_str, "    write/read    ");
    sprintf(oled_line2_str, "%016llx", testM); // llx for uint64_t hex
    addSpacesL2(); // adds spaces to oled_line2_str
    sprintf(oled_line3_str, "%016llx", memRead);
    addSpacesL3(); // adds spaces to oled_line3_str
    sprintf(oled_line6_str, " error count %d", errorCnt);
    display.clearDisplay();
    oledText(oled_line1_str, 8, 3, 1, false);
    oledText(oled_line2_str, 8, 13, 1, false);
    oledText(oled_line3_str, 8, 23, 1, false);
    oledText(oled_line6_str, 8, 53, 1, false);
    oledRect(); // also displays display
    //Serial.println();
    if (testM == 0xffffffffffffffff) {testM = 0x00;}
    else {testM = (testM + 0x1111111111111111);} // increment
    // end while loop when done
    if (testM == 0) {
      memPos = 0; // return to 0 for other functions
      selBitx = 0;
      selBity = 0;
      break;
      }
    for (int i = 0; i < 2; i++){
      delay(125); // update delay x2
      if (getKey()){
        lmCommand = millis();  // choose one or the other
        pmDisplayActive =  millis();  
        memPos = 0; // return to 0 for other functions
        selBitx = 0;
        selBity = 0;
        return;
        }
      }  
  }
}
if (key == '4'){ // write random numbers 64
  // void randomWriteInhRead(long int startCnt, long int endCnt, int inhOn, char* strEndCnt, char* oled_title_str)
  strcpy(strEndCnt, "64");
  strcpy(oled_title_str, "    write/read    ");
  randomWriteInhRead(0, 64, 0, strEndCnt, oled_title_str);  // no inhibit
  }
if (key == '5'){ // write random numbers 6400
  strcpy(strEndCnt, "6400");
  strcpy(oled_title_str, "    write/read    ");
  randomWriteInhRead(0, 6400, 0, strEndCnt, oled_title_str);  // no inhibit
  }
if (key == '6'){ // write random numbers 100K
  strcpy(strEndCnt, "100K");
  strcpy(oled_title_str, "    write/read    ");
  randomWriteInhRead(32, 100000, 0, strEndCnt, oled_title_str);  // no inhibit and start at 32 to end at 100k
  }
if (key == '7'){ // write random numbers 1M
  strcpy(strEndCnt, strMaxCnt);
  endCnt = maxCnt;
  strcpy(oled_title_str, "    write/read    ");
  randomWriteInhRead(0, endCnt, 0, strEndCnt, oled_title_str);  // no inhibit
  }
}

// 22222222222222222222222222
// 22222222222222222222222222
void writeInhibitExecute() // write/write inhibit/read
{
if (key == '2'){ // write alpha once
  errorCnt = 0;
  inhWriteM = 0; 
  // write core memory 
  for (memPos = 0; memPos < 64; memPos++) {
    selBitx = memPos / 8; // retruns 0-7
    selBity = memPos - (selBitx * 8); // returns 0-7
    int valBit = bitRead64(stringWrite0[stringNum0], memPos);
    writeMem(selBitx, selBity, valBit); 
    }
  inhibitFlag = 1; // set flag
  // write core memory with inhibit
  for (memPos = 0; memPos < 64; memPos++) { 
    selBitx = memPos / 8; // retruns 0-7
    selBity = memPos - (selBitx * 8); // returns 0-7
    //int valBit = bitRead(stringWrite[stringNum], memPos);
    writeMem(selBitx, selBity, inhWriteM); 
    }
  inhibitFlag = 0; // clear flag
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-8
      writeMem(selBitx, selBity, 0); 
      bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = stringWrite0[stringNum0] ^ memRead;
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  strcpy(oled_line1_str, "    write/inhw/read");
  // sprintf format
  // the mandatory % character
  // a 0 flag for padding
  // the 16 as "minimum field width"
  // the ll as "length modifiers"  (lomg lomg)
  // the x conversion specifier (X for upper case)
  sprintf(oled_line2_str, "%016llx", stringWrite0[stringNum0]);
  addSpacesL2(); // adds spaces to oled_line2_str
  sprintf(oled_line3_str, "%016llx", memRead);
  addSpacesL3(); // adds spaces to oled_line3_str
  sprintf(oled_line6_str, " error count %d", errorCnt);
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  stringNum0++; // roll through stings
  stringNum0 = stringNum0 % 4;
  memPos = 0; // return to 0 for other functions
  selBitx = 0;
  selBity = 0;
  }
if (key == '3'){  // inhibit write through range 0000000000000000-ffffffffffffffff - 0 write then 1 write
  errorCnt = 0;
  testM = 0;
  // while loop until done
  while(1){  
   memRead = 0; // clear variable
  // clear memory
  inhibitFlag = 0; // clear flag
  for (memPos = 0; memPos < 64; memPos++)
    { selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      //int valBit = bitRead(stringWrite[stringNum], memPos);
      //writeMem(selBitx, selBity, 1); 
      // int valBit = bitRead(test0, memPos);
      writeMem(selBitx, selBity, 0); 
      }
  inhibitFlag = 0; // clear flag
  for (memPos = 0; memPos < 64; memPos++)
    { 
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      //int valBit = bitRead(stringWrite[stringNum], memPos);
      //writeMem(selBitx, selBity, 1); 
      int valBit = bitRead64(testM, memPos);
      writeMem(selBitx, selBity, valBit); 
    }
  inhibitFlag = 1; // set flag
  for (memPos = 0; memPos < 64; memPos++)
    { // write 0x0 
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      //int valBit = bitRead(stringWrite[stringNum], memPos);
      writeMem(selBitx, selBity, inhWriteM); 
    }
  inhibitFlag = 0; // clear flag
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
    selBitx = memPos / 8; // retruns 0-7
    selBity = memPos - (selBitx * 8); // returns 0-7
    writeMem(selBitx, selBity, 0); 
    bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = testM ^ memRead;
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  strcpy(oled_line1_str, "    write/inhw/read");
  sprintf(oled_line2_str, "%016llx", testM); // llx for uint64_t hex
  addSpacesL2(); // adds spaces to oled_line2_str
  sprintf(oled_line3_str, "%016llx", memRead);
  addSpacesL3(); // adds spaces to oled_line3_str
  sprintf(oled_line5_str, " error count %d", errorCnt);
  sprintf(oled_line6_str, " inhibit write %d", inhWriteM);
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line5_str, 8, 43, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  //Serial.println();
  if (testM == 0xffffffffffffffff){
    testM = 0x00;
    if (inhWriteM == 0) {inhWriteM = 1;}
      else {inhWriteM = 0;}
    } 
  else {testM = (testM + 0x1111111111111111);} // increment
  // end while loop when done
  //delay(250); in loop below now
  if ((testM == 0) && (inhWriteM == 0)) {
    memPos = 0; // return to 0 for other functions
    selBitx = 0;
    selBity = 0;
    break;
    }
  for (int i = 0; i < 2; i++){
      delay(125); // update delay x2
      if (getKey()){
        lmCommand = millis();  // choose one or the other
        pmDisplayActive =  millis();  
        memPos = 0; // return to 0 for other functions
        selBitx = 0;
        selBity = 0;
        return;
        }
      }  
    }
  }
  
if (key == '4'){ // write random numbers 64
  // void randomWriteInhRead(long int startCnt, long int endCnt, int inhOn, char* strEndCnt, char* oled_title_str)
  // each inhibit write is a complement of then current memory bit value
  strcpy(strEndCnt, "64");
  strcpy(oled_title_str, "    write/inhw/read");
  randomWriteInhRead(0, 64, 1, strEndCnt, oled_title_str);  
  }
if (key == '5'){ // write random numbers 6400
  strcpy(strEndCnt, "6400");
  strcpy(oled_title_str, "    write/inhw/read");
  randomWriteInhRead(0, 6400, 1, strEndCnt, oled_title_str);  
  }
if (key == '6'){ // write random numbers 100K
  strcpy(strEndCnt, "100K");
  strcpy(oled_title_str, "    write/inhw/read");
  randomWriteInhRead(32, 100000, 1, strEndCnt, oled_title_str);  // start at 32 to end at 100K
  }
if (key == '7'){ // write random numbers 1M +
  strcpy(strEndCnt, strMaxCnt);
  endCnt = maxCnt;
  strcpy(oled_title_str, "    write/inhw/read");
  // for testing
  // void randomWriteInhRead(long int startCnt, long int endCnt, int inhOn, char* strEndCnt, char* oled_title_str)
  // randomWriteInhRead(99990000, 100000000, 1, "100M", oled_title_str);
  randomWriteInhRead(0, endCnt, 1, strEndCnt, oled_title_str);  
  }
}

// 33333333333333333333333333
// 33333333333333333333333333
void readWriteExecute() // read/write
{
if (key == '2'){ // write alpha once
  errorCnt = 0;
  // write core memory 
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(stringWrite0[stringNum0], memPos);
      writeMem(selBitx, selBity, valBit); 
    }
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-8
      writeMem(selBitx, selBity, 0); 
      bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = stringWrite0[stringNum0] ^ memRead;
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  // write core memory again to set up for reading
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(stringWrite0[stringNum0], memPos);
      writeMem(selBitx, selBity, valBit); 
    }
  strcpy(oled_line1_str, "    write");
  // sprintf format
  // the mandatory % character
  // a 0 flag for padding
  // the 16 as "minimum field width"
  // the ll as "length modifiers"  (lomg lomg)
  // the x conversion specifier (X for upper case)
  sprintf(oled_line2_str, "%016llx", stringWrite0[stringNum0]);
  addSpacesL2(); // adds spaces to oled_line2_str
  sprintf(oled_line3_str, "%016llx", memRead);
  addSpacesL3(); // adds spaces to oled_line3_str
  sprintf(oled_line6_str, " error count %d", errorCnt);
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  stringNum0++; // roll through stings
  stringNum0 = stringNum0 % 4;
  memPos = 0; // return to 0 for other functions
  selBitx = 0;
  selBity = 0;
  }
if (key == '3'){ // write names once
  uint64_t valChar = str8_to_uint64_reverse(stringWrite1[stringNum1]) ;
  errorCnt = 0;
  // write core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(valChar, memPos);
      writeMem(selBitx, selBity, valBit); 
    }
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-8
      writeMem(selBitx, selBity, 0); 
      bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = valChar ^ memRead;
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  // write core memory again to set up for reading
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(valChar, memPos);
      writeMem(selBitx, selBity, valBit); 
    }
  strcpy(oled_line1_str, "    write");
  strcpy(oled_line2_str, stringWrite1[stringNum1]);
  // convert memRead to to string
  uint64_to_str8_reverse(oled_text_str, memRead); 
  strcpy(oled_line3_str, oled_text_str);
  sprintf(oled_line6_str, " error count %d", errorCnt);
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  stringNum1++; // roll through stings
  stringNum1 = stringNum1 % 5;
  memPos = 0; // return to 0 for other functions
  selBitx = 0;
  selBity = 0;
  }
if (key == '4'){ // read hex once
  uint64_t storedMemContents = memContents;  // store for error checking read below
  errorCnt = 0;
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-8
      writeMem(selBitx, selBity, 0); // fills memContents with 0s
      bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = storedMemContents ^ memRead; // compare contents stored in data.ino with that read
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  // write core memory again to set up for reading
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(memRead, memPos); // store the value read earlier
      writeMem(selBitx, selBity, valBit); 
    }
  strcpy(oled_line1_str, "    read");
  // sprintf format
  // the mandatory % character
  // a 0 flag for padding
  // the 16 as "minimum field width"
  // the ll as "length modifiers"  (lomg lomg)
  // the x conversion specifier (X for upper case)
  sprintf(oled_line2_str, "%016llx", storedMemContents);
  addSpacesL2(); // adds spaces to oled_line2_str
  sprintf(oled_line3_str, "%016llx", memRead);
  addSpacesL3(); // adds spaces to oled_line3_str
  sprintf(oled_line6_str, " error count %d", errorCnt);
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  memPos = 0; // return to 0 for other functions
  selBitx = 0;
  selBity = 0;
  }
if (key == '5'){ // read ascii once - names
  uint64_t storedMemContents = memContents;  // store for error checking read below
  errorCnt = 0;
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-8
      writeMem(selBitx, selBity, 0); // fills memContents with 0s
      bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = storedMemContents ^ memRead; // compare contents stored in data.ino with that read
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  // write core memory again to set up for reading
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(memRead, memPos); // store the value read earlier
      writeMem(selBitx, selBity, valBit); 
    }
  strcpy(oled_line1_str, "    read");
  // convert storedMemContents to string
  uint64_to_str8_reverse(oled_text_str, storedMemContents); 
  strcpy(oled_line2_str, oled_text_str);
  if (isStr8Ascii(oled_line2_str) == 0)
    { 
    strcpy(oled_line2_str, " not ascii        ");
    }
  // convert memRead to to string
  uint64_to_str8_reverse(oled_text_str, memRead); 
  strcpy(oled_line3_str, oled_text_str);
  if (isStr8Ascii(oled_line3_str) == 0)
    { 
    strcpy(oled_line3_str, "                   ");
    }
  sprintf(oled_line6_str, " error count %d", errorCnt);
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  memPos = 0; // return to 0 for other functions
  selBitx = 0;
  selBity = 0;
  }
if (key == '6'){ // display map
  // assemble left side data
  // read core memory
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-8
      writeMem(selBitx, selBity, 0); // fills memContents with 0s
      bitWrite64(memRead, memPos, senseIn);
    }
  // write core memory again to set up for more reading
  for (memPos = 0; memPos < 64; memPos++)
    {
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead64(memRead, memPos); // store the value read earlier
      writeMem(selBitx, selBity, valBit); 
    }  
    // convert memRead to to string
    // hex two lines
  uint64_t line3 = 0;
  uint64_t line4 = 0;
  uint64_t storeData = 0;
  uint64_t memReadData = memRead;
  // uint64_t memReadData = 0x0123456789abcdef;    
  for (int i = 0; i < 8; i++){
    storeData = memReadData & 0xf;      
    line4 = line4 | storeData;
    line4 = line4 << 4;
    memReadData = memReadData >> 4;
    storeData = memReadData & 0xf;      
    line3 = line3 | storeData;
    line3 = line3 << 4;
    memReadData = memReadData >> 4;
    }
  line4 = line4 >> 4; // delete 0 from final shift
  line3 = line3 >> 4;
  strcpy(oled_line1_str, "                   "); 
  strcpy(oled_line2_str, "                   ");
  sprintf(oled_line3_str, "%08llx", line3);
  sprintf(oled_line4_str, "%08llx", line4);
  // ascci
  uint64_to_str8_reverse(oled_text_str, memRead); 
  strcpy(oled_line5_str, oled_text_str);
  if (isStr8Ascii(oled_line5_str) == 0) { 
    strcpy(oled_line5_str, "         ");
    }
  strcpy(oled_line6_str, "                   ");
  updateMenuDisplay();
  // add graphics on right side 
  for (int xc = 0; xc < 8; xc++){
    for (int yc = 0; yc < 8; yc++){
      if ((bitRead64(memContents, ((xc * 8) + yc))) == 1){
        // display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, WHITE);
        }
      }
    }
  oledRect(); // also displays display
  }
if (key == '7'){ // read 6400
  errorCnt = 0;
  long int writeCnt = 0;   
  long int writeCntMax = 6400;
  // while loop until done
  while(1) {
  uint64_t storedMemContents = memContents;  // store for error checking read below
  // read core memory
  for (memPos = 0; memPos < 64; memPos++){
    selBitx = memPos / 8; // retruns 0-7
    selBity = memPos - (selBitx * 8); // returns 0-8
    writeMem(selBitx, selBity, 0); // fills memContents with 0s
    bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = storedMemContents ^ memRead; // compare contents stored in data.ino with that read
  // comparedResult++; // add error for testing
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  // write core memory again to set up for reading
  for (memPos = 0; memPos < 64; memPos++) {
    selBitx = memPos / 8; // retruns 0-7
    selBity = memPos - (selBitx * 8); // returns 0-7
    int valBit = bitRead64(memRead, memPos); // store the value read earlier
    writeMem(selBitx, selBity, valBit); 
    }
  strcpy(oled_line1_str, "    read");
  // sprintf format
  // the mandatory % character
  // a 0 flag for padding
  // the 16 as "minimum field width"
  // the ll as "length modifiers"  (lomg lomg)
  // the x conversion specifier (X for upper case)
  sprintf(oled_line2_str, "%016llx", storedMemContents);
  addSpacesL2(); // adds spaces to oled_line2_str
  sprintf(oled_line3_str, "%016llx", memRead);
  addSpacesL3(); // adds spaces to oled_line3_str
  sprintf(oled_line5_str, " error count %d", errorCnt);
  sprintf(oled_line6_str, " count %ld         ", writeCnt);  // d int, ld long int, lu unsigned long, ll long long int, llu unsigned long long int
  oled_line6_str[15] = '6';
  oled_line6_str[16] = '4';
  oled_line6_str[17] = '0';
  oled_line6_str[18] = '0'; // display width is 19 characters
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line5_str, 8, 43, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  // end while loop when done
  writeCnt = writeCnt + 64;   
  if (writeCnt > writeCntMax) {
    lmCommand = millis();  
    pmDisplayActive = millis();  // Remember the time to reset time out
    memPos = 0; // return to 0 for other functions
    selBitx = 0;
    selBity = 0;
    break;
    }
  if (getKey()){
    lmCommand = millis();  // choose one or the other
    pmDisplayActive =  millis();  
    memPos = 0; // return to 0 for other functions
    selBitx = 0;
    selBity = 0;
    return;
    }
  }  
  }
}

// 44444444444444444444444444
// 44444444444444444444444444
void xyExecute() // x-y set bit
{
if (key == '2'){
  // get current x and y
  selBitx = memPos / 8; // retruns 0-7
  selBity = memPos - (selBitx * 8); // returns 0-7
  selBitx = (selBitx + 1) % 8;
  memPos = (selBitx * 8) + selBity;
  sprintf(selBitx_str, "%2.2d", selBitx);  
  sprintf(selBity_str, "%2.2d", selBity);
  strcpy(oled_line1_str, " x");
  strcat(oled_line1_str, selBitx_str);
  strcat(oled_line1_str, " y");
  strcat(oled_line1_str, selBity_str);
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  if (inhibitFlag == 0) strcpy(oled_line6_str, " inh off");
  else strcpy(oled_line6_str, " inh on ");
  updateMenuDisplay();
  // add graphics on right side 
  for (int xc = 0; xc < 8; xc++){
    for (int yc = 0; yc < 8; yc++){
      if ((bitRead64(memContents, ((xc * 8) + yc))) == 1){
        // display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, WHITE);
        }
      }
    }
  oledRect(); // also displays display
  }
if (key == '3'){
  // get current x and y
  selBitx = memPos / 8; // retruns 0-7
  selBity = memPos - (selBitx * 8); // returns 0-7
  selBity = (selBity + 1) % 8;
  memPos = (selBitx * 8) + selBity;
  sprintf(selBitx_str, "%2.2d", selBitx);  
  sprintf(selBity_str, "%2.2d", selBity);
  strcpy(oled_line1_str, " x");
  strcat(oled_line1_str, selBitx_str);
  strcat(oled_line1_str, " y");
  strcat(oled_line1_str, selBity_str);
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  if (inhibitFlag == 0) strcpy(oled_line6_str, " inh off");
  else strcpy(oled_line6_str, " inh on ");
  updateMenuDisplay();
  // add graphics on right side 
  for (int xc = 0; xc < 8; xc++){
    for (int yc = 0; yc < 8; yc++){
      if ((bitRead64(memContents, ((xc * 8) + yc))) == 1){
        // display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, WHITE);
        }
      }
    }
  oledRect(); // also displays display
  }
if (key == '4'){ // toggle inhibit flag
  sprintf(selBitx_str, "%2.2d", selBitx);  
  sprintf(selBity_str, "%2.2d", selBity);
  strcpy(oled_line1_str, " x");
  strcat(oled_line1_str, selBitx_str);
  strcat(oled_line1_str, " y");
  strcat(oled_line1_str, selBity_str);
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  if (inhibitFlag == 0){
    inhibitFlag = 1;
    strcpy(oled_line6_str, " inh on");
    }
  else {
    inhibitFlag = 0;
    strcpy(oled_line6_str, " inh off");
    }
  updateMenuDisplay();
  // add graphics on right side 
  for (int xc = 0; xc < 8; xc++){
    for (int yc = 0; yc < 8; yc++){
      if ((bitRead64(memContents, ((xc * 8) + yc))) == 1){
        // display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, WHITE);
        }
      }
    }
  oledRect(); // also displays display
  // this is here so inhibit pins do not need to be cleared each write
  digitalWrite(inhibitPin0, LOW); // active high
  digitalWrite(inhibitPin1, LOW); // active high
  }
if (key == '5'){ // display map graphic with current xy position and inh state
  sprintf(selBitx_str, "%2.2d", selBitx);  
  sprintf(selBity_str, "%2.2d", selBity);
  strcpy(oled_line1_str, " x");
  strcat(oled_line1_str, selBitx_str);
  strcat(oled_line1_str, " y");
  strcat(oled_line1_str, selBity_str);
  //strcpy(oled_line1_str, "                   ");
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, "                   ");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  //strcpy(oled_line6_str, "                   ");
  if (inhibitFlag == 0) strcpy(oled_line6_str, " inh off ");
  else strcpy(oled_line6_str, " inh on ");
  updateMenuDisplay();
  // add graphics on right side 
  for (int xc = 0; xc < 8; xc++){
    for (int yc = 0; yc < 8; yc++){
      if ((bitRead64(memContents, ((xc * 8) + yc))) == 1){
        // display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, WHITE);
        }
      }
    }
  oledRect(); // also displays display
  }
if (key == '6'){
  writeMem(selBitx, selBity, 0); 
  // sprintf format
  // % → Start of a format specifier.
  // 2 → Minimum field width: The output will be at least 2 characters wide. 
  // If the number has fewer characters, it will be left-padded with spaces 
  // (unless 0 flag is used).  
   // .2 → Precision for integers: Minimum number of digits to print. If the
  //  number has fewer digits, it will be left-padded with zeros.
  // d → Signed decimal integer.
  // Note - could do %02d - two digits, if less pad with 0s
  sprintf(selBitx_str, "%02d", selBitx);
  sprintf(selBity_str, "%02d", selBity);
  strcpy(oled_line1_str, " x");
  strcat(oled_line1_str, selBitx_str);
  strcat(oled_line1_str, " y");
  strcat(oled_line1_str, selBity_str);
  strcpy(oled_line2_str, "  set 0");
  strcpy(oled_line3_str, "  sense ");
  if (senseIn == 0) strcat(oled_line3_str, "0");
  if (senseIn == 1) strcat(oled_line3_str, "1");
  //Serial.println("out SW0 str setup");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  if (inhibitFlag == 0) strcpy(oled_line6_str, " inh off");
  else strcpy(oled_line6_str, " inh on ");
  updateMenuDisplay();
  // add graphics on right side 
  for (int xc = 0; xc < 8; xc++){
    for (int yc = 0; yc < 8; yc++){
      if ((bitRead64(memContents, ((xc * 8) + yc))) == 1){
        // display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, WHITE);
        }
      else {  
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, BLACK);
        }
      }
    }
  oledRect(); // also displays display
  }
if (key == '7'){
  writeMem(selBitx, selBity, 1); 
  sprintf(selBitx_str, "%2.2d", selBitx);
  sprintf(selBity_str, "%2.2d", selBity);
  strcpy(oled_line1_str, " x");
  strcat(oled_line1_str, selBitx_str);
  strcat(oled_line1_str, " y");
  strcat(oled_line1_str, selBity_str);
  strcpy(oled_line2_str, "  set 1");
  strcpy(oled_line3_str, "  sense ");
  if (senseIn == 0) strcat(oled_line3_str, "0");
  if (senseIn == 1) strcat(oled_line3_str, "1");
  strcpy(oled_line4_str, "                   ");
  strcpy(oled_line5_str, "                   ");
  if (inhibitFlag == 0) strcpy(oled_line6_str, " inh off");
  else strcpy(oled_line6_str, " inh on ");
  updateMenuDisplay();
  // add graphics on right side 
  for (int xc = 0; xc < 8; xc++){
    for (int yc = 0; yc < 8; yc++){
      if ((bitRead64(memContents, ((xc * 8) + yc))) == 1){
        // display.fillRect(upperLeftX, upperLeftY, width, height, WHITE)
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, WHITE);
        }
      else {  
        display.fillRect((69 + (xc * 7)), (54 - (yc * 7)), 6, 6, BLACK);
        }
      }
    }
  oledRect(); // also displays display
  }
}

// 55555555555555555555555555
// 55555555555555555555555555
void miscExecute() // miscellaneous
{
if (key == '2'){ 
  valuesExecute();
  }
if (key == '3'){ 
  timingExecute();
  }
if (key == '4'){ 
  setMaxCntExecute();
  }
if (key == '5'){ 
  aboutExecute();
  }
}

//22222222222222222222222
void valuesExecute()
{
key = scanKeypad();
if (key != 'x'){
// wait for key release
  while(1){
    key = scanKeypad();
    if (key == 'x') {break;}
    }
  }  
startTimeOut = millis();
while(1){  
  inputVoltageRead = analogRead(inputVoltagePin);
  VsVoltageRead = analogRead(VsVoltagePin);
  delay(1); // better read after delay
  inputVoltageRead = 0;
  VsVoltageRead = 0;
  int rCnt;
  for (rCnt = 0; rCnt < 16; rCnt++){
    inputVoltageRead = inputVoltageRead + analogRead(inputVoltagePin); 
    VsVoltageRead = VsVoltageRead + analogRead(VsVoltagePin); 
    delay(1);
    } 
  inputVoltageRead = inputVoltageRead / rCnt; 
  inputVoltageValue = (((float)inputVoltageRead * 3.3 * 11) / 4096) + 0.6;  // input was 1 / 11 and 4096 is full 
  // scale with 3.3 volts - + 0.6 is correction factor from volt meter measurement
  VsVoltageRead = VsVoltageRead / rCnt; 
  VsVoltageValue = (((float)VsVoltageRead * 3.3 * 3) / 4096) + 0.25;  // input was 1 / 3 and 4096 is full 
  // scale with 3.3 volts - + 0.7 is correction factor from volt meter measurement
  // Check thermistor
  thermistorRead = analogRead(thermistorPin); 
  delay(1); // better read after delay
  thermistorRead = 0;
  for (rCnt = 0; rCnt < 16; rCnt++){
    thermistorRead = thermistorRead + analogRead(thermistorPin); 
    delay(1);
    } 
  thermistorRead = thermistorRead / rCnt; 
  thermistorValue = 35 + ((2300 - thermistorRead) / 10);
  // 
  strcpy(oled_line1_str, "   values");
  strcpy(oled_line2_str, "                   ");
  strcpy(oled_line3_str, " input  ");
  sprintf(oled_text_str, "%4.2f", inputVoltageValue);
  strcat(oled_line3_str, oled_text_str);
  strcat(oled_line3_str, "v");
  strcpy(oled_line4_str, " Vs     ");
  sprintf(oled_text_str, "%3.2f", VsVoltageValue);
  strcat(oled_line4_str, oled_text_str);
  strcat(oled_line4_str, "v");
  strcpy(oled_line5_str, " Temp   ");
  sprintf(oled_text_str, "%d", thermistorValue);
  strcat(oled_line5_str, oled_text_str);
  strcat(oled_line5_str, "F");
  strcpy(oled_line6_str, "                   ");
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line4_str, 8, 33, 1, false);
  oledText(oled_line5_str, 8, 43, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  for (int i = 0; i < 10; i ++){
    delay(100); // times 10 is one second
    key = scanKeypad();
    if (key != 'x'){
      // wait for key release
       while(1){
         key = scanKeypad();
         if (key == 'x') {break;}
         }
      showInstructions();
      executeState = menuState;
      return;
      }
   if ((millis() - startTimeOut) > 20000) {
      showInstructions();
      executeState = menuState;
      return;
      }
    }
  }
}

// 333333333333333333333333
void timingExecute()
{
key = scanKeypad();
if (key != 'x'){
// wait for key release
  while(1){
    key = scanKeypad();
    if (key == 'x') {break;}
    }
  }  
startTimeOut = millis(); 
// counts per second first
errorCnt = 0;
memRead = 0; 
unsigned long timeStartMillis = millis();
unsigned long timeMillis = 0;
int cycleCnt = 0;
inhibitFlag = 0; // clear flag
strcpy(oled_line1_str, "    timing         ");
strcpy(oled_line2_str, "                   ");
strcpy(oled_line3_str, "      running ...  ");
strcpy(oled_line4_str, "                   ");
strcpy(oled_line5_str, "                   ");
strcpy(oled_line6_str, "                   ");
display.clearDisplay();
oledText(oled_line1_str, 8, 3, 1, false);
oledText(oled_line2_str, 8, 13, 1, false);
oledText(oled_line3_str, 8, 23, 1, false);
oledText(oled_line4_str, 8, 33, 1, false);
oledText(oled_line5_str, 8, 43, 1, false);
oledText(oled_line6_str, 8, 53, 1, false);
oledRect(); // also displays display
while(timeMillis < 1000){
  writeMem(0, 0, 1); // write a 1
  // calculate times
  cycleCnt++;
  timeMillis = millis() - timeStartMillis;
  }
// fill screen for cycles
strcpy(oled_line1_str, "    timing         ");
strcpy(oled_line2_str, "                   ");
strcpy(oled_line3_str, " write ");
sprintf(oled_text_str, "%d", cycleCnt);
strcat(oled_line3_str, oled_text_str);
strcat(oled_line3_str, " / s ");
// us times next
decodeStartus = 0;
writeStartus = 0;
writeEndus = 0;
errorCnt = 0;
memRead = 0; 
inhibitFlag = 0; // clear flag
unsigned long minWriteToEndus = 1000;
unsigned long minDecodeToEndus = 1000;
for (int i = 0; i < 10; i++){  // discard interupt time
  writeMem(0, 0, 1); // write a 1
  // calculate times
  writeToEndus = writeEndus - writeStartus;
  decodeToEndus = writeEndus - decodeStartus;
  minWriteToEndus = min((writeEndus - writeStartus), minWriteToEndus);
  minDecodeToEndus = min((writeEndus - decodeStartus), minDecodeToEndus);
  }
writeMem(0, 0, 0); // clear 1 write
memPos = 0; // return to 0 for other functions
selBitx = 0;
selBity = 0;
// complete screen for us 
strcpy(oled_line4_str, " write ");
sprintf(oled_text_str, "%lu", minWriteToEndus);
strcat(oled_line4_str, oled_text_str);
strcat(oled_line4_str, "us");
strcpy(oled_line5_str, " w/decode ");
sprintf(oled_text_str, "%lu", minDecodeToEndus);
strcat(oled_line5_str, oled_text_str);
strcat(oled_line5_str, "us");
// complete screen
strcpy(oled_line6_str, "                   ");
updateMenuDisplay();
// wait for key
while(1){
  key = scanKeypad();
  if (key != 'x'){
    // wait for key release
    while(1){
      key = scanKeypad();
      if (key == 'x') {break;}
      }
    showInstructions();
    executeState = menuState;
    return;
    }
  if ((millis() - startTimeOut) > 20000) {
    showInstructions();
    executeState = menuState;
    return;
    }
  }
}

// 444444444444444444444444444
void setMaxCntExecute()
{
startTimeOut = millis(); 
strcpy(oled_line1_str, "  2 - 200K         ");
strcpy(oled_line2_str, "  3 - 500K         ");
strcpy(oled_line3_str, "  4 - 1M           ");
strcpy(oled_line4_str, "  5 - 2M           ");
strcpy(oled_line5_str, "  6 - 5M           ");
strcpy(oled_line6_str, "  7 - 10M          ");
updateMenuDisplay();
// wait for key
while(1){
  if (getKey()) {break;}
  if ((millis() - startTimeOut) > 20000) {
    showInstructions();
    executeState = menuState;
    return;
    }
  }
if (key == '0'){ // go back to misc
  showInstructions();
  executeState = menuState;
  return;
  }
if (key == '1'){ // go back to misc
  showInstructions();
  executeState = menuState;
  return;                                                                                                                                                                                                                       ; 
  }
if (key == '2'){  
  maxCnt = 200000;
  strcpy(strMaxCnt, "200K");
  }
if (key == '3'){  
  maxCnt = 500000;
  strcpy(strMaxCnt, "500K");
  }
if (key == '4'){  
  maxCnt = 1000000;
  strcpy(strMaxCnt, "1M");
  }
if (key == '5'){  
  maxCnt = 2000000;
  strcpy(strMaxCnt, "2M");
  }
if (key == '6'){  
  maxCnt = 5000000;
  strcpy(strMaxCnt, "5M");
  }
if (key == '7'){  
  maxCnt = 10000000;
  strcpy(strMaxCnt, "10M");
  }
// wait for key
while(1){
  key = scanKeypad();
  if (key != 'x'){
    // wait for key release
    while(1){
      key = scanKeypad();
      if (key == 'x') {break;}
      }
    showInstructions();
    executeState = menuState;
    return;
    }
  }
}

// 55555555555555555555
void aboutExecute()
{
key = scanKeypad();
if (key != 'x'){
// wait for key release
  while(1){
    key = scanKeypad();
    if (key == 'x') {break;}
    }
  }    
startTimeOut = millis();   
strcpy(oled_line1_str, "  information, code");
strcpy(oled_line2_str, "  and schematics   ");
strcpy(oled_line3_str, "  available at:    ");
strcpy(oled_line4_str, "                   ");
strcpy(oled_line5_str, "  www.github.com/  ");
strcpy(oled_line6_str, "  jpwolfe31        ");
updateMenuDisplay();
// wait for key
while(1){
  key = scanKeypad();
  if (key != 'x'){
    // wait for key release
    while(1){
      key = scanKeypad();
      if (key == 'x') {break;}
      }
    showInstructions();
    executeState = menuState;
    return;
    }
  if ((millis() - startTimeOut) > 20000) {
    showInstructions();
    executeState = menuState;
    return;
    }
  }
}

// subroutine for random write reads 1-100 million with and without inhibit
void randomWriteInhRead(long int startCnt, long int endCnt, int inhOn, char* strEndCnt, char* oled_title_str)
{
errorCnt = 0;
// clear memory
inhibitFlag = 0; // clear flag
for (memPos = 0; memPos < 64; memPos++) {
  selBitx = memPos / 8; // retruns 0-7
  selBity = memPos - (selBitx * 8); // returns 0-7
  writeMem(selBitx, selBity, 0); 
  }
memRead = 0; 
long int writeCnt = startCnt;   
long int writeCntMax = endCnt;
// while loop until done
while(1){  
  testM = random_uint64(); // get random number fromfunction below
  inhibitFlag = 0; // clear flag
  for (memPos = 0; memPos < 64; memPos++) { 
    selBitx = memPos / 8; // retruns 0-7
    selBity = memPos - (selBitx * 8); // returns 0-7
    int valBit = bitRead64(testM, memPos);
    writeMem(selBitx, selBity, valBit); 
    }
  if (inhOn == 1) {
    inhibitFlag = 1; // set flag
    uint64_t testMComp = ~testM;
    // write core memory with inhibit
    for (memPos = 0; memPos < 64; memPos++) { 
      selBitx = memPos / 8; // retruns 0-7
      selBity = memPos - (selBitx * 8); // returns 0-7
      int valBit = bitRead(testMComp, memPos);
      writeMem(selBitx, selBity, valBit); 
      }
    inhibitFlag = 0; // clear flag
    }
  for (memPos = 0; memPos < 64; memPos++) {
    selBitx = memPos / 8; // retruns 0-7
    selBity = memPos - (selBitx * 8); // returns 0-7
    writeMem(selBitx, selBity, 0); 
    bitWrite64(memRead, memPos, senseIn);
    }
  // check and update error count
  comparedResult = testM ^ memRead;
  // comparedResult++; // add error for testing
  if ((comparedResult != 0) && (errorReportFlag == 1)){
    Serial.println("Errors");
    for (memPos = 0; memPos < 64; memPos++)
      {
      if (bitRead64(comparedResult, memPos) == 1){
        selBitx = memPos / 8; // retruns 0-7
        selBity = memPos - (selBitx * 8); // returns 0-7
        sprintf(oled_text_str, "%02d", memPos);
        strcpy(oled_error_str, oled_text_str);
        sprintf(oled_text_str, "  %02d", selBitx);
        strcat(oled_error_str, oled_text_str);
        sprintf(oled_text_str, "  %02d", selBity);
        strcat(oled_error_str, oled_text_str);
        int valErr = bitRead64(testM, memPos);
        sprintf(oled_text_str, "  %1d", valErr);
        strcat(oled_error_str, oled_text_str);
        Serial.println(oled_error_str);
        }
      }
    }
  errorCnt = errorCnt + popcount64_branchless(comparedResult); // function below counts 1s in a uint64_t
  strcpy(oled_line1_str, oled_title_str);
  sprintf(oled_line2_str, "%016llx", testM); // llx for uint64_t hex
  addSpacesL2(); // adds spaces to oled_line2_str
  sprintf(oled_line3_str, "%016llx", memRead);
  addSpacesL3(); // adds spaces to oled_line3_str
  sprintf(oled_line5_str, " error count %d", errorCnt);
  if (writeCnt < 9999500) { // add K is write count 10M or more
    sprintf(oled_line6_str, " count %ld         ", writeCnt);  // d int, ld long int, lu unsigned long, ll long long int, llu unsigned long long int
    }
  else {sprintf(oled_line6_str, " count %ldK        ", (writeCnt + 500)/1000);}  // round
  // add endCnt ascii to end of line 6
  int k = strlen(strEndCnt) - 1; 
  for (int i = 0; i <= k; i++) { 
    oled_line6_str[18-i] = strEndCnt[k-i];  // str length 0 - 18 or 19 char
    }
  display.clearDisplay();
  oledText(oled_line1_str, 8, 3, 1, false);
  oledText(oled_line2_str, 8, 13, 1, false);
  oledText(oled_line3_str, 8, 23, 1, false);
  oledText(oled_line5_str, 8, 43, 1, false);
  oledText(oled_line6_str, 8, 53, 1, false);
  oledRect(); // also displays display
  // end while loop when done
  writeCnt = writeCnt + 64;   
  if (writeCnt > writeCntMax) {
    lmCommand = millis();  
    pmDisplayActive = millis();  // Remember the time to reset time out
    memPos = 0; // return to 0 for other functions
    selBitx = 0;
    selBity = 0;
    return;
    }
  if (getKey()){
    lmCommand = millis();  // choose one or the other
    pmDisplayActive =  millis();  
    memPos = 0; // return to 0 for other functions
    selBitx = 0;
    selBity = 0;
    return;
    }
  }  
}  
