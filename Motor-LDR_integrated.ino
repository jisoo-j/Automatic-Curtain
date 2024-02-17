#define MotFwd  19  // Motor Forward pin
#define MotRev  21 // Motor Reverse pin

int sensorVal;
const int ANALOG_READ_PIN = 36;
const int RESOLUTION = 12;
int encoderPin1 = 27; //Encoder Output 'A' must connected with intreput pin of arduino.
int encoderPin2 = 15; //Encoder Otput 'B' must connected with intreput pin of arduino.
volatile int lastEncoded = 0; // Here updated value of encoder store.
volatile long encoderValue = 0; // Raw encoder value

void setup() {
pinMode(MotFwd, OUTPUT);
pinMode(MotRev, OUTPUT);
Serial.begin(9600); //initialize serial comunication
 pinMode(encoderPin1, INPUT_PULLUP);
pinMode(encoderPin2, INPUT_PULLUP);
digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
//call updateEncoder() when any high/low changed seen
//on interrupt 0 (pin 2), or interrupt 1 (pin 3)
attachInterrupt(0, updateEncoder, CHANGE);
attachInterrupt(1, updateEncoder, CHANGE);
}

int LDR = analogRead(ANALOG_READ_PIN);
//The analogReadResolution( ) will return the integer values between 0 and 4095 from the analogRead( ) function.
bool doneHighLightStuff = false;
int temp=0; // variable to operate the motor once when it triggers LDR threshold

void loop() {
analogReadResolution(RESOLUTION);
Serial.println(LDR);
delay(1000);
LDR = analogRead(36);
if (LDR > 3000 && temp== 0) {
 //doHighLightStuff();
 Serial.println(" - open the curtain, clockwise motor) "); //when temp is 0, we open the curtain, and set it to temp = 1
 temp = 1;
for (int i = 0; i <= 3000; i++){
digitalWrite(MotFwd, HIGH);
digitalWrite(MotRev, LOW);
Serial.print("Forward  ");
Serial.println(encoderValue);
}
}
else if (LDR > 3000 && temp== 1){
digitalWrite(MotFwd, LOW);
digitalWrite(MotRev, LOW);
Serial.print("STOP");
}
delay(1000);

if (LDR < 1200 && temp ==1) {//when temp is 1, we close the curtain, and set it to temp = 1
 Serial.println(" - close the curtain, counter clockwise motor");
 temp = 0; //after checking temp ==1 from the if statement, we now allocate 0 to temp.
for (int i = 0; i <= 3000; i++){
digitalWrite(MotFwd, LOW);
digitalWrite(MotRev, HIGH);
Serial.print("Reverse  ");
Serial.println(encoderValue);
}
}
else if (LDR < 1200 && temp== 0){
digitalWrite(MotFwd, LOW);
digitalWrite(MotRev, LOW);
Serial.print("STOP");
}
delay(1000);
}

void updateEncoder(){
int MSB = digitalRead(encoderPin1); //MSB = most significant bit
int LSB = digitalRead(encoderPin2); //LSB = least significant bit
int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue --;
if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue ++;
lastEncoded = encoded; //store this value for next time
}

