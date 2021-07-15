#include <TinyGPS++.h> #include <SoftwareSerial.h> #include <LiquidCrystal.h> int Contrast=135;
LiquidCrystal lcd(12, 11,10,9,8,7); String R;
short Tr=-1;
static const int RXPin = 3, TXPin = 2; static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object TinyGPSPlus gps;
int temp=0,i;
int temp_msg=0;
// The serial connection to the GPS device SoftwareSerial ss(RXPin, TXPin);
String stringVal = ""; int mems=A0,x,y,Z;
int smoke=A1; int vibrate=A2; void setup(){
Serial.begin(9600); ss.begin(GPSBaud); lcd.begin(16, 2); analogWrite(6,Contrast);
 
pinMode(13,OUTPUT); digitalWrite(13,LOW); lcd.print("Vehicle Tracking"); lcd.setCursor(0,1);
lcd.print("  System  "); delay(2000);
gsm_init(); lcd.clear();
lcd.print("GPS Initializing"); lcd.setCursor(0,1); lcd.print(" No GPS Range "); delay(2000);
lcd.clear();
lcd.print("GPS Range Found"); lcd.setCursor(0,1); lcd.print("GPS is Ready"); delay(2000);
lcd.clear(); lcd.print("System Ready"); temp=0;
}
void loop()
{
x=analogRead(mems); y=analogRead(smoke); Z=analogRead(vibrate); serialEvent(); Serial.print(x);
 
Serial.println(" MEMS"); delay(1000); Serial.print(y); Serial.println(" Smoke"); delay(1000); Serial.print(Z); Serial.println(" Vibration"); delay(1000);
         //Serial.println(x);
//delay(1000);
//Serial.println(y);
//delay(1000);
//Serial.println(Z);
//delay(1000);
//Serial.println(x+ " MEMS");
//delay(1000);
//Serial.println(y+ " Smoke");
//delay(1000);
//Serial.println(Z+ " Vibration");
//delay(1000); if(x>450)
{
temp=1;
}
if(y>250)
{

temp=1;
}
 
if(Z>250)
{

temp=1;
}


while(temp_msg)
{

while (ss.available() > 0)
{

gps.encode(ss.read());
if (gps.location.isUpdated())
{
temp_msg=0;


digitalWrite(13,HIGH); vehicletracking();
}
if(!temp) break;
}
}

digitalWrite(13,LOW); Tr=-1;
while(temp)
{

while (ss.available() > 0)
{

gps.encode(ss.read());
if (gps.location.isUpdated())
 
{
temp=0;


digitalWrite(13,HIGH); tracking();
}
if(!temp) break;
}
}

digitalWrite(13,LOW); Tr=-1;
}

void serialEvent()
{

boolean e_f=1; while(e_f)
{
Serial.println("AT+CNMI=2,2,0,0,0"); while(Serial.available()>0)
{

Serial.println("INMSG"); if(Serial.find("OK")) e_f=0;
}

delay(20000);
}


while(Serial.available()>0)
 
{
char ms=Serial.read(); Serial.print(ms); R.concat(ms); Tr=R.indexOf("Track"); if(Tr!=-1)
temp_msg=1;
}
             }
void gsm_init()
{

lcd.clear();
lcd.print("Finding Module.."); boolean at_flag=1; while(at_flag)
{

Serial.println("AT"); delay(1); while(Serial.available()>0)
{

if(Serial.find("OK")) at_flag=0;
}
delay(1000);
}

lcd.clear();
lcd.print("Module Connected..");
 
delay(1000); lcd.clear();
lcd.print("Disabling ECHO"); boolean echo_flag=1; while(echo_flag)
{

Serial.println("ATE0"); while(Serial.available()>0)
{

if(Serial.find("OK")) echo_flag=0;
}

delay(1000);
}

lcd.clear(); lcd.print("Echo OFF"); delay(1000); lcd.clear();
lcd.print("Finding Network.."); boolean net_flag=1; while(net_flag)
{

Serial.println("AT+CPIN?"); while(Serial.available()>0)
{

if(Serial.find("+CPIN: READY")) net_flag=0;
}

delay(1000);
 
}
lcd.clear(); lcd.print("Network Found..");


delay(1000); lcd.clear();
}

void init_sms()
{
Serial.println("AT+CMGF=1"); delay(400);
Serial.println("AT+CMGS=\"9491828071\""); // use your 10 digit cell no. here delay(400);
}
void send_data(String message)
{

Serial.print(message); delay(200);
}
void send_sms()
{

Serial.write(26);
}

void lcd_status()
{

lcd.clear(); lcd.print("Message Sent"); delay(2000);
lcd.clear();
 
lcd.print("System Ready"); return;
}

void tracking()
{

init_sms(); send_data("Accident Alert:"); Serial.println(" ");
send_data("Your Vehicle Current Location is:"); Serial.println(" ");
Serial.print("Latitude: "); Serial.print(gps.location.lat(), 6); Serial.print("\n Longitude: "); Serial.println(gps.location.lng(), 6);
//Serial.print("https://www.google.com/maps/@"); Serial.print("https://www.google.com/maps/?q="); Serial.print(gps.location.lat(), 6);
Serial.print(','); Serial.print(gps.location.lng(), 6); Serial.print(",");
send_sms(); delay(2000); lcd_status();
}
void vehicletracking()
{
init_sms();
send_data("Vehicle Tracking Alert:"); Serial.println(" ");
 
send_data("Your Vehicle Current Location is:"); Serial.println(" ");
Serial.print("Latitude: "); Serial.print(gps.location.lat(), 6); Serial.print("\n Longitude: "); Serial.println(gps.location.lng(), 6);
//Serial.print("https://www.google.com/maps/@"); Serial.print("https://www.google.com/maps/?q="); Serial.print(gps.location.lat(), 6);
Serial.print(','); Serial.print(gps.location.lng(), 6); Serial.print(",");
send_sms(); delay(2000); lcd_status();
}
