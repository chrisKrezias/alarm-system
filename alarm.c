int nd,detect=0;
int i=0;
unsigned short kp, y;
char txt[6];
     int j, flag;
     char x, buf[4];
     char cod[4]={'1','2','3','4'};
// Keypad module connections
char keypadPort at PORTB;
// End Keypad module connections

// LCD module connections
sbit LCD_RS at RC0_bit;
sbit LCD_EN at RC1_bit;
sbit LCD_D4 at RC2_bit;
sbit LCD_D5 at RC3_bit;
sbit LCD_D6 at RC4_bit;
sbit LCD_D7 at RC5_bit;
sbit LCD_RS_Direction at TRISC0_bit;
sbit LCD_EN_Direction at TRISC1_bit;
sbit LCD_D4_Direction at TRISC2_bit;
sbit LCD_D5_Direction at TRISC3_bit;
sbit LCD_D6_Direction at TRISC4_bit;
sbit LCD_D7_Direction at TRISC5_bit;
// End LCD module connections

sbit Sensor_IP0 at RA0_bit;
sbit Sensor_IP1 at RA1_bit;
sbit Turn_Off at RA5_bit;
sbit Sensor_IP0_Input_Direction at TRISA0_bit;
sbit Sensor_IP1_Input_Direction at TRISA1_bit;
sbit Turn_Off_Input_Direction at TRISA5_bit;
void init_delay();
void password(int snd);

void main() {
     UART1_Init(9600);
     ADCON1 = 0b00000110; // all PORTA pins as digital I/O
     TRISA = 0x3F; // all PORTA pins as digital inputs
     TRISA.F2=0; // Lock
     TRISA.F3=0; // Buzzer
     TRISA.F4=0; // Led
     PORTA.F2=0;
     PORTA.F3=0;
     PORTA.F4=1;
     Keypad_Init(); // Initialize Keypad
     Lcd_Init(); // Initialize LCD
     Lcd_Cmd(_LCD_CLEAR); // Clear display
     Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
     Sound_Init(&PORTA,3);
     nd=0;
     password(detect);
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1,1,"Initializing");
     init_delay();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1,1,"Alarm Armed");
     do{
              while (Sensor_IP0){
                   if (detect!=1){
                      PORTA.F2=1;
                   }
                   Sound_Play(1000,100);
                   Lcd_Cmd(_LCD_CLEAR);
                   Lcd_Out(1,1,"Room 1");
                   Delay_ms(100);
                   detect=1;
              }
              while (Sensor_IP1){
                    if (detect!=1){
                      PORTA.F2=1;
                   }
                   Sound_Play(1000,100);
                   Lcd_Cmd(_LCD_CLEAR);
                   Lcd_Out(1,1,"Room 2");
                   Delay_ms(100);
                   detect=1;
              }
              if (detect==1){
                   Sound_Play(500,100);
                   Delay_ms(100);
              }
              if (Turn_Off){
                   password(detect);
                   if (detect==0) PORTA.F2 = 0;
                   detect=0;
                   Lcd_Cmd(_LCD_CLEAR);
                   Lcd_Out(1,1,"Alarm Armed");
              }
     }while(1);
}

void init_delay(){

  for(i=0;i<30;i++)
    {
    PORTA.F4=0;
    Delay_ms(100);
    PORTA.F4=1;
    Delay_ms(100);
    }
  PORTA.F4=0;
}

void password(int snd){
    do{
     y=1;
     flag=0;
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1,1,"Enter Password");
     for (j=0;j<4;j++)
     {
         kp=0;
         do{
             if (UART1_Data_Ready()) kp=UART1_Read();
             else kp = Keypad_Key_Click();
             if (snd==1){
                Sound_Play(500,100);
             }
         } while (!kp);
         switch (kp)
            {
              case  1: kp = 49; break; // 1
              case  2: kp = 50; break; // 2
              case  3: kp = 51; break; // 3
              //case  4: kp = 65; break; // A for 4x4 pad
              case  5: kp = 52; break; // 4
              case  6: kp = 53; break; // 5
              case  7: kp = 54; break; // 6
              //case  8: kp = 66; break; // B for 4x4 pad
              case  9: kp = 55; break; // 7
              case 10: kp = 56; break; // 8
              case 11: kp = 57; break; // 9
              //case 12: kp = 67; break; // C for 4x4 pad
              case 13: kp = 42; break; // *
              case 14: kp = 48; break; // 0
              case 15: kp = 35; break; // #
              //case 16: kp = 67; break; // D for 4x4 pad

            }
            if (x){buf[j]=x;}
            else{buf[j]=kp;}
           y++;
           Lcd_Chr(2, y, buf[j]); // Print key ASCII value on Lcd
    }
    for (j=0;j<4;j++)
    {
        if (buf[j]!=cod[j])
        {
           flag=1;
        }
    }
    if (flag==1)
    {
      Lcd_Cmd(_LCD_CLEAR);
      Lcd_Out(1,1,"Password Wrong");
      Delay_ms(500);
    }
    } while(flag==1);
}