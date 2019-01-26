#ifndef NRF24L01LIB_H_
#define NRF24L01LIB_H_

#include <stdbool.h>

// nRF24L01-Library-Funktionen
bool nRF24L01_init_system(void);
void nRF24L01_send_string(char * s);
bool nRF24L01_data_ready(void);
bool nRF24L01_read_string(char *s);

// default address: 0x01 --> 0x0101010101 (40bit)
// Bsp.: @0x02 --> 0x0202020202 (40bit)
void nRF24L01_set_address(char address);
char nRF24L01_get_address(void);


#endif /* NRF24L01LIB_H_ */