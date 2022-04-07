# -*- coding: utf-8 -*
""" 
  @file  i2c.py
  @brief  通过I2C控制语音识别模块
  @n  可以获取识别的命令ID, 通过命令ID播放其对应的答复音频;
  @n  可以获取唤醒状态的持续周期, 也可以设置唤醒状态的持续周期
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2022-04-06
  @url https://github.com/DFRobot/DFRobot_SNR9816
"""
from __future__ import print_function
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))

from DFRobot_SNR9816 import *

'''
  @brief Module I2C communication init
  @param i2c_addr - I2C communication address
  @param bus - I2C bus
'''
SNR9816 = DFRobot_SNR9816_I2C(i2c_addr=SNR9816_I2C_ADDR, bus=1)

def setup():
  '''
    @brief 设置唤醒持续时间
    @param wakeTime - 唤醒持续时间(0~255)
  '''
  SNR9816.set_wake_time(20)

  '''
    @brief 获取唤醒持续时间
    @return 当前设置的唤醒周期
  '''
  print("wake_time = %u\n" %(SNR9816.get_wake_time()))

  '''
    @brief 根据命令词ID播放对应的答复音
    @param CMDID - 命令词ID
  '''
  SNR9816.play_by_CMDID(1)   # 唤醒词ID
  SNR9816.play_by_CMDID(23)   # 普通词ID


def loop():
  CMDID = SNR9816.get_CMDID()
  if(0 != CMDID):
    print("CMDID = %u\n" % CMDID)
  time.sleep(3)


if __name__ == "__main__":
  setup()
  while True:
    loop()
