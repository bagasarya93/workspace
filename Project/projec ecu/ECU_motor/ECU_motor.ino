/*90%*/
 
#define injector  6
#define FUELPUMP  4
#define TPS A0
//#define CHOKE A2

volatile boolean fire = false;
volatile int rpm;
volatile float newmicros;
volatile float oldmicros;
volatile long newmillis;
volatile long oldmillis;
volatile long lag;
int injectime;
int val;
int val1;
volatile long injecdelay ;
float tps_r = 0;
//float choke = 0;
static int deciSeconds = 0;

void setup()
{
  pinMode(injector, OUTPUT);
  pinMode(FUELPUMP, OUTPUT);
 //pinMode(CHOKE, INPUT);
  pinMode(2, INPUT);
  pinMode(TPS, INPUT);
  attachInterrupt(0, pulseISR, FALLING);  

  Serial.begin(115200);

      digitalWrite(FUELPUMP , HIGH);
      digitalWrite(13,HIGH);
      delay(800);
      digitalWrite(FUELPUMP , LOW);
      digitalWrite(13,LOW);
}

void loop(){
    timing();
    sensor();
    data();

  if ((micros() - newmicros) > 500000L) {
    newmicros = micros();
    rpm = 0;
    lag = 0;
  }
  static unsigned long timer = millis();
  if (millis() >= timer) {
    deciSeconds++;
    timer += 600;
    if ((deciSeconds == 220) && (rpm > 1)) { // Reset to 100.
      deciSeconds = 201;
    } else if (rpm == 0) {//reset to 0.
      deciSeconds = 0;
    }
  }

   // 0-11000rpm
  if (fire) {
    rpm = (60000000L / (newmicros - oldmicros)); 
    lag = map(rpm, 0, 10000, 0, 12);
    oldmicros = newmicros;
    delayMicroseconds(lag);
    digitalWrite(injector, HIGH);
    delayMicroseconds(injecdelay);
    digitalWrite(injector, LOW);
    fire = false;
  }
  if (rpm <=  1) {
    digitalWrite(FUELPUMP, LOW);
  }
  if (rpm > 1) {
    digitalWrite(FUELPUMP, HIGH);
  }
}
void data(){
  newmillis = millis();

  if ((newmillis - oldmillis) > 250) {
    Serial.print("  *rpm=");
    Serial.print(rpm);
    Serial.print("  *tps=");
    Serial.print(tps_r);
    Serial.print(" *dc=");
    Serial.print(deciSeconds);
    Serial.print(" *injectime=");
    Serial.print(injectime);
    Serial.print(" *injecdelay=");
    Serial.print(injecdelay);
    //Serial.print("*ch");
  //  Serial.print(choke);
    Serial.print(" *l=");
    Serial.print(lag);
    oldmillis = newmillis;
  }
}
void sensor(){
  val = analogRead(TPS);
  tps_r = map(val, 0,1023, -10,650 );
 // val1 = analogRead(CHOKE);
 // choke = map (val1,0,1023, 0,1);
}
//=======SETING BAHAN BAKAR =======================
void timing(){
switch (lag) {
    case 0:
      if (deciSeconds <= 1){
        lag = 80;
        injecdelay = 4000;
      }
    //==================STATUS CHOKE===================
 //if (choke <= 1)
// {
    //  if ((deciSeconds > 2)&&(deciSeconds <= 10)) {
    //    lag = 90000;
    //    injecdelay = 4000;
    //  }
      
//if ((deciSeconds > 2)&&(choke <= 0)) {
    //    lag = 90000;
    //    injecdelay = 350;//***************************
    //  }
      
    //  if (deciSeconds >10) {
    //    lag = 150000;
    //    injecdelay = 350;//************************
    //  }
    // }
//==============STATUS STASIONER=============================== 
     if (tps_r <= 20){
      if ((deciSeconds > 10)&&(deciSeconds <= 100)) {
        lag = 90000;
        injecdelay = 350;//********************************
      }
      if (deciSeconds >100) {
        lag = 150000;
        injecdelay = 350;//***************************
      }
     }
     if ((deciSeconds > 10) && (tps_r > 20)){
        lag = 30000;
        injecdelay = 2500;
       }
      break;
    case 1:
      if (deciSeconds <= 1){
        lag = 80;
        injecdelay = 4000;
      }
    if (tps_r <= 20){
      if ((deciSeconds > 10)&&(deciSeconds <= 100)) {
        lag = 90000;
        injecdelay = 350;//*******************************
      }
      if (deciSeconds > 100) {
        lag = 150000;
        injecdelay = 350;// *************************************
      }
     }                           
     if ((deciSeconds > 10) && (tps_r > 20)){
        lag = 30000;
        injecdelay = 2500;// max rpm 1800=========tarikan==
      }
      break;
      
      //===========================rpm 1800 - 2700===============================================
    case 2:
      if (deciSeconds <= 100) {
        if (tps_r <= 30) {
          lag = 80000;
          injecdelay = 1800;
        }
        if (tps_r > 30) {
          lag = 10000;
          injecdelay = 2000;
        }
      }                           
      if (deciSeconds > 100) {
        if (tps_r <= 30) {
          lag = 90000;
          injecdelay = 1600;
        }
        if (tps_r > 30) {
          lag = 30000;
          injecdelay = 2000;// max rpm 2700==================================
        }
      }
      break;
    //============================rpm 2700 - 3300================================================
    case 3:
      if (deciSeconds <= 100) {
        if (tps_r <= 40) {
          lag = 60000;
          injecdelay = 1900;
        } 
        if (tps_r > 40) {
          lag = 25000;
          injecdelay = 2500;
        }                        
      }
      if (deciSeconds > 100) {
        if (tps_r <= 40) {
          lag = 50000;
          injecdelay = 1700;// max rpm 2800=============================================
        }
        if (tps_r > 40) {
          lag = 25000;
          injecdelay = 2000;// max rpm 3300==========tarikan==========================================
        }
      }
      break;
      //===========================================================================================
    case 4:
       if (tps_r <= 50) {
          lag = 40000;
          injecdelay = 1800;
        }
        if (tps_r > 50) {           //3300-4100
          lag = 20000;
          injecdelay = 2200;//================tarikan======================
        }
      break;
    case 5:
      if (tps_r <= 60) {
          lag = 35000;
          injecdelay = 1925;
        }                        //4100-5700
        if (tps_r > 60) {
          lag = 10000;
          injecdelay = 2500;
        }
      break;
    case 6:
      if (tps_r <= 80) {
          lag = 25000;
          injecdelay = 2100;
        }                        //5700-6800
        if (tps_r > 80) {
          lag = 1000;
          injecdelay = 2800;
        }
      break;
    case 7:
      if (tps_r <= 100) {
          lag = 4000;
          injecdelay = 2400;
        }                        //6800-7400
        if (tps_r > 100) {
          lag = 200;
          injecdelay = 3000;
        }
      break;
    case 8:
      if (tps_r <= 120) {
          lag = 400;
          injecdelay = 2700;
        }                        //7400-8500
       if (tps_r > 120) {
          lag = 100;
          injecdelay = 3200;
        }
      break;
    case 9:
      lag = 100;
      injecdelay = 2900;  //>8500rpm
      break;
    case 10:
      lag = 100;
      injecdelay = 2900;  //std 3100
      break;
    case 11:
      lag = 100;
      injecdelay = 2900;
      break;
    case 12:
      lag = 100;
      injecdelay = 3000;
      break;
    }
  if (rpm >= 10000) {
    lag = 90;
    injecdelay = 3200;
  }
}
void pulseISR() {
  newmicros = micros();
  if ((newmicros - oldmicros) > 500000L )
  {
    oldmicros = newmicros;
  }
  else
  {
    fire = true;
  }
}
