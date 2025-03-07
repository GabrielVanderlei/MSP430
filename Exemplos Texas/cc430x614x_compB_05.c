/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
// CC430x614x Demo - COMPB Hysteresis, CBOUT Toggle in LPM4; High speed mode
//
// Description: Use CompB and shared reference to determine if input 'Vcompare'
//    is high of low.  Shared reference is configured to generate hysteresis.
//	  When Vcompare exceeds Vcc*3/4 CBOUT goes high and when Vcompare is less 
//    than Vcc*1/4 then CBOUT goes low.
//    Connect P1.6/CBOUT to P1.0 externally to see the LED toggle accordingly.
//
//                 CC430x614x
//             ------------------                        
//         /|\|                  |                       
//          | |                  |                       
//          --|RST      P2.0/CB0 |<--Vcompare            
//            |                  |                       
//            |        P1.6/CBOUT|----> 'high'(Vcompare>Vcc*3/4); 'low'(Vcompare<Vcc*1/4)
//            |                  |  |
//            |            P1.0  |__| LED 'ON'(Vcompare>Vcc*3/4); 'OFF'(Vcompare<Vcc*1/4)
//            |                  | 
//                                                       
// 	 G. Larmore 
//   Texas Instruments Inc.
//   June 2012
//   Built with CCS v5.2 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;     // Stop WDT                     
 
  P1DIR |= BIT6;                            // P1.6 output direction  
  P1SEL |= BIT6;                            // Select CBOUT function on P1.6/CBOUT
 
  PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs  
  P1MAP6 = PM_CBOUT0;                       // Map CBOUT output to P1.6 
  PMAPPWD = 0;                              // Lock port mapping registers 
  
  P2SEL |= BIT0;                            // Select CB0 input for P2.0 

  // Setup ComparatorB                                                                                           
  CBCTL0 |= CBIPEN + CBIPSEL_0; // Enable V+, input channel CB0              
  CBCTL1 |= CBPWRMD_0;          // CBMRVS=0 => select VREF1 as ref when CBOUT 
                                // is high and VREF0 when CBOUT is low  
                                // High-Speed Power mode        
  CBCTL2 |= CBRSEL;             // VRef is applied to -terminal  
  CBCTL2 |= CBRS_1+CBREF13;     // VREF1 is Vcc*1/4             
  CBCTL2 |= CBREF04+CBREF03;    // VREF0 is Vcc*3/4             
  CBCTL3 |= BIT0;               // Input Buffer Disable @P6.0/CB0    
  CBCTL1 |= CBON;               // Turn On ComparatorB           

  __bis_SR_register(LPM4_bits); // Enter LPM4
  __no_operation();             // For debug 
} 


