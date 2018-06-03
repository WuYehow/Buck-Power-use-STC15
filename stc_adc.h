#ifndef __STC_ADC_H_
#define __STC_ADC_H_

#include "stc15.h" //MCU:stc15w404as
#include "intrins.h"

//ADC����趨����
#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_SPEEDL  0x20            //360��ʱ��
#define ADC_SPEEDH  0x40            //180��ʱ��
#define ADC_SPEEDHH 0x60            //90��ʱ��

//BandGap��ز���
#define ID_ADDR_BANDGAP_RAM 0xef        //����256�ֽ�����RAM��MCU(�󲿷�ϵ��)��ŵ�ַΪ0EFH
//#define ID_ADDR_BANDGAP_RAM 0x6f        //����ֻ��128�ֽ�RAM��MCU(stc15f/w100ϵ��)��ŵ�ַΪ06fH

//ע��:��Ҫ�����ش���ʱѡ��"��ID��ǰ�����Ҫ���Բ���"ѡ��,�ſ��ڳ����л�ȡ�˲�����������ͬ�ĵ�Ƭ������Ĳ�ͬ��ֵ��
//#define ID_ADDR_BANDGAP_ROM 0x03f7      //1K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x07f7      //2K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x0bf7      //3K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x0ff7      //4K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x13f7      //5K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x1ff7      //8K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x27f7      //10K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x2ff7      //12K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x3ff7      //16K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x4ff7      //20K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x5ff7      //24K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x6ff7      //28K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x7ff7      //32K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0x9ff7      //40K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0xbff7      //48K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0xcff7      //52K����ռ��MCU
//#define ID_ADDR_BANDGAP_ROM 0xdff7      //56K����ռ��MCU
#define ID_ADDR_BANDGAP_ROM 0xeff7      //60K����ռ��MCU

// ADC�˲������
# define FILTER_COUT 11

extern unsigned int ADC_voltage_BandGap;       //BandGapԤ����У׼ֵ����λ����

//��������

//��ȡBandGap��ѹֵ
void ADC_ReadBandGapVoltage();
//��ȡADC���
unsigned int ADC_GetResult(unsigned char channel);
//��ȡ�ھ�ͨ��ADC���
unsigned int ADC_GetResult_BandGap();
//����ADC���
unsigned int ADC_GetVoltage(unsigned char channel);
//��ʼ��ADC
void InitADC();

#endif