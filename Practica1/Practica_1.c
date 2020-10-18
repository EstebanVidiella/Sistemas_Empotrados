
// DSPIC33EP32MC204 Configuration Bit Settings

// 'C' source line config statements

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (Watchdog timer always enabled)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FOSCSEL
#pragma config FNOSC = FRCDIVN          // Oscillator Source Selection (Internal Fast RC (FRC) Oscillator with postscaler)
#pragma config PWMLOCK = ON             // PWM Lock Enable bit (Certain PWM registers may only be written after key sequence)
#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <string.h>
#define baud_9600 1041 //estandar

void delay_ms(unsigned long time_ms)
{
    unsigned long u;
    for(u = 0; u < time_ms*900; u++) //calculo aproximado para una CPU a 4MHz
    {
       ams("NOP");
    }
}


void uart_config (unsigned int baud)
{
    //configuracion de pines tx y rx
    TRISCbits.TRISC0 = 1, //Pin de recepcion de uart establecido como entrada
    //PARA LOS PINES RC0 Y RC1
    //RPINR18bits.U1RXR = 16; //Pin de recepcion RC0 trabajando con el modulo uart (RP16) (registro que no dice la tabla y el numero se asigna segun el pin que usamos)
    //RPOR8bits.RP17R = 3; //U1TX conectado con el pin RC1 (RP17)
    //PARA LOS PINES RC2 Y RC3
    RPINR18bits.U1RXR = 18; //Pin de recepción RC2
    RPOR9bits.RP19R = 3; //U1TX conectado con el pin RC3 (RP19)

    //configuracion de registro de U1MODE
    U1MODEbits.UARTEN = 0; //Deshbilitar Uart
    U1MODEbits.USIDL = 0; //Continuar operación en modo IDLE
    U1MODEbits.IREN = 0; // IR no usado
    U1MODEbits.RTSMD = 1; // Control de flujo desactivado
    U1MODEbits.UEN = 0; //Solo usamos pin de Tx y pin de Rx
    U1MODEbits.WAKE = 0; //No quiero que la UART despierte del modo sleep
    U1MODEbits.LPBACK = 0; //Loopback deshabilitado
    U1MODEbits.ABAUD = 0; //Automedición de baudios (bps) deshabilitado
    U1MODEbits.URXINV = 0; //En estado de reposo, el receptor mantiene un estado alto, high
    U1MODEbits.BRGH = 1; //Modo High-Speed
    U1MODEbits.PDSEL = 0; //8 bits de datos y paridad nula (8N)
    U1MODEbits.STSEL = 0; //1-bit de stop al final de la trama de datos (8N1)



   //configuracion de registro de U1STA
    U1STAbits.UTXISEL0 = 0; //Tema de interrupciones(no mirar aun)
    U1STAbits.UTXISEL1 = 0; //Tema de interrupciones(no mirar aun)
    U1STAbits.UTXINV = 0; //El estado en reposo del pin en transmición es High
    U1STAbits.UTXBRK = 0; //No usamos trama de sincronización
    U1STAbits.UTXEN = 1;//El transmisor a pleno funcionamiento
    U1STAbits.URXISEL = 0;//Tema de interrupciones(no mirar aun)
    U1STAbits.ADDEN = 0; //No usamos direccionamieno
    //U1STAbits.RIDLE = 0;
    U1STAbits.OERR = 0; //Reseteamos buffer de recepcion
    
    
    //configuracion la velocidad de transmisión/recepción de los datos
    U1BRG = baud;
    U1MODEbits.UARTEN = 1; //Uart habilitada por completo
}
    
    
    
  
int main (void)
{
    
    //Configurar el oscilador para funcionar a 40 MHz
    //Fosc = Fin * M/(N1*N2), Fcy = Fosc/2
    //Fosc = 8M * 40/(2*2) = 80 MHz para un relos de 8MHz de entrada
    //Fcy = Fosc/2 = 80/2 = 40Mhz
    PLLFBD = 38;//M= 40
    CLKDIVbits.PLLPOST = 0; //N1= 2
    CLKDIVbits.PLLPRE = 0;//N2 = 2
    while(OSCCONbits.LOCK != 1); //Wait for PLL to lock
    
    AD1PCFGL = 0xFFFF; //Primer paso. Todos los pines configurados como pine digitales
    
    uart_config(baud_9600);
     
    while(1)
    {  
        
        if(U1STAbits.URXDA == 1)
        {
            var = U1RXREG;
        }
        
        sprintf(buffer, "Esteban y Bruno%d %c); 
        U1TXREG = 48;
        U1TRXREG = '\r';
        U1TRXREG = '\n';
        delay_ms(500);
    }
    
    
    
    
    return 0;
    
    
}  
    
    
    
    




    
