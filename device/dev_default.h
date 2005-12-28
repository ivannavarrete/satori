
#ifndef DEV_DEFAULT_H
#define DEV_DEFAULT_H


/* XXX: Check whether the values make sence. */
#define DEV_DEFAULT {\
	"default",\
	 0x60,						/* SRAM start */\
	 0x7F,						/* SRAM end */\
	 0x7F,						/* EEPROM end */\
	 0x7F,						/* FLASH end */\
	 0x20,						/* IO start */\
	 0x5F}						/* IO end */\
/*	 {{0x04, "ADC", "ADC Data Register", 2}}}*/
/*	  {"ADC0","ADC1","ADC2","ADC3","ADC4","ADC5","ADC6","ADC7",\
	   "ADC8","ADC9",NULL,NULL,NULL,NULL,NULL,NULL}},\
	  {0x00, NULL, NULL, 0, NULL}}\
}*/


#endif
