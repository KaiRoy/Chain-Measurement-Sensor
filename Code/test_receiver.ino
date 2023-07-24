/*
Team 15 Chain stretch monitor
2022-2023
Tianhao Lin
*/

#include <SPI.h>
#include "mcp_can.h" // source from: https://github.com/Seeed-Studio/Seeed_Arduino_CAN/tree/old
/* Set UNO borad as receiver*/

// CS pin of the receiver
const int spiCSPin = 10;

//variables for combined speed
byte hi,lo;
unsigned int speed; //int or unsigned int?  int length: 16 bit from -32,768 to 32,767(2^15 - 1)

MCP_CAN CAN(spiCSPin);

void setup()
{
    Serial.begin(9600);

    // check whether CAN is well connected
    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS Init Failed");
        delay(500);
    }
    Serial.println("CAN BUS  Init OK!");
}


void loop()
{
    unsigned char len = 0;  // length of the reveived data
    unsigned char stmp[14];   // to store the received data ; char length: 8 bit from 0 to 255
    unsigned long tst;


    // use serial monitor to test the result
    if(CAN_MSGAVAIL == CAN.checkReceive()) 
    {
        CAN.readMsgBuf(&len, stmp);  // read the data

        unsigned long canId = CAN.getCanId();   // check the source of the data (should be 15)

        // print where it is from
        Serial.println("-----------------------------");
        Serial.print("Group number: 0x");
        Serial.println(canId, HEX);

        // stmp [0] to [N-1]
        tst = (stmp[0]<<24) + (stmp[1]<<16) + (stmp[2]<<8) + stmp[3];
        Serial.print("variable 1: ");
        Serial.print(tst);
        Serial.print("\t\t");

        tst = (stmp[4]<<24) + (stmp[5]<<16) + (stmp[6]<<8) + stmp[7];
        Serial.print("variable 2: ");
        Serial.print(tst);
        Serial.print("\t\t");

        tst = (stmp[8]<<24) + (stmp[9]<<16) + (stmp[10]<<8) + stmp[11];
        Serial.print("variable 3: ");

        Serial.print(tst);
        Serial.println();

        }

//combined variable
// Serial.print("combined number: ");
// Serial.print(speed,DEC);
// Serial.print("\t");

        // print chain state

}
