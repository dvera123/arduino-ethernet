
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 0, 13 }; // ip in lan
EthernetServer server(80); //server port

int STBY = 6; //standby
char incomingChar = 'a';	// para el byte leido

//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction

//Motor B
int PWMB = 5; //Speed control
int BIN1 = 2; //Direction
int BIN2 = 7; //Direction


//////////////////////

void setup(){

  Serial.begin(9600);
  //pinMode(9,OUTPUT);
  // disable w5100 while setting up SD
  //pinMode(10,OUTPUT);
  //digitalWrite(10,HIGH);
  Serial.print("Starting SD..");
  if(!SD.begin(4)) Serial.println("failed");
  else Serial.println("ok");

  Ethernet.begin(mac, ip);
  digitalWrite(10,HIGH);

  server.begin();
  Serial.println("Ready");
  
  delay(1000);
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    String readString; 
    while (client.connected()) {
      if (client.available()) {
        
        
        char c = client.read();
        readString += c;
        
       
        //if HTTP request has ended
        if (c == '\n') {

          client.println("HTTP/1.1 200 OK"); //send new page
          
          File myFileCss = SD.open("STYLE.CSS");
          
          readFileCss(client, myFileCss);

          File myFileHtml = SD.open("INDEX~1.HTM");
          
          readFileHtml(client, myFileHtml); 
          
          File myFileJs = SD.open("MAIN.JS");
          
          readFileJs(client, myFileJs); 
          
          
          client.stop();
            
        }
        
        
      }
      
    }
    readString.replace("HTTP/1.1","");
    readString.replace("GET /","");
    readString.trim();
    
    if(readString.equals("?do=go")){
      Serial.print("go..");
      stop(); //stop
      move(1, 255, 1); //motor 1, full speed, left
      move(2, 255, 1); //motor 2, full speed, left
      delay(2000); //go for 1 second
      stop(); //stop
    
    }
    
    if(readString.equals("?do=reverse")){
      Serial.print("reverse..");
      stop(); //stop
      move(1, 128, 0); //motor 1, half speed, right
      move(2, 128, 0); //motor 2, half speed, right
      delay(2000); //go for 1 second
      stop(); //stop
   
    }
    
    
    
    
    
    
  } 
  
  if (Serial.available() > 0) {
	// lee el byte entrante:
	incomingChar = Serial.read();

	// dice lo que ha recibido:
        if(incomingChar!=10){ 
	  Serial.print("He recibido: ");
          Serial.println(incomingChar);
          if(incomingChar == 'a'){
             
             move(1, 255, 1); //motor 1, full speed, left
             move(2, 255, 1); //motor 2, full speed, left
             delay(1000); //go for 1 second
             stop(); //stop
             
           }
           if(incomingChar == 'b'){
             
             move(1, 128, 0); //motor 1, half speed, right
             move(2, 128, 0); //motor 2, half speed, right
             delay(1000); //go for 1 second
             stop(); //stop
             
           }
        }
     }
}

void readFileCss(EthernetClient client, File myFileCss){
  
  if (myFileCss) {
            byte clientBuf[64];
            int clientCount = 0;
            client.println();
            client.println("<style>");
            client.println();
            // read from the file until there's nothing else in it:
            while (myFileCss.available()) {
              clientBuf[clientCount] = myFileCss.read();
              clientCount++;

              if(clientCount > 63)
              {
                // Serial.println("Packet");
                client.write(clientBuf,64);
                clientCount = 0;
              }
            }
            // close the file:
            if(clientCount > 0) client.write(clientBuf,clientCount);   
            myFileCss.close();
            client.println();
            client.println("</style>");
          }

}

void readFileHtml(EthernetClient client, File myFileHtml){
  
  if (myFileHtml) {
            byte clientBuf[64];
            int clientCount = 0;
            
            client.println();
            // read from the file until there's nothing else in it:
            while (myFileHtml.available()) {
              clientBuf[clientCount] = myFileHtml.read();
              clientCount++;

              if(clientCount > 63)
              {
                // Serial.println("Packet");
                client.write(clientBuf,64);
                clientCount = 0;
              }
            }
            // close the file:
            if(clientCount > 0) client.write(clientBuf,clientCount);   
            myFileHtml.close();

          }

}

void readFileJs(EthernetClient client, File myFileJs){
  
  if (myFileJs) {
            byte clientBuf[64];
            int clientCount = 0;
            client.println();
            client.println("<script>");
            client.println();
            // read from the file until there's nothing else in it:
            while (myFileJs.available()) {
              clientBuf[clientCount] = myFileJs.read();
              clientCount++;

              if(clientCount > 63)
              {
                // Serial.println("Packet");
                client.write(clientBuf,64);
                clientCount = 0;
              }
            }
            // close the file:
            if(clientCount > 0) client.write(clientBuf,clientCount);   
            myFileJs.close();
            client.println();
            client.println("</script>");
          }

}

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}

