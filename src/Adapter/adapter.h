/*
 * adapter.h
 * SmartOCD
 *
 *  Created on: 2018-1-29
 *      Author: virusv
 */

#ifndef SRC_ADAPTER_INCLUDE_ADAPTER_H_
#define SRC_ADAPTER_INCLUDE_ADAPTER_H_

#include "Library/misc/list.h"
#include "smartocd.h"

/**
 * Adapter 接口状态码
 */
enum {
  ADPT_SUCCESS = 0,         // 成功
  ADPT_FAILED,              // 失败
  ADPT_ERR_TRANSPORT_ERROR, // 底层传输错误
  ADPT_ERR_PROTOCOL_ERROR,  // 传输协议错误
  ADPT_ERR_NO_DEVICE,       // 找不到设备
  ADPT_ERR_UNSUPPORT,       // 不支持的操作
  ADPT_ERR_INTERNAL_ERROR, // 内部错误,不是由于Adapter功能部分造成的失败
  ADPT_ERR_BAD_PARAMETER, // 无效的参数
};

/* 仿真器对象 */
typedef struct adapter *Adapter;

// 仿真器的状态
enum adapterStatus {
  ADPT_STATUS_CONNECTED,  // Adapter已连接
  ADPT_STATUS_DISCONNECT, // Adapter已断开
  ADPT_STATUS_RUNING,     // Adapter正在运行
  ADPT_STATUS_IDLE        // Adapter空闲
};

/**
 * Status - 设置仿真器状态
 * 如果仿真器有状态指示灯的话,则可以通过这个状态来控制这些指示灯
 * 参数:
 * 	self:Adapter对象自身
 * 	status:仿真器的状态
 * 返回:
 * 	ADPT_SUCCESS:成功
 * 	ADPT_FAILED:失败
 * 	或者其他错误
 */
typedef int (*ADPT_SET_STATUS)(IN Adapter self, IN enum adapterStatus status);

/**
 * SetFrequent - 设置仿真器的工作频率
 * 参数:
 * 	self:Adapter对象自身
 * 	clock:工作频率(Hz)
 * 返回:
 * 	ADPT_SUCCESS:成功
 * 	ADPT_FAILED:失败
 * 	或者其他错误
 */
typedef int (*ADPT_SET_FREQUENT)(IN Adapter self, IN unsigned int freq);

/**
 * target复位类型
 */
enum targetResetType {
  ADPT_RESET_SYSTEM_RESET, // 全部系统复位
  ADPT_RESET_DEBUG_RESET,  // 调试系统复位
};

/**
 * Reset - 仿真器复位
 * 参数:
 * 	self:Adapter对象自身
 * 	type:目标复位类型
 * 返回:
 * 	ADPT_SUCCESS:成功
 * 	ADPT_FAILED:失败
 * 	或者其他错误
 */
typedef int (*ADPT_RESET)(IN Adapter self, IN enum targetResetType type);

/**
 * 仿真器传输模式
 */
enum transferMode {
  // Joint Test Action Group，联合测试行动组
  ADPT_MODE_JTAG,
  // Serial wire Debug 串行调试
  ADPT_MODE_SWD,
  // TODO 其他传输模式,SWIM,UART,TCP等
  ADPT_MODE_MAX
};

/**
 * 仿真器能力类型
 */
enum capabilityType {
  // CMSIS-DAP或者STLINK等针对ARM ADI的仿真器
  ADPT_CAP_DAP,
  // 传统仿真器的JTAG接口
  ADPT_CAP_JTAG,
  ADPT_CAP_MAX
};
/**
 * 仿真器能力集
 * 设备所支持的能力
 */
struct capability {
  enum capabilityType type;    // 能力集类型
  struct list_head capabilities; // 传输模式链表
};

/**
 * SetTransferMode - 设置传输类型:DAP还是JTAG
 * 参数:
 *	self:Adapter对象自身
 *	mode:仿真器工作模式,DAP还是JTAG
 * 返回:
 * 	ADPT_SUCCESS:成功
 * 	ADPT_FAILED:失败
 * 	或者其他错误
 */
typedef int (*ADPT_SET_TRANSFER_MODE)(IN Adapter self,
                                      IN enum transferMode mode);

/**
 * Adapter接口对象
 */
struct adapter {
  /* 属性 */
  const enum transferMode currTransMode; // 当前传输方式
  struct list_head capabilities; // 设备支持的能力集列表

  /* 服务 */
  ADPT_SET_STATUS SetStatus;              // 仿真器状态指示
  ADPT_SET_FREQUENT SetFrequent;          // 设置仿真器的工作频率
  ADPT_RESET Reset;                       // 仿真器复位
  ADPT_SET_TRANSFER_MODE SetTransferMode; // 设置传输类型:DAP还是JTAG
};

#endif /* SRC_ADAPTER_ADAPTER_H_ */
