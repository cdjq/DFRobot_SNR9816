/*!
 * @file  DFRobot_SNR9816.h
 * @brief  Basic structure of DFRobot_SNR9816 class
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-02
 * @url  https://github.com/DFRobot/DFRobot_SNR9816
*/
#ifndef DFRobot_SNR9816_H
#define DFRobot_SNR9816_H

#include <Arduino.h>
#include <Wire.h>
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
#include <SoftwareSerial.h>
#endif

// #define ENABLE_DBG   //!< Open this macro and you can see the details of the program
#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif

#define SNR9816_I2C_ADDR                 uint8_t(0x64)   //!< i2c address

#define SNR9816_I2C_REG_CMDID            uint8_t(0x02)   //!< 请求得到命令词ID的寄存器地址
#define SNR9816_I2C_REG_PLAY_CMDID       uint8_t(0x03)   //!< 用命令词ID播放Audio的寄存器地址
#define SNR9816_I2C_REG_SECID            uint8_t(0x04)   //!< 请求得到语义ID的寄存器地址
#define SNR9816_I2C_REG_PLAY_SECID       uint8_t(0x05)   //!< 用语义ID播放Audio的寄存器地址
#define SNR9816_I2C_REG_WAKE_TIME        uint8_t(0x06)   //!< 请求得到当前唤醒时间的寄存器地址
#define SNR9816_I2C_REG_SET_WAKE_TIME    uint8_t(0x07)   //!< 请求修改当前唤醒时间的寄存器地址

#define SNR9816_I2C_MSG_TAIL             uint8_t(0x5A)

#define SNR9816_UART_BAUDRATE               long(9600)   //!< UART baud rate
#define SNR9816_UART_MSG_DATA_MAX_SIZE      uint8_t(8)   //!< 串口数据帧的最大数据长度

/*header*/
#define SNR9816_UART_MSG_HEAD_LOW    uint8_t(0xF4)
#define SNR9816_UART_MSG_HEAD_HIGH   uint8_t(0xF5)
#define SNR9816_UART_MSG_HEAD        ((SNR9816_UART_MSG_HEAD_HIGH<<8)|SNR9816_UART_MSG_HEAD_LOW)
/*tail*/
#define SNR9816_UART_MSG_TAIL        uint8_t(0xFB)
/*msgType*/
#define SNR9816_UART_MSG_TYPE_CMD_UP          uint8_t(0xA0)
#define SNR9816_UART_MSG_TYPE_CMD_DOWN        uint8_t(0xA1)
#define SNR9816_UART_MSG_TYPE_ACK             uint8_t(0xA2)
#define SNR9816_UART_MSG_TYPE_NOTIFY          uint8_t(0xA3)
/*msgCmd*/
#define SNR9816_UART_MSG_CMD_ASR_RESULT       uint8_t(0x91)           //报告语音识别结果
#define SNR9816_UART_MSG_CMD_PLAY_VOICE       uint8_t(0x92)           //播放本地播报音
#define SNR9816_UART_MSG_CMD_GET_FLASHUID     uint8_t(0x93)           //读取FLASH的序列号
#define SNR9816_UART_MSG_CMD_GET_VERSION      uint8_t(0x94)           //读取版本号
#define SNR9816_UART_MSG_CMD_RESET_MODULE     uint8_t(0x95)           //复位语音模块
#define SNR9816_UART_MSG_CMD_SET_CONFIG       uint8_t(0x96)           //设置
#define SNR9816_UART_MSG_CMD_ENTER_OTA_MODE   uint8_t(0x97)          //进入升级模式
#define SNR9816_UART_MSG_CMD_NOTIFY_STATUS    uint8_t(0x9A)           //事件通知
#define SNR9816_UART_MSG_CMD_ACK_COMMON       uint8_t(0xAA)
/* !!! if user want add please add form SNR9816_UART_MSG_CMD_USER_START*/
#define SNR9816_UART_MSG_CMD_USER_START       uint8_t(0xB0)
/*msgData  msgCmd:SNR9816_UART_MSG_CMD_PLAY_VOICE*/
#define SNR9816_UART_MSG_DATA_PLAY_START      uint8_t(0x80)
#define SNR9816_UART_MSG_DATA_PLAY_PAUSE      uint8_t(0x81)
#define SNR9816_UART_MSG_DATA_PLAY_RESUME     uint8_t(0x82)
#define SNR9816_UART_MSG_DATA_PLAY_STOP       uint8_t(0x83)
#define SNR9816_UART_MSG_DATA_PLAY_BY_VOICEID       uint8_t(0x90)
#define SNR9816_UART_MSG_DATA_PLAY_BY_SEMANTIC_ID   uint8_t(0x91)
#define SNR9816_UART_MSG_DATA_PLAY_BY_CMD_ID        uint8_t(0x92)
/*msgData  msg_cmd:SNR9816_UART_MSG_CMD_GET_VERSION*/
#define SNR9816_UART_MSG_DATA_VER_PROTOCOL    uint8_t(0x80)         // 串口协议版本号
#define SNR9816_UART_MSG_DATA_VER_SDK         uint8_t(0x81)         // SDK版本号
#define SNR9816_UART_MSG_DATA_VER_ASR         uint8_t(0x82)         // ASR组件版本号
#define SNR9816_UART_MSG_DATA_VER_PREPROCESS  uint8_t(0x83)         // 语音预处理算法版本号
#define SNR9816_UART_MSG_DATA_VER_PLAYER      uint8_t(0x84)         // 播放器版本号
#define SNR9816_UART_MSG_DATA_VER_APP         uint8_t(0x8A)         // 应用程序版本号
/*msgData  msg_cmd:SNR9816_UART_MSG_CMD_NOTIFY_STATUS*/
#define SNR9816_UART_MSG_DATA_NOTIFY_POWERON       uint8_t(0xB0)
#define SNR9816_UART_MSG_DATA_NOTIFY_WAKEUPENTER   uint8_t(0xB1)
#define SNR9816_UART_MSG_DATA_NOTIFY_WAKEUPEXIT    uint8_t(0xB2)
#define SNR9816_UART_MSG_DATA_NOTIFY_PLAYSTART     uint8_t(0xB3)
#define SNR9816_UART_MSG_DATA_NOTIFY_PLAYEND       uint8_t(0xB4)
/*msgData msg_cmd:SNR9816_UART_MSG_CMD_SET_CONFIG*/
#define SNR9816_UART_MSG_CMD_SET_VOLUME        uint8_t(0x80)
#define SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP   uint8_t(0x81)
#define SNR9816_UART_MSG_CMD_SET_PRT_MID_RST   uint8_t(0x82)
#define SNR9816_UART_MSG_CMD_SET_MUTE          uint8_t(0x83)
#define SNR9816_UART_MSG_CMD_SET_WAKE_TIME     uint8_t(0x84)
#define SNR9816_UART_MSG_CMD_SET_NEEDACK       uint8_t(0x90)
#define SNR9816_UART_MSG_CMD_SET_NEEDSTRING    uint8_t(0x91)
/*ACK error code*/
#define SNR9816_UART_MSG_ACK_ERR_NONE          uint8_t(0x0)
#define SNR9816_UART_MSG_ACK_ERR_CHECKSUM      uint8_t(0xff)
#define SNR9816_UART_MSG_ACK_ERR_NOSUPPORT     uint8_t(0xfe)

class DFRobot_SNR9816
{
public:
  /**
   * @fn DFRobot_SNR9816
   * @brief Constructor 
   * @return None
   */
  DFRobot_SNR9816();

  /**
   * @fn begin
   * @brief Init function
   * @return bool type, true if successful, false if error
   */
  virtual bool begin(void) = 0;

private:

};

/**************************************************************************
                          使用 I2C 接口通信的子类
 **************************************************************************/

class DFRobot_SNR9816_I2C: public DFRobot_SNR9816
{
public:
  /**
   * @fn DFRobot_SNR9816_I2C
   * @brief Constructor 
   * @param pWire I2C BUS pointer object， construct device, can pass parameter or not, default to Wire
   * @param address 7bits I2C address, the first three bits determine the value of the address, default to 0x50
   * @return None
   */
  DFRobot_SNR9816_I2C(TwoWire *pWire=&Wire, uint8_t i2cAddr=SNR9816_I2C_ADDR);

  /**
   * @fn begin
   * @brief Subclass init function
   * @return bool type, true if successful, false if error
   */
  virtual bool begin(void);

  /**
   * @fn getCMDID
   * @brief 获取命令词对应的ID
   * @return 返回获取的命令词ID, 返回0表示未获取到有效id
   */
  uint8_t getCMDID(void);

  /**
   * @fn playByCMDID
   * @brief 根据命令词ID播放对应的答复音
   * @param CMDID - 命令词ID
   * @return None
   */
  void playByCMDID(uint8_t CMDID);

  /**
   * @fn getWakeTime
   * @brief 获取唤醒持续时间
   * @return 当前设置的唤醒周期
   */
  uint8_t getWakeTime(void);

  /**
   * @fn setWakeTime
   * @brief 设置唤醒持续时间
   * @param wakeTime - 唤醒持续时间(0~255)
   * @return None
   */
  void setWakeTime(uint8_t wakeTime);

protected:
  /**
   * @fn playBySECID
   * @brief 获取命令词对应的语义ID
   * @return None
   */
  uint8_t getSECID(void);

  /**
   * @fn playBySECID
   * @brief 根据命令词语义ID播放对应的答复音
   * @param SECID - 命令词语义ID
   * @return None
   */
  void playBySECID(uint8_t SECID);

  /**
   * @fn writeReg
   * @brief Write register value through I2C bus
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be written
   * @param size Length of data to be written
   * @return None
   */
  void writeReg(uint8_t reg, const void* pBuf, size_t size);

  /**
   * @fn readReg
   * @brief Read register value through I2C bus
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be read
   * @param size Length of data to be read
   * @return Return the read length, returning 0 means reading failed
   */
  size_t readReg(uint8_t reg, void* pBuf, size_t size);

private:
  TwoWire *_pWire;   // Pointer to I2C communication method
  uint8_t _deviceAddr;   // Address of the device for I2C communication
};

/**************************************************************************
                          使用 UART 接口通信的子类
 **************************************************************************/

class DFRobot_SNR9816_UART: public DFRobot_SNR9816
{
public:
  #pragma pack(1)
  /**
   * @struct sUartMsg_t
   * @brief 串口数据帧 结构体
   */
  typedef struct
  {
    uint16_t header;
    uint16_t dataLength;
    uint8_t msgType;
    uint8_t msgCmd;
    uint8_t msgSeq;
    uint8_t msgData[SNR9816_UART_MSG_DATA_MAX_SIZE];
    /*uint16_t chkSum; send add auto*/
    /*uint8_t tail; send add auto*/
  }sUartMsg_t;

  /**
   * @enum eReceiveState_t
   * @brief receive use state machine method, so two char can arrive different time
   */
  typedef enum
  {
    REV_STATE_HEAD0   = 0x00,
    REV_STATE_HEAD1   = 0x01,
    REV_STATE_LENGTH0 = 0x02,
    REV_STATE_LENGTH1 = 0x03,
    REV_STATE_TYPE    = 0x04,
    REV_STATE_CMD     = 0x05,
    REV_STATE_SEQ     = 0x06,
    REV_STATE_DATA    = 0x07,
    REV_STATE_CKSUM0  = 0x08,
    REV_STATE_CKSUM1  = 0x09,
    REV_STATE_TAIL    = 0x0a,
  }eRecvState_t;

public:
  /**
   * @fn DFRobot_SNR9816_UART
   * @brief Constructor 
   * @param serial - serial ports for communication, supporting hard and soft serial ports
   * @param rx - UART 接收数据的引脚
   * @param tx - UART 发送数据的引脚
   * @return None
   */
  DFRobot_SNR9816_UART(HardwareSerial *hardSerial);
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
  DFRobot_SNR9816_UART(SoftwareSerial *softSerial);
#elif defined(ESP32)
  DFRobot_SNR9816_UART(HardwareSerial *hardSerial, uint8_t rx, uint8_t tx);
#endif

  /**
   * @fn begin
   * @brief Subclass init function
   * @return bool type, true if successful, false if error
   */
  virtual bool begin();

  /**
   * @fn getCMDID
   * @brief 获取命令词对应的ID
   * @return 返回获取的命令词ID, 返回0表示未获取到有效id
   */
  uint8_t getCMDID(void);

  /**
   * @fn playByCMDID
   * @brief 根据命令词id播放答复音频
   * @param play_id - 命令词id
   * @return None
   */
  void playByCMDID(uint32_t play_id);

  /**
   * @fn resetModule
   * @brief 复位模块
   * @return None
   */
  void resetModule(void);

  /**
   * @fn settingCMD
   * @brief 设置模块的命令
   * @param setType - 设置类型
   * @n       SNR9816_UART_MSG_CMD_SET_VOLUME : 设置音量, 设置值范围0~20
   * @n       SNR9816_UART_MSG_CMD_SET_ENTERWAKEUP : 进入唤醒状态; 设置值 0
   * @n       SNR9816_UART_MSG_CMD_SET_MUTE : 静音模式; 设置值 1: mute, 0: unmute
   * @n       SNR9816_UART_MSG_CMD_SET_WAKE_TIME : 唤醒持续时间; 设置值范围0~255秒
   * @param setValue - 设置值, 范围参见上述设置类型
   * @return None
   */
  void settingCMD(uint8_t setType, uint32_t setValue);

protected:
  /**
   * @fn setVolume
   * @brief Set voice volume
   * @param voc - Volume value(0-9)
   * @return None
   */
  void setVolume(uint8_t voc);

  /**
   * @fn setVolume
   * @brief Set voice volume
   * @param voc - Volume value(0-9)
   * @return None
   */
  void getUniqueID(void);

  /**
   * @fn sendPacket
   * @brief Write data through UART
   * @param msg - 待发送的数据包
   * @return None
   */
  void sendPacket(const sUartMsg_t * msg);

  /**
   * @fn recvPacket
   * @brief Read data through UART
   * @param msg - 接收数据包的缓冲区
   * @return Return the read length, returning 0 means reading failed
   */
  uint8_t recvPacket(sUartMsg_t * msg);

private:
  Stream *_ser;
  uint8_t sendSequence, uartCmdId;
};

#endif
