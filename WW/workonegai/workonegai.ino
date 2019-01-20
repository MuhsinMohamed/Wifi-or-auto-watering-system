// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <DHT.h>



#define DHTTYPE DHT11
#define dht_dpin 10  //digital pin 4 for data output of the Temperature sensor
DHT dht (dht_dpin, DHTTYPE);



// Replace with your network credentials
const char* ssid     = "codefest";
const char* password = "OakCodefest@2019";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output13State = "off";

// Assign output variables to GPIO pins
const int output13 = 13;




void setup() {
  Serial.begin(9600);
  // Initialize the output variables as outputs
  pinMode(output13, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output13, HIGH);
 


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
      Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
   
   float humid = dht.readHumidity(); //temperature data output
  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              output13State = "on";
              digitalWrite(output13, LOW);
            } else if (header.indexOf("GET /13/off") >= 0) {
              Serial.println("GPIO 13 off");
              output13State = "off";
              digitalWrite(output13, HIGH);
            } 

            /* if (header.indexOf("GET /replace") >= 0) {
              Serial.println("Timer mode");
              digitalWrite(output13, LOW);
              delay(replace); 
            }*/

            if (header.indexOf("GET /auto") >= 0){
              Serial.println("Moisture mode");
              if (humid <= 30)
              {
              digitalWrite(output13, LOW);
              }
            }
              
              
            
            // Display the HTML web page
client.println("<!doctype html>");
client.println("<html background = #404040>");
client.println("<style> body { margin: 0; /* margin-top: 50px; */ }");

client.println("html {background: #f3f3f3;}");
client.println("@font-face { font-family: Futura, sans-serif; src: url(Futura.ttf);}");
client.println(".top { background: #404040; color: #ffffff; font-size: 500%; border: 0; font-family: Futura, sans-serif; padding: 300px;}");
client.println(".top center[quote] { font-size: 50%; font-style: italic; }");

client.println(".top center[startbutton] a { background-color: #ffffff; font-size: 45%; overflow: hidden; padding: 5px;}");
client.println(".top center[startbutton] a:link { text-decoration: none; color: #404040;}");
client.println(".top center[startbutton] a:visited { text-decoration: none; color: #404040;}");
client.println(".top center[startbutton] a:hover {background-color: #800000;color: #ffffff;}");
client.println("article [quote]{ background: #ffe599; font-family: Pacifico; font-size: 950%; text-align: center; padding: 0px; margin-top: 50px;}");
client.println("footer{ background: #404040; color: #ffffff ; padding: 0px; font-size: ;}");
client.println(".nav { background: #a2c4c9; font-family: ""Courier New"", Courier, Monospace; font-size: 18px; text-align: center; position: fixed; top: 0; color: #ffffff; width: 100%; float: none; height: 56px; }");
client.println(".nav a {padding: 12px 55px; padding-top: 15px; margin-top: 10px;}");
client.println(".dropdown-content { display: none; position: absolute; background-color: #a2c4c9; min-width: 160px; box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2); z-index: 1;   color: #a2c4c9; }");
client.println(".dropdown-content a { float: none; color: #000000; padding: 12px 16px; text-decoration: none; display: block; text-align: left; color: #a2c4c9; }");
client.println(".dropdown-content a:hover { background-color: #ddd; }");
client.println(".dropdown:hover .dropdown-content { display: block; color: a2c4c9; }");
client.println(".dropdown { float: none; overflow: hidden; padding: 17px 55px; }");
client.println(".nav a:hover { background-color: #ffffff; color: #a2c4c9; }");
client.println(".nav a {padding: 17px 55px;}");
client.println(".nav a:link {color: #ffffff;text-decoration: none;}");
client.println(".nav a:visited {color: #ffffff;text-decoration: none;}");
client.println(".nav a:hover {color: #a2c4c9}");
client.println(".nav a:active { background-color: #7d8ab1; color: #ffffff; }");
client.println(".navi a:hover { background-color: #ffffff; color: #404040; }");
client.println(".navi { background: #404040; font-family: ""Courier New"", Courier, Monospace; font-size: 15px; padding: 0px; text-align: center; }");
client.println(".navi a{ padding-top: 30px; padding-bottom: 30px; padding-left: 45px; padding-right: 45px; }");
client.println(".navi tr[copyright]{ height: 100px; }");
client.println(".navi tr { height: 75px; }");
client.println(".navi a:link { color: #ffffff; text-decoration: none; }");
client.println(".navi a:visited {color: #ffffff;text-decoration: none;}");
client.println(".navi a:hover { background-color: #ffffff; color: #404040; text-decoration: none; }");
client.println(".pagetop { margin-top: 50px; background-color: #404040; color: #ffffff; font-family: Futura, sans-serif; font-size: 500%; font-style: italic; text-align: center; }");
client.println(".pagetop a{ color: #ffffff; text-decoration: none; padding-left: 200px; padding-right: 200px; }");
client.println(".Diseases h1 {text-align: left;}");
client.println(".infoscreen img{ width: 1280px; height: 800px; border: 2px solid black; }");
client.println(".infoscreen a:hover { background-color: #551A8B; color: #ffffff; }");
client.println("table {width: 100%;}");
client.println("h1 {text-align: center;font-family: Futura, sans-serif;}");
client.println(".VaccineChart th{ border: 2px solid black; background-color: #acdda7; }");
client.println(".VaccineChart td{ border: 2px solid black; background-color: #acdda7; text-align: center; }");
client.println(".VaccineChart tbody {border-collapse: collapse;}");
client.println(".VaccineChart center {text-align: center;}");
client.println(".VaccineChart [vacc] {text-align: left;}");
client.println(".VaccineChart [ChartTop] {text-align=""center""height: 50px;}");
client.println(".VaccineChart [blankcell] {background-color: #d9d9d9;}");
client.println("#GettingStarted {padding-top: 55px;}");
client.println("#MoreInfo { text-align: left; font-size: 125%; padding-top: 55px; }");
client.println(".content { font-family: verdana, sans-serif; }");
client.println("h2, h3, h4 {font-family: Futura, sans-serif;}</style>");
client.println("<head background=#404040>");
client.println("<title>Wifi Watering</title>");
client.println("<link rel=""icon"" type=""image/png"" href=""wwlogo.png"" sizes=""32x32"">");
//<!-- CSS STYLE STARTS HERE -->
client.println("<link rel=""stylesheet"" type=""text/css"" href=""WW.css"">");
//<!--CSS STYLE END-->
client.println("<HEADER class = ""top"" id=""home""><center>Wifi Watering</center><center quote><i>Control your irrigation.</i></center><center style=""font-size: 50%;"">&nbsp;</center><center startbutton><a href=""gettingstarted.html"">Get Started</a></center></HEADER>");
//<!--TOP NAVIGATION-->
client.println("<table class=""nav""><div class=""topnav""><tr class=""topnavtable""><td><a href=""Wifi Watering.html"">Home</a></td><td><a href=""VaccMyths.html"">Why the project is important</a></td><td><a href=""VaccDebate.html"">About the project</a></td></tr></div></table>");
//<!--FOOTER NAVIGATION-->
client.println("<footer><table class=""navi""><tr>");
client.println("<u><td><a href=""Wifi Watering.html"">Home</a></td><td><a href=""VaccMyths.html"">Why the project is improtant</a></td><td><a href=""VaccDebate.html"">About the project</a></td></u>");
client.println("</tr><tr style=""height: 100px;""><td colspan=""10"">Done in 24 hours for OakCodefest 2019 <br />Unofficial &copy; #include&lt;name.h&gt; 2019</td>");
client.println("</tr></table></footer></head>");
            

            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
