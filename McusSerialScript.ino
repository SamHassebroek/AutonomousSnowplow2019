/**
 * 0xE1 - Set Motor Mode (Without stop)
 * 0x8A - Set Direction Mode
 *   #DATA#
 *   0x00 - Stop
 *   0x01 - Forward
 *   0x02 - Backward
 *   0x03 - Brake
 *   
 * 0x84
 * 
 * 
 */

void setup() {
  Serial.begin(9600);
  Serial.println("Serial-1 established:");
  Serial3.begin(9600);
  Serial.println("Serial-3 established:");
  Example1();
}

void loop() {
    //Example1();
}

void Example1()
{
  Serial.println("Running...");
  Serial3.write(0xE1);
  Serial3.write(0x8A);
  Serial3.write(0x01);
  Serial3.write(0x84);
  Serial3.write(0x0F);
  Serial3.write(0x80);
  Serial3.write(0x1F);
  Serial3.write(0x80);
  Serial3.write(0x3F);

  //Serial3.write(0xCA);
  //Serial3.write(0xCC);
}



