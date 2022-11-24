#include <Adafruit_MCP4728.h>
#include <Wire.h>

Adafruit_MCP4728 mcp;

const byte numItems = 255; 
char receivedItems[numItems];
boolean newMessage = false;

float receivedFloats[4];
int v0;
int v1;
int v2;
int v3;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  // Try to initialize!
  if (!mcp.begin(0x64)) {
    Serial.println("Failed to find MCP4728 chip");
  } else {
    Serial.println("Found it");
  }
}

void loop() {
  stringWithMarks();
  showNewMessage();
  
  mcp.setChannelValue(MCP4728_CHANNEL_A, v0);
  mcp.setChannelValue(MCP4728_CHANNEL_B, v1);
  mcp.setChannelValue(MCP4728_CHANNEL_C, v2);
  mcp.setChannelValue(MCP4728_CHANNEL_D, v3);
}

void stringWithMarks() {
  static boolean Receiving = false;
  static byte index = 0;
  char startMark = '<';
  char endMark = '>';
  char bag;
  while (Serial.available() > 0 && newMessage == false) {
    bag = Serial.read();
    if (Receiving == true) {
      if (bag != endMark) {receivedItems[index] = bag;
      index++;
      if (index >= numItems) {
        index = numItems - 1;
      }
    }
    else {
      receivedItems[index] = '\0';
      Receiving = false;
      index = 0;
      newMessage = true;}
    }
    else if (bag == startMark) {
      Receiving = true;
    }
  }
}

void showNewMessage() {
  if (newMessage == true) {
    String sv0 = getValue(receivedItems, ' ', 0);
    String sv1 = getValue(receivedItems, ' ', 1);
    String sv2 = getValue(receivedItems, ' ', 2);
    String sv3 = getValue(receivedItems, ' ', 3);
    float fv0 = sv0.toFloat();
    float fv1 = sv1.toFloat();
    float fv2 = sv2.toFloat();
    float fv3 = sv3.toFloat();
    
    float voltage = atof(receivedItems);
    v0 = map(fv0*100, 0.0, 5.0*100, 0.0, 4095.0);
    v1 = map(fv1*100, 0.0, 5.0*100, 0.0, 4095.0);
    v2 = map(fv2*100, 0.0, 5.0*100, 0.0, 4095.0);
    v3 = map(fv3*100, 0.0, 5.0*100, 0.0, 4095.0);

    newMessage = false;
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
