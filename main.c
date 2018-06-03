#include "stc15.h"
#include "stc_adc.h"

// PWM����ΪT0���ʱ�䣬���ΪP3.6��

#define MAIN_Fosc 24000000UL  //������ʱ��

#define PWM_HIGH_MIN  10.0      //����PWM�������Сռ�ձȡ�
#define PWM_HIGH_MAX  90.0      //����PWM��������ռ�ձȡ�

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
  while(TI == 0);  //�ȴ��������
  TI = 0;
}
/*
void UartSendStr(unsigned char uartsend_str[])
{
  while(*uartsend_str != '\0')
  {
    UartSend(*uartsend_str++);
    UartSend('\n'); //����
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
  UartSend('\n'); //����
}

void GetVolBattery(void)
{
  voltage_battery = (float)ADC_GetVoltage(VOL_BATTERY) * 3.06; //* VOL_BATTERY_ACTUAL / VOL_BATTERY_SAMPLE;
}

void Timer0Init(void)    //pwmƵ��25khz��
{
  AUXR |= 0x80;    //��ʱ��ʱ��1Tģʽ
  TMOD &= 0xF0;    //���ö�ʱ��ģʽ
  TL0 = 0xFC;    //���ö�ʱ��ֵ
  TH0 = 0xFF;    //���ö�ʱ��ֵ
  TF0 = 0;    //���TF0��־
  TR0 = 1;    //��ʱ��0��ʼ��ʱ
}

void InitUart()
{
  SCON = 0x50;       //���ô���Ϊ8λ�ɱ䲨����
  T2L = 0x8F;        //�趨��ʱ��ֵ
  T2H = 0xFD;        //�趨��ʱ��ֵ
  AUXR |= 0x04;       //T2Ϊ1Tģʽ, ��������ʱ��2
  AUXR |= 0x01;      //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
  AUXR |= 0x10;
  ES = 1;            //�������ж�
}

void PWM_Update(float pwm_duty)
{
  unsigned int pwm_low;
  if(pwm_duty > PWM_HIGH_MAX)  
    pwm_duty = PWM_HIGH_MAX;  //���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
  if(pwm_duty < PWM_HIGH_MIN)  
    pwm_duty = PWM_HIGH_MIN;  //���д��С����Сռ�ձ����ݣ�ǿ��Ϊ��Сռ�ձȡ�
// ����ռ�ձȶ�Ӧ������
  pwm_low = 255 - (255 * pwm_duty / 100);
  //CR = 0;            //ֹͣPCA��
  CCAP1H = CCAP1L = pwm_low;
  //CR = 1;            //����PCA��
}

void PCA_Init(void)
{
  ACC = P_SW1;
  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
  ACC |= CCP_S0;                  //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
  P_SW1 = ACC;

  CR = 0;
  CL = 0; // ���PCA�Ĵ���
  CH = 0;
  CMOD = 0x04; // Ƶ��ΪT0�������
  PCA_PWM1 = 0X00; // PCAģ��2������8λPWMģʽ
  CCAPM1 = 0x42; //PCAģ��2������8λPWMģʽ,���ж�
  PWM_Update(50.0);
  CR = 1;  // ����PCA��ʱ��
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
  PCA_Init();  //PCA��ʼ��
  InitADC();
  Buck_PI_Init();
  EA = 1;
  P3M1 = 0x00;
  P3M0 = 0x40; //P3.6Ϊ�������
  P1M1 = 0xFF;  // AD������Ϊ����
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