import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val, screen_increment, old_x=0, old_y=0;      // Data received from the serial port
String inString;  // Input string from serial port
int temp,freefall,pos;
int lf = 10;      // ASCII linefeed 
void setup() 
{

  size(displayWidth-100, 600);
  String portName = "COM4";
  
  myPort = new Serial(this, portName, 9600);//Set up the serial port
  myPort.bufferUntil(lf);//read in data until a line feed, so the arduino must do a println
  background(255,255,255);//red
}//setup

void draw()
{
//nothing in here, this is kind of like the void loop in arduino
}

void serialEvent(Serial myPort) {
  inString = myPort.readString();//read in the new data, and store in inString
  inString = trim(inString);//get rid of any crap that isn't numbers, like the line feed
  String[] list = split(inString, ',');
  val = int(list[0]);
  freefall = int(list[1]);
  pos = int(list[2]);
  temp = int(list[3]);
  
  println(inString);
  
  val = val/2;
  strokeWeight(4);//width
  stroke(208,24,24);//make the line white

  line(old_x, old_y, screen_increment, 600-val);
  
  //store the current x, y as the old x,y, so it is used next time
  old_x = screen_increment;
  old_y = 600-val;
  
  //increment the x coordinate,  you can play with this value to speed things up
  screen_increment=screen_increment+4;
  
  stroke(255);          // Setting the outline (stroke) to black
  fill(255);          // Setting the interior of a shape (fill) to grey 
  rect(0, 0, 1500, 90);
  
      textSize(32);
    fill(0, 102, 153);
    text("Temp: " + temp + " C" , 1050, 30); 
  
  if(pos == 1){
    textSize(32);
    fill(0, 102, 153);
    text("Body position: Vertical" , 30, 30); 

  }
  else
  {
    textSize(32);
    fill(0, 102, 153);
    text("Body position: Horizontal" , 30, 30); 

  }
  
  if(freefall == 1)
  {
    textSize(32);
    fill(0, 102, 153);
    text("Body is in freefall" , 30, 70);
  }
  
  
  
  //this is needed to reset things when the line crashes into the end of the screen
  if(screen_increment>(displayWidth-100)){
    background(255,255,255); //refresh the screen, erases everything
    screen_increment=-50; 
  old_x = -50;
  old_y = 0;
  
  }// if screen... blah blah
  
}//serial event
