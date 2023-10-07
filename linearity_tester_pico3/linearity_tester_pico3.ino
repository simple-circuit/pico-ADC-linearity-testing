//ADC linearity test using integrator ramping. Simple-Circuit 2023
volatile long analog_avg;
volatile unsigned long t;
void setup() {
 analogReadResolution(12); 
 pinMode(18,OUTPUT);
 pinMode(15,OUTPUT); 
 digitalWrite(18,0);
 Serial.begin(460800);       //set your serial terminal to 460800 baud 
}

void readA0(void){
  digitalWrite(15,1);
      analog_avg = 0;
      for (int j = 0; j<8; j++) analog_avg += analogRead(A0);  //average 8 readings to reduce noise
      analog_avg =  analog_avg >> 3;                          //shift to 12-bit value 
   digitalWrite(15,0);
}

void loop() {
  char c;
   c=' ';
   digitalWrite(18,0); 
   while (c!='r'){                     //start the integrator test when an 'r' is typed
     while (Serial.available() == 0);
     c = Serial.read();
   }  
     Serial.println("Ramping Down");  
     digitalWrite(18,1);              //Set to max positive voltage
     readA0(); 
     while (analog_avg > 30){        //wait for the integrator to reach zero
      readA0();
     // Serial.println(analog_avg);
     }   
     Serial.println("Ramping Up");
     delay(100);                    //delay to let integrator go slightly negative
     digitalWrite(18,0);            //set to zero volage to ramp positive
     readA0();
     while (analog_avg < 25){      //noise can cause false starts, so wait for
      readA0();                    //a higher value
     }   
     t = millis()+5;               //set to sample at 5ms intevals
     while (analog_avg < 4070){    //print ADC values intil max is reached 
      readA0();
      Serial.println(analog_avg);
      while (millis() < t); 
      t = t + 5;
     }
}
