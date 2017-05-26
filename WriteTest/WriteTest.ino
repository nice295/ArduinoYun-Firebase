// Arduino libraries
#include <Bridge.h>
#include <Console.h>
#include <Process.h>

// Constants
const char *FIREBASE = "https://smart-pen-8075c.firebaseio.com/";
const char *NODE = ".json";

const int  UPDATE_RATE = 1000;

// Leverage Yun Linux (curl)
Process process;

// Buffer for parameters
char buffer[80];

// Setup
void setup() 
{  
  // Bridge communication
  Bridge.begin();
  
  // Debugging
  #ifdef DEBUG
    Console.begin();
    while( !Console ) {;}
  #endif
}

// Loop
void loop() 
{
  // Checksum from DHT
  int check;

  // Update value on Firebase
  request( random(0, 100), random(101, 200) );
  wait();
  response(); 
  
  // Rinse, wash, and repeat
  delay( UPDATE_RATE );
}

// Send the data to Parse.com
void request( double x, double y )
{
  // Buffer for string conversion
  // The sprintf function does not like doubles
  char x_buf[10];
  char y_buf[10];  
  
  // Temperature as character string
  dtostrf( x, 3, 2, x_buf );
  
  // Humidity as character string
  dtostrf( y, 3, 2, y_buf );  
  
  // Build curl command line
  // Includes HTTPS support
  // PATCH
  // Single value so no JSON
  process.begin( "curl" );
  process.addParameter( "-k" );
  process.addParameter( "-X" );
  process.addParameter( "PATCH" );
 
  // Body
  // Value to update
  process.addParameter( "-d" );
  sprintf( 
    buffer, 
    "{\"x-position\": %s, \"y-position\": %s}", 
    x_buf,
    y_buf
  );
  process.addParameter( buffer );
 
  // URI
  sprintf( 
    buffer, 
    "%s%s", 
    FIREBASE, 
    NODE
  );
  process.addParameter( buffer );  

  // Run the command 
  // Synchronous
  process.run();
}

// Response from Parse.com
void response()
{
  bool print = true;
  char c;
  
  // While there is data to read
  while( process.available() ) 
  {
    // Get character
    c = process.read();
    Console.print( c );
  }
}

// Wait for a response from Parse.com
void wait()
{
  // Periodically check curl process
  while( !process.available() ) 
  {
    delay( 100 );
  }
}
