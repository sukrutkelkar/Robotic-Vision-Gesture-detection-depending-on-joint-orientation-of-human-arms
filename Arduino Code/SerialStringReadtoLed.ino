//**************************************************
//ECE 578
//Project Gesture Recognition using Kinect and Open CV

//Softwares used: Visual Studios 2015
//language: C++
//Tools and HW: Kinect and Arduino

//Author: Sukrut Kelkar

//Description: Using Kinect Sensor the skeleton and the join location
//of the joints (20 joints) is read. Using this location in terms of x and y
//we are setting a character variable with a specific string indicating a 
//particular motion. This charater is then sent to a serial port.
//The arduino reads this character and depending on the value of the character
//it glows a particular led.
//****************************************************
//String incomingString;  // for incoming serial data
char incomingString [20];

int RightArmUp_Pin = 8;
int LeftArmUp_Pin = 6;
int BothArmsUp_Pin = 12;
int ArmsonHip_Pin = 13;
int a=0;

char RightArmUp [20] = "R";//"----Right Arm Up---";
char LeftArmUp [20]= "L";//"-----Left Arm Up---";
char BothArmsUp [20]= "B";//"----Both Arms Up---"; //I don't know gesture
char ArmsonHip [20]= "H";//"--Both Arms on Hip-"; // I am hungry Gesture


void setup() 
{
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        pinMode(RightArmUp_Pin, OUTPUT);
        pinMode(LeftArmUp_Pin, OUTPUT);
        pinMode(BothArmsUp_Pin, OUTPUT);
        pinMode(ArmsonHip_Pin, OUTPUT);
}

void loop() 
{
  // send data only when you receive data:
  if (Serial.available()) 
    {
      // read the incoming string:  
      Serial.readBytes(incomingString,1);
      
      //Setting a flag if the string matches
      if (strcmp(incomingString, RightArmUp)==0) {a =1;}
      else if (strcmp(incomingString,LeftArmUp)==0) {a =2;}
      else if (strcmp(incomingString,BothArmsUp)==0) {a =3;}
      else if (strcmp(incomingString,ArmsonHip)==0) {a =4;}  
      else {a =0;}  
    
      switch(a)
      {
        case 1:
                digitalWrite (RightArmUp_Pin, HIGH); 
                digitalWrite (LeftArmUp_Pin, LOW); 
                digitalWrite (BothArmsUp_Pin, LOW);
                digitalWrite (ArmsonHip_Pin, LOW);
                break;
        case 2:
                digitalWrite (RightArmUp_Pin, LOW); 
                digitalWrite (LeftArmUp_Pin, HIGH); 
                digitalWrite (BothArmsUp_Pin, LOW);
                digitalWrite (ArmsonHip_Pin, LOW);
                break; 
        case 3:
                digitalWrite (RightArmUp_Pin, LOW); 
                digitalWrite (LeftArmUp_Pin, LOW); 
                digitalWrite (BothArmsUp_Pin, HIGH);
                digitalWrite (ArmsonHip_Pin, LOW);
                break;
        case 4:
                digitalWrite (RightArmUp_Pin, LOW); 
                digitalWrite (LeftArmUp_Pin, LOW); 
                digitalWrite (BothArmsUp_Pin, LOW);
                digitalWrite (ArmsonHip_Pin, HIGH);
                break;
        
        default:
                digitalWrite (RightArmUp_Pin, LOW); 
                digitalWrite (LeftArmUp_Pin, LOW); 
                digitalWrite (BothArmsUp_Pin, LOW);
                digitalWrite (ArmsonHip_Pin, LOW); 
                break;
      }      
      
      Serial.print("I received: ");
      
      Serial.println(incomingString);
      
      
     }
}
