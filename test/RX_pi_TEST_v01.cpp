#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>

using namespace std;

RF24 rx_module(25, 0);

#define MAX_LEN 30

uint16_t nrf_data_packet[MAX_LEN];

//==============code pasted from arduino nano==================

uint16_t h = 0; //humidity value as read from dht_sensor1 NOTE for NRF use - 2 bytes
uint16_t t = 0; //temperature value as read from dht_sensor1 NOTE for NRF use - 2 bytes

uint16_t gen1_switch_state = 0; //state of gen1 (ON/OFF) NOTE for NRF use - 2 byte
uint16_t gen1_volts_val = 0; //voltage on gen1 NOTE for NRF use - 2 bytes
uint16_t gen1_amps_val = 0; //current on gen1NOTE for NRF use - 2 bytes

uint16_t gen2_switch_state = 0; //state of gen2 (ON/OFF) NOTE for NRF use - 2 byte
uint16_t gen2_volts_val = 0; //voltage on gen2 NOTE for NRF use - 2 bytes
uint16_t gen2_amps_val = 0; //current on gen2 NOTE for NRF use - 2 bytes

uint16_t pump1_switch_state = 0; //state of pump1 (ON/OFF) NOTE for NRF use - 2 byte
uint16_t pump1_volts_val = 0; //voltage on pump1 NOTE for NRF use - 2 bytes
uint16_t pump1_amps_val = 0; //current on pump1 NOTE for NRF use - 2 bytes

uint16_t pump2_switch_state = 0; //state of pump2 (ON/OFF) NOTE for NRF use - 2 byte
uint16_t pump2_volts_val = 0; //voltage on pump2 NOTE for NRF use - 2 bytes
uint16_t pump2_amps_val = 0; //current on pump2 NOTE for NRF use - 2 bytes

uint16_t powerlim_val = 0; //power limit value NOTE for NRF use - 2 bytes

//=======================================\

int main() {

while (true){
	rx_module.begin(); //initialize the RX module
	rx_module.openReadingPipe(0, 0x7878787878LL); //open communication pipe
	rx_module.setChannel(110); //set the radio channel, must be same as on TX
	rx_module.setPALevel(RF24_PA_MAX); //set power of NRF module, can be changed to RF24_PA_MIN for lower power consumptiom, BUT MUST BE SAME ON THE RX MODULE!!!
	rx_module.setDataRate(RF24_250KBPS);
	rx_module.startListening(); //puts the NRF module into RECEIVE MODE
		
	if (rx_module.available()) {
			rx_module.read(&nrf_data_packet, MAX_LEN);

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

		
		  printf("h = %d", h);
  		printf(" t = %d", t);

		  printf(" Gen1 ON/OFF: %d", gen1_switch_state);
  		printf(" Gen1 V: %d", gen1_volts_val);
  		printf(" Gen1 I: %d", gen1_amps_val);

  		printf(" Gen2 ON/OFF: %d", gen2_switch_state);
  		printf(" Gen2 V: %d", gen2_volts_val);
  		printf(" Gen2 I: %d", gen2_amps_val);

  		printf(" Pump1 ON/OFF: %d", pump1_switch_state);
  		printf(" Pump1 V: %d", pump1_volts_val);
  		printf(" Pump1 I = %d", pump1_amps_val);

  		printf(" Pump2 ON/OFF: %d", pump2_switch_state);
  		printf(" Pump2 V: %d", pump2_volts_val);
  		printf(" Pump2 I: %d", pump2_amps_val);

  		printf(" Power Limit : %d\n", powerlim_val);
			}

	else printf("No transmission \n");

}
return 0;
}

//printf("Value of i = %d\n",i);
