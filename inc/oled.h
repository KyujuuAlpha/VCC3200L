#ifndef OLED_H_
#define OLED_H_

#define DELAYS_HWFILL	    (3)
#define DELAYS_HWLINE       (1)

#define CMD_SETCOLUMN 		0x15
#define CMD_SETROW    		0x75
#define CMD_WRITERAM   		0x5C
#define CMD_READRAM   		0x5D
#define CMD_SETREMAP 		0xA0
#define CMD_STARTLINE 		0xA1
#define CMD_DISPLAYOFFSET 	0xA2
#define CMD_DISPLAYALLOFF 	0xA4
#define CMD_DISPLAYALLON  	0xA5
#define CMD_NORMALDISPLAY 	0xA6
#define CMD_INVERTDISPLAY 	0xA7
#define CMD_FUNCTIONSELECT 	0xAB
#define CMD_DISPLAYOFF 		0xAE
#define CMD_DISPLAYON     	0xAF
#define CMD_PRECHARGE 		0xB1
#define CMD_DISPLAYENHANCE	0xB2
#define CMD_CLOCKDIV 		0xB3
#define CMD_SETVSL 		0xB4
#define CMD_SETGPIO 		0xB5
#define CMD_PRECHARGE2 		0xB6
#define CMD_SETGRAY 		0xB8
#define CMD_USELUT 		0xB9
#define CMD_PRECHARGELEVEL 	0xBB
#define CMD_VCOMH 		0xBE
#define CMD_CONTRASTABC		0xC1
#define CMD_CONTRASTMASTER	0xC7
#define CMD_MUXRATIO            0xCA
#define CMD_COMMANDLOCK         0xFD
#define CMD_HORIZSCROLL         0x96
#define CMD_STOPSCROLL          0x9E
#define CMD_STARTSCROLL         0x9F

#endif