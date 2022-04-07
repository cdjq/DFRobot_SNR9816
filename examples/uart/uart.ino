/*!
 * @file uart.ino
 * @brief  通过UART控制语音识别模块
 * @n  可以获取识别的命令ID, 通过命令ID播放其对应的答复音频;
 * @n  可以设置唤醒状态的持续周期, 设置静音模式, 设置音量, 进入唤醒状态, 复位模块
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-02
 * @url  https://github.com/DFRobot/DFRobot_SNR9816
 */
#include "DFRobot_SNR9816.h"

/**
 * @brief DFRobot_URM13_RTU constructor
 * @param serial - serial ports for communication, supporting hard and soft serial ports
 * @param rx - UART 接收数据的引脚
 * @param tx - UART 发送数据的引脚
 */
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)   // 使用软串口
  SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
  DFRobot_SNR9816_UART SNR9816(/*softSerial =*/&softSerial);
#elif defined(ESP32)   // 使用 可重映射引脚的 硬串口 : Serial1
  DFRobot_SNR9816_UART SNR9816(/*hardSerial =*/&Serial1, /*rx =*/D3, /*tx =*/D2);
#else   // 使用硬串口 : Serial1
  DFRobot_SNR9816_UART SNR9816(/*hardSerial =*/&Serial1);
#endif

void setup()
{
  Serial.begin(115200);

  // Init the sensor
  while( !( SNR9816.begin() ) ) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   * @brief 复位模块
   */
  // SNR9816.resetModule();

  /**
   * @brief 设置模块的命令
   * @param setType - 设置类型
   * @n       SNR9816_UART_MSG_CMD_SET_VOLUME : 设置音量, 设置值范围0~20
   * @n       SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP : 进入唤醒状态; 设置值 0
   * @n       SNR9816_UART_MSG_CMD_SET_MUTE : 静音模式; 设置值 1: mute, 0: unmute
   * @n       SNR9816_UART_MSG_CMD_SET_WAKE_TIME : 唤醒持续时间; 设置值范围0~255秒
   * @param setValue - 设置值, 范围参见上述设置类型
   */
  SNR9816.settingCMD(SNR9816_UART_MSG_CMD_SET_MUTE, 0);
  SNR9816.settingCMD(SNR9816_UART_MSG_CMD_SET_VOLUME, 10);
  SNR9816.settingCMD(SNR9816_UART_MSG_CMD_SET_WAKE_TIME, 20);
  SNR9816.settingCMD(SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP, 0);

  /**
   * @brief 根据命令词ID播放对应的答复音
   * @param CMDID - 命令词ID
   */
  SNR9816.playByCMDID(23);

}

void loop()
{
  /**
   * @brief 获取命令词对应的ID
   * @return 返回获取的命令词ID, 返回0表示未获取到有效id
   */
  uint8_t CMDID = 0;
  CMDID = SNR9816.getCMDID();
  if(0 != CMDID) {
    Serial.print("CMDID = ");
    Serial.println(CMDID);
  }
  delay(2000);
}
