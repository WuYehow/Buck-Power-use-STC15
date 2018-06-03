#include "stc_adc.h" //MCU:stc15w404as

//ȫ�ֱ���
unsigned int ADC_voltage_BandGap;       //BandGapԤ����У׼ֵ����λ����

//��ȡBandGap��ѹֵ
void ADC_ReadBandGapVoltage()
{
  //unsigned char code *cptr;           //����ROM(����)��ָ��
  unsigned char idata *iptr;       //����RAM(�ڴ�)��ָ��
/*
  //����ķ�����Ҫ������ʽ��ѡ"��ID��ǰ�����Ҫ���Բ���"ѡ��,�ſ��ڳ����л�ȡ�˲���
  cptr = ID_ADDR_BANDGAP_ROM;           //�ӳ�������ȡBandGap��ѹֵ(��λ:����mV)
  ADC_voltage_BandGap = *cptr++;
  ADC_voltage_BandGap <<= 8;
  ADC_voltage_BandGap += *cptr;
*/

  iptr = ID_ADDR_BANDGAP_RAM;           //���ڴ�����ȡBandGap��ѹֵ(��λ:����mV)��ò�Ʋ��Ƽ�
  ADC_voltage_BandGap =* iptr++;            //���ַ������һ����
  ADC_voltage_BandGap <<= 8;                //��������ַ�������Ҫ
  ADC_voltage_BandGap += *iptr;

}

//��ȡADC���
unsigned int ADC_GetResult(unsigned char channel)
{
  unsigned int ADC_10BIT_RES;

  ADC_CONTR = ADC_POWER | ADC_SPEEDLL | channel | ADC_START;
  _nop_();                          //�ȴ�4��NOP
  _nop_();
  _nop_();
  _nop_();
  while (!(ADC_CONTR & ADC_FLAG));  //�ȴ�ADCת�����
  ADC_CONTR &= ~ADC_FLAG;           //�ر� ADC
  ADC_10BIT_RES = ADC_RES;          //�õ���8λ
  ADC_10BIT_RES <<= 2;
  ADC_10BIT_RES |= ADC_RESL;        //�õ���2λ
  return ADC_10BIT_RES;             //����ADC���
}

//��ȡ�ھ�ͨ��ADC���
unsigned int ADC_GetResult_BandGap()
{
  unsigned int ADC_res_BandGap;

  ADC_RES = 0;                     //�������Ĵ���
  P1ASF = 0x00;                    //���ö��ھ�ͨ��
  ADC_GetResult(0);                //��BandGapʱ�����ô˺���ʱͨ����ֻ����0
  ADC_GetResult(0);                //�����λ���ȶ�
  ADC_res_BandGap = ADC_GetResult(0);
  return ADC_res_BandGap;
}

//����ADC���
unsigned int ADC_GetVoltage(unsigned char channel)
{
  unsigned int ADC_res,       //�����趨ͨ��adcֵ
               ADC_res_buf = 0,
               ADC_res_min = 1024,
               ADC_res_max = 0,
               ADC_res_BandGap,
               ADC_res_buf_BandGap = 0,
               ADC_res_min_BandGap = 1024,
               ADC_res_max_BandGap = 0,
               ADC_res_num,
               ADC_voltage;          //�趨ͨ����ѹֵ����λ����
         float ADC_res_ave,
               ADC_BandGap_ave;
  unsigned char i;

  if(channel < 8 && channel >= 0)
  {
    //�����趨ͨ��adcֵ
    ADC_RES = 0;                      //�������Ĵ���

    P1ASF = 0xff;                     //����P1��ΪAD��
  }
    //ȥ��ֵƽ���˲��㷨
  for(i = 0;i < FILTER_COUT;i++)
  {
    if(channel < 8 && channel >= 0)
    { 
      ADC_res = ADC_GetResult(channel);
      if(ADC_res_min > ADC_res)
        ADC_res_min = ADC_res;
      if(ADC_res_max < ADC_res)
        ADC_res_max = ADC_res;
      ADC_res_buf += ADC_res;
    }
    ADC_res_BandGap = ADC_GetResult_BandGap();
    if(ADC_res_min_BandGap > ADC_res_BandGap)
      ADC_res_min_BandGap = ADC_res_BandGap;
    if(ADC_res_max_BandGap < ADC_res_BandGap)
      ADC_res_max_BandGap = ADC_res_BandGap;
    ADC_res_buf_BandGap += ADC_res_BandGap;
  }
  ADC_res_num = FILTER_COUT - 2;
  if(channel < 8 && channel >= 0)
  {
    ADC_BandGap_ave = (float)(ADC_res_buf_BandGap - ADC_res_min_BandGap - ADC_res_max_BandGap) / ADC_res_num;
    ADC_res_ave = (float)(ADC_res_buf - ADC_res_min - ADC_res_max) / ADC_res_num;
    ADC_voltage = (float)ADC_voltage_BandGap * ADC_res_ave / ADC_BandGap_ave;
  }
  else
  {
    ADC_BandGap_ave = (float)(ADC_res_buf_BandGap - ADC_res_min_BandGap - ADC_res_max_BandGap) / ADC_res_num;
    ADC_voltage = (float)ADC_voltage_BandGap * 1023 / ADC_BandGap_ave;
  }
    return ADC_voltage;
}

//��ʼ��ADC
void InitADC()
{
  unsigned int i;

  P1ASF = 0xff;                   //����P1��ΪAD��
  ADC_RES = 0;                    //�������Ĵ���
  ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
  for(i = 10000; i > 0; i--);     //ADC�ϵ粢��ʱ
  ADC_ReadBandGapVoltage();       //��ȡBandGapԤ����У׼ֵ
}