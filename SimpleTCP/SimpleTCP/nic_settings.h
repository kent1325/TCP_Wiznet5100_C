#ifndef NIC_SETTINGS_H_
#define NIC_SETTINGS_H_

// Ethernet specifikke settings
unsigned char mac_addr[] = {0x90,0x1B,0x0E,0x67,0x6B,0x98};
unsigned char ip_addr[] = {192,168,0,5};
unsigned char sub_mask[] = {255,255,255,0};
unsigned char gtw_addr[] = {192,168,0,1}; 

// Max størrelse på vores RX/TX Buffer her i microen.
#define MAX_BUF 25

#endif /* NIC_SETTINGS_H_ */
