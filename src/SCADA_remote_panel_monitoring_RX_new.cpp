#include <Arduino.h> //as usual, delete this if working in Arduino IDE
#include <SPI.h> //include the SPI (serial peripheral interface) library
#include <RF24.h> //include the RF24 library for the NRF24 module
#include <nRF24L01.h> //probably needed for proper functioning of the NRF24 module

//==============PINOUT==============
/*
========NRF pins========
CE - 7
CSN - 10
*/
int pinCE = 7;
int pinCSN = 10;

RF24 rx_module(pinCE, pinCSN); //name of the module and pin connections

const byte address[6] = {"42"}; //address for the module, AFAIK both TX and RX modules should have same value here
int h = 0; //humidity value as read from dht_sensor1 NOTE for NRF use - 2 bytes
int t = 0; //temperature value as read from dht_sensor1 NOTE for NRF use - 2 bytes

int gen1_switch_state = 0; //state of gen1 (ON/OFF) NOTE for NRF use - 2 byte
int gen1_volts_val = 0; //voltage on gen1 NOTE for NRF use - 2 bytes
int gen1_amps_val = 0; //current on gen1NOTE for NRF use - 2 bytes

int gen2_switch_state = 0; //state of gen2 (ON/OFF) NOTE for NRF use - 2 byte
int gen2_volts_val = 0; //voltage on gen2 NOTE for NRF use - 2 bytes
int gen2_amps_val = 0; //current on gen2 NOTE for NRF use - 2 bytes

int pump1_switch_state = 0; //state of pump1 (ON/OFF) NOTE for NRF use - 2 byte
int pump1_volts_val = 0; //voltage on pump1 NOTE for NRF use - 2 bytes
int pump1_amps_val = 0; //current on pump1 NOTE for NRF use - 2 bytes

int pump2_switch_state = 0; //state of pump2 (ON/OFF) NOTE for NRF use - 2 byte
int pump2_volts_val = 0; //voltage on pump2 NOTE for NRF use - 2 bytes
int pump2_amps_val = 0; //current on pump2 NOTE for NRF use - 2 bytes

int powerlim_val = 0; //power limit value NOTE for NRF use - 2 bytes

//==============================STRINGS FOR EASIER READING OF SERIAL==============================
char gen1_onstate[] = "ON ";
char gen1_offstate[] = "OFF";
char gen2_onstate[] = "ON ";
char gen2_offstate[] = "OFF";
char pump1_onstate[] = "ON ";
char pump1_offstate[] = "OFF";
char pump2_onstate[] = "ON ";
char pump2_offstate[] = "OFF";

//============================================================
void setup() 
{
Serial.begin(115200);
 
rx_module.begin(); //initialize the RX module
rx_module.openReadingPipe(0, address); //open communication pipe
rx_module.setPALevel(RF24_PA_MAX); //set power of NRF module, can be changed to RF24_PA_MIN for lower power consumptiom, BUT MUST BE SAME ON THE RX MODULE!!!
rx_module.setDataRate(RF24_250KBPS);
rx_module.startListening(); //puts the NRF module into RECEIVE MODE
}

//============================================================
void loop() 
{

int nrf_data_packet[15]; //array of variables to be received, the array seems one smaller than total number of variables because 0-9 not 1-10

  // put your main code here, to run repeatedly:
delay(10); //stability pause
//  while (rx_module.available()) { //control loop to keep reading NRF module as long as it's available
rx_module.read(&nrf_data_packet, sizeof(nrf_data_packet));//}

  h = nrf_data_packet[0];
  t = nrf_data_packet[1];

  gen1_switch_state = nrf_data_packet[2];
  gen1_volts_val = nrf_data_packet[3];
  gen1_amps_val = nrf_data_packet[4];

  gen2_switch_state = nrf_data_packet[5];
  gen2_volts_val = nrf_data_packet[6];
  gen2_amps_val = nrf_data_packet[7];

  pump1_switch_state = nrf_data_packet[8];
  pump1_volts_val = nrf_data_packet[9];
  pump1_amps_val = nrf_data_packet[10];

  pump2_switch_state = nrf_data_packet[11];
  pump2_volts_val = nrf_data_packet[12];
  pump2_amps_val = nrf_data_packet[13];

  powerlim_val = nrf_data_packet[14];


/*Serial.print("h =:"); Serial.print(h); Serial.print("%"); */Serial.print("["); Serial.print(nrf_data_packet[0]); Serial.print("]");//output humidity to serial, and value stored in array position[0]
/*Serial.print(" t =:"); Serial.print(t); Serial.print("C"); */Serial.print("["); Serial.print(nrf_data_packet[1]); Serial.print("]"); //output temperature to serial, and value stored in array position[1]

/*if (gen1_switch_state == 1){Serial.print("   G1S:"); Serial.print(gen1_onstate);} //output gen1 ON state
else {Serial.print(" G1S:"); Serial.print(gen1_offstate);} //output gen1 OFF state */
Serial.print("["); Serial.print(nrf_data_packet[2]); Serial.print("]"); //value stored in array position[x]
/*Serial.print(" G1V:"); Serial.print(gen1_volts_val); Serial.print("V");*/Serial.print("["); Serial.print(nrf_data_packet[3]); Serial.print("]"); //output voltage on gen1, and value stored in array position[x]
/*Serial.print(" G1A:"); Serial.print(gen1_amps_val); Serial.print("A");*/Serial.print("["); Serial.print(nrf_data_packet[4]); Serial.print("]"); //output current on gen1

/*if (gen2_switch_state == 1){Serial.print("   G2S:"); Serial.print(gen2_onstate);} //output gen2 ON state
else {Serial.print("   G2S:"); Serial.print(gen2_offstate);} //output gen2 OFF state */
Serial.print("["); Serial.print(nrf_data_packet[5]); Serial.print("]"); //value stored in array position[x]
/*Serial.print(" G2V:"); Serial.print(gen2_volts_val); */ Serial.print("["); Serial.print(nrf_data_packet[6]); Serial.print("]"); Serial.print("V"); //output gen2 voltage, and value stored in array position[x]
/*Serial.print(" G2A:"); Serial.print(gen2_amps_val); */ Serial.print("A"); Serial.print("["); Serial.print(nrf_data_packet[7]); Serial.print("]"); //output gen2 current, and value stored in array position[x]

/*if (pump1_switch_state == 1){Serial.print("   P1S:"); Serial.print(pump1_onstate);} //output pump1 ON state
else {Serial.print(" P1S:"); Serial.print(pump1_offstate);} //output pump1 OFF state */
Serial.print("["); Serial.print(nrf_data_packet[8]); Serial.print("]"); //value stored in array position[x]
/*Serial.print(" P1V:"); Serial.print(pump1_volts_val); Serial.print("V"); */ Serial.print("["); Serial.print(nrf_data_packet[9]); Serial.print("]"); //output pump1 voltage, and value stored in array position[x]
/*Serial.print(" P1A:"); Serial.print(pump1_amps_val); Serial.print("A"); */ Serial.print("["); Serial.print(nrf_data_packet[10]); Serial.print("]"); //output pump1 current, and value stored in array position[x]

/*if (pump2_switch_state == 1){Serial.print("   P2S:"); Serial.print(pump2_onstate);} //output pump2 ON state
else {Serial.print("   P2S:"); Serial.print(pump2_offstate);} //output pump2 OFF state*/
Serial.print("["); Serial.print(nrf_data_packet[11]); Serial.print("]"); //value stored in array position[x]
/*Serial.print(" P2V:"); Serial.print(pump2_volts_val); */ Serial.print("V"); Serial.print("["); Serial.print(nrf_data_packet[12]); Serial.print("]"); //output pump2 voltage, and value stored in array position[x]
/*Serial.print(" P2A:"); Serial.print(pump2_amps_val); */ Serial.print("A"); Serial.print("["); Serial.print(nrf_data_packet[13]); Serial.print("]"); //output pump2 current, and value stored in array position[x]

Serial.print(" P_lim(%)"); Serial.print(powerlim_val); Serial.print("%"); Serial.print("["); Serial.print(nrf_data_packet[14]); Serial.print("]"); //output power limit value, and value stored in array position[x]

Serial.println(" "); //carriage return
}