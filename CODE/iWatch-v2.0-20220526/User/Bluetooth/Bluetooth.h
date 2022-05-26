#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

#define	BTSTA		P00
#define	BTEN		P45

void BtOn(void);
void BtOff(void);
unsigned char BtCheckConnection(void);
unsigned char BtReceiveString(char *dptr);
void BtSendString(const char *str);

#endif