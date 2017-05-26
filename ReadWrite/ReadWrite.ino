#include <Bridge.h>
#include <Console.h>
#include <Process.h>

long linuxBaud = 250000;
void setup()
{
 Bridge.begin(); // Initialize the Bridge
 Serial.begin(9600); // open serial connection via USB-Serial
 Serial.print("start"); 
 
}
 
void loop()
{
//Generates a random value 
 int x = random(0,10);
 int y = random(11,20);
 Process p;
 p.runShellCommand("curl -k -X PUT -d '{ \"x-position\": "+String(x)+", \"y-position\": "+String(y)+" }' \'https://smart-pen-8075c.firebaseio.com/Test.json'");
 while(p.running()); 
 
 delay(2000); 
 
// This
 p.runShellCommand("curl -k 'https://smart-pen-8075c.firebaseio.com/Test/x-position.json' ");
 while(p.running()); 
 
 while (p.available()) {
 int result = p.parseInt(); 
 Serial.println(result); 
 }
 delay(2000); 
}
