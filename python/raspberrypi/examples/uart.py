# -*- coding: utf-8 -*
""" 
  @file  uart.py
  @brief  通过UART控制语音识别模块
  @n  可以获取识别的命令ID, 通过命令ID播放其对应的答复音频;
  @n  可以设置唤醒状态的持续周期, 设置静音模式, 设置音量, 进入唤醒状态, 复位模块
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
  @brief Module UART communication init
'''
SNR9816 = DFRobot_SNR9816_UART()

def setup():
  '''
    @brief 复位模块
  '''
  # SNR9816.reset_module(20)

  '''
    @brief 设置模块的命令
    @param set_type - 设置类型
    @n       SNR9816_UART_MSG_CMD_SET_VOLUME : 设置音量, 设置值范围0~20
    @n       SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP : 进入唤醒状态; 设置值 0
    @n       SNR9816_UART_MSG_CMD_SET_MUTE : 静音模式; 设置值 1: mute, 0: unmute
    @n       SNR9816_UART_MSG_CMD_SET_WAKE_TIME : 唤醒持续时间; 设置值范围0~255秒
    @param set_value - 设置值, 范围参见上述设置类型
  '''
  SNR9816.setting_CMD(SNR9816_UART_MSG_CMD_SET_MUTE, 0)
  SNR9816.setting_CMD(SNR9816_UART_MSG_CMD_SET_VOLUME, 10)
  SNR9816.setting_CMD(SNR9816_UART_MSG_CMD_SET_WAKE_TIME, 20)
  # SNR9816.setting_CMD(SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP, 0)

  '''
    @brief 根据命令词ID播放对应的答复音
    @param CMDID - 命令词ID
  '''
  SNR9816.play_by_CMDID(23)


def loop():
  CMDID = SNR9816.get_CMDID()
  if(0 != CMDID):
    print("CMDID = %u\n" % CMDID)
  time.sleep(2)


if __name__ == "__main__":
  setup()
  while True:
    loop()
