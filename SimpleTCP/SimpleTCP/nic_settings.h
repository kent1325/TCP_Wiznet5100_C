#ifndef NIC_SETTINGS_H_
#define NIC_SETTINGS_H_

// Ethernet specifikke settings
unsigned char mac_addr[] = {0x65,0xCF,0x1C,0xE7,0xF0,0x76};
unsigned char ip_addr[] = {192,168,0,5};
unsigned char sub_mask[] = {255,255,255,0};
unsigned char gtw_addr[] = {192,168,0,1}; 

// Max størrelse på vores RX/TX Buffer her i microen.
#define MAX_BUF 128

#endif /* NIC_SETTINGS_H_ */