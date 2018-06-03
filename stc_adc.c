#include "stc_adc.h" //MCU:stc15w404as

//全局变量
unsigned int ADC_voltage_BandGap;       //BandGap预储存校准值，单位毫伏

//读取BandGap电压值
void ADC_ReadBandGapVoltage()
{
  //unsigned char code *cptr;           //定义ROM(代码)区指针
  unsigned char idata *iptr;       //定义RAM(内存)区指针
/*
  //下面的方法需要在下载式勾选"在ID号前添加重要测试参数"选项,才可在程序中获取此参数
  cptr = ID_ADDR_BANDGAP_ROM;           //从程序区读取BandGap电压值(单位:毫伏mV)
  ADC_voltage_BandGap = *cptr++;
  ADC_voltage_BandGap <<= 8;
  ADC_voltage_BandGap += *cptr;
*/

  iptr = ID_ADDR_BANDGAP_RAM;           //从内存区读取BandGap电压值(单位:毫伏mV)，貌似不推荐
  ADC_voltage_BandGap =* iptr++;            //两种方法结果一样，
  ADC_voltage_BandGap <<= 8;                //下面的这种方法不需要
  ADC_voltage_BandGap += *iptr;

}

//读取ADC结果
unsigned int ADC_GetResult(unsigned char channel)
{
  unsigned int ADC_10BIT_RES;

  ADC_CONTR = ADC_POWER | ADC_SPEEDLL | channel | ADC_START;
  _nop_();                          //等待4个NOP
  _nop_();
  _nop_();
  _nop_();
  while (!(ADC_CONTR & ADC_FLAG));  //等待ADC转换完成
  ADC_CONTR &= ~ADC_FLAG;           //关闭 ADC
  ADC_10BIT_RES = ADC_RES;          //得到高8位
  ADC_10BIT_RES <<= 2;
  ADC_10BIT_RES |= ADC_RESL;        //得到低2位
  return ADC_10BIT_RES;             //返回ADC结果
}

//读取第九通道ADC结果
unsigned int ADC_GetResult_BandGap()
{
  unsigned int ADC_res_BandGap;

  ADC_RES = 0;                     //清除结果寄存器
  P1ASF = 0x00;                    //设置读第九通道
  ADC_GetResult(0);                //测BandGap时，调用此函数时通道数只能填0
  ADC_GetResult(0);                //读三次获得稳定
  ADC_res_BandGap = ADC_GetResult(0);
  return ADC_res_BandGap;
}

//计算ADC结果
unsigned int ADC_GetVoltage(unsigned char channel)
{
  unsigned int ADC_res,       //测量设定通道adc值
               ADC_res_buf = 0,
               ADC_res_min = 1024,
               ADC_res_max = 0,
               ADC_res_BandGap,
               ADC_res_buf_BandGap = 0,
               ADC_res_min_BandGap = 1024,
               ADC_res_max_BandGap = 0,
               ADC_res_num,
               ADC_voltage;          //设定通道电压值，单位毫伏
         float ADC_res_ave,
               ADC_BandGap_ave;
  unsigned char i;

  if(channel < 8 && channel >= 0)
  {
    //测量设定通道adc值
    ADC_RES = 0;                      //清除结果寄存器

    P1ASF = 0xff;                     //设置P1口为AD口
  }
    //去极值平均滤波算法
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

//初始化ADC
void InitADC()
{
  unsigned int i;

  P1ASF = 0xff;                   //设置P1口为AD口
  ADC_RES = 0;                    //清除结果寄存器
  ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
  for(i = 10000; i > 0; i--);     //ADC上电并延时
  ADC_ReadBandGapVoltage();       //读取BandGap预储存校准值
}