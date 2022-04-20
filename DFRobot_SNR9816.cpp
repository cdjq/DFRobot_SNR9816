/*!
 * @file DFRobot_SNR9816.h
 * @brief Implementation of DFRobot_SNR9816 class
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-02
 * @url  https://github.com/DFRobot/DFRobot_SNR9816
 */
#include "DFRobot_SNR9816.h"

DFRobot_SNR9816::DFRobot_SNR9816()
{
}

/**************************************************************************
                          使用 I2C 接口通信的子类
 **************************************************************************/

DFRobot_SNR9816_I2C::DFRobot_SNR9816_I2C(TwoWire *pWire, uint8_t i2cAddr)
{
  _pWire = pWire;
  _deviceAddr = i2cAddr;
}

bool DFRobot_SNR9816_I2C::begin()
{
  _pWire->begin();   // Wire.h(I2C)library function initialize wire library
  return true;
}

uint8_t DFRobot_SNR9816_I2C::getCMDID(void)
{
  uint8_t CMDID = 0;
  readReg(SNR9816_I2C_REG_CMDID, &CMDID);
  // DBG(CMDID);
  return CMDID;
}

void DFRobot_SNR9816_I2C::playByCMDID(uint8_t CMDID)
{
  writeReg(SNR9816_I2C_REG_PLAY_CMDID, &CMDID);
  delay(1000);
}

uint8_t DFRobot_SNR9816_I2C::getWakeTime(void)
{
  uint8_t wakeTime = 0;
  readReg(SNR9816_I2C_REG_WAKE_TIME, &wakeTime);
  return wakeTime;
}

void DFRobot_SNR9816_I2C::setWakeTime(uint8_t wakeTime)
{
  writeReg(SNR9816_I2C_REG_WAKE_TIME, &wakeTime);
}

void DFRobot_SNR9816_I2C::setVolume(uint8_t vol)
{
  if (vol < 0)
    vol = 0;
  else if (vol > 20)
    vol = 20;
  writeReg(SNR9816_I2C_REG_SET_VOLUME, &vol);
}

void DFRobot_SNR9816_I2C::setMuteMode(uint8_t mode)
{
  if (0 != mode)
    mode = 1;
  writeReg(SNR9816_I2C_REG_SET_MUTE, &mode);
}

void DFRobot_SNR9816_I2C::writeReg(uint8_t reg, const void* pBuf)
{
  if(pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;

  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(reg);
  _pWire->write(*_pBuf);
  _pWire->endTransmission();
}

size_t DFRobot_SNR9816_I2C::readReg(uint8_t reg, void* pBuf)
{
  if(NULL == pBuf) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t*)pBuf;

  size_t count = 0;

  _pWire->beginTransmission(_deviceAddr);
  _pWire->write(reg);
  if(0 != _pWire->endTransmission(false)) {   // Used Wire.endTransmission() to end a slave transmission started by beginTransmission() and arranged by write().
    DBG("endTransmission ERROR!!");

  } else {
    _pWire->requestFrom(_deviceAddr, (uint8_t)1);   // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()

    while (_pWire->available()) {
      _pBuf[count++] = _pWire->read();   // Use read() to receive and put into buf
      // DBG(_pBuf[count-1], HEX);
    }
    // _pWire->endTransmission();
  }

  return count;
}

/**************************************************************************
                          使用 UART 接口通信的子类
 **************************************************************************/

DFRobot_SNR9816_UART::DFRobot_SNR9816_UART(HardwareSerial *hardSerial)
{
  hardSerial->begin(SNR9816_UART_BAUDRATE);
  _ser = hardSerial;
}

#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
DFRobot_SNR9816_UART::DFRobot_SNR9816_UART(SoftwareSerial *softSerial)
{
  softSerial->begin(SNR9816_UART_BAUDRATE);
  _ser = softSerial;
}
#elif defined(ESP32)
DFRobot_SNR9816_UART::DFRobot_SNR9816_UART(HardwareSerial *hardSerial, uint8_t rx, uint8_t tx)
{
  hardSerial->begin(SNR9816_UART_BAUDRATE, SERIAL_8N1, rx, tx);
  _ser = hardSerial;
}
#endif

bool DFRobot_SNR9816_UART::begin()
{
  sendSequence = 0;

  if(NULL == _ser){
    return false;
  }
  return true;
}

uint8_t DFRobot_SNR9816_UART::getCMDID(void)
{
  sUartMsg_t msg;
  recvPacket(&msg);
  uint8_t temp = uartCmdId;
  uartCmdId = 0;
  return temp;
}

void DFRobot_SNR9816_UART::playByCMDID(uint32_t play_id)
{
    sUartMsg_t msg;

    msg.header = SNR9816_UART_MSG_HEAD;
    msg.dataLength = 6;
    msg.msgType = SNR9816_UART_MSG_TYPE_CMD_DOWN;
    msg.msgCmd = SNR9816_UART_MSG_CMD_PLAY_VOICE;
    msg.msgSeq = sendSequence++;
    msg.msgData[0] = SNR9816_UART_MSG_DATA_PLAY_START;
    msg.msgData[1] = SNR9816_UART_MSG_DATA_PLAY_BY_CMD_ID;
    memcpy(&msg.msgData[2], &play_id, sizeof(uint32_t));

    sendPacket(&msg);
    delay(1000);
}

void DFRobot_SNR9816_UART::resetModule(void)
{
    sUartMsg_t msg;

    msg.header = SNR9816_UART_MSG_HEAD;
    msg.dataLength = 5;
    msg.msgType = SNR9816_UART_MSG_TYPE_CMD_DOWN;
    msg.msgCmd = SNR9816_UART_MSG_CMD_RESET_MODULE;
    msg.msgSeq = sendSequence++;
    msg.msgData[0] = 'r';
    msg.msgData[1] = 'e';
    msg.msgData[2] = 's';
    msg.msgData[3] = 'e';
    msg.msgData[4] = 't';

    sendPacket(&msg);
    delay(3000);
}

void DFRobot_SNR9816_UART::settingCMD(uint8_t setType, uint32_t setValue)
{
    sUartMsg_t msg;

    msg.header = SNR9816_UART_MSG_HEAD;
    msg.dataLength = 5;
    msg.msgType = SNR9816_UART_MSG_TYPE_CMD_DOWN;
    msg.msgCmd = SNR9816_UART_MSG_CMD_SET_CONFIG;
    msg.msgSeq = sendSequence++;
    msg.msgData[0] = setType;
    memcpy(&msg.msgData[1], &setValue, sizeof(setValue));

    sendPacket(&msg);
}

void DFRobot_SNR9816_UART::sendPacket(const sUartMsg_t * msg)
{
  if(NULL == msg) {
    DBG("msg ERROR!! : null pointer");
  }
  uint8_t *_pBuf = (uint8_t *)msg;

  while (_ser->available()) {
    _ser->read();
  }

  int i = 0;
  /*header and data*/
  for(i; i<4; i++) {
    _ser->write( _pBuf[i] );
  }

  uint16_t chkSum = 0;
  for(i; i<(msg->dataLength + 7); i++) {
    chkSum += _pBuf[i];
    _ser->write( _pBuf[i] );
  }

  /*check sum*/
  _ser->write( chkSum & 0xff );
  _ser->write( (chkSum >> 8) & 0xff );

  /*tail*/
  _ser->write( SNR9816_UART_MSG_TAIL );

  delay(100);
}

uint8_t DFRobot_SNR9816_UART::recvPacket(sUartMsg_t * msg)
{
  if(NULL == msg) {
    DBG("msg ERROR!! : null pointer");
  }

  uint8_t revState = REV_STATE_HEAD0, receiveChar;
  uint16_t length0 = 0, length1 = 0;
  uint16_t chkSum0 = 0, chkSum1 = 0;
  uint16_t dataRevCount = 0;

  while(_ser->available()) {
    receiveChar = _ser->read();
    switch(revState) {
      case REV_STATE_HEAD0:
        if(SNR9816_UART_MSG_HEAD_LOW == receiveChar) {
          revState = REV_STATE_HEAD1;
        }
        break;
      case REV_STATE_HEAD1:
        if(SNR9816_UART_MSG_HEAD_HIGH == receiveChar) {
          revState = REV_STATE_LENGTH0;
          msg->header = SNR9816_UART_MSG_HEAD;
        } else {
          if(SNR9816_UART_MSG_HEAD_LOW != receiveChar) {
            revState = REV_STATE_HEAD0;
          }
        }
        break;
      case REV_STATE_LENGTH0:
        length0 = receiveChar;
        revState = REV_STATE_LENGTH1;
        break;
      case REV_STATE_LENGTH1:
        length1 = receiveChar;
        length1 <<= 8;
        length1 += length0;
        if(length1 <= SNR9816_UART_MSG_DATA_MAX_SIZE) {
            msg->dataLength = length1;
            revState = REV_STATE_TYPE;
        } else {
            revState = REV_STATE_HEAD0;
        }
        break;
      case REV_STATE_TYPE:
        msg->msgType = receiveChar;
        revState = REV_STATE_CMD;
        break;
      case REV_STATE_CMD:
        msg->msgCmd = receiveChar;
        revState = REV_STATE_SEQ;
        break;
      case REV_STATE_SEQ:
        msg->msgSeq = receiveChar;
        if(length1 > 0) {
            revState = REV_STATE_DATA;
            dataRevCount = 0;
        } else {
            revState = REV_STATE_CKSUM0;
        }
        break;
      case REV_STATE_DATA:
        msg->msgData[dataRevCount] = receiveChar;
        dataRevCount += 1;
        if(dataRevCount == length1) {
            revState = REV_STATE_CKSUM0;
        }
        break;
      case REV_STATE_CKSUM0:
        chkSum0 = receiveChar;
        revState = REV_STATE_CKSUM1;
        break;
      case REV_STATE_CKSUM1:
      {
        uint16_t packetChkSum = 0;
        uint8_t *data = (uint8_t *)msg;
        for(int i=4; i<(msg->dataLength + 7); i++) {
          packetChkSum += data[i];
        }

        chkSum1 = receiveChar;
        chkSum1 <<= 8;
        chkSum1 += chkSum0;

        /*msg->chksum = chkSum1; just used as judgement*/
        if(chkSum1 == packetChkSum) {
          revState = REV_STATE_TAIL;
        } else {
          revState = REV_STATE_HEAD0;
        }
        break;
      }
      case REV_STATE_TAIL:
        if(receiveChar == SNR9816_UART_MSG_TAIL) {   /*receive ok*/
          /*msg->tail = receiveChar; just used as judgement*/
          if(SNR9816_UART_MSG_TYPE_CMD_UP == msg->msgType) {
            uartCmdId = msg->msgData[0];
    DBG(uartCmdId, HEX);
          }

        } else {
          dataRevCount = 0;
        }
        revState = REV_STATE_HEAD0;
        break;
      default:
        revState = REV_STATE_HEAD0;
        break;
    }
  }

  return dataRevCount;
}
