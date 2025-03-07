
/*
  MC_0 - Não conta
  MC_1 - Conta de 0 a TA1CCR0 e interrompe
  MC_2 - Conta de 0 a 65535
  MC_3 - Conta de 0 a TA1CCR0 e vai diminuindo até voltar a ser 0.
  Quando o valor chega a 0 o contador interrompe.

  ID_3 -> Divide a contagem por 8
  ID_2 -> Divide a contagem por 4
  ID_1 -> Divide a contagem por 2
  ID_0 -> Divide a contagem por 1
  
  TASSEL_3 - TACLK invertido
  TASSEL_2 - clock secundário
  TASSEL_1 - clock primário
  TASSEL_0 - TACLK (clock externo)


*/

#include <msp430.h>
#define intervalo 62500;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 
  P1DIR |= BIT0;                            
                      
  TA1CCR0 = intervalo;
  TA1CCTL0 = CCIE; // Ativa a interrupção
  
  // 1 Segundo
  // TASSEL_2 -> SMCLK = 1MHz
  // MC_3 = Ida e volta -> 2
  // ID_3 = Divide por 8 -> 1MHz/8 -> 125KHz (8us)
  // Valor = 62500
  // 2*62500*8us = 1s
  TA1CTL = TASSEL_2 + MC_3 + ID_3;        
  
  __bis_SR_register(LPM0_bits + GIE); // Habilita as interrupções      
  __no_operation(); // Debug
}

// O trecho a seguir indica ao MSP430 uma função para interrupção de código
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TemporizadorInterrompeu(void)
{
  P1OUT ^= BIT0;
}
