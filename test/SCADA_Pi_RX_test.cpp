/*
=============scanner code from examples_linux

+++++++CODE START (KNOWN WORKING)+++++++


 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 03/17/2013 : Charles-Henri Hallard (http://hallard.me)
              Modified to use with Arduipi board http://hallard.me/arduipi
						  Changed to use modified bcm2835 and RF24 library
 


 * Channel scanner
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html

#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>

using namespace std;

/****************** Linux **********************
// Radio CE Pin, CSN Pin, SPI Speed
// CE Pin uses GPIO number with BCM and SPIDEV drivers, other platforms use their own pin numbering
// CS Pin addresses the SPI bus number at /dev/spidev<a>.<b>
// ie: RF24 radio(<ce_pin>, <a>*10+<b>); spidev1.0 is 10, spidev1.1 is 11 etc..

// Generic:
RF24 radio(25, 0);
/****************** Linux (BBB,x86,etc) ***********************

// See http://nRF24.github.io/RF24/pages.html for more information on usage
// See http://iotdk.intel.com/docs/master/mraa/ for more information on MRAA
// See https://www.kernel.org/doc/Documentation/spi/spidev for more information on SPIDEV

// Channel info
const uint8_t num_channels = 126;
uint8_t values[num_channels];

const int num_reps = 100;
int reset_array = 0;

int main(int argc, char** argv)
{
    // Print preamble

    // print example's name
    printf("%s", argv[0]);

    //
    // Setup and configure rf radio
    //
    radio.begin();

    radio.setAutoAck(false);

    // Get into standby mode
    radio.startListening();
    radio.stopListening();

    radio.printDetails();

    // Print out header, high then low digit
    int i = 0;

    while (i < num_channels) {
        printf("%x", i >> 4);
        ++i;
    }
    printf("\n");

    i = 0;
    while (i < num_channels) {
        printf("%x", i & 0xf);
        ++i;
    }
    printf("\n");

    // forever loop
    while (1) {
        // Clear measurement values
        memset(values, 0, sizeof(values));

        // Scan all channels num_reps times
        int rep_counter = num_reps;
        while (rep_counter--) {

            int i = num_channels;
            while (i--) {

                // Select this channel
                radio.setChannel(i);

                // Listen for a little
                radio.startListening();
                delayMicroseconds(128);
                radio.stopListening();

                // Did we get a carrier?
                if (radio.testCarrier()) {
                    ++values[i];
                }
            }
        }

        // Print out channel measurements, clamped to a single hex digit
        i = 0;
        while (i < num_channels) {
            if (values[i])
                printf("%x", min(0xf, (values[i] & 0xf)));
            else
                printf("-");

            ++i;
        }
        printf("\n");
    }

    return 0;
}


-------CODE END---------
*/

//=============MAIN CODE START===========

#include <cstdlib>
#include <iostream>
#include <RF24/RF24.h>

using namespace std;

RF24 radio(25, 0);

#define BCM_PIN 7
#define SPI_DEV 0
#define MAX_LEN 14

uint8_t nrf_data_packet[MAX_LEN];
uint8_t pipeNumber;


//==============code pasted from arduino nano==================

uint8_t address[6] = {"42"}; //address for the module, AFAIK both TX and RX modules should have same value here
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
int nrf_data_packet[14]; //array of variables to be received, the array seems one smaller than total number of variables because 0-9 not 1-10


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

int main() {

rx_module.begin(); //initialize the RX module
rx_module.openReadingPipe(0x7878787878LL); //open communication pipe
rx_module.setChannel(110); //set the radio channel, must be same as on TX
rx_module.setPALevel(RF24_PA_MAX); //set power of NRF module, can be changed to RF24_PA_MIN for lower power consumptiom, BUT MUST BE SAME ON THE RX MODULE!!!
rx_module.setDataRate(RF24_250KBPS);
rx_module.startListening(); //puts the NRF module into RECEIVE MODE

rx_module.read(nrf_data_packet, sizeof(nrf_data_packet));

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

  //printf("Value of i = %d\n",i);

  printf("h = %d\n", h);
  printf("t = %d\n", t);

  printf(" Gen1 ON/OFF: %d\n", gen1_switch_state);
  printf(" Gen1 V: %d\n", gen1_volts_val);
  printf(" Gen1 I: %d\n", gen1_amps_val);

  printf(" Gen2 ON/OFF: %d\n", gen2_switch_state);
  printf(" Gen2 V: %d\n", gen2_volts_val);
  printf(" Gen2 I: %d\n", gen2_amps_val);

  printf(" Pump1 ON/OFF: %d\n", pump1_switch_state);
  printf(" Pump1 V: %d\n", pump1_volts_val);
  printf(" Pump1 I = %d\n", pump1_amps_val);

  printf(" Pump2 ON/OFF: %d\n", pump2_switch_state);
  printf(" Pump2 V: %d\n", pump2_volts_val);
  printf(" Pump2 I: %d\n", pump2_amps_val);

  printf(" Power Limit : %d\n", powerlim_val);
}