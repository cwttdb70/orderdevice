// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_SSD1306.h>

// use hardware SPI
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Define the global variables for the photon//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
char val[100] = "0;0;0"; // Char array for accepting data from the web component

int loopTimerState = -1;

int LED = 7; //on-board LED

//Beep/flash/vibrate intervals or max attempts
int short1 = 500;
int long1 = 1000;
int short2 = 500;
int long2 = 2000;
int short3 = 1000;
int long3 = 16000;
int short1q = 4;
int short2q = 4;
int longq = 20;
int currentshort1 = 0;
int currentshort2 = 0;
int currentlong = 0;
int lastHigh = 0; //When buzzing/flashing, this tells you whether the buzz/flash was last on or off

//Item prices, quantities, and names
int q1 = -1;
int p1 = -1;
String item1Name = "Soup";
String item1Short = "sou";
int q2 = -1;
int p2 = -1;
String item2Name = "Pizza";
String item2Short = "piz";
int q3 = -1;
int p3 = -1;
String item3Name = "Hamburger";
String item3short = "ham";
int q4 = -1;
int p4 = -1;
String item4Name = "Fries";
String item4Short = "ffs";
int q5 = -1;
int p5 = -1;
String item5Name = "Soda";
String item5short = "sod";
float totalP = -1;

//Item prep times
int item1QT = -1;
int item2QT = -1;
int item3QT = -1;
int item4QT = -1;
int item5QT = -1;
int item1PT = -1;
int item2PT = -1;
int item3PT = -1;
int item4PT = -1;
int item5PT = -1;

//status (-1=NA, 0=queue, 1=prep, 2=complete) and time for each item
int status1 = -1;
int status2 = -1;
int status3 = -1;
int status4 = -1;
int status5 = -1;
String status1String;
String status2String;
String status3String;
String status4String;
String status5String;
int status1Time = -1;
int status2Time = -1;
int status3Time = -1;
int status4Time = -1;
int status5Time = -1;

//Using an 0/1 int to remember last state and take certain when orders are submitted
int orderSubmittedBool = -1;

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Define the main buzz timers and their functions//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
Timer shortTimerStart(short1, shortTimerStartFunction);
Timer longTimerStart(short3, longTimerStartFunction);

void shortTimerStartFunction() {
  if (currentshort1 >= short1q) { //If timer is done running
    shortTimerStart.stopFromISR();
    currentshort1 = 0;
    digitalWrite(D1, LOW);
    digitalWrite(LED, LOW);
    lastHigh = 0;
  }
  else { //If timer should still run more
    currentshort1 = currentshort1 + 1;
    if (lastHigh == 1) { //If it was last beeping
      digitalWrite(D1, LOW);
      digitalWrite(LED, LOW);
      lastHigh = 0;
    }
    else if (lastHigh == 0) { //If not last beeping
      tone(D0, 5000, 250);
      digitalWrite(D1, HIGH);
      digitalWrite(LED, HIGH);
      lastHigh = 1;
    }
    else {}
    currentshort1 = currentshort1 + 1;
  }
}

void longTimerStartFunction() {
  if (currentlong >= longq) { //If timer done running
    longTimerStart.stop();
    currentlong = 0;
    digitalWrite(D1, LOW);
    digitalWrite(LED, LOW);
    lastHigh = 0;
  }
  else { //If timer not done running
    currentlong = currentlong + 1;
    if (lastHigh == 1) {
      digitalWrite(D1, LOW);
      digitalWrite(LED, LOW);
      lastHigh = 0;
    }
    else if (lastHigh == 0) {
      tone(D0, 5000, 250);
      digitalWrite(D1, HIGH);
      digitalWrite(LED, HIGH);
      lastHigh = 1;
    }
    else {}
    currentlong = currentlong + 1;
  }
}



/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Binary dot art for the items stored in progmem//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//soup
static const unsigned char PROGMEM q1Array [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x40, 0x41, 0x00, 0x00, 0x60, 0x41, 0x00, 0x00, 0x20, 0xc1, 0x00, 0x00, 0x30, 0x81, 0x80, 0x00, 0x10, 0x80, 0x80, 0x00, 0x10, 0x80, 0x80, 0x00, 0x10, 0x00, 0x80, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x10, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x08, 0x18, 0x00, 0x00, 0x18, 0x08, 0x00, 0x00, 0x10, 0x0e, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x03, 0x00, 0x00, 0xc0, 0x01, 0x80, 0x01, 0x80, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//pizza
static const unsigned char PROGMEM q2Array [] = {0x7f, 0xff, 0xff, 0xfe, 0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02, 0x7f, 0xff, 0xff, 0xfe, 0x40, 0x00, 0x00, 0x92, 0x60, 0xc0, 0x00, 0x66, 0x21, 0x20, 0x00, 0x04, 0x32, 0x10, 0x1c, 0x0c, 0x12, 0x10, 0x22, 0x08, 0x19, 0x20, 0x41, 0x18, 0x08, 0xc0, 0x80, 0x90, 0x0c, 0x00, 0x80, 0xb0, 0x04, 0x00, 0x80, 0xa0, 0x06, 0x00, 0x41, 0x60, 0x02, 0x00, 0x22, 0x40, 0x03, 0x00, 0x1c, 0xc0, 0x01, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0xa0, 0x01, 0x00, 0x00, 0xd0, 0x03, 0x00, 0x00, 0x48, 0x02, 0x00, 0x00, 0x68, 0x66, 0x00, 0x00, 0x30, 0x94, 0x00, 0x00, 0x31, 0x0c, 0x00, 0x00, 0x11, 0x08, 0x00, 0x00, 0x18, 0x98, 0x00, 0x00, 0x08, 0x70, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00};
//burger
static const unsigned char PROGMEM q3Array [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x74, 0x0e, 0x00, 0x00, 0xc1, 0x23, 0x00, 0x01, 0x88, 0x09, 0x80, 0x01, 0x40, 0x80, 0x80, 0x03, 0x12, 0x0a, 0xc0, 0x02, 0x80, 0x40, 0x40, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x04, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x20, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x02, 0x10, 0x40, 0x40, 0x03, 0x42, 0x12, 0xc0, 0x01, 0x00, 0x40, 0x80, 0x01, 0x89, 0x05, 0x80, 0x00, 0xc0, 0x23, 0x00, 0x00, 0x71, 0x0e, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//fries
static const unsigned char PROGMEM q4Array [] = {0x00, 0x07, 0xbc, 0x00, 0x03, 0xc4, 0xa4, 0x00, 0x02, 0x7c, 0xbe, 0xf0, 0x0e, 0x44, 0xa2, 0x90, 0x0a, 0x44, 0xa2, 0x90, 0x0a, 0x44, 0xfb, 0xd0, 0x0a, 0x44, 0x8a, 0x50, 0x0a, 0x44, 0x8a, 0x50, 0x0a, 0x5e, 0x8a, 0x50, 0x0a, 0x52, 0x8a, 0x50, 0x0a, 0x52, 0x8a, 0x50, 0x0a, 0x52, 0x8a, 0x50, 0x0a, 0x52, 0x8a, 0x50, 0x0a, 0x52, 0x8a, 0x50, 0x0a, 0x52, 0x8a, 0x50, 0x0a, 0x52, 0x8a, 0x50, 0x0e, 0x52, 0x8a, 0x70, 0x0b, 0x52, 0x8a, 0xd0, 0x09, 0xd2, 0x8b, 0x90, 0x08, 0x7f, 0xfe, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00};
//soda
static const unsigned char PROGMEM q5Array [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xff, 0xff, 0x80, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00};


/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Function that runs and parses data sent from web//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
int setValue(String inputValue) { // Function for getting data when sent from the web
  strcpy(val, inputValue.c_str()); //copy it into our char array
  Particle.publish("FulStrng", String(inputValue)); //see what the full string was for good measure

  //Let's do some extraction
  String command = inputValue.substring(0, inputValue.indexOf("#")); // Get command
  Spark.publish("Command", String(command)); //publish command

  if (command == "submitorder") { //order submitted
    orderSubmittedBool = 1;
    //Set all items to queue (status=0)
    status1 = 0;
    status2 = 0;
    status3 = 0;
    status4 = 0;
    status5 = 0;

    //The long and winding road of substrings
    String temp1 = inputValue.substring(inputValue.indexOf("#") + 1); //Cut down string before 1
    String q1value = temp1.substring(3, temp1.indexOf("#")); //q1
    q1 = atoi(q1value);

    String temp2 = temp1.substring(temp1.indexOf("#") + 1); //Cut down string before 2
    String q2value = temp2.substring(3, temp2.indexOf("#")); //q2
    q2 = atoi(q2value);

    String temp3 = temp2.substring(temp2.indexOf("#") + 1); //Cut down string before 3
    String q3value = temp3.substring(3, temp3.indexOf("#")); //q3
    q3 = atoi(q3value);

    String temp4 = temp3.substring(temp3.indexOf("#") + 1); //Cut down string before 4
    String q4value = temp4.substring(3, temp4.indexOf("#")); //q4
    q4 = atoi(q4value);

    String temp5 = temp4.substring(temp4.indexOf("#") + 1); //Cut down string before 5
    String q5value = temp5.substring(3, temp5.indexOf("#")); //q5
    q5 = atoi(q5value);

    String temp6 = temp5.substring(temp5.indexOf("#")); //Cut down string before price
    String pricevalue = temp6.substring(3); //price
    totalP = pricevalue.toFloat();
  }

  else if (command == "completeorder") { // order completed
    longTimerStart.reset(); // start the completed timer
    Particle.publish("Twilio-SMS/+TWILIO#HERE", "Your order is ready!", PRIVATE); //send sms alert

    //reset all of the variables
    status1 = -1;
    status2 = -1;
    status3 = -1;
    status4 = -1;
    status5 = -1;
    q1 = -1;
    q2 = -1;
    q3 = -1;
    q4 = -1;
    q5 = -1;
    p1 = -1;
    p2 = -1;
    p3 = -1;
    p4 = -1;
    p5 = -1;
    totalP = -1;
    status1 = -1;
    status2 = -1;
    status3 = -1;
    status4 = -1;
    status5 = -1;
    status1String = NULL;
    status2String = NULL;
    status3String = NULL;
    status4String = NULL;
    status5String = NULL;
    status1Time = -1;
    status2Time = -1;
    status3Time = -1;
    status4Time = -1;
    status5Time = -1;
  }

  else if (command == "resetorders") { //if reset button pushed
    //Let's reset all of the relevant variables without sending an sms alert
    status1 = -1;
    status2 = -1;
    status3 = -1;
    status4 = -1;
    status5 = -1;
    q1 = -1;
    q2 = -1;
    q3 = -1;
    q4 = -1;
    q5 = -1;
    p1 = -1;
    p2 = -1;
    p3 = -1;
    p4 = -1;
    p5 = -1;
    totalP = -1;
    status1 = -1;
    status2 = -1;
    status3 = -1;
    status4 = -1;
    status5 = -1;
  }

  else if (command == "modeta") { //if the eta for any item was modified

    String temp1 = inputValue.substring(inputValue.indexOf("#") + 1, inputValue.indexOf("#") + 3); //Cut down string before 1
    Spark.publish("temp1=", String(temp1));
    int getValue = atoi(inputValue.substring(inputValue.indexOf("#") + 4)); //get the value of the new eta

    //depending on which item eta was modified, set that items eta to the correct new value
    if (temp1 == "1q") {
      item1QT = getValue;
    }
    else if (temp1 == "1p") {
      item1PT = getValue;
    }
    else if (temp1 == "2q") {
      item2QT = getValue;
    }
    else if (temp1 == "2p") {
      item2PT = getValue;
    }
    else if (temp1 == "3q") {
      item3QT = getValue;
    }
    else if (temp1 == "3p") {
      item3PT = getValue;
    }
    else if (temp1 == "4q") {
      item4QT = getValue;
    }
    else if (temp1 == "4p") {
      item4PT = getValue;
    }
    else if (temp1 == "5q") {
      item5QT = getValue;
    }
    else if (temp1 == "5p") {
      item5PT = getValue;
    }
    else {
      display.println("ERROR w/change eta");
    }

  }

  else if (command == "moditemstatus") { //if the status of an item was modified
    String temp1 = inputValue.substring(inputValue.indexOf("#") + 1, inputValue.indexOf("#") + 3); //Cut down string before 1
    int getValue = atoi(inputValue.substring(inputValue.indexOf("#") + 4));
    shortTimerStart.resetFromISR(); // run the short timer

    //depending on which item status was modified, set that items status to the correct new value
    if (temp1 == "i1") {
      status1 = getValue;
    }
    else if (temp1 == "i2") {
      status2 = getValue;
    }
    else if (temp1 == "i3") {
      status3 = getValue;
    }
    else if (temp1 == "i4") {
      status4 = getValue;
    }
    else if (temp1 == "i5") {
      status5 = getValue;
    }
    else {
      display.println("ERROR w/change item status");
    }
  }

  else if (command == "ieta") { //this command is when the page first loads, it sends the etas of each item
    //get the etas of each item in queue and prep, and set accordingly

    String temp1 = inputValue.substring(inputValue.indexOf("#") + 1); //Cut down string before 1
    String q1value = temp1.substring(3, temp1.indexOf("#"));
    item1QT = atoi(q1value);

    String temp2 = temp1.substring(temp1.indexOf("#") + 1);
    String q2value = temp2.substring(3, temp2.indexOf("#"));
    item1PT = atoi(q2value);

    String temp3 = temp2.substring(temp2.indexOf("#") + 1);
    String q3value = temp3.substring(3, temp3.indexOf("#"));
    item2QT = atoi(q3value);

    String temp4 = temp3.substring(temp3.indexOf("#") + 1);
    String q4value = temp4.substring(3, temp4.indexOf("#"));
    item2PT = atoi(q4value);

    String temp5 = temp4.substring(temp4.indexOf("#") + 1);
    String q5value = temp5.substring(3, temp5.indexOf("#"));
    item3QT = atoi(q5value);

    String temp6 = temp5.substring(temp5.indexOf("#") + 1);
    String q6value = temp6.substring(3, temp6.indexOf("#"));
    item3PT = atoi(q6value);

    String temp7 = temp6.substring(temp6.indexOf("#") + 1);
    String q7value = temp7.substring(3, temp7.indexOf("#"));
    item4QT = atoi(q7value);

    String temp8 = temp7.substring(temp7.indexOf("#") + 1);
    String q8value = temp8.substring(3, temp8.indexOf("#"));
    item4PT = atoi(q8value);

    String temp9 = temp8.substring(temp8.indexOf("#") + 1);
    String q9value = temp9.substring(3, temp9.indexOf("#"));
    item5QT = atoi(q9value);

    String temp10 = temp9.substring(temp9.indexOf("="));
    String q10value = temp10.substring(1);
    item5PT = atoi(q10value);
  }
  else {
    display.print("ERROR");
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Function that transitions through the applicable LED screens
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void loopTimerFunction() {
  if (orderSubmittedBool == 1) { // suborder wasn't publishing 100% of time in normal position for some reason. bit of a hack here.
    Particle.publish("FulStrng", "123test");
    Spark.publish("suborder", String(totalP));
    orderSubmittedBool = -1;
  }

  display.clearDisplay(); // clear the display
  if (q1 > 0 || q2 > 0 || q3 > 0 || q4 > 0 || q5 > 0) { //if an order is present
    if (loopTimerState == 0) { //This next long part determines what the "next" screen will be
      if (q1 > 0) {
        loopTimerState = 1;
      }
      else if (q2 > 0) {
        loopTimerState = 2;
      }
      else if (q3 > 0) {
        loopTimerState = 3;
      }
      else if (q4 > 0) {
        loopTimerState = 4;
      }
      else if (q5 > 0) {
        loopTimerState = 5;
      }
      else {
        loopTimerState = 0;
      }
    }
    else if (loopTimerState == -1) {
      loopTimerState = 0;
    }
    else if (loopTimerState == 1) {
      if (q2 > 0) {
        loopTimerState = 2;
      }
      else if (q3 > 0) {
        loopTimerState = 3;
      }
      else if (q4 > 0) {
        loopTimerState = 4;
      }
      else if (q5 > 0) {
        loopTimerState = 5;
      }
      else {
        loopTimerState = 0;
      }
    }
    else if (loopTimerState == 2) {
      if (q3 > 0) {
        loopTimerState = 3;
      }
      else if (q4 > 0) {
        loopTimerState = 4;
      }
      else if (q5 > 0) {
        loopTimerState = 5;
      }
      else {
        loopTimerState = 0;
      }
    }
    else if (loopTimerState == 3) {
      if (q4 > 0) {
        loopTimerState = 4;
      }
      else if (q5 > 0) {
        loopTimerState = 5;
      }
      else {
        loopTimerState = 0;
      }
    }
    else if (loopTimerState == 4) {
      if (q5 > 0) {
        loopTimerState = 5;
      }
      else {
        loopTimerState = 0;
      }
    }
    else if (loopTimerState == 5) {
      loopTimerState = 0;
    }
  }
  else { // if there is no order, go back to inactive screen
    loopTimerState = -1;
  }

  if (loopTimerState == -1) { //print screen inactive
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Welcome to Meepos bar");
    display.setCursor(0, 10);
    display.println("and grill!");
    display.setCursor(0, 30);
    display.println("Please place an order");
    display.setCursor(0, 40);
    display.println("with the app.");
    display.display();
  }
  else if (loopTimerState == 0) { //overview screen
    display.clearDisplay();
    display.setCursor(0, 0);
    if (status1Time > 0 || status2Time > 0 || status3Time > 0 || status4Time > 0 || status5Time > 0) { //make sure items have status times, then print applicable ones
      if (q1 > 0) {
        display.println();
        display.print("Soup: ");
        display.print(status1Time);
        display.print(" mins");
      }
      if (q2 > 0) {
        display.println();
        display.print("Pizza: ");
        display.print(status2Time);
        display.print(" mins");
      }
      if (q3 > 0) {
        display.println();
        display.print("Burger: ");
        display.print(status3Time);
        display.print(" mins");
      }
      if (q4 > 0) {
        display.println();
        display.print("Fries: ");
        display.print(status4Time);
        display.print(" mins");
      }
      if (q5 > 0) {
        display.println();
        display.print("Soda: ");
        display.print(status5Time);
        display.print(" mins");
      }
      display.println();
      display.print("Order ETA: ");

      //Get the orderEta by finding the MAX of any individual item ETA
      int orderETA = status1Time;
      if (status2Time > orderETA) {
        orderETA = status2Time;
      }
      if (status3Time > orderETA) {
        orderETA = status3Time;
      }
      if (status4Time > orderETA) {
        orderETA = status4Time;
      }
      if (status5Time > orderETA) {
        orderETA = status5Time;
      }
      display.print(orderETA);
      display.print(" mins");
      display.println();
      display.print("Order Total: $");
      display.print(totalP);
      display.display();
    }

    else { //Run this screen instead of the overview screen the first loop through after order is made
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Thanks for ordering!");
      display.display();
    }
  }

  else if (loopTimerState == 1) { //If an order is present and item 1 is ordered and we are in item 1's state
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.print("Item: ");
    display.print(item1Name);
    display.println();
    display.print("Quantity: ");
    display.print(q1);
    display.println();
    display.print("Status: ");
    if (status1 == -1) { //Get correct string based on status
      status1String = "Not ordered";
    }
    else if (status1 == 0) {
      status1String = "Queue";
    }
    else if (status1 == 1) {
      status1String = "Prep";
    }
    else if (status1 == 2) {
      status1String = "Done";
    }
    else {
      status1String = "ERR";
    }
    if (status1 == 0) {
      status1Time = item1QT + item1PT; //if in queue,time is queue time + prep time
    }
    else if (status1 == 1) {
      status1Time = item1PT; //if in prep, time is just prep time
    }
    else if (status1 == 2) {
      status1Time = 0; //if item done, time is 0
    }
    else {
      status1Time = -1;
    }
    display.print(status1String);
    display.println();
    display.print("ETA: ");
    display.print(status1Time);
    display.print(" mins");
    display.drawBitmap(85, 5, q1Array, 32, 32, WHITE);
    display.display();
    delay(3000);
    display.clearDisplay();
  }

  else if (loopTimerState == 2) { //If an order is present and item 2 is ordered and we are in item 2's state
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.print("Item: ");
    display.print(item2Name);
    display.println();
    display.print("Quantity: ");
    display.print(q2);
    display.println();
    display.print("Status: ");
    if (status2 == -1) {
      status2String = "Not ordered";
    }
    else if (status2 == 0) {
      status2String = "Queue";
    }
    else if (status2 == 1) {
      status2String = "Prep";
    }
    else if (status2 == 2) {
      status2String = "Done";
    }
    else {
      status2String = "ERR";
    }
    if (status2 == 0) {
      status2Time = item2QT + item2PT;
    }
    else if (status2 == 1) {
      status2Time = item2PT;
    }
    else if (status2 == 2) {
      status2Time = 0;
    }
    else {
      status2Time = -1;
    }
    display.print(status2String);
    display.println();
    display.print("ETA: ");
    display.print(status2Time);
    display.print(" mins");
    display.drawBitmap(85, 5, q2Array, 32, 32, WHITE);
    display.display();
    delay(3000);
    display.clearDisplay();
  }

  else if (loopTimerState == 3) { //If an order is present and item 3 is ordered and we are in item 3's state
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.print("Item: ");
    display.print(item3Name);
    display.println();
    display.print("Quantity: ");
    display.print(q3);
    display.println();
    display.print("Status: ");
    if (status3 == -1) {
      status3String = "Not ordered";
    }
    else if (status3 == 0) {
      status3String = "Queue";
    }
    else if (status3 == 1) {
      status3String = "Prep";
    }
    else if (status3 == 2) {
      status3String = "Done";
    }
    else {
      status3String = "ERR";
    }
    if (status3 == 0) {
      status3Time = item3QT + item3PT;
    }
    else if (status3 == 1) {
      status3Time = item3PT;
    }
    else if (status3 == 2) {
      status3Time = 0;
    }
    else {
      status3Time = -1;
    }
    display.print(status3String);
    display.println();
    display.print("ETA: ");
    display.print(status3Time);
    display.print(" mins");
    display.drawBitmap(85, 5, q3Array, 32, 32, WHITE);
    display.display();
  }

  else if (loopTimerState == 4) { //If an order is present and item 4 is ordered and we are in item 4's state
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.print("Item: ");
    display.print(item4Name);
    display.println();
    display.print("Quantity: ");
    display.print(q4);
    display.println();
    display.print("Status: ");
    if (status4 == -1) {
      status4String = "Not ordered";
    }
    else if (status4 == 0) {
      status4String = "Queue";
    }
    else if (status4 == 1) {
      status4String = "Prep";
    }
    else if (status4 == 2) {
      status4String = "Done";
    }
    else {
      status4String = "ERR";
    }
    if (status4 == 0) {
      status4Time = item4QT + item4PT;
    }
    else if (status4 == 1) {
      status4Time = item4PT;
    }
    else if (status4 == 2) {
      status4Time = 0;
    }
    else {
      status4Time = -1;
    }
    display.print(status4String);
    display.println();
    display.print("ETA: ");
    display.print(status4Time);
    display.print(" mins");
    display.drawBitmap(85, 5, q4Array, 32, 32, WHITE);
    display.display();
  }

  else if (loopTimerState == 5) { //If an order is present and item 5 is ordered and we are in item 5's state
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.print("Item: ");
    display.print(item5Name);
    display.println();
    display.print("Quantity: ");
    display.print(q5);
    display.println();
    display.print("Status: ");
    if (status5 == -1) {
      status5String = "Not ordered";
    }
    else if (status5 == 0) {
      status5String = "Queue";
    }
    else if (status5 == 1) {
      status5String = "Prep";
    }
    else if (status5 == 2) {
      status5String = "Done";
    }
    else {
      status5String = "ERR";
    }
    if (status5 == 0) {
      status5Time = item5QT + item5PT;
    }
    else if (status5 == 1) {
      status5Time = item5PT;
    }
    else if (status5 == 2) {
      status5Time = 0;
    }
    else {
      status5Time = -1;
    }
    display.print(status5String);
    display.println();
    display.print("ETA: ");
    display.print(status5Time);
    display.drawBitmap(85, 5, q5Array, 32, 32, WHITE);
    display.display();
  }
}


/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Create the loopTimer timer that transitions through the screens
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
Timer loopTimer(2500, loopTimerFunction);

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Setup
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC);
  Spark.function("setval", setValue); //This is a variable/function we can send to the web
  Spark.variable("getval", val, STRING); //This is the variable we can get from the web

  //Display defaults
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //PINS
  pinMode(D0, OUTPUT); //beeper
  pinMode(D1, OUTPUT); //buzzer
  pinMode(LED, OUTPUT);//LED

  //start the screen transition timer
  loopTimer.reset();
  loopTimer.start();

}

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//Loop (nothing should go in here in the current setup)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void loop() {
}