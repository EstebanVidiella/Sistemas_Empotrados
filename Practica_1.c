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
