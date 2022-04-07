/*!
 * @file  i2c.ino
 * @brief  通过I2C控制语音识别模块
 * @n  可以获取识别的命令ID, 通过命令ID播放其对应的答复音频;
 * @n  可以获取唤醒状态的持续周期, 也可以设置唤醒状态的持续周期
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-02
 * @url  https://github.com/DFRobot/DFRobot_SNR9816
 */
#include "DFRobot_SNR9816.h"

//I2C communication
DFRobot_SNR9816_I2C SNR9816;

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
   * @brief 设置唤醒持续时间
   * @param wakeTime - 唤醒持续时间(0~255)
   */
  SNR9816.setWakeTime(20);

  /**
   * @brief 获取唤醒持续时间
   * @return 当前设置的唤醒周期
   */
  uint8_t wakeTime = 0;
  wakeTime = SNR9816.getWakeTime();
  Serial.print("wakeTime = ");
  Serial.println(wakeTime);

  /**
   * @brief 根据命令词ID播放对应的答复音
   * @param CMDID - 命令词ID
   */
  SNR9816.playByCMDID(1);   // 唤醒词ID
  SNR9816.playByCMDID(23);   // 普通词ID

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
  delay(3000);
}
