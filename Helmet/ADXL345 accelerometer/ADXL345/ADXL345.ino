#include <SPI.h>
#include <stdio.h>

int CS=10;
char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	//X-Axis Data 0
char DATAX1 = 0x33;	//X-Axis Data 1
char DATAY0 = 0x34;	//Y-Axis Data 0
char DATAY1 = 0x35;	//Y-Axis Data 1
char DATAZ0 = 0x36;	//Z-Axis Data 0
char DATAZ1 = 0x37;	//Z-Axis Data 1
//regirster values
char values[10];
int x,y,z,outputValue;
float xg,yg,zg;
int freefall, pos;
double temp;

String data,tmp;

void setup(){ 
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  Serial.begin(9600);
  const int analogInPin = A0;
  
  pinMode(CS, OUTPUT);
  //Before communication starts, the Chip Select pin needs to be set high.
  digitalWrite(CS, HIGH);
  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeRegister(DATA_FORMAT, 0x01);
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeRegister(POWER_CTL, 0x08);  //Measurement mode  
}

void loop(){
//Reading accelerometer registers
  readRegister(DATAX0, 6, values);
  x = ((int)values[1]<<8)|(int)values[0];
  y = ((int)values[3]<<8)|(int)values[2];
  z = ((int)values[5]<<8)|(int)values[4];
  //converting to multiples of G
  xg = x * 0.0078;
  yg = y * 0.0078;
  zg = z * 0.0078;
 //freefall detection
 if ((zg < 0.2 && zg > -0.2) && (yg < 0.2 && yg > -0.2) && (xg < 0.2 && xg > -0.2))
    freefall = 1;
 else
    freefall = 0;
 //body position detection
 if (xg > 0.3 || xg < -0.3)
     pos = 1;
 else
     pos = 0;
  
   outputValue = analogRead(A0);
   
   temp = (analogRead(A2) * .004882814);
   temp = (temp - .5) * 100;
   
  //String = sprintf("%d,%d,%d,%d",outputValue,freefall,pos,temp);
  data = String(outputValue,DEC);
  data = String(data + ",");
  tmp = String(freefall,DEC);
  tmp = String(tmp+",");
  data = String(data + tmp);
  tmp = String(pos,DEC);
  tmp = String(tmp+",");
  data = String(data + tmp);
  int t = (int)temp;
  tmp = String(t,DEC);
  data = String(data + tmp);
 
 // data = String(data + freefall);
  Serial.println(data);
  delay(100); 
}

//This function will write a value to a register on the ADXL345.
//Parameters:
//  char registerAddress - The register to write a value to
//  char value - The value to be written to the specified register.
void writeRegister(char registerAddress, char value){
  //Set Chip Select pin low to signal the beginning of an SPI packet.
  digitalWrite(CS, LOW);
  //Transfer the register address over SPI.
  SPI.transfer(registerAddress);
  //Transfer the desired register value over SPI.
  SPI.transfer(value);
  //Set the Chip Select pin high to signal the end of an SPI packet.
  digitalWrite(CS, HIGH);
}

//This function will read a certain number of registers starting from a specified address and store their values in a buffer.
//Parameters:
//  char registerAddress - The register addresse to start the read sequence from.
//  int numBytes - The number of registers that should be read.
//  char * values - A pointer to a buffer where the results of the operation should be stored.
void readRegister(char registerAddress, int numBytes, char * values){
  //Since we're performing a read operation, the most significant bit of the register address should be set.
  char address = 0x80 | registerAddress;
  //If we're doing a multi-byte read, bit 6 needs to be set as well.
  if(numBytes > 1)address = address | 0x40;
  
  //Set the Chip select pin low to start an SPI packet.
  digitalWrite(CS, LOW);
  //Transfer the starting register address that needs to be read.
  SPI.transfer(address);
  //Continue to read registers until we've read the number specified, storing the results to the input buffer.
  for(int i=0; i<numBytes; i++){
    values[i] = SPI.transfer(0x00);
  }
  //Set the Chips Select pin high to end the SPI packet.
  digitalWrite(CS, HIGH);
}


