#ifndef     __STC32G_H__
#define     __STC32G_H__

/////////////////////////////////////////////////

sfr         P0          =           0x80;
sbit        P00         =           P0^0;
sbit        P01         =           P0^1;
sbit        P02         =           P0^2;
sbit        P03         =           P0^3;
sbit        P04         =           P0^4;
sbit        P05         =           P0^5;
sbit        P06         =           P0^6;
sbit        P07         =           P0^7;
sfr         SP          =           0x81;
sfr         DPL         =           0x82;
sfr         DPH         =           0x83;
sfr         DPXL        =           0x84;
sfr         SPH         =           0x85;
sfr         PCON        =           0x87;
sfr         TCON        =           0x88;
sbit        TF1         =           TCON^7;
sbit        TR1         =           TCON^6;
sbit        TF0         =           TCON^5;
sbit        TR0         =           TCON^4;
sbit        IE1         =           TCON^3;
sbit        IT1         =           TCON^2;
sbit        IE0         =           TCON^1;
sbit        IT0         =           TCON^0;
sfr         TMOD        =           0x89;
sfr         TL0         =           0x8a;
sfr         TL1         =           0x8b;
sfr         TH0         =           0x8c;
sfr         TH1         =           0x8d;
sfr         AUXR        =           0x8e;
sfr         INTCLKO     =           0x8f;
sfr         P1          =           0x90;
sbit        P10         =           P1^0;
sbit        P11         =           P1^1;
sbit        P12         =           P1^2;
sbit        P13         =           P1^3;
sbit        P14         =           P1^4;
sbit        P15         =           P1^5;
sbit        P16         =           P1^6;
sbit        P17         =           P1^7;
sfr         P1M1        =           0x91;
sfr         P1M0        =           0x92;
sfr         P0M1        =           0x93;
sfr         P0M0        =           0x94;
sfr         P2M1        =           0x95;
sfr         P2M0        =           0x96;
sfr         AUXR2       =           0x97;
sfr         SCON        =           0x98;
sbit        SM0         =           SCON^7;
sbit        SM1         =           SCON^6;
sbit        SM2         =           SCON^5;
sbit        REN         =           SCON^4;
sbit        TB8         =           SCON^3;
sbit        RB8         =           SCON^2;
sbit        TI          =           SCON^1;
sbit        RI          =           SCON^0;
sfr         SBUF        =           0x99;
sfr         S2CON       =           0x9a;
sfr         S2BUF       =           0x9b;
sfr         IRCBAND     =           0x9d;
sfr         LIRTRIM     =           0x9e;
sfr         IRTRIM      =           0x9f;
sfr         P2          =           0xa0;
sbit        P20         =           P2^0;
sbit        P21         =           P2^1;
sbit        P22         =           P2^2;
sbit        P23         =           P2^3;
sbit        P24         =           P2^4;
sbit        P25         =           P2^5;
sbit        P26         =           P2^6;
sbit        P27         =           P2^7;
sfr         BUSSPEED    =           0xa1;
sfr         P_SW1       =           0xa2;
sfr         V33TRIM     =           0xa3;
sfr         BGTRIM      =           0xa5;
sfr         VRTRIM      =           0xa6;
sfr         IE          =           0xa8;
sbit        EA          =           IE^7;
sbit        ELVD        =           IE^6;
sbit        EADC        =           IE^5;
sbit        ES          =           IE^4;
sbit        ET1         =           IE^3;
sbit        EX1         =           IE^2;
sbit        ET0         =           IE^1;
sbit        EX0         =           IE^0;
sfr         SADDR       =           0xa9;
sfr         WKTCL       =           0xaa;
sfr         WKTCH       =           0xab;
sfr         S3CON       =           0xac;
sfr         S3BUF       =           0xad;
sfr         TA          =           0xae;
sfr         IE2         =           0xaf;
sfr         P3          =           0xb0;
sbit        P30         =           P3^0;
sbit        P31         =           P3^1;
sbit        P32         =           P3^2;
sbit        P33         =           P3^3;
sbit        P34         =           P3^4;
sbit        P35         =           P3^5;
sbit        P36         =           P3^6;
sbit        P37         =           P3^7;
sfr         P3M1        =           0xb1;
sfr         P3M0        =           0xb2;
sfr         P4M1        =           0xb3;
sfr         P4M0        =           0xb4;
sfr         IP2         =           0xb5;
sfr         IP2H        =           0xb6;
sfr         IPH         =           0xb7;
sfr         IP          =           0xb8;
sbit        PLVD        =           IP^6;
sbit        PADC        =           IP^5;
sbit        PS          =           IP^4;
sbit        PT1         =           IP^3;
sbit        PX1         =           IP^2;
sbit        PT0         =           IP^1;
sbit        PX0         =           IP^0;
sfr         SADEN       =           0xb9;
sfr         P_SW2       =           0xba;
sfr         P_SW3       =           0xbb;
sfr         ADC_CONTR   =           0xbc;
sfr         ADC_RES     =           0xbd;
sfr         ADC_RESL    =           0xbe;
sfr         P4          =           0xc0;
sbit        P40         =           P4^0;
sbit        P41         =           P4^1;
sbit        P42         =           P4^2;
sbit        P43         =           P4^3;
sbit        P44         =           P4^4;
sbit        P45         =           P4^5;
sbit        P46         =           P4^6;
sbit        P47         =           P4^7;
sfr         WDT_CONTR   =           0xc1;
sfr         IAP_DATA    =           0xc2;
sfr         IAP_ADDRH   =           0xc3;
sfr         IAP_ADDRL   =           0xc4;
sfr         IAP_CMD     =           0xc5;
sfr         IAP_TRIG    =           0xc6;
sfr         IAP_CONTR   =           0xc7;
sfr         P5          =           0xc8;
sbit        P50         =           P5^0;
sbit        P51         =           P5^1;
sbit        P52         =           P5^2;
sbit        P53         =           P5^3;
sbit        P54         =           P5^4;
sbit        P55         =           P5^5;
sbit        P56         =           P5^6;
sbit        P57         =           P5^7;
sfr         P5M1        =           0xc9;
sfr         P5M0        =           0xca;
sfr         P6M1        =           0xcb;
sfr         P6M0        =           0xcc;
sfr         SPSTAT      =           0xcd;
sfr         SPCTL       =           0xce;
sfr         SPDAT       =           0xcf;
sfr         PSW         =           0xd0;
sbit        CY          =           PSW^7;
sbit        AC          =           PSW^6;
sbit        F0          =           PSW^5;
sbit        RS1         =           PSW^4;
sbit        RS0         =           PSW^3;
sbit        OV          =           PSW^2;
sbit        P           =           PSW^0;
sfr         PSW1        =           0xd1;
sfr         T4H         =           0xd2;
sfr         T4L         =           0xd3;
sfr         T3H         =           0xd4;
sfr         T3L         =           0xd5;
sfr         T2H         =           0xd6;
sfr         T2L         =           0xd7;
sfr         USBCLK      =           0xdc;
sfr         T4T3M       =           0xdd;
sfr         ADCCFG      =           0xde;
sfr         IP3         =           0xdf;
sfr         ACC         =           0xe0;
sfr         P7M1        =           0xe1;
sfr         P7M0        =           0xe2;
sfr         DPS         =           0xe3;
sfr         DPL1        =           0xe4;
sfr         DPH1        =           0xe5;
sfr         CMPCR1      =           0xe6;
sfr         CMPCR2      =           0xe7;
sfr         P6          =           0xe8;
sbit        P60         =           P6^0;
sbit        P61         =           P6^1;
sbit        P62         =           P6^2;
sbit        P63         =           P6^3;
sbit        P64         =           P6^4;
sbit        P65         =           P6^5;
sbit        P66         =           P6^6;
sbit        P67         =           P6^7;
sfr         WTST        =           0xe9;
sfr         CKCON       =           0xea;
sfr         MXAX        =           0xeb;
sfr         USBDAT      =           0xec;
sfr         DMAIR       =           0xed;
sfr         IP3H        =           0xee;
sfr         AUXINTIF    =           0xef;
sfr         B           =           0xf0;
sfr         CANICR      =           0xf1;
sfr         USBCON      =           0xf4;
sfr         IAP_TPS     =           0xf5;
sfr         IAP_ADDRE   =           0xf6;
sfr         ICHECR      =           0xf7;
sfr         P7          =           0xf8;
sbit        P70         =           P7^0;
sbit        P71         =           P7^1;
sbit        P72         =           P7^2;
sbit        P73         =           P7^3;
sbit        P74         =           P7^4;
sbit        P75         =           P7^5;
sbit        P76         =           P7^6;
sbit        P77         =           P7^7;
sfr         LINICR      =           0xf9;
sfr         LINAR       =           0xfa;
sfr         LINDR       =           0xfb;
sfr         USBADR      =           0xfc;
sfr         S4CON       =           0xfd;
sfr         S4BUF       =           0xfe;
sfr         RSTCFG      =           0xff;

//如下特殊功能寄存器位于扩展RAM区域
//访问这些寄存器,需先将P_SW2的BIT7设置为1,才可正常读写

/////////////////////////////////////////////////
//7E:FF00H-7E:FFFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//7E:FE00H-7E:FEFFH
/////////////////////////////////////////////////

#define     CLKSEL                  (*(unsigned char volatile far *)0x7efe00)
#define     CLKDIV                  (*(unsigned char volatile far *)0x7efe01)
#define     HIRCCR                  (*(unsigned char volatile far *)0x7efe02)
#define     XOSCCR                  (*(unsigned char volatile far *)0x7efe03)
#define     IRC32KCR                (*(unsigned char volatile far *)0x7efe04)
#define     MCLKOCR                 (*(unsigned char volatile far *)0x7efe05)
#define     IRCDB                   (*(unsigned char volatile far *)0x7efe06)
#define     IRC48MCR                (*(unsigned char volatile far *)0x7efe07)
#define     X32KCR                  (*(unsigned char volatile far *)0x7efe08)
#define     IRC48ATRIM              (*(unsigned char volatile far *)0x7efe09)
#define     IRC48BTRIM              (*(unsigned char volatile far *)0x7efe0a)
#define     HSCLKDIV                (*(unsigned char volatile far *)0x7efe0b)

#define     P0PU                    (*(unsigned char volatile far *)0x7efe10)
#define     P1PU                    (*(unsigned char volatile far *)0x7efe11)
#define     P2PU                    (*(unsigned char volatile far *)0x7efe12)
#define     P3PU                    (*(unsigned char volatile far *)0x7efe13)
#define     P4PU                    (*(unsigned char volatile far *)0x7efe14)
#define     P5PU                    (*(unsigned char volatile far *)0x7efe15)
#define     P6PU                    (*(unsigned char volatile far *)0x7efe16)
#define     P7PU                    (*(unsigned char volatile far *)0x7efe17)
#define     P0NCS                   (*(unsigned char volatile far *)0x7efe18)
#define     P1NCS                   (*(unsigned char volatile far *)0x7efe19)
#define     P2NCS                   (*(unsigned char volatile far *)0x7efe1a)
#define     P3NCS                   (*(unsigned char volatile far *)0x7efe1b)
#define     P4NCS                   (*(unsigned char volatile far *)0x7efe1c)
#define     P5NCS                   (*(unsigned char volatile far *)0x7efe1d)
#define     P6NCS                   (*(unsigned char volatile far *)0x7efe1e)
#define     P7NCS                   (*(unsigned char volatile far *)0x7efe1f)
#define     P0SR                    (*(unsigned char volatile far *)0x7efe20)
#define     P1SR                    (*(unsigned char volatile far *)0x7efe21)
#define     P2SR                    (*(unsigned char volatile far *)0x7efe22)
#define     P3SR                    (*(unsigned char volatile far *)0x7efe23)
#define     P4SR                    (*(unsigned char volatile far *)0x7efe24)
#define     P5SR                    (*(unsigned char volatile far *)0x7efe25)
#define     P6SR                    (*(unsigned char volatile far *)0x7efe26)
#define     P7SR                    (*(unsigned char volatile far *)0x7efe27)
#define     P0DR                    (*(unsigned char volatile far *)0x7efe28)
#define     P1DR                    (*(unsigned char volatile far *)0x7efe29)
#define     P2DR                    (*(unsigned char volatile far *)0x7efe2a)
#define     P3DR                    (*(unsigned char volatile far *)0x7efe2b)
#define     P4DR                    (*(unsigned char volatile far *)0x7efe2c)
#define     P5DR                    (*(unsigned char volatile far *)0x7efe2d)
#define     P6DR                    (*(unsigned char volatile far *)0x7efe2e)
#define     P7DR                    (*(unsigned char volatile far *)0x7efe2f)
#define     P0IE                    (*(unsigned char volatile far *)0x7efe30)
#define     P1IE                    (*(unsigned char volatile far *)0x7efe31)
#define     P2IE                    (*(unsigned char volatile far *)0x7efe32)
#define     P3IE                    (*(unsigned char volatile far *)0x7efe33)
#define     P4IE                    (*(unsigned char volatile far *)0x7efe34)
#define     P5IE                    (*(unsigned char volatile far *)0x7efe35)
#define     P6IE                    (*(unsigned char volatile far *)0x7efe36)
#define     P7IE                    (*(unsigned char volatile far *)0x7efe37)

#define     LCMIFCFG                (*(unsigned char volatile far *)0x7efe50)
#define     LCMIFCFG2               (*(unsigned char volatile far *)0x7efe51)
#define     LCMIFCR                 (*(unsigned char volatile far *)0x7efe52)
#define     LCMIFSTA                (*(unsigned char volatile far *)0x7efe53)
#define     LCMIFDATL               (*(unsigned char volatile far *)0x7efe54)
#define     LCMIFDATH               (*(unsigned char volatile far *)0x7efe55)

#define     RTCCR                   (*(unsigned char volatile far *)0x7efe60)
#define     RTCCFG                  (*(unsigned char volatile far *)0x7efe61)
#define     RTCIEN                  (*(unsigned char volatile far *)0x7efe62)
#define     RTCIF                   (*(unsigned char volatile far *)0x7efe63)
#define     ALAHOUR                 (*(unsigned char volatile far *)0x7efe64)
#define     ALAMIN                  (*(unsigned char volatile far *)0x7efe65)
#define     ALASEC                  (*(unsigned char volatile far *)0x7efe66)
#define     ALASSEC                 (*(unsigned char volatile far *)0x7efe67)
#define     INIYEAR                 (*(unsigned char volatile far *)0x7efe68)
#define     INIMONTH                (*(unsigned char volatile far *)0x7efe69)
#define     INIDAY                  (*(unsigned char volatile far *)0x7efe6a)
#define     INIHOUR                 (*(unsigned char volatile far *)0x7efe6b)
#define     INIMIN                  (*(unsigned char volatile far *)0x7efe6c)
#define     INISEC                  (*(unsigned char volatile far *)0x7efe6d)
#define     INISSEC                 (*(unsigned char volatile far *)0x7efe6e)
#define     YEAR                    (*(unsigned char volatile far *)0x7efe70)
#define     MONTH                   (*(unsigned char volatile far *)0x7efe71)
#define     DAY                     (*(unsigned char volatile far *)0x7efe72)
#define     HOUR                    (*(unsigned char volatile far *)0x7efe73)
#define     MIN                     (*(unsigned char volatile far *)0x7efe74)
#define     SEC                     (*(unsigned char volatile far *)0x7efe75)
#define     SSEC                    (*(unsigned char volatile far *)0x7efe76)

#define     I2CCFG                  (*(unsigned char volatile far *)0x7efe80)
#define     I2CMSCR                 (*(unsigned char volatile far *)0x7efe81)
#define     I2CMSST                 (*(unsigned char volatile far *)0x7efe82)
#define     I2CSLCR                 (*(unsigned char volatile far *)0x7efe83)
#define     I2CSLST                 (*(unsigned char volatile far *)0x7efe84)
#define     I2CSLADR                (*(unsigned char volatile far *)0x7efe85)
#define     I2CTXD                  (*(unsigned char volatile far *)0x7efe86)
#define     I2CRXD                  (*(unsigned char volatile far *)0x7efe87)
#define     I2CMSAUX                (*(unsigned char volatile far *)0x7efe88)

#define     SPFUNC                  (*(unsigned char volatile far *)0x7efe98)
#define     RSTFLAG                 (*(unsigned char volatile far *)0x7efe99)
#define     RSTCR0                  (*(unsigned char volatile far *)0x7efe9a)
#define     RSTCR1                  (*(unsigned char volatile far *)0x7efe9b)
#define     RSTCR2                  (*(unsigned char volatile far *)0x7efe9c)
#define     RSTCR3                  (*(unsigned char volatile far *)0x7efe9d)
#define     RSTCR4                  (*(unsigned char volatile far *)0x7efe9e)
#define     RSTCR5                  (*(unsigned char volatile far *)0x7efe9f)

#define     TM0PS                   (*(unsigned char volatile far *)0x7efea0)
#define     TM1PS                   (*(unsigned char volatile far *)0x7efea1)
#define     TM2PS                   (*(unsigned char volatile far *)0x7efea2)
#define     TM3PS                   (*(unsigned char volatile far *)0x7efea3)
#define     TM4PS                   (*(unsigned char volatile far *)0x7efea4)
#define     ADCTIM                  (*(unsigned char volatile far *)0x7efea8)
#define     T3T4PS                  (*(unsigned char volatile far *)0x7efeac)
#define     ADCEXCFG                (*(unsigned char volatile far *)0x7efead)
#define     CMPEXCFG                (*(unsigned char volatile far *)0x7efeae)

#define     PWMA_ETRPS              (*(unsigned char volatile far *)0x7efeb0)
#define     PWMA_ENO                (*(unsigned char volatile far *)0x7efeb1)
#define     PWMA_PS                 (*(unsigned char volatile far *)0x7efeb2)
#define     PWMA_IOAUX              (*(unsigned char volatile far *)0x7efeb3)
#define     PWMB_ETRPS              (*(unsigned char volatile far *)0x7efeb4)
#define     PWMB_ENO                (*(unsigned char volatile far *)0x7efeb5)
#define     PWMB_PS                 (*(unsigned char volatile far *)0x7efeb6)
#define     PWMB_IOAUX              (*(unsigned char volatile far *)0x7efeb7)
#define     CANAR                   (*(unsigned char volatile far *)0x7efebb)
#define     CANDR                   (*(unsigned char volatile far *)0x7efebc)
#define     PWMA_CR1                (*(unsigned char volatile far *)0x7efec0)
#define     PWMA_CR2                (*(unsigned char volatile far *)0x7efec1)
#define     PWMA_SMCR               (*(unsigned char volatile far *)0x7efec2)
#define     PWMA_ETR                (*(unsigned char volatile far *)0x7efec3)
#define     PWMA_IER                (*(unsigned char volatile far *)0x7efec4)
#define     PWMA_SR1                (*(unsigned char volatile far *)0x7efec5)
#define     PWMA_SR2                (*(unsigned char volatile far *)0x7efec6)
#define     PWMA_EGR                (*(unsigned char volatile far *)0x7efec7)
#define     PWMA_CCMR1              (*(unsigned char volatile far *)0x7efec8)
#define     PWMA_CCMR2              (*(unsigned char volatile far *)0x7efec9)
#define     PWMA_CCMR3              (*(unsigned char volatile far *)0x7efeca)
#define     PWMA_CCMR4              (*(unsigned char volatile far *)0x7efecb)
#define     PWMA_CCER1              (*(unsigned char volatile far *)0x7efecc)
#define     PWMA_CCER2              (*(unsigned char volatile far *)0x7efecd)
#define     PWMA_CNTR               (*(unsigned int  volatile far *)0x7efece)
#define     PWMA_CNTRH              (*(unsigned char volatile far *)0x7efece)
#define     PWMA_CNTRL              (*(unsigned char volatile far *)0x7efecf)
#define     PWMA_PSCR               (*(unsigned int  volatile far *)0x7efed0)
#define     PWMA_PSCRH              (*(unsigned char volatile far *)0x7efed0)
#define     PWMA_PSCRL              (*(unsigned char volatile far *)0x7efed1)
#define     PWMA_ARR                (*(unsigned int  volatile far *)0x7efed2)
#define     PWMA_ARRH               (*(unsigned char volatile far *)0x7efed2)
#define     PWMA_ARRL               (*(unsigned char volatile far *)0x7efed3)
#define     PWMA_RCR                (*(unsigned char volatile far *)0x7efed4)
#define     PWMA_CCR1               (*(unsigned int  volatile far *)0x7efed5)
#define     PWMA_CCR1H              (*(unsigned char volatile far *)0x7efed5)
#define     PWMA_CCR1L              (*(unsigned char volatile far *)0x7efed6)
#define     PWMA_CCR2               (*(unsigned int  volatile far *)0x7efed7)
#define     PWMA_CCR2H              (*(unsigned char volatile far *)0x7efed7)
#define     PWMA_CCR2L              (*(unsigned char volatile far *)0x7efed8)
#define     PWMA_CCR3               (*(unsigned int  volatile far *)0x7efed9)
#define     PWMA_CCR3H              (*(unsigned char volatile far *)0x7efed9)
#define     PWMA_CCR3L              (*(unsigned char volatile far *)0x7efeda)
#define     PWMA_CCR4               (*(unsigned int  volatile far *)0x7efedb)
#define     PWMA_CCR4H              (*(unsigned char volatile far *)0x7efedb)
#define     PWMA_CCR4L              (*(unsigned char volatile far *)0x7efedc)
#define     PWMA_BKR                (*(unsigned char volatile far *)0x7efedd)
#define     PWMA_DTR                (*(unsigned char volatile far *)0x7efede)
#define     PWMA_OISR               (*(unsigned char volatile far *)0x7efedf)
#define     PWMB_CR1                (*(unsigned char volatile far *)0x7efee0)
#define     PWMB_CR2                (*(unsigned char volatile far *)0x7efee1)
#define     PWMB_SMCR               (*(unsigned char volatile far *)0x7efee2)
#define     PWMB_ETR                (*(unsigned char volatile far *)0x7efee3)
#define     PWMB_IER                (*(unsigned char volatile far *)0x7efee4)
#define     PWMB_SR1                (*(unsigned char volatile far *)0x7efee5)
#define     PWMB_SR2                (*(unsigned char volatile far *)0x7efee6)
#define     PWMB_EGR                (*(unsigned char volatile far *)0x7efee7)
#define     PWMB_CCMR1              (*(unsigned char volatile far *)0x7efee8)
#define     PWMB_CCMR2              (*(unsigned char volatile far *)0x7efee9)
#define     PWMB_CCMR3              (*(unsigned char volatile far *)0x7efeea)
#define     PWMB_CCMR4              (*(unsigned char volatile far *)0x7efeeb)
#define     PWMB_CCER1              (*(unsigned char volatile far *)0x7efeec)
#define     PWMB_CCER2              (*(unsigned char volatile far *)0x7efeed)
#define     PWMB_CNTR               (*(unsigned int  volatile far *)0x7efeee)
#define     PWMB_CNTRH              (*(unsigned char volatile far *)0x7efeee)
#define     PWMB_CNTRL              (*(unsigned char volatile far *)0x7efeef)
#define     PWMB_PSCR               (*(unsigned int  volatile far *)0x7efef0)
#define     PWMB_PSCRH              (*(unsigned char volatile far *)0x7efef0)
#define     PWMB_PSCRL              (*(unsigned char volatile far *)0x7efef1)
#define     PWMB_ARR                (*(unsigned int  volatile far *)0x7efef2)
#define     PWMB_ARRH               (*(unsigned char volatile far *)0x7efef2)
#define     PWMB_ARRL               (*(unsigned char volatile far *)0x7efef3)
#define     PWMB_RCR                (*(unsigned char volatile far *)0x7efef4)
#define     PWMB_CCR1               (*(unsigned int  volatile far *)0x7efef5)
#define     PWMB_CCR1H              (*(unsigned char volatile far *)0x7efef5)
#define     PWMB_CCR1L              (*(unsigned char volatile far *)0x7efef6)
#define     PWMB_CCR2               (*(unsigned int  volatile far *)0x7efef7)
#define     PWMB_CCR2H              (*(unsigned char volatile far *)0x7efef7)
#define     PWMB_CCR2L              (*(unsigned char volatile far *)0x7efef8)
#define     PWMB_CCR3               (*(unsigned int  volatile far *)0x7efef9)
#define     PWMB_CCR3H              (*(unsigned char volatile far *)0x7efef9)
#define     PWMB_CCR3L              (*(unsigned char volatile far *)0x7efefa)
#define     PWMB_CCR4               (*(unsigned int  volatile far *)0x7efefb)
#define     PWMB_CCR4H              (*(unsigned char volatile far *)0x7efefb)
#define     PWMB_CCR4L              (*(unsigned char volatile far *)0x7efefc)
#define     PWMB_BKR                (*(unsigned char volatile far *)0x7efefd)
#define     PWMB_DTR                (*(unsigned char volatile far *)0x7efefe)
#define     PWMB_OISR               (*(unsigned char volatile far *)0x7efeff)

typedef struct TAG_PWM_STRUCT
{
    unsigned char CR1;
    unsigned char CR2;
    unsigned char SMCR;
    unsigned char ETR;
    unsigned char IER;
    unsigned char SR1;
    unsigned char SR2;
    unsigned char EGR;
    unsigned char CCMR1;
    unsigned char CCMR2;
    unsigned char CCMR3;
    unsigned char CCMR4;
    unsigned char CCER1;
    unsigned char CCER2;
    unsigned int CNTR;
    unsigned int PSCR;
    unsigned int ARR;
    unsigned char RCR;
    unsigned int CCR1;
    unsigned int CCR2;
    unsigned int CCR3;
    unsigned int CCR4;
    unsigned char BKR;
    unsigned char DTR;
    unsigned char OISR;
} PWM_STRUCT;

#define     PWMA                    ((PWM_STRUCT volatile far *)0x7efec0)
#define     PWMB                    ((PWM_STRUCT volatile far *)0x7efee0)

/////////////////////////////////////////////////
//7E:FD00H-7E:FDFFH
/////////////////////////////////////////////////
#define     PWM2_OISR               (*(unsigned char volatile far *)0x7efeff)

#define     P0INTE                  (*(unsigned char volatile far *)0x7efd00)
#define     P1INTE                  (*(unsigned char volatile far *)0x7efd01)
#define     P2INTE                  (*(unsigned char volatile far *)0x7efd02)
#define     P3INTE                  (*(unsigned char volatile far *)0x7efd03)
#define     P4INTE                  (*(unsigned char volatile far *)0x7efd04)
#define     P5INTE                  (*(unsigned char volatile far *)0x7efd05)
#define     P6INTE                  (*(unsigned char volatile far *)0x7efd06)
#define     P7INTE                  (*(unsigned char volatile far *)0x7efd07)
#define     P0INTF                  (*(unsigned char volatile far *)0x7efd10)
#define     P1INTF                  (*(unsigned char volatile far *)0x7efd11)
#define     P2INTF                  (*(unsigned char volatile far *)0x7efd12)
#define     P3INTF                  (*(unsigned char volatile far *)0x7efd13)
#define     P4INTF                  (*(unsigned char volatile far *)0x7efd14)
#define     P5INTF                  (*(unsigned char volatile far *)0x7efd15)
#define     P6INTF                  (*(unsigned char volatile far *)0x7efd16)
#define     P7INTF                  (*(unsigned char volatile far *)0x7efd17)
#define     P0IM0                   (*(unsigned char volatile far *)0x7efd20)
#define     P1IM0                   (*(unsigned char volatile far *)0x7efd21)
#define     P2IM0                   (*(unsigned char volatile far *)0x7efd22)
#define     P3IM0                   (*(unsigned char volatile far *)0x7efd23)
#define     P4IM0                   (*(unsigned char volatile far *)0x7efd24)
#define     P5IM0                   (*(unsigned char volatile far *)0x7efd25)
#define     P6IM0                   (*(unsigned char volatile far *)0x7efd26)
#define     P7IM0                   (*(unsigned char volatile far *)0x7efd27)
#define     P0IM1                   (*(unsigned char volatile far *)0x7efd30)
#define     P1IM1                   (*(unsigned char volatile far *)0x7efd31)
#define     P2IM1                   (*(unsigned char volatile far *)0x7efd32)
#define     P3IM1                   (*(unsigned char volatile far *)0x7efd33)
#define     P4IM1                   (*(unsigned char volatile far *)0x7efd34)
#define     P5IM1                   (*(unsigned char volatile far *)0x7efd35)
#define     P6IM1                   (*(unsigned char volatile far *)0x7efd36)
#define     P7IM1                   (*(unsigned char volatile far *)0x7efd37)
#define     P0WKUE                  (*(unsigned char volatile far *)0x7efd40)
#define     P1WKUE                  (*(unsigned char volatile far *)0x7efd41)
#define     P2WKUE                  (*(unsigned char volatile far *)0x7efd42)
#define     P3WKUE                  (*(unsigned char volatile far *)0x7efd43)
#define     P4WKUE                  (*(unsigned char volatile far *)0x7efd44)
#define     P5WKUE                  (*(unsigned char volatile far *)0x7efd45)
#define     P6WKUE                  (*(unsigned char volatile far *)0x7efd46)
#define     P7WKUE                  (*(unsigned char volatile far *)0x7efd47)

#define     PIN_IP                  (*(unsigned char volatile far *)0x7efd60)
#define     PIN_IPH                 (*(unsigned char volatile far *)0x7efd61)

#define     S2CFG                   (*(unsigned char volatile far *)0x7efdb4)
#define     S2ADDR                  (*(unsigned char volatile far *)0x7efdb5)
#define     S2ADEN                  (*(unsigned char volatile far *)0x7efdb6)
#define     USARTCR1                (*(unsigned char volatile far *)0x7efdc0)
#define     USARTCR2                (*(unsigned char volatile far *)0x7efdc1)
#define     USARTCR3                (*(unsigned char volatile far *)0x7efdc2)
#define     USARTCR4                (*(unsigned char volatile far *)0x7efdc3)
#define     USARTCR5                (*(unsigned char volatile far *)0x7efdc4)
#define     USARTGTR                (*(unsigned char volatile far *)0x7efdc5)
#define     USARTBR1                (*(unsigned char volatile far *)0x7efdc6)
#define     USARTBR2                (*(unsigned char volatile far *)0x7efdc7)
#define     USART2CR1               (*(unsigned char volatile far *)0x7efdc8)
#define     USART2CR2               (*(unsigned char volatile far *)0x7efdc9)
#define     USART2CR3               (*(unsigned char volatile far *)0x7efdca)
#define     USART2CR4               (*(unsigned char volatile far *)0x7efdcb)
#define     USART2CR5               (*(unsigned char volatile far *)0x7efdcc)
#define     USART2GTR               (*(unsigned char volatile far *)0x7efdcd)
#define     USART2BR1               (*(unsigned char volatile far *)0x7efdce)
#define     USART2BR2               (*(unsigned char volatile far *)0x7efdcf)

#define     CHIPID                  ( (unsigned char volatile far *)0x7efde0)

#define     CHIPID0                 (*(unsigned char volatile far *)0x7efde0)
#define     CHIPID1                 (*(unsigned char volatile far *)0x7efde1)
#define     CHIPID2                 (*(unsigned char volatile far *)0x7efde2)
#define     CHIPID3                 (*(unsigned char volatile far *)0x7efde3)
#define     CHIPID4                 (*(unsigned char volatile far *)0x7efde4)
#define     CHIPID5                 (*(unsigned char volatile far *)0x7efde5)
#define     CHIPID6                 (*(unsigned char volatile far *)0x7efde6)
#define     CHIPID7                 (*(unsigned char volatile far *)0x7efde7)
#define     CHIPID8                 (*(unsigned char volatile far *)0x7efde8)
#define     CHIPID9                 (*(unsigned char volatile far *)0x7efde9)
#define     CHIPID10                (*(unsigned char volatile far *)0x7efdea)
#define     CHIPID11                (*(unsigned char volatile far *)0x7efdeb)
#define     CHIPID12                (*(unsigned char volatile far *)0x7efdec)
#define     CHIPID13                (*(unsigned char volatile far *)0x7efded)
#define     CHIPID14                (*(unsigned char volatile far *)0x7efdee)
#define     CHIPID15                (*(unsigned char volatile far *)0x7efdef)
#define     CHIPID16                (*(unsigned char volatile far *)0x7efdf0)
#define     CHIPID17                (*(unsigned char volatile far *)0x7efdf1)
#define     CHIPID18                (*(unsigned char volatile far *)0x7efdf2)
#define     CHIPID19                (*(unsigned char volatile far *)0x7efdf3)
#define     CHIPID20                (*(unsigned char volatile far *)0x7efdf4)
#define     CHIPID21                (*(unsigned char volatile far *)0x7efdf5)
#define     CHIPID22                (*(unsigned char volatile far *)0x7efdf6)
#define     CHIPID23                (*(unsigned char volatile far *)0x7efdf7)
#define     CHIPID24                (*(unsigned char volatile far *)0x7efdf8)
#define     CHIPID25                (*(unsigned char volatile far *)0x7efdf9)
#define     CHIPID26                (*(unsigned char volatile far *)0x7efdfa)
#define     CHIPID27                (*(unsigned char volatile far *)0x7efdfb)
#define     CHIPID28                (*(unsigned char volatile far *)0x7efdfc)
#define     CHIPID29                (*(unsigned char volatile far *)0x7efdfd)
#define     CHIPID30                (*(unsigned char volatile far *)0x7efdfe)
#define     CHIPID31                (*(unsigned char volatile far *)0x7efdff)

/////////////////////////////////////////////////
//7E:FC00H-7E:FCFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//7E:FB00H-7E:FBFFH
/////////////////////////////////////////////////

#define     HSPWMA_CFG              (*(unsigned char volatile far *)0x7efbf0)
#define     HSPWMA_ADR              (*(unsigned char volatile far *)0x7efbf1)
#define     HSPWMA_DAT              (*(unsigned char volatile far *)0x7efbf2)

#define     HSPWMB_CFG              (*(unsigned char volatile far *)0x7efbf4)
#define     HSPWMB_ADR              (*(unsigned char volatile far *)0x7efbf5)
#define     HSPWMB_DAT              (*(unsigned char volatile far *)0x7efbf6)

#define     HSSPI_CFG               (*(unsigned char volatile far *)0x7efbf8)
#define     HSSPI_CFG2              (*(unsigned char volatile far *)0x7efbf9)
#define     HSSPI_STA               (*(unsigned char volatile far *)0x7efbfa)


/////////////////////////////////////////////////
//7E:FA00H-7E:FAFFH
/////////////////////////////////////////////////

#define     DMA_M2M_CFG             (*(unsigned char volatile far *)0x7efa00)
#define     DMA_M2M_CR              (*(unsigned char volatile far *)0x7efa01)
#define     DMA_M2M_STA             (*(unsigned char volatile far *)0x7efa02)
#define     DMA_M2M_AMT             (*(unsigned char volatile far *)0x7efa03)
#define     DMA_M2M_DONE            (*(unsigned char volatile far *)0x7efa04)
#define     DMA_M2M_TXAH            (*(unsigned char volatile far *)0x7efa05)
#define     DMA_M2M_TXAL            (*(unsigned char volatile far *)0x7efa06)
#define     DMA_M2M_RXAH            (*(unsigned char volatile far *)0x7efa07)
#define     DMA_M2M_RXAL            (*(unsigned char volatile far *)0x7efa08)

#define     DMA_ADC_CFG             (*(unsigned char volatile far *)0x7efa10)
#define     DMA_ADC_CR              (*(unsigned char volatile far *)0x7efa11)
#define     DMA_ADC_STA             (*(unsigned char volatile far *)0x7efa12)
#define     DMA_ADC_RXAH            (*(unsigned char volatile far *)0x7efa17)
#define     DMA_ADC_RXAL            (*(unsigned char volatile far *)0x7efa18)
#define     DMA_ADC_CFG2            (*(unsigned char volatile far *)0x7efa19)
#define     DMA_ADC_CHSW0           (*(unsigned char volatile far *)0x7efa1a)
#define     DMA_ADC_CHSW1           (*(unsigned char volatile far *)0x7efa1b)

#define     DMA_SPI_CFG             (*(unsigned char volatile far *)0x7efa20)
#define     DMA_SPI_CR              (*(unsigned char volatile far *)0x7efa21)
#define     DMA_SPI_STA             (*(unsigned char volatile far *)0x7efa22)
#define     DMA_SPI_AMT             (*(unsigned char volatile far *)0x7efa23)
#define     DMA_SPI_DONE            (*(unsigned char volatile far *)0x7efa24)
#define     DMA_SPI_TXAH            (*(unsigned char volatile far *)0x7efa25)
#define     DMA_SPI_TXAL            (*(unsigned char volatile far *)0x7efa26)
#define     DMA_SPI_RXAH            (*(unsigned char volatile far *)0x7efa27)
#define     DMA_SPI_RXAL            (*(unsigned char volatile far *)0x7efa28)
#define     DMA_SPI_CFG2            (*(unsigned char volatile far *)0x7efa29)

#define     DMA_UR1T_CFG            (*(unsigned char volatile far *)0x7efa30)
#define     DMA_UR1T_CR             (*(unsigned char volatile far *)0x7efa31)
#define     DMA_UR1T_STA            (*(unsigned char volatile far *)0x7efa32)
#define     DMA_UR1T_AMT            (*(unsigned char volatile far *)0x7efa33)
#define     DMA_UR1T_DONE           (*(unsigned char volatile far *)0x7efa34)
#define     DMA_UR1T_TXAH           (*(unsigned char volatile far *)0x7efa35)
#define     DMA_UR1T_TXAL           (*(unsigned char volatile far *)0x7efa36)
#define     DMA_UR1R_CFG            (*(unsigned char volatile far *)0x7efa38)
#define     DMA_UR1R_CR             (*(unsigned char volatile far *)0x7efa39)
#define     DMA_UR1R_STA            (*(unsigned char volatile far *)0x7efa3a)
#define     DMA_UR1R_AMT            (*(unsigned char volatile far *)0x7efa3b)
#define     DMA_UR1R_DONE           (*(unsigned char volatile far *)0x7efa3c)
#define     DMA_UR1R_RXAH           (*(unsigned char volatile far *)0x7efa3d)
#define     DMA_UR1R_RXAL           (*(unsigned char volatile far *)0x7efa3e)

#define     DMA_UR2T_CFG            (*(unsigned char volatile far *)0x7efa40)
#define     DMA_UR2T_CR             (*(unsigned char volatile far *)0x7efa41)
#define     DMA_UR2T_STA            (*(unsigned char volatile far *)0x7efa42)
#define     DMA_UR2T_AMT            (*(unsigned char volatile far *)0x7efa43)
#define     DMA_UR2T_DONE           (*(unsigned char volatile far *)0x7efa44)
#define     DMA_UR2T_TXAH           (*(unsigned char volatile far *)0x7efa45)
#define     DMA_UR2T_TXAL           (*(unsigned char volatile far *)0x7efa46)
#define     DMA_UR2R_CFG            (*(unsigned char volatile far *)0x7efa48)
#define     DMA_UR2R_CR             (*(unsigned char volatile far *)0x7efa49)
#define     DMA_UR2R_STA            (*(unsigned char volatile far *)0x7efa4a)
#define     DMA_UR2R_AMT            (*(unsigned char volatile far *)0x7efa4b)
#define     DMA_UR2R_DONE           (*(unsigned char volatile far *)0x7efa4c)
#define     DMA_UR2R_RXAH           (*(unsigned char volatile far *)0x7efa4d)
#define     DMA_UR2R_RXAL           (*(unsigned char volatile far *)0x7efa4e)

#define     DMA_UR3T_CFG            (*(unsigned char volatile far *)0x7efa50)
#define     DMA_UR3T_CR             (*(unsigned char volatile far *)0x7efa51)
#define     DMA_UR3T_STA            (*(unsigned char volatile far *)0x7efa52)
#define     DMA_UR3T_AMT            (*(unsigned char volatile far *)0x7efa53)
#define     DMA_UR3T_DONE           (*(unsigned char volatile far *)0x7efa54)
#define     DMA_UR3T_TXAH           (*(unsigned char volatile far *)0x7efa55)
#define     DMA_UR3T_TXAL           (*(unsigned char volatile far *)0x7efa56)
#define     DMA_UR3R_CFG            (*(unsigned char volatile far *)0x7efa58)
#define     DMA_UR3R_CR             (*(unsigned char volatile far *)0x7efa59)
#define     DMA_UR3R_STA            (*(unsigned char volatile far *)0x7efa5a)
#define     DMA_UR3R_AMT            (*(unsigned char volatile far *)0x7efa5b)
#define     DMA_UR3R_DONE           (*(unsigned char volatile far *)0x7efa5c)
#define     DMA_UR3R_RXAH           (*(unsigned char volatile far *)0x7efa5d)
#define     DMA_UR3R_RXAL           (*(unsigned char volatile far *)0x7efa5e)

#define     DMA_UR4T_CFG            (*(unsigned char volatile far *)0x7efa60)
#define     DMA_UR4T_CR             (*(unsigned char volatile far *)0x7efa61)
#define     DMA_UR4T_STA            (*(unsigned char volatile far *)0x7efa62)
#define     DMA_UR4T_AMT            (*(unsigned char volatile far *)0x7efa63)
#define     DMA_UR4T_DONE           (*(unsigned char volatile far *)0x7efa64)
#define     DMA_UR4T_TXAH           (*(unsigned char volatile far *)0x7efa65)
#define     DMA_UR4T_TXAL           (*(unsigned char volatile far *)0x7efa66)
#define     DMA_UR4R_CFG            (*(unsigned char volatile far *)0x7efa68)
#define     DMA_UR4R_CR             (*(unsigned char volatile far *)0x7efa69)
#define     DMA_UR4R_STA            (*(unsigned char volatile far *)0x7efa6a)
#define     DMA_UR4R_AMT            (*(unsigned char volatile far *)0x7efa6b)
#define     DMA_UR4R_DONE           (*(unsigned char volatile far *)0x7efa6c)
#define     DMA_UR4R_RXAH           (*(unsigned char volatile far *)0x7efa6d)
#define     DMA_UR4R_RXAL           (*(unsigned char volatile far *)0x7efa6e)

#define     DMA_LCM_CFG             (*(unsigned char volatile far *)0x7efa70)
#define     DMA_LCM_CR              (*(unsigned char volatile far *)0x7efa71)
#define     DMA_LCM_STA             (*(unsigned char volatile far *)0x7efa72)
#define     DMA_LCM_AMT             (*(unsigned char volatile far *)0x7efa73)
#define     DMA_LCM_DONE            (*(unsigned char volatile far *)0x7efa74)
#define     DMA_LCM_TXAH            (*(unsigned char volatile far *)0x7efa75)
#define     DMA_LCM_TXAL            (*(unsigned char volatile far *)0x7efa76)
#define     DMA_LCM_RXAH            (*(unsigned char volatile far *)0x7efa77)
#define     DMA_LCM_RXAL            (*(unsigned char volatile far *)0x7efa78)

#define     DMA_M2M_AMTH            (*(unsigned char volatile far *)0x7efa80)
#define     DMA_M2M_DONEH           (*(unsigned char volatile far *)0x7efa81)
#define     DMA_SPI_AMTH            (*(unsigned char volatile far *)0x7efa84)
#define     DMA_SPI_DONEH           (*(unsigned char volatile far *)0x7efa85)
#define     DMA_LCM_AMTH            (*(unsigned char volatile far *)0x7efa86)
#define     DMA_LCM_DONEH           (*(unsigned char volatile far *)0x7efa87)
#define     DMA_UR1T_AMTH           (*(unsigned char volatile far *)0x7efa88)
#define     DMA_UR1T_DONEH          (*(unsigned char volatile far *)0x7efa89)
#define     DMA_UR1R_AMTH           (*(unsigned char volatile far *)0x7efa8a)
#define     DMA_UR1R_DONEH          (*(unsigned char volatile far *)0x7efa8b)
#define     DMA_UR2T_AMTH           (*(unsigned char volatile far *)0x7efa8c)
#define     DMA_UR2T_DONEH          (*(unsigned char volatile far *)0x7efa8d)
#define     DMA_UR2R_AMTH           (*(unsigned char volatile far *)0x7efa8e)
#define     DMA_UR2R_DONEH          (*(unsigned char volatile far *)0x7efa8f)
#define     DMA_UR3T_AMTH           (*(unsigned char volatile far *)0x7efa90)
#define     DMA_UR3T_DONEH          (*(unsigned char volatile far *)0x7efa91)
#define     DMA_UR3R_AMTH           (*(unsigned char volatile far *)0x7efa92)
#define     DMA_UR3R_DONEH          (*(unsigned char volatile far *)0x7efa93)
#define     DMA_UR4T_AMTH           (*(unsigned char volatile far *)0x7efa94)
#define     DMA_UR4T_DONEH          (*(unsigned char volatile far *)0x7efa95)
#define     DMA_UR4R_AMTH           (*(unsigned char volatile far *)0x7efa96)
#define     DMA_UR4R_DONEH          (*(unsigned char volatile far *)0x7efa97)

#define     DMA_I2CT_CFG            (*(unsigned char volatile far *)0x7efa98)
#define     DMA_I2CT_CR             (*(unsigned char volatile far *)0x7efa99)
#define     DMA_I2CT_STA            (*(unsigned char volatile far *)0x7efa9a)
#define     DMA_I2CT_AMT            (*(unsigned char volatile far *)0x7efa9b)
#define     DMA_I2CT_DONE           (*(unsigned char volatile far *)0x7efa9c)
#define     DMA_I2CT_TXAH           (*(unsigned char volatile far *)0x7efa9d)
#define     DMA_I2CT_TXAL           (*(unsigned char volatile far *)0x7efa9e)
#define     DMA_I2CR_CFG            (*(unsigned char volatile far *)0x7efaa0)
#define     DMA_I2CR_CR             (*(unsigned char volatile far *)0x7efaa1)
#define     DMA_I2CR_STA            (*(unsigned char volatile far *)0x7efaa2)
#define     DMA_I2CR_AMT            (*(unsigned char volatile far *)0x7efaa3)
#define     DMA_I2CR_DONE           (*(unsigned char volatile far *)0x7efaa4)
#define     DMA_I2CR_RXAH           (*(unsigned char volatile far *)0x7efaa5)
#define     DMA_I2CR_RXAL           (*(unsigned char volatile far *)0x7efaa6)

#define     DMA_I2CT_AMTH           (*(unsigned char volatile far *)0x7efaa8)
#define     DMA_I2CT_DONEH          (*(unsigned char volatile far *)0x7efaa9)
#define     DMA_I2CR_AMTH           (*(unsigned char volatile far *)0x7efaaa)
#define     DMA_I2CR_DONEH          (*(unsigned char volatile far *)0x7efaab)

#define     DMA_I2C_CR              (*(unsigned char volatile far *)0x7efaad)
#define     DMA_I2C_ST1             (*(unsigned char volatile far *)0x7efaae)
#define     DMA_I2C_ST2             (*(unsigned char volatile far *)0x7efaaf)


/////////////////////////////////////////////////

#define     INT0_VECTOR             0       //0003H
#define     TMR0_VECTOR             1       //000BH
#define     INT1_VECTOR             2       //0013H
#define     TMR1_VECTOR             3       //001BH
#define     UART1_VECTOR            4       //0023H
#define     ADC_VECTOR              5       //002BH
#define     LVD_VECTOR              6       //0033H
#define     PCA_VECTOR              7       //003BH
#define     UART2_VECTOR            8       //0043H
#define     SPI_VECTOR              9       //004BH
#define     INT2_VECTOR             10      //0053H
#define     INT3_VECTOR             11      //005BH
#define     TMR2_VECTOR             12      //0063H
#define     USER_VECTOR             13      //006BH
#define     BRK_VECTOR              14      //0073H
#define     EXBRK_VECTOR            15      //007BH
#define     INT4_VECTOR             16      //0083H
#define     UART3_VECTOR            17      //008BH
#define     UART4_VECTOR            18      //0093H
#define     TMR3_VECTOR             19      //009BH
#define     TMR4_VECTOR             20      //00A3H
#define     CMP_VECTOR              21      //00ABH
#define     PWM_VECTOR              22      //00B3H
#define     PWMFD_VECTOR            23      //00BBH
#define     I2C_VECTOR              24      //00C3H
#define     P0INT_VECTOR            37      //012BH
#define     P1INT_VECTOR            38      //0133H
#define     P2INT_VECTOR            39      //013BH
#define     P3INT_VECTOR            40      //0143H
#define     P4INT_VECTOR            41      //014BH
#define     P5INT_VECTOR            42      //0153H
#define     P6INT_VECTOR            43      //015BH
#define     P7INT_VECTOR            44      //0163H
#define     M2MDMA_VECTOR           47      //017BH
#define     ADCDMA_VECTOR           48      //0183H
#define     SPIDMA_VECTOR           49      //018BH
#define     U1TXDMA_VECTOR          50      //0193H
#define     U1RXDMA_VECTOR          51      //019BH
#define     U2TXDMA_VECTOR          52      //01A3H
#define     U2RXDMA_VECTOR          53      //01ABH
#define     U3TXDMA_VECTOR          54      //01B3H
#define     U3RXDMA_VECTOR          55      //01BBH
#define     U4TXDMA_VECTOR          56      //01C3H
#define     U4RXDMA_VECTOR          57      //01CBH
#define     LCMDMA_VECTOR           58      //01D3H
#define     LCM_VECTOR              59      //01DBH
#define     I2CTXDMA_VECTOR         60      //01E3H
#define     I2CRXDMA_VECTOR         61      //01EBH

/////////////////////////////////////////////////

#endif

