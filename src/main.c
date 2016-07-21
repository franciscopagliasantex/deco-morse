
////////////////////////Decodificador Código Morse/////////////////////////
////                                                                   ////
////              Proyecto Final - Técnicas Digitales 2                ////
////                   Universidad Blas Pascal 2014.                   ////
////                         Paglia Francisco                          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////

/////////////////////////////Especificaciones//////////////////////////////
////                                                                   ////
////                      Dispositivo: PIC18F452.                      ////
////                          Cristal: 10Mhz.                          ////
////                      Compilador: XC8 (V1.33).                     ////
////                                                                   ////
////                                                                   ////
////               Ültima Revisión: 30/10/2015 - 23:14hs.              ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////

/*--Librería Compilador:  XC8--------------------------------------------*/

#include <xc.h>

/*--Librerías C: Math y Stdio--------------------------------------------*/

#include <math.h>
#include <stdio.h>

/*--Bits de configuración-------------------------------------------------*/

#pragma config OSC = HSPLL, OSCS = OFF
#pragma config PWRT = OFF, BOR = ON, BORV = 42
#pragma config WDT = OFF, WDTPS = 1
#pragma config CCP2MUX = OFF
#pragma config DEBUG = OFF, LVP = OFF, STVR = ON

/*--Librerías: Display LCD y Puerto Serie---------------------------------*/

#include "../lib/lcd/lcd.h"
#include "../lib/serial/serial.h"

/*--Variable Global: Posición Puntero LCD Línea 2-------------------------*/

volatile unsigned char posLCD = 0x0;

/*--Función: Equivalencias ANSCII-----------------------------------------*/

int equivANSCII(int sum){

    ///////////////////////////////////////////////////////////////
    //                                                           //
    //  Esta función toma el valor checkSum ingresado en el ar-  //
    //  gumento y busca cual es la equivalencia en ANSCII.       //
    //                                                           //
    ///////////////////////////////////////////////////////////////


    switch (sum){
        case 282:
            return 65;//A
        case 161:
            return 66;//B
        case 165:
            return 67;//C
        case 241:
            return 68;//D
        case 300:
            return 69;//E
        case 164:
            return 70;//F
        case 243:
            return 71;//G
        case 160:
            return 72;//H
        case 280:
            return 73;//I
        case 174:
            return 74;//J
        case 245:
            return 75;//K
        case 162:
            return 76;//L
        case 283:
            return 77;//M
        case 281:
            return 78;//N
        case 27:
            return 78;//Ñ
        case 247:
            return 79;//O
        case 166:
            return 80;//P
        case 171:
            return 81;//Q
        case 242:
            return 82;//R
        case 240:
            return 83;//S
        case 301:
            return 84;//T
        case 244:
            return 85;//U
        case 168:
            return 86;//V
        case 246:
            return 87;//W
        case 169:
            return 88;//X
        case 173:
            return 89;//Y
        case 163:
            return 90;//Z
        case 30:
            return 49;//1
        case 28:
            return 50;//2
        case 24:
            return 51;//3
        case 16:
            return 52;//4
        case 0:
            return 53;//5
        case 1:
            return 54;//6
        case 3:
            return 55;//7
        case 7:
            return 56;//8
        case 15:
            return 57;//9
        case 31:
            return 48;//0
        case 14:
            return 32;//Espacio
        case 4:
            lcd_clear(); posLCD = 0x0; break;//LCD
        default:
            return 35;//Error de suma, letra no existe ("#").
    }

    //Error de código, no entro en el switch. ("*").
    return 42;
}

/*--Función: Retardo------------------------------------------------------*/

void delayLCD(int multiploRet){

    ///////////////////////////////////////////////////////////////
    //                                                           //
    //  Esta función realiza un bucle de retardo de aproxima-    //
    //  damente 10mseg por vuelta. El argumento multiploRet le   //
    //  indica a la función cuantas veces realizará el bucle.    //
    //                                                           //
    //          Tiempo Retardo = 10mseg x multiploRet            //
    //                                                           //
    ///////////////////////////////////////////////////////////////


    unsigned int j;


    for(j=0; j<multiploRet; ++j){ _delay(100000); }

    
    return;
}

/*--Función: Mover Puntero LCD--------------------------------------------*/
void moverPuntero(){

    if(!RB4){

        if(posLCD == 0xE){return;}

        delayLCD(40);
        posLCD++;
        

    }

    if(!RB5){

        if(posLCD == 0x0){return;}
        
        delayLCD(40);
        posLCD--;
        
    }

    lcd_goto(0x40 + posLCD);

    return;
    
}

/*--Función: Detección de Pulsos------------------------------------------*/

void mainPulso(){

    ///////////////////////////////////////////////////////////////
    //                                                           //
    //  Esta función determina el ancho de los pulsos enviados   //
    //  desde el botón conectado a RB0/INT0. Luego interpreta    //
    //  la combinación de pulsos e imprime un caractér ANSCII    //
    //  equivalente.                                             //
    //                                                           //
    ///////////////////////////////////////////////////////////////


    unsigned int longPulso = 0, timeOut = 250;
    unsigned char morseLetra[5], morsePuntero = 0;

    lcd_home();
    
    //Bucle discriminador de pulsos.
    while(timeOut){


        //Sumar +1 al auxiliar cada 10mseg que el botón esté presionado.
        while(!RB0){
            
            longPulso++;
            delayLCD(1);

            //Buzzer Encendido.
            LATA4 = 1;
        }

        //Buzzer Apagado.
        LATA4 = 0;


        //Si se presionó el botón mas de 30mseg (para evitar ruidos):
        if(longPulso > 3){
            
            //Pulso largo = 1 cuando longPulso > 600mseg.
            //Pulso corto = 0 cuando longPulso < 600mseg
            morseLetra[morsePuntero] = longPulso > 60 ? 1 : 0;

            //Imprimo una línea ("-") si el pulso es largo (1).
            //Imprimo un  punto (".") si el pulso es corto (0).
            morseLetra[morsePuntero] ? lcd_puts("-") : lcd_puts(".");

            //Muevo una posición el puntero del array morseLetra[i+1].
            morsePuntero++;


            //Vuelvo a poner en cero longPulso para contar de nuevo.
            longPulso = 0;


            //Vuelvo a poner el máx. de tiempo de espera de pulso.
            timeOut = 250;
        }
        
        //Espero 10mseg.
        delayLCD(1);

        //Resto uno al tiempo de espera.
        timeOut--;
    }


    //Vuelvo a la primera posición de la primera línea del Display.
    //Imprimo 16 espacios en blanco para limpiar la línea.
    lcd_home();
    lcd_puts("                ");


    //Asigno un "10" a todos los campos no utilizados del array.
    for(morsePuntero; morsePuntero < 5; morsePuntero++){
        morseLetra[morsePuntero] = 10;
    }


    //Hago un "CheckSum" del array morseLetra.
    //Sumatoria de: (Valor_de_la_celda_del_array) * (2 ^ Posición).
    unsigned int checkSum = 0;
    
    for(morsePuntero = 0; morsePuntero < 5; morsePuntero++){
        checkSum = checkSum + (morseLetra[morsePuntero] * pow(2, morsePuntero));
    }


    //Si la segunda fila está llena.
    if (posLCD > 0x0F){

        //Borrar LCD.
        lcd_clear();

        //Resetear Puntero.
        posLCD = 0x0;

        //Salto de Línea en Pto. S.
        printf("\r\n");
    }

    
    //Muevo el puntero del LCD a la segunda línea (0x40).
    //Luego muevo el puntero (+) hasta la última posición escrita.
    lcd_goto(0x40 + posLCD);


    //Sumo una posición al puntero del LCD.
    posLCD++;

   
    //Busco en mi tabla el equivalente ANSCII a mi CheckSum.
    unsigned char letraAnscii = equivANSCII(checkSum);


    //Escribo en el Display la letra ANSCII.
    lcd_putch(letraAnscii);

    //Escribo en Puerto Serie la letra ANSCII.
    printf(" "); putch(letraAnscii); printf(" ");

    
    //Vuelvo a la posición del puntero.
    lcd_goto(0x40 + posLCD);


    //Vuelvo a main();
    return;
}

/*--Función: Principal----------------------------------------------------*/

void main(){

    //Inicializar Display LCD y Comunicación Puerto Serie.
    init_comms();
    lcd_init();


    //Mensaje Bienvenida Puerto Serie.
    printf("Decodificador Codigo Morse: \r\nFrancisco Paglia "
            "y Juan Cruz Mirgone.\r\n \r\n");
    
    //Mensaje Bienvenida Display LCD.
    lcd_home(); lcd_puts("  Decodificador");
    lcd_goto(0x40); lcd_puts("  Codigo Morse");
    delayLCD(150); lcd_clear();


    //Configuro RB0/INT0 como entrada.
    //RB4 y RB5 como entradas de Matricial.
    TRISB = 0x31;

    RBPU = 0;

    //Configuro RE0 como salida de Voltaje para Matricial.
    TRISE0 = 0;
    LATE0 = 0;
    
    //Configuro el Buzzer.
    ADCON1 = 0x07;
    TRISA = 0x00;
    LATA4 = 0;

    lcd_puts("    Presione");
    lcd_goto(0x40);
    lcd_puts("   RB0 / INT0");
    while(RB0);
    lcd_clear();
    
    //Bucle infinito.
    while(1){

        if(!RB0){ mainPulso(); }

        if(!RB4 || !RB5){ moverPuntero(); }
        
    }

}

/*-----------------------------------------------------------------------*/

