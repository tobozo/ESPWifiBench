
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#define maxrounds 50 // how many scans will be performed before printing stats
#define maxmacs 150 // increase this if you get LIST FULL messages, decrease if you run in a reset loop

unsigned int rounds = 0;
unsigned int aplist[maxrounds];
unsigned int totalaps = 0;

bool done = false;

String maclist[maxmacs][3] = {
  {"", "0", "0"}
};



unsigned int findmac(String mac) {
  for(int i=0;i<maxmacs;i++) {
    if(maclist[i][0]==mac) {
      return i;
    }
  }
  return -1;
}

void setmac(String mac, int rssi) {
  int id = findmac(mac);
  if(id==-1) {
    for(int i=0;i<maxmacs;i++) {
      if(maclist[i][1]=="0") { 
        maclist[i][0] = mac; // mac addr
        maclist[i][1] = "1"; // hits
        maclist[i][2] = String(rssi); // total
        return;
      }
    }
    Serial.println("Memory full, can't register " + mac + " / " + String(rssi));
    // increase anyway
    totalaps++;
  } else {
    maclist[id][1] = String( (maclist[id][1]).toInt() + 1);
    maclist[id][2] = String( (maclist[id][2]).toInt() + rssi); 
  }
}


void setup() {
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    #ifdef ESP32
    WiFi.mode(WIFI_MODE_APSTA);
    #else
    WiFi.mode(WIFI_AP_STA);
    #endif
    WiFi.disconnect();
    delay(100);
    Serial.println("Setup done");
    // om nom nom nom
    for(int i=0;i<maxmacs;i++) {
      maclist[i][0] = "";
      maclist[i][1] = "0";
      maclist[i][2] = "0";
    }

    WiFi.printDiag(Serial);

}

void loop() {

  if(done==true) {
    return;
  }

  if(rounds>=maxrounds) {
    Serial.println();
    Serial.println("Done");
    Serial.println();
    
    done = true;
    
    long avgaps = 0;
    long maxaps = 0;
    long minaps = maxmacs;
    long avgrssi = 0;
    long totalrssi = 0;
    int totalhits = 0;
    
    for(int i=0;i<maxmacs;i++) {
      if(maclist[i][0]=="") continue;
      totalaps++;
      avgrssi = (long) ((maclist[i][2]).toInt() / (maclist[i][1]).toInt());
      Serial.println("Average RSSI for " + maclist[i][0] + " is " + String(avgrssi) + " ("+ maclist[i][1] +" hits)");
      totalhits += (maclist[i][1]).toInt();
    }

    for(int i=0;i<maxrounds;i++) {
      long apsize = aplist[i];
      if(apsize>maxaps) maxaps = apsize;
      if(apsize<minaps) minaps = apsize;
      avgaps+= apsize;
    }
    avgaps = avgaps / maxrounds;

    Serial.println("Scanned " + String(maxrounds) + " rounds");
    Serial.println("Total APs found: " + String(totalaps) + " / hits: " + String(totalhits));
    if(totalaps>=maxmacs) {
      Serial.println("Registry was full, averages and total may be inacurate");
    }
    Serial.println("Average APs found " + String(avgaps)  + " / highest: " + String(maxaps) + " / lowest: " + String(minaps));
    Serial.println();
    
    return;
  }
  
  Serial.print("round #");
  Serial.print(rounds);
  Serial.print(" .. ");

  // WiFi.scanNetworks will return the number of networks found
  #ifdef ESP32
  int n = WiFi.scanNetworks(false, true, false, 300);
  #else
  int n = WiFi.scanNetworks(false, true);
  #endif
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" results");

    aplist[rounds] = n+1;

    for (int i = 0; i < n; ++i) {
        setmac(WiFi.BSSIDstr(i), WiFi.RSSI(i));
        delay(10);
    }
  }
  // Scan aggressively
  delay(200);
  rounds++;
}
