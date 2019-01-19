#include <gp20u7_particle.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_HMC5883.h>

int mux[3]={A0,A1,A2};
int muxz=A3;
int an[8]={A4,2,3,4,5,6,7,A7};
int avg=10;
double speed[8]={0,0,0,0,0,0,0,0};
double dir[8]={0,0,0,0,0,0,0,0};

//Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
//GP20U7 gps = GP20U7(Serial);

//Geolocation currentLocation;

void setup() {
//    gps.begin();
    Serial.begin(9600);
    for(int i=0; i<3; i++){
        pinMode(mux[i], OUTPUT);
    }
    pinMode(muxz, INPUT);
    for(int j=0; j<8; j++){
        pinMode(an[j], INPUT_PULLUP);
    }
    Serial.println("GPS,mag(°),sp1(mph),dir1(°),sp2(mph),dir2(°),sp3(mph),dir3(°),sp4(mph),dir4(°),sp5(mph),dir5(°),sp6(mph),dir6(°),sp7(mph),dir7(°),sp8(mph),dir8(°)");
}

void loop() {
    if(gps.read()){
         currentLocation = gps.getGeolocation();
         Serial.print(currentLocation.latitude,5);
         Serial.print(",");
         Serial.print(currentLocation.longitude,5);
     }
    Serial.print(",");
    //sensors_event_t event;
    //mag.getEvent(&event);
    //double heading = atan2(event.magnetic.y, event.magnetic.x);
    for(int pin=0; pin<8; pin++){
        for (int k=0; k<3; k++){
            if (pin & (1<<k))
                digitalWrite(mux[k], HIGH);
            else
                digitalWrite(mux[k], LOW);
        }
        dir[pin]=map(analogRead(muxz),0,4095,0,360);
    }
    for(int l=0; l<8; l++){
        for(int m=0; m<avg; m++){
            speed[l]+=pulseIn(an[l], LOW);
        }
        speed[l]/=avg*1000000;
        speed[l]*=2.25;
    }
    for(int n=0; n<8; n++){
        Serial.print(",");
        Serial.print(speed[n]);
        Serial.print(",");
        Serial.print(dir[n]);
    }
    Serial.println();
}
