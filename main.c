#include "stc15.h"
#include "stc_adc.h"

// PWM周期为T0溢出时间，输出为P3.6。

#define MAIN_Fosc 24000000UL  //定义主时钟

#define PWM_HIGH_MIN  10.0      //限制PWM输出的最小占空比。
#define PWM_HIGH_MAX  90.0      //限制PWM输出的最大占空比。

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

#define VOL_BATTERY 1

unsigned int voltage_battery;

int Int_term_Buck;
unsigned int Kp = 30;
unsigned int Ki = 15;

/*
void  delay_ms(unsigned int ms)
{
     unsigned int i;
   do{
      i = MAIN_Fosc / 13000;
      while(--i);
     }while(--ms);
}
*/

void UartSend(unsigned char uart_data)
{
  SBUF = uart_data;
  while(TI == 0);  //等待发送完成
  TI = 0;
}
/*
void UartSendStr(unsigned char uartsend_str[])
{
  while(*uartsend_str != '\0')
  {
    UartSend(*uartsend_str++);
    UartSend('\n'); //换行
  }
}
*/
void UartSendInt(unsigned int uartsend_int)
{
  unsigned char uartsendint[5],
                i = 0;
  do {
    uartsendint[i++]=(48 + uartsend_int % 10);
    uartsend_int = uartsend_int / 10;
  } while(uartsend_int);
  do {
    UartSend(uartsendint[--i]);
  } while(i != 0);
  UartSend('\n'); //换行
}

void GetVolBattery(void)
{
  voltage_battery = (float)ADC_GetVoltage(VOL_BATTERY) * 3.06; //* VOL_BATTERY_ACTUAL / VOL_BATTERY_SAMPLE;
}

void Timer0Init(void)    //pwm频率25khz。
{
  AUXR |= 0x80;    //定时器时钟1T模式
  TMOD &= 0xF0;    //设置定时器模式
  TL0 = 0xFC;    //设置定时初值
  TH0 = 0xFF;    //设置定时初值
  TF0 = 0;    //清除TF0标志
  TR0 = 1;    //定时器0开始计时
}

void InitUart()
{
  SCON = 0x50;       //设置串口为8位可变波特率
  T2L = 0x8F;        //设定定时初值
  T2H = 0xFD;        //设定定时初值
  AUXR |= 0x04;       //T2为1T模式, 并启动定时器2
  AUXR |= 0x01;      //选择定时器2为串口1的波特率发生器
  AUXR |= 0x10;
  ES = 1;            //开串口中断
}

void PWM_Update(float pwm_duty)
{
  unsigned int pwm_low;
  if(pwm_duty > PWM_HIGH_MAX)  
    pwm_duty = PWM_HIGH_MAX;  //如果写入大于最大占空比数据，强制为最大占空比。
  if(pwm_duty < PWM_HIGH_MIN)  
    pwm_duty = PWM_HIGH_MIN;  //如果写入小于最小占空比数据，强制为最小占空比。
// 计算占空比对应的脉冲
  pwm_low = 255 - (255 * pwm_duty / 100);
  //CR = 0;            //停止PCA。
  CCAP1H = CCAP1L = pwm_low;
  //CR = 1;            //启动PCA。
}

void PCA_Init(void)
{
  ACC = P_SW1;
  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
  ACC |= CCP_S0;                  //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
  P_SW1 = ACC;

  CR = 0;
  CL = 0; // 清空PCA寄存器
  CH = 0;
  CMOD = 0x04; // 频率为T0溢出脉冲
  PCA_PWM1 = 0X00; // PCA模块2工作于8位PWM模式
  CCAPM1 = 0x42; //PCA模块2工作于8位PWM模式,无中断
  PWM_Update(50.0);
  CR = 1;  // 运行PCA定时器
}

void Buck_PI_Init(void)
{
  Int_term_Buck = 0;
}

float Buck_PI(unsigned int vol_rel, unsigned int vol_set)
{
  float PI_out,
        Ek,
        Up,
        Ui;
  Ek = vol_rel - vol_set;
  Up = (-Kp * Ek) / 200;
  Ui = (-Ki * Ek) / 200;
  Int_term_Buck += Ui;
  PI_out = Int_term_Buck + Up + 50.0;
  return PI_out;
}

void main(void)
{
  unsigned int i;
  Timer0Init();
  InitUart();
  PCA_Init();  //PCA初始化
  InitADC();
  Buck_PI_Init();
  EA = 1;
  P3M1 = 0x00;
  P3M0 = 0x40; //P3.6为推挽输出
  P1M1 = 0xFF;  // AD口设置为高阻
  P1M0 = 0x00;
  UartSendInt(1123);
  while (1)
  {
    if(i == 65530)
      {  
      GetVolBattery();
      UartSendInt(voltage_battery);
      i = 0;
      }
    i++;
  }
}