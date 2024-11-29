/*
 * JY61P.h
 *
 *  Created on: 2024年11月29日
 *      Author: 26448
 */

#ifndef APP_DRIVERS_JY61P_JY61P_H_
#define APP_DRIVERS_JY61P_JY61P_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "JY61PSdk.h"
#include "usart2.h"

#define JY61P_DATA_LENGTH (11u)
#define JY61P_A_DATA 32768.0*16*9.8
#define JY61P_WSPEED_DATA 32768.0*2000
#define JY61P_ROWANGLE_DATA 32768.0*180

typedef struct AngleData
{
    float x;
    float y;
    float z;
}AngleData;

typedef struct JY61PData
{
    uint8_t size;           //数据数量，按组来算
    AngleData a;
    AngleData wSpeed;
    AngleData rowAngle;
    uint16_t quater[4];
    struct tm time;
    int16_t temperature;
}JY61PData;

void JY61P_Init();

void JY61PData_Time(uint8_t* timeData,uint8_t size);

void JY61PData_A(uint8_t* aData,uint8_t size);

void JY61PData_WSpeed(uint8_t* wSpeedData,uint8_t size);

void JY61PData_RowAngle(uint8_t* rowAngleData,uint8_t size);

void JY61PData_Magrntic(uint8_t* magenticData,uint8_t size);

void JY61PData_Quater(uint8_t* quaterData,uint8_t size);

void Print_JY61PSourceData(uint8_t* sourceData,uint8_t size);

void JY61PData_Chose(uint8_t* sourceData,uint8_t size);

void JY61P_Test();

#ifdef __cplusplus
}
#endif

#endif /* APP_DRIVERS_JY61P_JY61P_H_ */
