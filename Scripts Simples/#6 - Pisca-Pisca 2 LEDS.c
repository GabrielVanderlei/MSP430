
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 
  P1DIR |= BIT0;                           
  P3DIR |= BIT6;
  
  while(1){
      P1OUT ^= BIT0;
      __delay_cycles(60000);
      P3OUT ^= BIT6;
      __delay_cycles(60000);
  }
}
