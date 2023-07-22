#include <SPI.h>
#include <mcp_can.h>
/* Any other libraries if needed 
 * 
 */

// CAN VARIABLE
int REPLACED    = 1; // Transmit 1 if the chain need to be replaced
int GOOD     = 0; // Transmit 0 if the chain is good
unsigned char stmp[2] = {GOOD, REPLACED};

//Microcontroller VARIABLE
int sensorin1 = A5; // PWM
int sensorin2 = A6; // ASYNC INTERRUPT
int sensorin3 = A7;
  /* B0 and B1 will also be available but they don't have pull-up resistor.
   * B0 and B1 have PWM.
   * B2 and B3 are connected to a crystal oscillator. B2 has both PWM and ASYNC INTERRUPT. */
int Chainlength = 0;
const int exp_length = 6; // need to be changed according to chain model
  /* Any other variables if needed. 
   *
   */


MCP_CAN CAN(SS);

void setup()
{

//   SPI.begin ();
/* Initialization of sensor pins and external oscillators if needed
 * 
 * 
 */
 
  pinMode(SS, OUTPUT);
//  SPI.setClockDivider(SPI_CLOCK_DIV8);
//  pinMode(sensorin1,INPUT);
//  pinMode(sensorin2,INPUT);
    
    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS init Failed");
        delay(100);
    }
    Serial.println("CAN BUS Shield Init OK!");

/* Setups about sensors and settings for calculation like external oscillator etc.
 * 
 * 
 */
}

void loop()
{ 
/* Calculation part here (maybe?)
 * 
 * 
*/

/* If loop to judge the information to be transmitted according to calculated result
 *  
 */

 /* Transmit part (CAN) */
  Serial.println("In loop");
  CAN.sendMsgBuf(0x15, 0, 2, stmp);
  delay(100);
}


/* Any other functions?
 * 
 */
