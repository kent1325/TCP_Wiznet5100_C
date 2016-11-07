#include "Wiznet5100.h"
#include "nic_settings.h"
#include "spi.h"

uint8_t getSocketState(void)
{
	uint8_t state = SPI_Read8(S0_SR);
	// Læser SR for at få status
	
	return state;
}

uint16_t getAvailableData(void)
{
   uint16_t dataSize = 0;
	
   //Henter størrelsen på Datasegmentet for den aktuelle pakke
   dataSize = SPI_Read16(S0_RX_RSR);
	
   return dataSize;
}

void disconnect(void)
{
	//Sender disconnect command 
	SPI_Write8(S0_CR, CR_DISCON);
	// Venter på at den fuldfører.
	while((char)SPI_Read8(S0_CR));
}

uint8_t close(void)
{
	uint8_t status = 0;
	
	//Sender close command 
	SPI_Write8(S0_CR, CR_CLOSE);
	
	// Venter på at den fuldfører.
	while((char)SPI_Read8(S0_CR));
	
	// Checker for om socket er lukket
	if (SPI_Read8(S0_MR) != SOCK_CLOSED)
	{
		//Hvis ikke indiker dette med retur værdi
		status = 1;
	}
	return status;
}

uint8_t listen(void)
{
	uint8_t status = 0;
	
	if (((char)SPI_Read8(S0_SR)) == SOCK_INIT) // Vi Socket er initieret, vi kan starte en listen
	{
		// Send listen command
		SPI_Write8(S0_CR, CR_LISTEN);
		
		// Venter på at den fuldfører.
		while((char)SPI_Read8(S0_CR));
		
		// Checker for om vi er i listen state
		if (SPI_Read8(S0_SR) == SOCK_LISTEN)
		{
			status = 1;
		}
		else
		{
			 // (1) Der var fejl, luk den pågældende socket igen.
		}
	}
	return status;	
}

uint8_t openSocket(uint8_t eth_protocol, uint16_t tcp_port)
{
	uint8_t status = 0;
	
	// Kigger først efter om den skulle være åben!
	if (((char)SPI_Read8(S0_MR)) != SOCK_CLOSED)
	{
		close();
	}
	
	// Angiv protocol ud fra #defines i Wiznet5100.h)
	SPI_Write8(S0_MR, eth_protocol);

	// Angiv port
	SPI_Write16(S0_PORT, tcp_port);	

	// Åbner socket'en
	SPI_Write8(S0_CR, CR_OPEN);

	//Venter på at den åbner	
	while (((char)SPI_Read8(S0_CR)));	
	
	if (SPI_Read8(S0_MR) == SOCK_CLOSED)
	{
		status = 1; // Sætter status til andet end 0, for at indikere fejl.
	}
	return status;
}

uint16_t send(const uint8_t *buf, uint16_t bufLen)
{
	uint16_t offadr, realadr, txSize, timeout;
		
	if(bufLen <= 0)
	{
		return 0;
	}	
	
	// Ser om der er plads til at sende data i NIC-TX buffer.
	
	timeout =0;

	// Loopet kigger efter om der er plads til at sende mere.
	// Hvis denne fejler 1000 gange... Laaaaaang tid... 
	// Så lukker vi forbindelsen og frigør den pågældende socket.
	do
	{
		txSize = SPI_Read16(S0_TX_FSR);
		
		if(timeout++ > 1000)
		{
			// Luk forbindelsen!
			SPI_Write8(S0_CR, CR_CLOSE);
			while((char)SPI_Read8(S0_CR));

			return 0;
		}
	} while (txSize<bufLen);

	
	// Finder offset adressen vi skal skrive til.
	offadr = SPI_Read16(S0_TX_WR);
	
	while (bufLen)
	{
		 bufLen--;
		 
		 realadr = TXBUFADDR + (offadr & TX_BUF_MASK);
		 
		 SPI_Write8(realadr, *buf);
		 offadr++;
		 buf++;
	}
	
	// Opdaterer netkortets buffer pointer
	SPI_Write16(S0_TX_WR, offadr);
	
	//sender "Send" command til NIC
	SPI_Write8(S0_CR, CR_SEND);
	
	//Venter på at der er sendt.
	while((char)SPI_Read8(S0_CR));
	
	return 1;
}

uint16_t recieve(uint8_t *buf, uint16_t bufLen)
{
	uint16_t offAddr, realAddr;

	// hvis angive længde er større end den angiven intern array længde i MCU'en
	// afkorter vi den! – ikke ret pænt...
	if (bufLen > MAX_BUF)
	{
		bufLen = MAX_BUF -2;
	}
	
	// 1a Læs Read pointer fra nic
	offAddr = SPI_Read16(S0_RX_RD);

	// 1b Læs alt data. 
	while (bufLen)
	{
		bufLen--;
		realAddr = RXBUFADDR +(offAddr & RX_BUF_MASK);
		*buf = SPI_Read8(realAddr);
		offAddr++;
		buf++;
	}
		
	// 2 Opdaterer vores NIC, så adr. pointer passer med den næste pakke i nic mem.
	SPI_Write16(S0_RX_RD, offAddr);
	
	// 3 Indikerer at vi har modtaget en pakke.
	SPI_Write8(S0_CR, CR_RECV);
	while((char)SPI_Read8(S0_CR));
	
	return 1;
}

void WizInit(void)
{
	// Basic ethernet Setup
  
	// MR = 0b10000000; Soft Reset...
	SPI_Write8(MR,0x80); 

	// Gateway addr
	SPI_Write8(GAR + 0,gtw_addr[0]);
	SPI_Write8(GAR + 1,gtw_addr[1]);
	SPI_Write8(GAR + 2,gtw_addr[2]);
	SPI_Write8(GAR + 3,gtw_addr[3]);

	// MAC addr
	SPI_Write8(SAR + 0,mac_addr[0]);
	SPI_Write8(SAR + 1,mac_addr[1]);
	SPI_Write8(SAR + 2,mac_addr[2]);
	SPI_Write8(SAR + 3,mac_addr[3]);
	SPI_Write8(SAR + 4,mac_addr[4]);
	SPI_Write8(SAR + 5,mac_addr[5]);

	// Subnet mask
	SPI_Write8(SUBR + 0,sub_mask[0]);
	SPI_Write8(SUBR + 1,sub_mask[1]);
	SPI_Write8(SUBR + 2,sub_mask[2]);
	SPI_Write8(SUBR + 3,sub_mask[3]);

	//IP Addr
	SPI_Write8(SIPR + 0,ip_addr[0]);
	SPI_Write8(SIPR + 1,ip_addr[1]);
	SPI_Write8(SIPR + 2,ip_addr[2]);
	SPI_Write8(SIPR + 3,ip_addr[3]);
 
	// Recieve and transmission
	SPI_Write8(RMSR,0x55);		// Tildeler 0b01010101 hvilket er 2Kb til hver socket's RX buffer. (Maximum 8 KB i alt for alle 4!)
	SPI_Write8(TMSR,0x55);		// Samme for TX
  
	// Basic Init er overstået, nu kan vi pinge...
}
