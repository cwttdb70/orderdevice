

1. Executive Summary

This project uses technologies and devices taught in this class to create a high-tech/improved version of an everyday IOT gadget. Many of us are familiar with restaurant buzzers--the small often circular pucks that buzz when your order or table is ready at a restaurant. While there are no fatal flaws with these devices, they leave much to be desired. How long until my order is ready? What's the status of each of my items? With such information, these devices would be more useful to the customer, thus providing an added value and perhaps making the customer more likely to patronize the restaurant. On the restaurant side, implementing this system/devices would provide them with more granular knowledge on how long it takes to make items. Trends could be made--what days are busiest? What items on what days? And this information could be used to improve efficiency.

This project sets up a dual system that includes a built PhoneGap app using Onsen and a Photon connected to wireless. On the web side, a user can input the items they want, see their total, and order. Their "buzzer" (LCD screen) will then cycle through the items they have ordered, showing them where in the preparation process each item is, as well as providing an image representation of the item, a reminder of how many they ordered, an ETA for each item, an overview screen of all items ordered, and an ETA for the order as a whole. On the restaurant side, one can see the orders as they come in and adjust the status of each item on the order, change the time it takes in queue or preparation for each item, reset all orders, and mark the order done (to buzz the buzzer) when the order is complete.


4. A summary of “needs”.

4.1 What hardware or networking technologies will you use?
The main hardware will be a Photon. It will use Wifi to connect to the cloud and get/set functions and variables. Connected to the photon will be an LCD screen and a vibration device.

4.2 What platforms will you use?
The photon will be using the Particle cloud development IDE and console. On the web side will be a PhoneGap built into an app using Onsen styling.

4.3 Do you need hardware that you don’t currently have access to?
No, all required hardware is available.

4.4 What will you merely emulate as a proof-of-concept and how will you emulate it?
There are a few minor differences between the project version and a potential mass-market verison. First, the web app can only connect to one buzzer/photon right now to monitor one order. In a release version, the web app would be connected to multiple photons/buzzers and would be able to communicate and interact with all of them. Furthermore, a mass-market version would not use PhoneGap--it would likely be running on a permanent desktop at the restaurant (and the data perhaps stored in the cloud), thus making it more reliable/redundant.

5 Draft of formal message exchange formats and data.

5.1 Data exchanged from the app to the photon

Because of the high number/amount of variables that the web app must send to the Photon, and the limited amount of variables/functions allowed to be registered to the cloud, the web app sends a bunch of data together in a string to the photon via one specific function when particular actions are done. The major actions are: Submitting an order on the user side, changing item preparation status on the restaurant side, marking order as done on the restaurant side, resetting orders on the restaurant side, and changing item queue or preparation time on the restaurant side.

The data is concatenated into a string in the pound sign # separated format that follows:
NOTE: The string must be less than 64 characters. As such, keep the “variable” names as short as possible.
String stringExample = “command#variable1=#variable2=#variable3=#variable4=.     ...etc
First is the command, which tells the photon what action was taken—was it an order submission, or a change to an item status, or was an order marked done?
The commands are: 

(1)submitorder
This is sent when an order by a customer is submitted.
The format expected by the Photon with this command is:
“submitorder#q1=#q2=#q3=#q4=#q5=#p=“
where q[x] is quantity of item q, and p is total price of the order. 

(2)completeorder
This is sent when the restaurant marks an order complete.
The format expected by the Photon with this command is:
“completeorder#“
No variables are sent with this command.

(3)resetorders
This is sent when the restaurant resets all orders in the system.
The format expected by the Photon with this command is:
“resetorders#“
No variables are sent with this command.

(4)modeta
This is sent when the restaurant changes the ETA for an item’s queue or prep time.
The format expected by the Photon with this command is:
“modeta#x[p or q]=z”
Where x is the number of the item (1-5 in this case), p represents preparation time or q represents queue time, and z is the new time.

(5)moditemstatus
This is sent when the restaurant changes the status of an item in an order. 
The format expected by the Photon with this command is:
“moditemstatus#ix=z”
Where i is the letter "i", x is the numerical value of the item (1-5), and z is the new status, 0, 1, or 2, where 0=queue, 1=preparation, 2=done. 

(6)ieta
This is sent at first load, and when an order is submitted, and tells the photon the prep and queue time for each item.
The format expected by the Photon with this command is:
“ieta#1q=#1p=#2q=#2p=#3q=#3p=#4q=#4p=#5q=#5p=“
Where the value after each equals sign is the preparation (p) or queue (q) time for the item associated with the number 1-5.


Then, a post request is sent to the Photon cloud with the concatenated string.

On the photon side, a character array of 100 characters has been defined at the top as a global variable. When the string arrives via the cloud function, it is passed in a string, and copied over to the existing char array:
strcpy(val, inputValue.c_str());

The photon isolates the command, and then splits up the string into the corresponding variables, and reacts to those new variables, accordingly.

6: The App
The app is a PhoneGap built app using onsen styling. There are three tabs in the app:

(1) Menu - A customer tab
This displays each item and price. Users can select quantities of each item, and the price will dynamically update, including an effective tax rate (can be modified in the js file). The submit button sends a “submitorder” command to the photon as described above.

(2) Order Control - A restaurant tab
This always displays a reset button, which sends a “resetorders” command. When an order is present, it displays that order. Each item is listed under the order, and the restaurant can set the item status as “Queue”, “Preparing”, or “Done” for each item in the order. Doing that sends a “moditemstatus” command. The restaurant can also mark the order as complete, which sends a “completeorder” command.

(3) Times - A restaurant tab
The times tab displays each item as a row, and Queue and Preparaton time for each item in the 2 columns. To the side of the time value, there is an up and down arrow, which let the restaurant change the current queue time or preparation time for each item. This sends a “modeta” command as described above.

Aside from built-in Onsen styling, the app uses minimal additional CSS (to fix some Onsen quirks). The only javascript library imported other than those built in to PhoneGap/Onsen is Jquery.

7: The Javascript
The index.js file is heavily commented, but some important information is as follows:
You must place your photon’s deviceID and accessToken in the variables named accordingly as fixed strings.
getVal is set up to receive data from the Photon, while setVal is used in sending data to the photon.
The item prices and prep times are set initially (p1-5, item1-5QT/PT), and can be adjusted.

The most important function is that which sends data to the Photon, and it is as follows:
    function sparkSend(newValue) {
        var requestURL = "https://api.spark.io/v1/devices/" + deviceID + "/" + setval + "/";
        $.post(requestURL, {
            params: newValue,
            access_token: accessToken
        });
    }

Make sure your deviceID and accessToken are correct, and that your setVal string is less than 64 characters.


8: The Photon and Peripherals

This configuration uses the Particle Cloud to flash code wirelessly to the Photon: https://build.particle.io, although flashing code locally should work as well.

8.1: The Photon Code
The photon code is heavily commented, but a few important things are as follows:
char val[100] is the char array that takes in the strings sent from the app. In order to do so, you must strcpy it over as follows:
int setValue(String inputValue) { // Function for getting data when sent from the web. inputValue is the string sent over.
  strcpy(val, inputValue.c_str());
From there, it uses string methods to get the command, and then go to the appropriate if statement.

The beep intervals can all be modified in the global variables section.

The images for each item are stored in unsigned char arrays in PROGMEM. This is (allegedly) the only way to do it. Since PROGMEM is limited, I do not recommend adding too many of these. Storing 5 has yet to present any problems, but YMMV. To create a new image, go to http://dot2pic.com/, choose a 32x32 grid (or any size you want, but multiples of 8 b/c byte sizes are preferable), and choose Monochromatic 8 pixels per byte horizontal. Create your masterpiece (I have only tested the default black), and then hit convert to the array. You will end up with something like this (for those wondering, this is an exclamation point):

const int array[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

Simply modify it to be: 
static const unsigned char PROGMEM yourArrayName [] ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
to store it in PROGMEM, and you can now print it on the LCD display.

There are three timers used:

1. shortTimerStart — runs when an item status changes, and does a short beep/buzz/flash.
This does NOT use a second timer to notify it when to stop running. Instead, it increments an integer every time it runs (i.e currentshort1), and once it has run more than the configured # of times, it is told to stop.

2. longTimerStart - runs when an order is complete, and does a long beep/buzz/flash.
This does NOT use a second timer to notify it when to stop running. Instead, it increments an integer every time it runs (i.e currentlong), and once it has run more than the configured # of times, it is told to stop.

3. loopTimer - runs ever 2.5 seconds (configurable) and shuffles through the appropriate screens. There are 7 screens/states that it can be in:
-1: No order has been placed
0: Overview of the order screen. NOTE: The first time this screen hits after an ordered is placed, it displays a thank you screen instead.
1: Item 1 overview
2: Item 2 overview
3: Item 3 overview:
4: Item 4 overview
5: Item 5 overview
The loopTimerFunction look at the current loopTimerState. If no order is present, it stays in -1. If an order is placed, it will go in sequence (0-5, repeating), but only including the states for which items are included in the order. I.e, if you ordered just items 3 and 5, it would loop from state 0 to 3 to 5 to 0, etc.

SETUP:
Defines the cloud variable(s) and function(s). It is necessary to use cloud-types in order to use POST requests, and thus communicate with them from the web/app side. Sets the LCD defaults (more info below). Sets the pins (D0=beeper, D1=buzzer, LED=7=internal LED). Starts the loopTimer.

LOOP:
There is no code in the loop, as all “repeating” functions are done with timers.

8.2: External Libraries
The Adafruit_SSD1306 library must be imported. If you are using the Particle Cloud, you can click on libraries on the left, and search for it to import it. You CAN NOT simply add the lines at the top, you must import it through the interface, and doing so will add the import code lines. Once you have imported it, it should look as follows:
#include <Adafruit_SSD1306.h>
// use hardware SPI
#define OLED_DC     D3
#define OLED_CS     D4
#define OLED_RESET  D5
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

If any of those lines are missing (and you did the interface import), then add them.
For information on the Adafruit_SSD1306 hardware or library, see: https://github.com/adafruit/Adafruit_SSD1306


8.3: Peripheral Hardware

8.3.1: Adafruit_SSD1306
This LCD is used to shuffle between the current items and overview (or idle screen if no order). It can display pixel/dot art as described above. Information on importing the library is found above.
More information can be found here: https://github.com/adafruit/Adafruit_SSD1306
Wiring information for a Photon can be found here: https://docs.particle.io/tutorials/projects/maker-kit/#connecting-the-oled-screen

8.3.2: Piezo Buzzer (aka beeper)
No library necessary. Simply connect the negative to a ground, and the positive to a GPIO pin (D0 works!).
Information available here: https://product.tdk.com/info/en/catalog/datasheets/ef532_ps.pdf

8.3.3: Vibrating Mini Motor Disc (aka Vibrator)
No library required. The wires must be connected. I did so by supergluing them to a wire-connector, but there are definitely better ways.
Blue is ground, red is power. However, do not connect directly to power. I was unable to wire it as some online guides suggested using a NPN Transistor, so I just tried with a resistor. However, if you can follow along and have an NPN transistor and a resistor, do try to wire it as this guide shows:
http://learningaboutelectronics.com/Articles/Vibration-motor-circuit.php
This setup has it connected to D1. You can turn it on/off using:
digitalWrite(D1, LOW);
digitalWrite(D1, HIGH); 


9: Connected Services

9.1 IFTTT
IFTTT is used to populate a google spreadsheet with the customer name, date, and order total whenever an order is made. This allows you to keep track of the total monetary value of orders placed. No web hooks are needed—IFTTT integration is tightly bound with the Particle Cloud. Simply have an IFTTT Particle look for a publishing event and react accordingly. In this project, it looks for “suborder”, and the price is the data:
I.e:
Spark.publish("suborder", String(totalP));
Is called whenever an order is submitted with the event “suborder” that IFTTT is looking for, and String(totalP) is the total price of the order.

9.2 Twilio
Twilio is integrated through a web server running on Heroku. Information on how to set this up can be found here:
https://www.hackster.io/Affarram/sms-alerts-using-particle-photon-twilio-0374b2
When you want to send to Twilio:
Particle.publish("Twilio-SMS/+1YOURPHONE#”, “YOUR MESSAGE TO SEND“, PRIVATE);
Your phone number should be in the format: +13234564567 . Make sure to include the + and the 1.
Things to note: Although this guide gets you pretty far, I was still having major errors on the Heroku side. Simply, there were substantial fatal errors and the php would not run. Research and fiddling led me to the solution. Follow the hackster.io link as is, but make the following changes:
Directly in the root (Twilio-Alerts) folder, add a no-extension file (make sure you’re using a plain text editor) called:
Procfile
The text of the file should say:
web: vendor/bin/heroku-php-apache2 public/
Next, in the root (Twilio-Alerts) folder, add a folder called public.
Drag index.php into public.
Finally, make the following change to index.php:
Instead of: require_once '/vendor/autoload.php';
Change to: require_once '../vendor/autoload.php';
Since the vendor folder is a level up, you must navigate back once first with “..”







