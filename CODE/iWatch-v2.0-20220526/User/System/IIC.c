#include "IIC.h"
#include "STC32G.h"
#include "Delay.h"

unsigned int data timeout;

void IIC_Init(void)
{
	P_SW2 |= 0x80;
	I2CCFG = 0xC0 | 0x0A;                              //ʹ��I2C����ģʽ
  I2CMSST = 0x00;
	P_SW2 &= 0x7f;
}
void IIC_Deinit(void)
{
	P_SW2 |= 0x80;
	I2CCFG = 0x00;
  I2CMSST = 0x00;
	P_SW2 &= 0x7f;
}
void Wait()
{
	timeout = 300;
	while (!(I2CMSST & 0x40))
	{
		if(--timeout == 0)
			break;
	}
  I2CMSST &= ~0x40;
}
void I2C_Start()
{
	I2CMSCR = 0x01;                             //����START����
	Wait();
}
void I2C_Stop()
{
	I2CMSCR = 0x06;                             //����STOP����
  Wait();
}
void I2C_SendByte(unsigned char dat)
{
	I2CTXD = dat;                               //д���ݵ����ݻ�����
  I2CMSCR = 0x02;                             //����SEND����
	Wait();
}
unsigned char I2C_RecvByte()
{
	I2CMSCR = 0x04;                             //����RECV����
  Wait();
  return I2CRXD;
}
void I2C_RecvACK()
{
	I2CMSCR = 0x03;                             //���Ͷ�ACK����
  Wait();
}
void I2C_SendACK()
{
	I2CMSST = 0x00;                             //����ACK�ź�
  I2CMSCR = 0x05;                             //����ACK����
  Wait();
}
void I2C_SendNAK()
{
	I2CMSST = 0x01;                             //����NAK�ź�
  I2CMSCR = 0x05;                             //����ACK����
  Wait();
}
//**************************************
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteIIC(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
{
	P_SW2 |= 0x80;
  I2C_Start();                  //��ʼ�ź�
  I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
	I2C_RecvACK();
	I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
	I2C_RecvACK();
	I2C_SendByte(REG_data);       //�ڲ��Ĵ�������
	I2C_RecvACK();
	I2C_Stop();                   //����ֹͣ�ź�
	P_SW2 &= 0x7f;
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
unsigned char Single_ReadIIC(unsigned char SlaveAddress, unsigned char REG_Address)
{
	unsigned char REG_data;
	P_SW2 |= 0x80;
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_RecvACK();
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
	I2C_RecvACK();
	I2C_Start();                   //��ʼ�ź�
	I2C_SendByte(SlaveAddress | 0x01);  //�����豸��ַ+���ź�
	I2C_RecvACK();
	REG_data=I2C_RecvByte();       //�����Ĵ�������
	I2C_SendNAK();								 //����Ӧ���ź�
	I2C_Stop();                    //ֹͣ�ź�
	P_SW2 &= 0x7f;
	return REG_data;
}
void I2C_Read_MultiBytes(unsigned char DeviceAddr, unsigned char address, unsigned char BytesNum, unsigned char *buf)
{
	unsigned char i;
	P_SW2 |= 0x80;
  I2C_Start();                  //��ʼ�ź�
  I2C_SendByte(DeviceAddr);   //�����豸��ַ+д�ź�
	I2C_RecvACK();
	I2C_SendByte(address);    //�ڲ��Ĵ�����ַ
	I2C_RecvACK();
	I2C_Start();
	I2C_SendByte(DeviceAddr | 0x01);
	I2C_RecvACK();
	for(i = 0; i < (BytesNum - 1); i++)
	{
		buf[i] = I2C_RecvByte();
		I2C_SendACK();	
	}
	buf[i] = I2C_RecvByte();
	I2C_SendNAK();
	I2C_Stop();                   //����ֹͣ�ź�
	P_SW2 &= 0x7f;
}
void I2C_Write_MultiBytes(unsigned char DeviceAddr, unsigned char address, unsigned char BytesNum, unsigned char *buf)
{
	unsigned char i;
	P_SW2 |= 0x80;
  I2C_Start();                  //��ʼ�ź�
  I2C_SendByte(DeviceAddr);   //�����豸��ַ+д�ź�
	I2C_RecvACK();
	I2C_SendByte(address);    //�ڲ��Ĵ�����ַ
	I2C_RecvACK();
	for(i = 0; i < BytesNum; i++)
	{
		I2C_SendByte(buf[i]);
		I2C_RecvACK();	
	}
	I2C_Stop();                   //����ֹͣ�ź�
	P_SW2 &= 0x7f;
}