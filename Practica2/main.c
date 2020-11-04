// DSPIC33FJ32MC204 Configuration Bit Settings

// 'C' source line config statements

// FBS
#pragma config BWRP = WRPROTECT_OFF // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = PRIPLL // Oscillator Mode (Internal Fast RC (FRC) with divide by N)
#pragma config IESO = ON // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = EC // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = OFF // OSC2 Pin Function (OSC2 pin has clock out function)
#pragma config IOL1WAY = ON // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768 // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128 // WDT Prescaler (1:128)
#pragma config WINDIS = OFF // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = ON // Watchdog Timer Enable (Watchdog timer always enabled)

// FPOR
#pragma config FPWRT = PWR128 // POR Timer Value (128ms)
#pragma config ALTI2C = OFF // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
#pragma config LPOL = ON // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-high output polarity)
#pragma config HPOL = ON // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-high output polarity)
#pragma config PWMPIN = ON // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD1 // Comm Channel Select (Communicate on PGC2/EMUC2 and PGD2/EMUD2)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>         
#include <stdbool.h>        
#include <math.h>

#define baud_9600 51 // Para un valor de 2MHz

#define led_rojo LATBbits.LATB7 // inicializamos el led rojo en el pin RB4
#define P1_pressed   !PORTBbits.RB4
#define P2_pressed   !PORTBbits.RB7

void delay_ms(unsigned long time_ms) {
  unsigned long u;
  for (u = 0; u < time_ms * 90; u++) // Cálculo aproximado para una CPU a 2MHz
  {
    asm("NOP");
  }
}

                                              

void uart_config(unsigned int baud) {
  // Configuración de pines tx y rx
  TRISCbits.TRISC0 = 1; // Pin de recepcion de uart establecido como entrada.
  RPINR18bits.U1RXR = 16; // pin de recepcion rc0 trabajando con el modulo uart (RP16)
  RPOR8bits.RP17R = 3; // U1TX conectado con el pin RC1 (RP17)

  // Configuración de registro de U1MODE
  U1MODEbits.UARTEN = 0; // Deshabilitar Uart.
  U1MODEbits.USIDL = 0; // Continuar operación en modo IDLE
  U1MODEbits.IREN = 0; // IR no usado
  U1MODEbits.RTSMD = 1; // Control de flujo desactivado.
  U1MODEbits.UEN = 0; // Solo usamos pin de Tx y pin de Rx
  U1MODEbits.WAKE = 0; // No quiero que la UART despierte del modo sleep
  U1MODEbits.LPBACK = 0; // Loopback deshabilitado.
  U1MODEbits.ABAUD = 0; // Automedición de baudios (bps) deshabilidada
  U1MODEbits.URXINV = 0; // En estado de reposo, el receptor mantiene un estado alto, high
  U1MODEbits.BRGH = 1; // Modo High-Speed
  U1MODEbits.PDSEL = 0; // 8 Bits de datos y paridad Nula (8N)
  U1MODEbits.STSEL = 0; // 1-bit de stop al final de la trama de datos.   (8N1)

  // Configuración de registro de U1STA
  U1STAbits.UTXISEL0 = 0; // Tema interrupciones (no mirar aun)
  U1STAbits.UTXISEL1 = 0; // Tema interrupciones (no mirar aun)
  U1STAbits.UTXINV = 0; // El estado en reposo del pin de transmisión es High
  U1STAbits.UTXBRK = 0; // No usamos trama de sincronización
  U1STAbits.UTXEN = 1; // El transmisor a pleno funcionamiento.
  U1STAbits.URXISEL = 0; // Tema interrupciones (no mirar aun)
  U1STAbits.ADDEN = 0; // No usamos direccionamiento.
  //U1STAbits.RIDLE    = 0;
  U1STAbits.OERR = 0; // Reseteamos buffer de recepción

  // Configuramos la velocidad de transmisión/recepcción de los datos
  U1BRG = baud;

  U1MODEbits.UARTEN = 1; // Uart habilitada por completo
}

// Configuración timer 1
void timer1_config(void) {
  T1CONbits.TON = 0; //asegurarse de que está apagado
  T1CONbits.TSIDL = 0;//continuar trabajando en idle mode
  T1CONbits.TCS = 0;//frecuencia de reloj interna
  T1CONbits.TGATE = 0; //desabilitamos esta funcion
  T1CONbits.TSYNC = 0;//no importa
  T1CONbits.TCKPS = 3;//el más alto (256)

  PR1 = 0xFFFF;//para que no se resetee

  T1CONbits.TON = 1;//activamos el timer
}

// Configuración timer 2 - apartado 3??
void timer2_config(void) {
  T1CONbits.TON = 0;
  T1CONbits.TSIDL = 0;
  T1CONbits.TCS = 0;
  T1CONbits.TGATE = 0;
  T1CONbits.TSYNC = 0;
  T1CONbits.TCKPS = 3;

  PR1 = 0xFFFF;

  T1CONbits.TON = 1;
}

char txbuffer[200];

int main(void) {
  //Configurar el oscilador para hacer funcionar la CPU a 2 MHz a partir de un reloj de entrada de 8MHz
  //Fosc = Fin * M/(N1 * N2), Fcy = Fosc/2
  //Fosc = 8M * 2/(2 * 2) = 4 MHz para un reloj de 8MHz de entrada
  //Fcy = Fosc/2 = 4/2 = 2MHz (Frecuencia CPU)
  PLLFBD = 0; // M  = 2
  CLKDIVbits.PLLPOST = 0; // N1 = 2
  CLKDIVbits.PLLPRE = 0; // N2 = 2
  while (OSCCONbits.LOCK != 1); // Esperar a un PLL estable      

  AD1PCFGL = 0xFFFF; // Primer paso. Todos los pines configurados como pines digitales
  TRISAbits.TRISA0 = 0;
  TRISBbits.TRISB3 = 0;
  LATAbits.LATA0 = 0;
  LATBbits.LATB3 = 0;
  LATBbits.LATB4 = 0;
 
  TRISBbits.TRISB7 = 0; //configuramos RB3 como salida (led rojo)
  LATBbits.LATB4 = 1; //configuramos RB4 como entrada (botón 1)
  LATBbits.LATB7 = 1; //configuramos RB7 como entrada (botón 2)
  
  uart_config(baud_9600);
  timer1_config();
  /* Bucle infinito apartado 1
  while (1) {
    
       TMR1 = 0; //inicializamos
       while (TMR1 < 7812)  //a partir de la formula counter time (1 segundo) = prescaler(256)*(TMRx + 1)/Fclk_timer(hz) (2Mhz)
       LATAbits.LATA1 = !PORTAbits.RA1;
  } */
  
  while (1){
     
    
     while (TMR1  < 15624){ //para que esto salga cada 2 segundos 
      if (P1_pressed) {//si el registro del primer boton está a 1 
        printf("P1: Pulsado\r\n);        
        
      } else {//si está a 0 
       printf("P1: NO Pulsado\r\n); 
      }

      if (P2_pressed) {//si el registro del segundo boton está a 1 
        printf("P2: Pulsado\r\n);  
      } else {//si está a 0 
        printf("P2: NO Pulsado\r\n); 
      }
      
    }
       while (TMR1 < 7812){//para que esto salga cada segundo
          led_rojo = !led_rojo;  //led rojo cambia su estado cada segundo para parpadear 
       }  
   }
  
  }
}

  /* while (!IFS0bits.T1IF)//se hará cuando este registro valga 0
      IFS0bits.T1IF = 0;//porque se tiene que poner el registro manualmente a 0
      LATBbits.LATB1 = !PORTBbits.RB1;   

*/
  
