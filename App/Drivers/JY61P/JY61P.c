/*
 * JY61P.c
 *
 *  Created on: 2024年11月29日
 *      Author: 26448
 */

#include "JY61P.h"

JY61PData data = {.size = 0, .a = {0}, .quater = {0}, .rowAngle = {0}, .temperature = 0, .time = {0}, .wSpeed = {0}};

void JY61P_Init()
{
    USART2_Init(9600);
    DMA1_USART2_Init();
}

void JY61PData_Time(uint8_t *timeData, uint8_t size)
{
    if (timeData == NULL || size == 0)
    {
        return;
    }
    data.time.tm_year = timeData[2];
    data.time.tm_mon = timeData[3];
    data.time.tm_mday = timeData[4];
    data.time.tm_hour = timeData[5];
    data.time.tm_min = timeData[6];
    data.time.tm_sec = timeData[7];
    // msec=(uint16_t)(((uint16_t)(timeData[9]<<8)|timeData[10])/1024.0*1000)
}

void JY61PData_A(uint8_t *aData, uint8_t size)
{
    if (aData == NULL || size == 0)
    {
        return;
    }
    data.a.x = (int16_t)((aData[3] << 8) | aData[2]) / JY61P_A_DATA;
    data.a.y = (int16_t)((aData[5] << 8) | aData[4]) / JY61P_A_DATA;
    data.a.z = (int16_t)((aData[7] << 8) | aData[6]) / JY61P_A_DATA;
}

void JY61PData_WSpeed(uint8_t *wSpeedData, uint8_t size)
{
    if (wSpeedData == NULL || size == 0)
    {
        return;
    }
    data.wSpeed.x = (int16_t)((wSpeedData[3] << 8) | wSpeedData[2]) / JY61P_WSPEED_DATA;
    data.wSpeed.y = (int16_t)((wSpeedData[5] << 8) | wSpeedData[4]) / JY61P_WSPEED_DATA;
    data.wSpeed.z = (int16_t)((wSpeedData[7] << 8) | wSpeedData[6]) / JY61P_WSPEED_DATA;
}

void JY61PData_RowAngle(uint8_t *rowAngleData, uint8_t size)
{
    if (rowAngleData == NULL || size == 0)
    {
        return;
    }
    data.rowAngle.x = (int16_t)((rowAngleData[3] << 8) | rowAngleData[2]) / JY61P_ROWANGLE_DATA;
    data.rowAngle.y = (int16_t)((rowAngleData[5] << 8) | rowAngleData[4]) / JY61P_ROWANGLE_DATA;
    data.rowAngle.z = (int16_t)((rowAngleData[7] << 8) | rowAngleData[6]) / JY61P_ROWANGLE_DATA;
}

void JY61PData_Magrntic(uint8_t *magenticData, uint8_t size)
{
}

void JY61PData_Quater(uint8_t *quaterData, uint8_t size)
{
    if (quaterData == NULL || size == 0)
    {
        return;
    }
    for (uint8_t i = 2; i < 6; i++)
    {
        data.quater[i - 2] = (uint16_t)((quaterData[i + 1] << 8) | quaterData[i]);
    }
}

void Print_JY61PSourceData(uint8_t *sourceData, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++)
    {
        printf("%02x ", sourceData[i]);
    }
    printf("\n");
}

void JY61PData_Chose(uint8_t *sourceData, uint8_t size)
{
    if (sourceData == NULL || size == 0)
    {
        return;
    }
    switch (sourceData[1])
    {
    case 0x50:
        JY61PData_Time(sourceData, size);
        printf("TIME:%2d-%2d-%2d %2d:%2d:%2d\n", data.time.tm_year, data.time.tm_mon, data.time.tm_mday, data.time.tm_hour, data.time.tm_min, data.time.tm_sec);
        return;
    case 0x51:
        JY61PData_A(sourceData, size);
        printf("aX:%8.2f\taY:%8.2f\taZ:%8.2f\n", data.a.x, data.a.y, data.a.z);
        return;
    case 0x52:
        JY61PData_WSpeed(sourceData, size);
        printf("wX:%8.2f\twY:%8.2f\twZ:%8.2f\n", data.wSpeed.x, data.wSpeed.y, data.wSpeed.z);
        return;
    case 0x53:
        JY61PData_RowAngle(sourceData, size);
        printf("rAX:%8.2f\trAY:%8.2f\trAZ:%8.2f\n", data.rowAngle.x, data.rowAngle.y, data.rowAngle.z);
        return;
    case 0x54:
        JY61PData_Magrntic(sourceData, size);
        // printf("rAX:%8.2f\trAY:%8.2f\trAZ:%8.2f\n", data.rowAngle.x, data.rowAngle.y, data.rowAngle.z);
        return;
    case 0x59:
        JY61PData_Quater(sourceData, size);
        printf("D0:%5d\tD1:%5d\tD2:%5d\tD3:%5d\n", data.quater[0], data.quater[1], data.quater[2], data.quater[3]);
        return;
    default:
        break;
    }
    Print_JY61PSourceData(sourceData, size);
}

void JY61P_Test()
{
    // printf("JY61P_Test\n");
    uint8_t sourceData[JY61P_DATA_LENGTH] = {0};
    Delay_Ms(10);

    data.size = getRingBufferUsart2()->RemainCount % JY61P_DATA_LENGTH;
    uint16_t i = 0;
    while (getRingBufferUsart2()->RemainCount != 0)
    {
        sourceData[i] = Ring_Buffer_Pop_Usart2();
        i++;
        if (i % 11 == 0)
        {
            JY61PData_Chose(sourceData, JY61P_DATA_LENGTH);
            i = 0;
        }
    }
}
