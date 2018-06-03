#ifndef __STC_ADC_H_
#define __STC_ADC_H_

#include "stc15.h" //MCU:stc15w404as
#include "intrins.h"

//ADC相关设定参数
#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_SPEEDL  0x20            //360个时钟
#define ADC_SPEEDH  0x40            //180个时钟
#define ADC_SPEEDHH 0x60            //90个时钟

//BandGap相关参数
#define ID_ADDR_BANDGAP_RAM 0xef        //对于256字节以上RAM的MCU(大部分系列)存放地址为0EFH
//#define ID_ADDR_BANDGAP_RAM 0x6f        //对于只有128字节RAM的MCU(stc15f/w100系列)存放地址为06fH

//注意:需要在下载代码时选择"在ID号前添加重要测试参数"选项,才可在程序中获取此参数，容量不同的单片机请更改不同数值。
//#define ID_ADDR_BANDGAP_ROM 0x03f7      //1K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x07f7      //2K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x0bf7      //3K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x0ff7      //4K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x13f7      //5K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x1ff7      //8K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x27f7      //10K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x2ff7      //12K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x3ff7      //16K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x4ff7      //20K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x5ff7      //24K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x6ff7      //28K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x7ff7      //32K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0x9ff7      //40K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0xbff7      //48K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0xcff7      //52K程序空间的MCU
//#define ID_ADDR_BANDGAP_ROM 0xdff7      //56K程序空间的MCU
#define ID_ADDR_BANDGAP_ROM 0xeff7      //60K程序空间的MCU

// ADC滤波器相关
# define FILTER_COUT 11

extern unsigned int ADC_voltage_BandGap;       //BandGap预储存校准值，单位毫伏

//函数声明

//读取BandGap电压值
void ADC_ReadBandGapVoltage();
//读取ADC结果
unsigned int ADC_GetResult(unsigned char channel);
//读取第九通道ADC结果
unsigned int ADC_GetResult_BandGap();
//计算ADC结果
unsigned int ADC_GetVoltage(unsigned char channel);
//初始化ADC
void InitADC();

#endif