# -*- coding: utf-8 -*
""" 
  @file  DFRobot_SNR9816.py
  @note  DFRobot_SNR9816 Class infrastructure, implementation of underlying methods
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @licence  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2022-04-02
  @url  https://github.com/DFRobot/DFRobot_SNR9816
"""
import sys
import time

import array
import smbus
import serial
import numpy as np

import logging
from ctypes import *


logger = logging.getLogger()
# logger.setLevel(logging.INFO)   # Display all print information
logger.setLevel(logging.FATAL)   # If you don’t want to display too many prints, only print errors, please use this option
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter) 
logger.addHandler(ph)


## i2c address
SNR9816_I2C_ADDR                  = 0x64

## 请求得到命令词ID的寄存器地址
SNR9816_I2C_REG_CMDID             = 0x02
## 用命令词ID播放Audio的寄存器地址
SNR9816_I2C_REG_PLAY_CMDID        = 0x03
## 请求得到语义ID的寄存器地址
SNR9816_I2C_REG_SECID             = 0x04
## 用语义ID播放Audio的寄存器地址
SNR9816_I2C_REG_PLAY_SECID        = 0x05
## 请求得到当前唤醒时间的寄存器地址
SNR9816_I2C_REG_WAKE_TIME         = 0x06
## 请求修改当前唤醒时间的寄存器地址
SNR9816_I2C_REG_SET_WAKE_TIME     = 0x07

# tail
SNR9816_I2C_MSG_TAIL              = 0x5A

## UART baud rate
SNR9816_UART_BAUDRATE             = 9600
## 串口数据帧的最大数据长度
SNR9816_UART_MSG_DATA_MAX_SIZE    = 8

# header
SNR9816_UART_MSG_HEAD_LOW         = 0xF4
SNR9816_UART_MSG_HEAD_HIGH        = 0xF5
SNR9816_UART_MSG_HEAD             = 0xF5F4
# tail
SNR9816_UART_MSG_TAIL             = 0xFB
# msg_type
SNR9816_UART_MSG_TYPE_CMD_UP      = 0xA0
SNR9816_UART_MSG_TYPE_CMD_DOWN    = 0xA1
SNR9816_UART_MSG_TYPE_ACK         = 0xA2
SNR9816_UART_MSG_TYPE_NOTIFY      = 0xA3
# msg_cmd
## 报告语音识别结果
SNR9816_UART_MSG_CMD_ASR_RESULT        = 0x91
## 播放本地播报音
SNR9816_UART_MSG_CMD_PLAY_VOICE        = 0x92
## 读取FLASH的序列号
SNR9816_UART_MSG_CMD_GET_FLASHUID      = 0x93
## 读取版本号
SNR9816_UART_MSG_CMD_GET_VERSION       = 0x94
## 复位语音模块
SNR9816_UART_MSG_CMD_RESET_MODULE      = 0x95
## 设置
SNR9816_UART_MSG_CMD_SET_CONFIG        = 0x96
## 进入升级模式
SNR9816_UART_MSG_CMD_ENTER_OTA_MODE    = 0x97
## 事件通知
SNR9816_UART_MSG_CMD_NOTIFY_STATUS     = 0x9A
SNR9816_UART_MSG_CMD_ACK_COMMON        = 0xAA
# if user want add please add form SNR9816_UART_MSG_CMD_USER_START
SNR9816_UART_MSG_CMD_USER_START        = 0xB0
# msg_data  msg_cmd:SNR9816_UART_MSG_CMD_PLAY_VOICE
SNR9816_UART_MSG_DATA_PLAY_START            = 0x80
SNR9816_UART_MSG_DATA_PLAY_PAUSE            = 0x81
SNR9816_UART_MSG_DATA_PLAY_RESUME           = 0x82
SNR9816_UART_MSG_DATA_PLAY_STOP             = 0x83
SNR9816_UART_MSG_DATA_PLAY_BY_VOICEID       = 0x90
SNR9816_UART_MSG_DATA_PLAY_BY_SEMANTIC_ID   = 0x91
SNR9816_UART_MSG_DATA_PLAY_BY_CMD_ID        = 0x92
# msg_data  msg_cmd:SNR9816_UART_MSG_CMD_GET_VERSION
## 串口协议版本号
SNR9816_UART_MSG_DATA_VER_PROTOCOL          = 0x80
## SDK版本号
SNR9816_UART_MSG_DATA_VER_SDK               = 0x81
## ASR组件版本号
SNR9816_UART_MSG_DATA_VER_ASR               = 0x82
## 语音预处理算法版本号
SNR9816_UART_MSG_DATA_VER_PREPROCESS        = 0x83
## 播放器版本号
SNR9816_UART_MSG_DATA_VER_PLAYER            = 0x84
## 应用程序版本号
SNR9816_UART_MSG_DATA_VER_APP               = 0x8A
# msg_data  msg_cmd:SNR9816_UART_MSG_CMD_NOTIFY_STATUS
SNR9816_UART_MSG_DATA_NOTIFY_POWERON        = 0xB0
SNR9816_UART_MSG_DATA_NOTIFY_WAKEUPENTER    = 0xB1
SNR9816_UART_MSG_DATA_NOTIFY_WAKEUPEXIT     = 0xB2
SNR9816_UART_MSG_DATA_NOTIFY_PLAYSTART      = 0xB3
SNR9816_UART_MSG_DATA_NOTIFY_PLAYEND        = 0xB4
# msg_data msg_cmd:SNR9816_UART_MSG_CMD_SET_CONFIG
SNR9816_UART_MSG_CMD_SET_VOLUME             = 0x80
SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP        = 0x81
SNR9816_UART_MSG_CMD_SET_PRT_MID_RST        = 0x82
SNR9816_UART_MSG_CMD_SET_MUTE               = 0x83
SNR9816_UART_MSG_CMD_SET_WAKE_TIME          = 0x84
SNR9816_UART_MSG_CMD_SET_NEEDACK            = 0x90
SNR9816_UART_MSG_CMD_SET_NEEDSTRING         = 0x91
# ACK error code
SNR9816_UART_MSG_ACK_ERR_NONE               = 0x00
SNR9816_UART_MSG_ACK_ERR_CHECKSUM           = 0xff
SNR9816_UART_MSG_ACK_ERR_NOSUPPORT          = 0xfe


class DFRobot_SNR9816(object):
  '''!
    @brief Define DFRobot_SNR9816 basic class
  '''

  def __init__(self):
    '''!
      @brief Module init
    '''
    pass


class DFRobot_SNR9816_I2C(DFRobot_SNR9816):
  '''!
    @brief Define DFRobot_SNR9816_I2C basic class
  '''

  def __init__(self, i2c_addr=SNR9816_I2C_ADDR, bus=1):
    '''!
      @brief Module I2C communication init
      @param i2c_addr - I2C communication address
      @param bus - I2C bus
    '''
    self._addr = i2c_addr
    self._i2c = smbus.SMBus(bus)
    super(DFRobot_SNR9816_I2C, self).__init__()

  def get_CMDID(self):
    '''!
      @brief 获取命令词对应的ID
      @return 返回获取的命令词ID, 返回0表示未获取到有效id
    '''
    return self._read_reg(SNR9816_I2C_REG_CMDID)

  def play_by_CMDID(self, CMDID):
    '''!
      @brief 根据命令词ID播放对应的答复音
      @param CMDID - 命令词ID
    '''
    self._write_reg(SNR9816_I2C_REG_PLAY_CMDID, CMDID)
    time.sleep(1)

  def get_wake_time(self):
    '''!
      @brief 获取唤醒持续时间
      @return 当前设置的唤醒周期
    '''
    return self._read_reg(SNR9816_I2C_REG_WAKE_TIME)

  def set_wake_time(self, wake_time):
    '''!
      @brief 设置唤醒持续时间
      @param wakeTime - 唤醒持续时间(0~255)
    '''
    wake_time = wake_time & 0xFF
    self._write_reg(SNR9816_I2C_REG_SET_WAKE_TIME, wake_time)

  def _write_reg(self, reg, data):
    '''!
      @brief writes data to a register
      @param reg - register address
      @param data - written data
    '''
    if isinstance(data, int):
      data = [data]
      #logger.info(data)
    buf = [0] * 3
    buf[0] = data[0]
    buf[1] = buf[0] + reg
    buf[2] = SNR9816_I2C_MSG_TAIL
    self._i2c.write_i2c_block_data(self._addr, reg, buf)

  def _read_reg(self, reg):
    '''!
      @brief read the data from the register
      @param reg - register address
      @return read data
    '''
    data = self._i2c.read_i2c_block_data(self._addr, reg, 3)
    if(((reg + data[0]) == data[1]) and (SNR9816_I2C_MSG_TAIL == data[2])):   # 验证数据是否无误
      return data[0]
    return 0


class DFRobot_SNR9816_UART(DFRobot_SNR9816):
  '''!
    @brief Define DFRobot_SNR9816_UART basic class
  '''

  REV_STATE_HEAD0   = 0x00
  REV_STATE_HEAD1   = 0x01
  REV_STATE_LENGTH0 = 0x02
  REV_STATE_LENGTH1 = 0x03
  REV_STATE_TYPE    = 0x04
  REV_STATE_CMD     = 0x05
  REV_STATE_SEQ     = 0x06
  REV_STATE_DATA    = 0x07
  REV_STATE_CKSUM0  = 0x08
  REV_STATE_CKSUM1  = 0x09
  REV_STATE_TAIL    = 0x0a

  class uart_msg():
    '''!
      @brief 串口数据帧 结构体  类
    '''
    def __init__(self):
      '''!
        @brief sensor_status structure init
      '''
      self.header = 0
      self.data_length = 0
      self.msg_type = 0
      self.msg_cmd = 0
      self.msg_seq = 0
      self.msg_data = [0] * 8

  def __init__(self):
    '''!
      @brief Module UART communication init
    '''
    self.uart_cmd_ID = 0
    self._send_sequence = 0
    self._ser = serial.Serial("/dev/ttyAMA0", baudrate=SNR9816_UART_BAUDRATE, bytesize=8, parity='N', stopbits=1, timeout=0.5)
    if self._ser.isOpen == False:
      self._ser.open()
    super(DFRobot_SNR9816_UART, self).__init__()

  def get_CMDID(self):
    '''!
      @brief 获取命令词对应的ID
      @return 返回获取的命令词ID, 返回0表示未获取到有效id
    '''
    self._recv_packet()
    temp = self.uart_cmd_ID
    self.uart_cmd_ID = 0
    return temp

  def play_by_CMDID(self, play_id):
    '''!
      @brief 根据命令词ID播放对应的答复音
      @param CMDID - 命令词ID
    '''
    msg = self.uart_msg()
    msg.header = SNR9816_UART_MSG_HEAD
    msg.data_length = 6
    msg.msg_type = SNR9816_UART_MSG_TYPE_CMD_DOWN
    msg.msg_cmd = SNR9816_UART_MSG_CMD_PLAY_VOICE
    msg.msg_seq = self._send_sequence
    self._send_sequence += 1
    msg.msg_data[0] = SNR9816_UART_MSG_DATA_PLAY_START
    msg.msg_data[1] = SNR9816_UART_MSG_DATA_PLAY_BY_CMD_ID
    msg.msg_data[2] = play_id

    self._send_packet(msg)
    time.sleep(1)

  def reset_module(self):
    '''!
      @brief 复位模块
    '''
    msg = self.uart_msg()
    msg.header = SNR9816_UART_MSG_HEAD
    msg.data_length = 5
    msg.msg_type = SNR9816_UART_MSG_TYPE_CMD_DOWN
    msg.msg_cmd = SNR9816_UART_MSG_CMD_RESET_MODULE
    msg.msg_seq = self._send_sequence
    self._send_sequence += 1
    msg.msg_data[0] = 'r'
    msg.msg_data[1] = 'e'
    msg.msg_data[2] = 's'
    msg.msg_data[3] = 'e'
    msg.msg_data[4] = 't'

    self._send_packet(msg)
    time.sleep(3)

  def setting_CMD(self, set_type, set_value):
    '''!
      @brief 设置模块的命令
      @param set_type - 设置类型
      @n       SNR9816_UART_MSG_CMD_SET_VOLUME : 设置音量, 设置值范围0~20
      @n       SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP : 进入唤醒状态; 设置值 0
      @n       SNR9816_UART_MSG_CMD_SET_MUTE : 静音模式; 设置值 1: mute, 0: unmute
      @n       SNR9816_UART_MSG_CMD_SET_WAKE_TIME : 唤醒持续时间; 设置值范围0~255秒
      @param set_value - 设置值, 范围参见上述设置类型
    '''
    msg = self.uart_msg()
    msg.header = SNR9816_UART_MSG_HEAD
    msg.data_length = 5
    msg.msg_type = SNR9816_UART_MSG_TYPE_CMD_DOWN
    msg.msg_cmd = SNR9816_UART_MSG_CMD_SET_CONFIG
    msg.msg_seq = self._send_sequence
    self._send_sequence += 1
    msg.msg_data[0] = set_type
    msg.msg_data[1] = set_value

    self._send_packet(msg)

  def _send_packet(self, msg):
    '''
      @brief Write data through UART
      @param msg - 待发送的数据包
    '''
    chk_sum = 0x0000
    data = []
    data.append(msg.header & 0xFF)
    data.append((msg.header >> 8) & 0xFF)
    data.append(msg.data_length & 0xFF)
    data.append((msg.data_length >> 8) & 0xFF)
    data.append(msg.msg_type & 0xFF)
    chk_sum += msg.msg_type
    data.append(msg.msg_cmd & 0xFF)
    chk_sum += msg.msg_cmd
    data.append(msg.msg_seq & 0xFF)
    chk_sum += msg.msg_seq
    for i in range(0, msg.data_length):
      data.append(msg.msg_data[i] & 0xFF)
      chk_sum += msg.msg_data[i]
    data.append(chk_sum & 0xFF)
    data.append((chk_sum >> 8) & 0xFF)
    data.append(SNR9816_UART_MSG_TAIL & 0xFF)
    logger.info(data)
    self._ser.write(data)
    time.sleep(0.1)

  def _recv_packet(self):
    '''
      @brief Read data through UART
      @param msg - 接收数据包的缓冲区
    '''
    msg = self.uart_msg()
    rev_state = self.REV_STATE_HEAD0
    receive_char = 0
    chk_sum = 0
    data_rev_count = 0
    while self._ser.in_waiting:
      receive_char = ord(self._ser.read(1))
      if(self.REV_STATE_HEAD0 == rev_state):
        if(SNR9816_UART_MSG_HEAD_LOW == receive_char):
          rev_state = self.REV_STATE_HEAD1
      elif(self.REV_STATE_HEAD1 == rev_state):
        if(SNR9816_UART_MSG_HEAD_HIGH == receive_char):
          rev_state = self.REV_STATE_LENGTH0
          msg.header = SNR9816_UART_MSG_HEAD
        else:
          rev_state = self.REV_STATE_HEAD0
      elif(self.REV_STATE_LENGTH0 == rev_state):
        msg.data_length = receive_char
        rev_state = self.REV_STATE_LENGTH1
      elif(self.REV_STATE_LENGTH1 == rev_state):
        msg.data_length += receive_char << 8
        rev_state = self.REV_STATE_TYPE
      elif(self.REV_STATE_TYPE == rev_state):
        msg.msg_type = receive_char
        rev_state = self.REV_STATE_CMD
      elif(self.REV_STATE_CMD == rev_state):
        msg.msg_cmd = receive_char
        rev_state = self.REV_STATE_SEQ
      elif(self.REV_STATE_SEQ == rev_state):
        msg.msg_seq = receive_char
        rev_state = rev_state
        if(msg.data_length > 0):
          rev_state = self.REV_STATE_DATA
          data_rev_count = 0
        else:
          rev_state = self.REV_STATE_CKSUM0
      elif(self.REV_STATE_DATA == rev_state):
        msg.msg_data[data_rev_count] = receive_char
        data_rev_count += 1
        if(msg.data_length == data_rev_count):
          rev_state = self.REV_STATE_CKSUM0
      elif(self.REV_STATE_CKSUM0 == rev_state):
        chk_sum = receive_char
        rev_state = self.REV_STATE_CKSUM1
      elif(self.REV_STATE_CKSUM1 == rev_state):
        chk_sum += receive_char << 8

        rev_state = self.REV_STATE_TAIL
      elif(self.REV_STATE_TAIL == rev_state):
        if(SNR9816_UART_MSG_TAIL == receive_char):
          if(SNR9816_UART_MSG_TYPE_CMD_UP == msg.msg_type):
            self.uart_cmd_ID = msg.msg_data[0]
        else:
          data_rev_count = 0
        rev_state = self.REV_STATE_HEAD0
      else:
        rev_state = self.REV_STATE_HEAD0

    return data_rev_count


