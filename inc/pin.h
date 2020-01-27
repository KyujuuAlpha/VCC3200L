//*****************************************************************************
//
//  pin.h
//
//  Defines and Macros for the pin mux module
//
//  Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

#ifndef __PIN_H__
#define __PIN_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
// Macros Defining Pins
//*****************************************************************************

#define PIN_01 	 0
#define PIN_02 	 1
#define PIN_03   2
#define PIN_04 	 3
#define PIN_05   4
#define PIN_06   5
#define PIN_07   6
#define PIN_08   7
#define PIN_11   8
#define PIN_12   9
#define PIN_13   10
#define PIN_14   11
#define PIN_15   12
#define PIN_16   13
#define PIN_17   14
#define PIN_18   15
#define PIN_19 	 16
#define PIN_20   17
#define PIN_21   18
#define PIN_45   19
#define PIN_46   20
#define PIN_47   21
#define PIN_48   22
#define PIN_49   23
#define PIN_50   24
#define PIN_52   25
#define PIN_53   26
#define PIN_55   27
#define PIN_56   28
#define PIN_57   29
#define PIN_58   30
#define PIN_59   31
#define PIN_60   32
#define PIN_61	 33
#define PIN_62	 34
#define PIN_63	 35
#define PIN_64	 36



//*****************************************************************************
// Macros that can be used with PinConfigSet(), PinTypeGet(), PinStrengthGet()
//*****************************************************************************

#define PIN_MODE_0	 0x00000000
#define PIN_MODE_1	 0x00000001
#define PIN_MODE_2	 0x00000002
#define PIN_MODE_3	 0x00000003
#define PIN_MODE_4	 0x00000004
#define PIN_MODE_5	 0x00000005
#define PIN_MODE_6	 0x00000006
#define PIN_MODE_7	 0x00000007
#define PIN_MODE_8	 0x00000008
#define PIN_MODE_9	 0x00000009
#define PIN_MODE_10	 0x0000000A
#define PIN_MODE_11	 0x0000000B
#define PIN_MODE_12	 0x0000000C
#define PIN_MODE_13	 0x0000000D
#define PIN_MODE_14	 0x0000000E
#define PIN_MODE_15	 0x0000000F
// Note : PIN_MODE_255 is a dummy define for pinmux utility code generation
// PIN_MODE_255 should never be used in any user code.  
#define PIN_MODE_255 0x000000FF  

//*****************************************************************************
// Macros that can be used with PinDirModeSet() and returned from
// PinDirModeGet().
//*****************************************************************************
#define PIN_DIR_MODE_IN  0x00000C00 // Pin is input
#define PIN_DIR_MODE_OUT 0x00000800 // Pin is output
#define PIN_DIR_MODE_HW  0x00000000 // Pin is peripheral function

//*****************************************************************************
// Macros that can be used with PinConfigSet()
//*****************************************************************************
#define PIN_STRENGTH_2MA  0x00000020
#define PIN_STRENGTH_4MA  0x00000040
#define PIN_STRENGTH_6MA  0x00000060

#define PIN_TYPE_STD      0x00000000
#define PIN_TYPE_STD_PU   0x00000100
#define PIN_TYPE_STD_PD   0x00000200

#define PIN_TYPE_OD       0x00000010
#define PIN_TYPE_OD_PU    0x00000110
#define PIN_TYPE_OD_PD    0x00000210
#define PIN_TYPE_ANALOG   0x10000000


//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern void PinModeSet(unsigned long ulPin, unsigned long ulPinMode);
extern void PinDirModeSet(unsigned long ulPin, unsigned long ulPinIO);
extern unsigned long PinDirModeGet(unsigned long ulPin);
extern unsigned long PinModeGet(unsigned long ulPin);
extern void PinConfigGet(unsigned long ulPin,unsigned long  *pulPinStrength,
	       					unsigned long *pulPinType);
extern void PinConfigSet(unsigned long ulPin,unsigned long  ulPinStrength,
						unsigned long ulPinType);
extern void PinTypeUART(unsigned long ulPin,unsigned long ulPinMode);
extern void PinTypeI2C(unsigned long ulPin,unsigned long ulPinMode);
extern void PinTypeSPI(unsigned long ulPin,unsigned long ulPinMode);
extern void PinTypeI2S(unsigned long ulPin,unsigned long ulPinMode);
extern void PinTypeTimer(unsigned long ulPin,unsigned long ulPinMode);
extern void PinTypeCamera(unsigned long ulPin,unsigned long ulPinMode);
extern void PinTypeGPIO(unsigned long ulPin,unsigned long ulPinMode,
                        tBoolean bOpenDrain);
extern void PinTypeADC(unsigned long ulPin,unsigned long ulPinMode);
extern void PinTypeSDHost(unsigned long ulPin,unsigned long ulPinMode);

void setPin(unsigned long ulPin, unsigned char val);
void setPinGPIO(unsigned long ulPort, unsigned char ucPin, unsigned char val);
void setPinSPI(unsigned long ulPin, unsigned char val);

unsigned char getPin(unsigned long ulPin);
unsigned long getPinMode(unsigned long ulPin);
unsigned char getPinGPIO(unsigned long ulPort, unsigned char ucPin);

#ifdef __cplusplus
}
#endif

#endif //__PIN_H__
