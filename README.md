# SmartOCD

## 简介

SmartOCD(Smart On-Chip Debugger)是用于ARMv8架构的交互式片外调试工具。目的是使用它来更方便地调试Uboot，UEFI程序或操作系统内核。

它可以使用Lua脚本语言编程，通过DAP协议和MEM-AP与CoreSight组件进行交互，或者直接读写总线上挂载的设备（ROM、RAM或Peripherals）等。

*此项目为实验性项目，开发它只是为了更好的去理解和学习底层的一些东西。*

目前支持的仿真器有：
- CMSIS-DAP
+ USB Blaster *（即将支持）*
+ J-Link *（即将支持）*

## DAP

> The Debug Access Port(DAP) is an implementation of an ARM Debug Interface version 5.1 (ADIv5.1) comprising a number of components supplied in a single configuration.  
> All the supplied components fit into the various architectural components for Debug Ports(DPs), which are used to access the DAP from an external debugger and Access Ports (APs), to access on-chip system resources.  
> The DAP provides **real-time** access for the debugger without halting the processor to:
> * AMBA system memory and peripheral registers.
> * All debug configuration registers.

简单来说，DAP是仿真器和被调试芯片之间的桥梁。它由DP和AP两个部分组成：
- DP：仿真器通过DP来访问DAP，它一般有三类：
	- SW-DP：通过SW（Serial Wire）协议访问DAP  
	注：SW协议只存在于ADIv5以及以上版本中
	- JTAG-DP：通过JTAG协议访问DAP
	- SWJ-DP：通过SWD/JTAG协议访问DAP
- AP：DAP通过AP来访问片上资源，它一般有四种：
	- JTAG-AP：片上TAP控制器的DAP接口，用来访问SoC中的JTAG设备。它作为JTAG Master驱动JTAG Chain
	- AHB-AP：AHB System的DAP接口，可以访问AHB总线上的设备。
	- APB-AP：Debug APB的DAP接口，一般用它来访问调试组件。
	- AXI-AP：AXI总线的DAP接口。

## CoreSight

> Modern SoCs are continually increasing in complexity and packing in more and more features.  Large designs with mul-tiple CPU architectures, GPUs, numerous power domains, various DVFS regions, big.LITTLE™ and advanced security schemes are very common. This in turn adds a big overhead for silicon bring-up and software devel-opment/optimization while at the same time designers are under pressure to keep costs down.  
> CoreSight, ARM’s debug and trace product family, is the most complete on-chip debug and trace solution for the entire System-On-Chip (SoC), making ARM-based SoCs the easiest to debug and optimize.   
> 
> **Key Benefits**
> - Higher visibility of complete system operation through fewer pins
> - Standard solution across all silicon vendors for widest tools support
> - Re-usable for single ARM core, multi-core or core and DSP systems
> - Enables faster time-to-market for more reliable and higher performance products
> - Supports for high performance, low power SoCs

CoreSight是面对越来越复杂、功能越来越多的片上系统而提出的Debug & Trace的解决方案。CoreSight包含有许多调试组件，这些组件与CoreSight架构一起构成了CoreSight系统。

## JTAG、SWD
JTAG和SWD是仿真器与被调试芯片之间两种不同的通信方式。JTAG遵循IEEE 1149.7，主要用于芯片内部测试，属于一种比较“古老”的协议，多用在DSP、FPGA和较早系列的ARM中。SWD是串行调试协议，它是在ADIv5（ARM Debug Interface version 5）标准中提出来的一种串行协议。SWD比JTAG在高速模式下面更加可靠，比JTAG所占用的引脚资源更少。所以推荐使用SWD。

## Lua
1993 年在巴西里约热内卢天主教大学(Pontifical Catholic University of Rio de Janeiro in Brazil)诞生了一门编程语言，发明者是该校的三位研究人员，他们给这门语言取了个浪漫的名字——Lua，在葡萄牙语里代表美丽的月亮。事实证明她没有糟蹋这个优美的单词，Lua 语言正如它名字所预示的那样成长为一门简洁、优雅且富有乐趣的语言。

Lua 从一开始就是作为一门方便嵌入(到其它应用程序)并可扩展的轻量级脚本语言来设计的，因此她一直遵从着简单、小巧、可移植、快速的原则，官方实现完全采用 ANSI C 编写，能以 C 程序库的形式嵌入到宿主程序中。标准 Lua 5.1 解释器采用的是著名的 MIT 许可协议。正由于上述特点，所以 Lua 在游戏开发、机器人控制、分布式应用、图像处理、生物信息学等各种各样的领域中得到了越来越广泛的应用。其中尤以游戏开发为最，许多著名的游戏，比如 Escape from Monkey Island、World of Warcraft、大话西游，都采用了 Lua 来配合引擎完成数据描述、配置管理和逻辑控制等任务。即使像 Redis 这样中性的内存键值数据库也提供了内嵌用户 Lua 脚本的官方支持。

作为一门过程型动态语言，Lua 有着如下的特性：

1. 变量名没有类型，值才有类型，变量名在运行时可与任何类型的值绑定;
2. 语言只提供唯一一种数据结构，称为表(table)，它混合了数组、哈希，可以用任何类型的值作为 key 和 value。提供了一致且富有表达力的表构造语法，使得 Lua 很适合描述复杂的数据;
3. 函数是一等类型，支持匿名函数和正则尾递归(proper tail recursion);
4. 支持词法定界(lexical scoping)和闭包(closure);
5. 提供 thread 类型和结构化的协程(coroutine)机制，在此基础上可方便实现协作式多任务;
6. 运行期能编译字符串形式的程序文本并载入虚拟机执行;
7. 通过元表(metatable)和元方法(metamethod)提供动态元机制(dynamic meta-mechanism)，从而允许程序运行时根据需要改变或扩充语法设施的内定语义;
8. 能方便地利用表和动态元机制实现基于原型(prototype-based)的面向对象模型;
9. 从 5.1 版开始提供了完善的模块机制，从而更好地支持开发大型的应用程序;

Lua 的语法类似 PASCAL 和 Modula 但更加简洁，所有的语法产生式规则(EBNF)不过才 60 几个。熟悉 C 和 PASCAL 的程序员一般只需半个小时便可将其完全掌握。而在语义上 Lua 则与 Scheme 极为相似，她们完全共享上述的 1 、3 、4 、6 点特性，Scheme 的 continuation 与协程也基本相同只是自由度更高。最引人注目的是，两种语言都只提供唯一一种数据结构：Lua 的表和 Scheme 的列表(list)。正因为如此，有人甚至称 Lua 为“只用表的 Scheme”。

## Demo

### 初始化
```lua
adapter = require("Adapter")
cmsis_dap = require("CMSIS-DAP"); -- 加载CMSIS-DAP库
cmObj = cmsis_dap.Create(); -- 创建新的CMSIS-DAP Adapter对象
-- CMSIS-DAP的VID和PID
local vid_pids = {
	-- Keil Software
	{0xc251, 0xf001},	-- LPC-Link-II CMSIS_DAP
	{0xc251, 0xf002}, 	-- OPEN-SDA CMSIS_DAP (Freedom Board)
	{0xc251, 0x2722}, 	-- Keil ULINK2 CMSIS-DAP
	-- MBED Software
	{0x0d28, 0x0204}	-- MBED CMSIS-DAP
}
-- 连接CMSIS-DAP仿真器
cmObj:Connect(vid_pids, nil)
-- 配置CMSIS-DAP Transfer参数
cmObj:TransferConfig(5, 5, 5)
-- SWD参数
cmObj:SwdConfig(0)
-- 设置传输频率
cmObj:SetFrequency(5000000)	-- 500KHz
print("CMSIS-DAP: Current frequent is 5MHz")
-- 选择SWD传输模式
local transMode = cmObj:TransferMode()
if transMode == adapter.SWD then
	print("CMSIS-DAP: Current transfer mode is SWD")
elseif transMode == adapter.JTAG then
	print("CMSIS-DAP: Current transfer mode is JTAG")
else
	print("CMSIS-DAP: Can't auto select a transfer mode! Default SWD!")
	cmObj:TransferMode(adapter.SWD)
end

-- 读取JTAG Pins
local pins_data = cmObj:JtagPins(0, 0, 0)
print("JTAG Pins: ")
if pins_data & adapter.PIN_SWCLK_TCK > 0 then
	print(" TCK = 1")
else 
	print(" TCK = 0")
end
if pins_data & adapter.PIN_SWDIO_TMS > 0 then
	print(" TMS = 1")
else 
	print(" TMS = 0")
end
if pins_data & adapter.PIN_TDI > 0 then
	print(" TDI = 1")
else 
	print(" TDI = 0")
end
if pins_data & adapter.PIN_TDO > 0 then
	print(" TDO = 1")
else 
	print(" TDO = 0")
end
if pins_data & adapter.PIN_nTRST > 0 then
	print(" nTRST = 1")
else 
	print(" nTRST = 0")
end
if pins_data & adapter.PIN_nRESET > 0 then
	print(" nRESET = 1")
else 
	print(" nRESET = 0")
end

-- 系统复位
cmObj:Reset(adapter.RESET_SYSTEM)
```

### 简单示例——打印STM32F407的ROM的第1个Word：
```lua
dofile("scripts/adapters/cmsis_dap.lua")    -- 获得CMSIS-DAP对象
-- 系统复位
cmObj:Reset(adapter.RESET_SYSTEM)
-- 切换到SWD模式
cmObj:TransferMode(adapter.SWD)
print("Change transfer mode to SWD")
-- 读取DP IDR
local dpidr = cmObj:DapSingleRead(adapter.REG_DP, 0)
print(string.format("DPIDR: 0x%X", dpidr))
-- 写ABORT寄存器
cmObj:DapSingleWrite(adapter.REG_DP, 0, 0x1e)
-- 写SELECT寄存器
cmObj:DapSingleWrite(adapter.REG_DP, 0x08, 0x0)
-- 写CTRL_STAT,系统域和调试域上电
cmObj:DapSingleWrite(adapter.REG_DP, 0x04, 0x50000000)
repeat
    -- 等待上电完成
    local ctrl_stat = cmObj:DapSingleRead(adapter.REG_DP, 0x04)
until (ctrl_stat & 0xA0000000) == 0xA0000000
print("System & Debug power up.")
-- 读 AHB AP 的CSW和IDR寄存器
cmObj:DapSingleWrite(adapter.REG_DP, 0x08, 0x0F000000)  -- 写SELECT
local apidr = cmObj:DapSingleRead(adapter.REG_AP, 0x0C) -- 读APIDR
cmObj:DapSingleWrite(adapter.REG_DP, 0x08, 0x00000000)  -- 写SELECT
local apcsw = cmObj:DapSingleRead(adapter.REG_AP, 0x00) -- 读APCSW
print(string.format("AP[0].IDR: 0x%X, AP[0].CSW: 0x%X.", apidr, apcsw))
-- 读ROM的第一个uint32
cmObj:DapSingleWrite(adapter.REG_AP, 0x04, 0x08000000)  -- 写TAR_LSB, 0x0800000为ROM的起始地址
local data = cmObj:DapSingleRead(adapter.REG_AP, 0x0C) -- 读DRW
print(string.format("0x08000000 => 0x%X.", data))
print("Hello World")
AdapterObj:SetStatus(adapter.STATUS_IDLE)	-- 熄灭RUN状态灯
```
输出如下：
```
$ sudo ./smartocd -f ./scripts/target/stm32f4x.lua -e
   _____                      __  ____  __________ 
  / ___/____ ___  ____ ______/ /_/ __ \/ ____/ __ \
  \__ \/ __ `__ \/ __ `/ ___/ __/ / / / /   / / / /
 ___/ / / / / / / /_/ / /  / /_/ /_/ / /___/ /_/ / 
/____/_/ /_/ /_/\__,_/_/   \__/\____/\____/_____/  
 * SmartOCD v1.1.0 By: Virus.V <virusv@live.com>
 * Complile time: 2018-08-24T22:20:39+0800
 * Github: https://github.com/Virus-V/SmartOCD
12:11:52 INFO  cmsis-dap.c:117: Successful connection vid: 0xc251, pid: 0xf001 usb device.
12:11:52 INFO  usb.c:291: Currently it is configuration indexed by:0, bConfigurationValue is 1.
12:11:52 INFO  cmsis-dap.c:189: CMSIS-DAP Init.
12:11:52 INFO  cmsis-dap.c:212: CMSIS-DAP the maximum Packet Size is 1024.
12:11:52 INFO  cmsis-dap.c:223: CMSIS-DAP FW Version is 1.100000.
12:11:52 INFO  cmsis-dap.c:229: CMSIS-DAP the maximum Packet Count is 4.
12:11:52 INFO  cmsis-dap.c:262: CMSIS-DAP Capabilities 0x13.
12:11:52 INFO  cmsis-dap.c:1404: Set Transfer Configure: Idle:5, Wait:5, Match:5.
12:11:52 INFO  cmsis-dap.c:1421: SWD: Turnaround clock period: 1 clock cycles. DataPhase: No.
12:11:52 INFO  cmsis-dap.c:518: CMSIS-DAP Clock Frequency: 500.00KHz.
12:11:52 INFO  cmsis-dap.c:380: Switch to SWD mode.
JTAG Pins: TCK:0 TMS:0 TDI:0 TDO:1 nTRST:0 nRESET:1
12:11:52 INFO  DAP.c:30: DAP DPIDR:0x2BA01477.
12:11:52 INFO  DAP.c:184: Probe AP 0, AP_IDR: 0x24770011, CTRL/STAT: 0xF0000040.
Support Packed Transfer.
Support Less Word Transfer.
ROM Table Base: 0xE00FF003
[0x08000000]:0x2001FFFF.
Hello World
CTRL/STAT:0xF0000040
Hello World
```

## Package
SmartOCD的API以Package的方式封装到Lua中。
### Adapter
#### 介绍
Adapter包中提供仿真器相关常量定义。在SmartOCD中，所有仿真器都要用到该包。

在介绍常量和方法之前，假设Lua代码中已经引入Adapter包并赋值给全局变量adapter：
```lua
adapter = require("Adapter")	-- 加载Adapter Package
```

#### 常量

传输模式：
- JTAG
- SWD

JTAG引脚定义：
- PIN_SWCLK_TCK
- PIN_SWDIO_TMS
- PIN_TDI
- PIN_TDO
- PIN_nTRST
- PIN_nRESET

复位类型：
- RESET_SYSTEM
- RESET_DEBUG

DAP寄存器类型：
- REG_DP
- REG_AP

TAP状态：
- TAP_RESET
- TAP_IDLE
- TAP_DR_SELECT
- TAP_DR_CAPTURE
- TAP_DR_SHIFT
- TAP_DR_EXIT1
- TAP_DR_PAUSE
- TAP_DR_EXIT2
- TAP_DR_UPDATE
- TAP_IR_SELECT
- TAP_IR_CAPTURE
- TAP_IR_SHIFT
- TAP_IR_EXIT1
- TAP_IR_PAUSE
- TAP_IR_EXIT2
- TAP_IR_UPDATE

仿真器状态：
- STATUS_CONNECTED
- STATUS_DISCONNECT
- STATUS_RUNING
- STATUS_IDLE

#### 方法
无

----

### CMSIS-DAP
#### 介绍
此Package提供CMSIS-DAP仿真器接口。
#### 常量
无
#### 方法
- Create()  
  作用：创建新的CMSIS-DAP的Adapter对象  
  参数：无  
  返回值：
  1. cmdapObj Adapter对象

  示例：
  ```lua
  -- 加载CMSIS-DAP库
  cmsis_dap = require("CMSIS-DAP")
  -- 创建新的CMSIS-DAP Adapter对象
  cmdapObj = cmsis_dap.Create()
  ```
  ----
- Connect(*self, vid_pids, serialNumber*)  
  作用：连接CMSIS-DAP仿真器设备  
  参数：
  1. Adapter对象
  2. Array：CMSIS-DAP设备的USB VID和PID参数
  3. String：CMSIS-DAP设备的序列号，此参数可选。

  返回值：无  
  示例：
  ```lua
  -- CMSIS-DAP的VID和PID
  local vid_pids = {
	-- Keil Software
  	{0xc251, 0xf001},	-- LPC-Link-II CMSIS_DAP
  	{0xc251, 0xf002}, 	-- OPEN-SDA CMSIS_DAP (Freedom Board)
  	{0xc251, 0x2722}, 	-- Keil ULINK2 CMSIS-DAP
  	-- MBED Software
  	{0x0d28, 0x0204}	-- MBED CMSIS-DAP
  }
  -- 连接CMSIS-DAP仿真器
  cmdapObj:Connect(vid_pids, nil)
  -- cmdapObj.Connect(cmdapObj, vid_pids, nil)
  ```
  ----
- TransferConfig(*self, idleCycles, waitRetry, matchRetry*)  
  作用：配置CMSIS-DAP传输参数  
  参数：
  1. Adapter对象
  2. Number：两次传输之间等待的间隔周期
  3. Number：当接传输收到WAIT应答时，重试次数
  4. Number：当在传输比较模式下，数据不匹配重试的次数

  返回值：无  
  示例：
  ```lua
  -- 配置CMSIS-DAP Transfer参数
  cmdapObj:TransferConfig(5, 5, 5)
  ```
  ----
- JtagConfig(*self, tapInfo*)  
  作用：CMSIS-DAP内部维护一个TAP信息列表，此函数用来配置该列表。  
  参数：
  1. Adapter对象
  2. Array：TAP信息数组

  返回值：无  
  示例：
  ```lua
  -- 设置仿真器中JTAG扫描链信息：TAP个数和IR长度
  cmdapObj:JtagConfig({4, 5})
  ```
  ----
- SwdConfig(*self, swdCfg*)  
  作用：配置SWD传输参数。  
  参数：
  1. Adapter对象
  2. Number：SWD传输参数  
      - Bit 1 .. 0: Turnaround clock period of the SWD device (should be identical with the WCR [Write Control Register] value of the target): 0 = 1 clock cycle (default), 1 = 2 clock cycles, 2 = 3 clock cycles, 3 = 4 clock cycles.
      - Bit 2: DataPhase: 0 = Do not generate Data Phase on WAIT/FAULT (default), 1 = Always generate Data Phase (also on WAIT/FAULT; Required for Sticky Overrun behavior).

  返回值：无  
  示例：
  ```lua
  -- SWD参数
  cmdapObj:SwdConfig(0)
  ```
  ----
- WriteAbort(*self, abort*)  
  作用：写DP ABORT寄存器。  
  参数：
  1. Adapter对象
  2. Number：要写入ABORT寄存器的值

  返回值：无  
  示例：
  ```lua
  -- 清除STICKER flags
  cmdapObj:WriteAbort(0x1e)
  ```
  ----
- SetTapIndex(*self, index*)  
  作用：选择JTAG扫描链中当前活动的TAP对象。  
  参数：
  1. Adapter对象
  2. Number：该TAP对象在JTAG扫描链中的索引

  返回值：无  
  示例：
  ```lua
  -- 选中第一个TAP
  cmdapObj:SetTapIndex(0)
  ```
  ----

- SetStatus(*self, status*)  
  作用：设置仿真器的状态指示灯（如果有的话）  
  参数：  
  1. Adapter对象
  2. Enumeration：Status状态  
   参考Adapter常量 **仿真器状态** 部分
  
  返回值：无  
  示例：
  ```lua
  -- 点亮连接状态指示灯
  cmdapObj:SetStatus(adapter.STATUS_CONNECTED)
  -- 点亮运行状态指示灯
  cmdapObj:SetStatus(adapter.STATUS_RUNING)
  -- 熄灭运行状态指示灯
  cmdapObj:SetStatus(adapter.STATUS_IDLE)
  -- 熄灭连接状态指示灯
  cmdapObj:SetStatus(adapter.STATUS_DISCONNECT)
  ```
  ----
- SetFrequency(*self, clock*)  
  作用：设置仿真器JTAG/SWD同步时钟频率  
  参数：  
  1. Adapter对象
  2. Number：Clock频率，单位Hz
  
  返回值：无  
  示例：
  ```lua
  -- 设置仿真器通信时钟速率
  cmdapObj:SetFrequency(500000)	-- 500KHz
  ```
  ----
- TransferMode(*self [, setType]*)  
  作用：设置或读取仿真器当前传输模式（必须仿真器支持此模式）  
  参数：  
  1. Adapter对象
  2. Enumeration：传输模式（可选参数）  
   参考Adapter常量 **传输模式** 部分
  
  返回值：无 或者  
  1. Enumeration：仿真器当前传输模式  
   
  示例：
  ```lua
  -- 设置当前仿真器为SWD传输模式
  cmdapObj:TransferMode(adapter.SWD)
  -- 设置当前仿真器为JTAG模式
  cmdapObj:TransferMode(adapter.JTAG)
  -- 读取当前仿真器传输模式
  local currTransType = cmdapObj:TransferMode()
  ```
  ----
- Reset(*self, type*)  
  作用：对目标芯片进行复位操作  
  参数：  
  1. Adapter对象
  2. Enumeration：复位类型  
   参考Adapter常量**复位类型**部分  
   调试复位只复位调试部分逻辑，JTAG模式下使TAP进入RESET状态，SWD模式下发送Line Reset信号  
   系统复位包括调试复位，会对整个系统进行复位
  
  返回值：无  
  示例：
  ```lua
  -- 系统复位
  cmdapObj:Reset(adapter.RESET_SYSTEM)
  ```
  ----
- JtagToState(*self, newStatus*)  
  作用：在JTAG模式下，切换TAP状态机的状态  
  参数：  
  1. Adapter对象
  2. Enumeration：要切换到的TAP状态  
   参考Adapter常量 **TAP状态** 部分
  
  返回值：无  
  示例：
  ```lua
  -- TAP到RESET状态
  cmdapObj:jtagStatusChange(adapter.TAP_RESET)
  -- TAP到DR-Shift状态
  cmdapObj:jtagStatusChange(adapter.TAP_DR_SHIFT)
  ```
  ----
- JtagExchangeData(*self, data, bitCnt*)  
  作用：交换TDI和TDO之间的数据。将data参数的数据发送到TDI，然后捕获TDO的数据作为函数返回值。  
  参数：  
  1. Adapter对象
  2. String：要发送的数据
  3. Number：要发送多少个二进制位，不得大于data参数的数据实际长度
  
  返回值：  
  1. String：捕获的TDO数据  

  示例：
  ```lua
  -- TAP到RESET状态，默认连接IDCODE扫描链
  cmdapObj:JtagToState(adapter.TAP_RESET)
  -- TAP到DR-Shift状态，读取idcode
  cmdapObj:JtagToState(adapter.TAP_DR_SHIFT)
  local raw_idcodes = cmdapObj:JtagExchangeData(string.pack("I4I4", 0, 0), 64)
  -- string.unpack 额外返回一个参数是next position
  local idcodes = {string.unpack("I4I4", raw_idcodes)}
  for key=1, #idcodes-1 do
  	print(string.format("TAP #%d : 0x%08X", key-1, idcodes[key]))
  end
  ```
  ----
- JtagIdle(*self, cycles*)  
  作用：进入TAP的Run-Test/Idle状态等待cycle个周期。此函数常用来等待JTAG耗时操作的完成。  
  参数：  
  1. Adapter对象
  2. Number：等待的周期数
  
  返回值：无  
  示例：
  ```lua
  -- TAP进入Run-Test/Idle等待50个TCLK时钟周期
  cmdapObj:JtagIdle(50)
  ```
  ----
- JtagPins(*self, pinMask, pinData, pinWait*)
  作用：设置JTAG引脚电平并返回设置之后的引脚电平状态  
  参数：  
  1. Adapter对象
  2. Number：引脚掩码，LSB有效
  3. Number：引脚数据，LSB有效
  4. Number：死区时间，单位µs
  
  返回值：  
  1. Number：JTAG全部引脚最新的状态，也就时执行设置JTAG引脚动作之后的JTAG引脚状态，LSB有效  

  示例：
  ```lua
  -- 读取JTAG Pins
  local pins_data = cmdapObj:JtagPins(0, 0, 0)
  print("JTAG Pins: ")
  if pins_data & adapter.PIN_SWCLK_TCK > 0 then
    print(" TCK = 1")
  else 
    print(" TCK = 0")
  end
  if pins_data & adapter.PIN_SWDIO_TMS > 0 then
    print(" TMS = 1")
  else 
    print(" TMS = 0")
  end
  if pins_data & adapter.PIN_TDI > 0 then
    print(" TDI = 1")
  else 
    print(" TDI = 0")
  end
  if pins_data & adapter.PIN_TDO > 0 then
    print(" TDO = 1")
  else 
    print(" TDO = 0")
  end
  if pins_data & adapter.PIN_nTRST > 0 then
    print(" nTRST = 1")
  else 
    print(" nTRST = 0")
  end
  if pins_data & adapter.PIN_nRESET > 0 then
    print(" nRESET = 1")
  else 
    print(" nRESET = 0")
  end
  -- 设置JTAG的TMS TDI引脚电平，并返回所有引脚最新的状态
  local pins_data = cmdapObj:JtagPins(adapter.PIN_SWDIO_TMS | adapter.PIN_TDI, adapter.PIN_SWDIO_TMS, 300)
  print(string.format("JTAG Pins: 0x%X.", pins_data))
  ```
  ----
- DapSingleRead(*self, regType, reg*)  
  作用：读DAP寄存器  
  参数：  
  1. Adapter对象
  2. Enumeration：寄存器类型，AP还是DP  
   参考Adapter常量 **DAP寄存器类型** 部分
  3. Number：寄存器地址，参考ADI手册
  
  返回值：  
  1. Number：寄存器的值  

  示例：
  ```lua
  -- 读取DP IDR
  local dpidr = cmObj:DapSingleRead(adapter.REG_DP, 0)
  print(string.format("DPIDR: 0x%X", dpidr))
  ```
  ----
- DapSingleWrite(*self, regType, reg, data*)  
  作用：读DAP寄存器  
  参数：  
  1. Adapter对象
  2. Enumeration：寄存器类型，AP还是DP  
   参考Adapter常量 **DAP寄存器类型** 部分
  3. Number：寄存器地址，参考ADI手册
  4. Number：写入寄存器的值
  
  返回值：无
  
  示例：
  ```lua
  -- 写SELECT寄存器
  cmObj:DapSingleWrite(adapter.REG_DP, 0x08, 0x0)
  ```
  ----
- DapMultiRead(*self, regType, reg*)  
  作用：多次读DAP寄存器  
  参数：  
  1. Adapter对象
  2. Enumeration：寄存器类型，AP还是DP  
   参考Adapter常量 **DAP寄存器类型** 部分
  3. Number：寄存器地址，参考ADI手册
  
  返回值：  
  1. String：寄存器的值  
  
  示例：
  ```lua
  -- 暂无
  ```
  ----
- DapMultiWrite(*self, regType, reg*)  
  作用：读DAP寄存器  
  参数：  
  1. Adapter对象
  2. Enumeration：寄存器类型，AP还是DP  
   参考Adapter常量 **DAP寄存器类型** 部分
  3. Number：寄存器地址，参考ADI手册
  
  返回值：  
  1. Number：寄存器的值  
  
  示例：
  ```lua
  -- 暂无
  ```
  ----




- jtagWriteTAPIR(*self, tapIdx, irData*)  
  作用：将 _irData_ 写入JTAG扫描链中位于 _tapIdx_ 的TAP的IR寄存器  
  参数：  
  1. Adapter对象
  2. Number：tapIdx TAP在JTAG扫描链中的位置，从0开始
  3. Number：irData 要写入IR寄存器的数据，将要写入的二进制位个数由 `jtagSetTAPInfo` 的 _tapInfo_ 参数指定。
  
  返回值：无  
  示例：参考 `jtagSetTAPInfo` 方法的示例部分  
  ____
- jtagExchangeTAPDR(*self, tapIdx, data, bitCnt*)  
  作用：交换JTAG扫描链中位于 _tapIdx_ 的TAP的DR寄存器  
  参数：  
  1. Adapter对象
  2. Number：tapIdx TAP在JTAG扫描链中的位置，从0开始
  3. Number：data 要交换进去的DR的数据
  4. Number：bitCnt 指定DR寄存器的二进制位个数，不得大于 _data_ 数据的长度
  
  返回值：  
  1. String：交换出来的DR寄存器数据  

  示例：参考 `jtagSetTAPInfo` 方法的示例部分

----

### DAP
#### 介绍
DAP包提供一些DAP通用操作。

#### 常量
AP类型：

- AP_TYPE_JTAG
- AP_TYPE_AMBA_AHB
- AP_TYPE_AMBA_APB
- AP_TYPE_AMBA_AXI

地址自增模式：

- ADDRINC_OFF
- ADDRINC_SINGLE
- ADDRINC_PACKED

传输数据宽度：

- DATA_SIZE_8
- DATA_SIZE_16
- DATA_SIZE_32
- DATA_SIZE_64
- DATA_SIZE_128
- DATA_SIZE_256

DAP相关寄存器：

- DP_REG_CTRL_STAT
- DP_REG_SELECT
- DP_REG_RDBUFF
- DP_REG_DPIDR
- DP_REG_ABORT
- DP_REG_DLCR
- DP_REG_RESEND
- DP_REG_TARGETID
- DP_REG_DLPIDR
- DP_REG_EVENTSTAT
- DP_REG_TARGETSEL
+ AP_REG_CSW
+ AP_REG_TAR_LSB
+ AP_REG_TAR_MSB
+ AP_REG_DRW
+ AP_REG_BD0
+ AP_REG_BD1
+ AP_REG_BD2
+ AP_REG_BD3
+ AP_REG_ROM_MSB
+ AP_REG_CFG
+ AP_REG_ROM_LSB
+ AP_REG_IDR

ARM JTAG扫描链：

- JTAG_ABORT
- JTAG_DPACC
- JTAG_APACC
- JTAG_IDCODE
- JTAG_BYPASS


#### 方法

- Init(*adapterObj*)  
  作用：对DAP进行初始化  
  参数：  
  1. adapterObj Adapter对象

  返回值：无  
  示例：  
  ```lua
  -- 设置DAP在JTAG扫描链中的索引，SWD模式下忽略
  dap.SetIndex(AdapterObj, 0)
  -- 设置WAIT重试次数
  dap.SetRetry(AdapterObj, 5)
  -- 设置两次传输之间的空闲等待周期
  dap.SetIdleCycle(AdapterObj, 5)
  -- 初始化DAP
  dap.Init(AdapterObj)
  ```
  ----
- SetIndex(*adapterObj, index*)  
  作用：在JTAG模式下，设置DAP在JTAG扫描链的位置  
  参数：  
  1. adapterObj Adapter对象
  2. Number：index DAP在JTAG扫描链中的索引，从0开始
  
  返回值：无  
  示例：参考 `Init` 方法的示例代码
  ____
- SetRetry(*adapterObj, retry*)  
  作用：设置遇到WAIT响应时重试次数  
  参数：
  1. adapterObj Adapter对象
  2. Number：retry 重试次数

  返回值：无  
  示例：参考 `Init` 方法的示例代码
  ____
- SetIdleCycle(*adapterObj, idleCycles*)  
  作用：设置两次传输之间的等待周期。JTAG模式下在UPDATE-xR之后进入Run-test/Idle状态等待；SWD模式下发送空闲周期  
  参数：  
  1. adapterObj Adapter对象
  2. Number：idleCycles 等待时钟周期个数

  返回值：无  
  示例：参考 `Init` 方法的示例代码
  ____
- SelectAP(*adapterObj, apIdx*)  
  作用：选中索引为 _apIdx_ 的AP，作为后续DAP操作的默认AP。  
  参数：  
  1. adapterObj Adapter对象
  2. Number：apIdx AP的索引，范围0~255

  返回值：无  
  示例：
  ```lua
  -- 选择索引为0的AP
  dap.SelectAP(AdapterObj, 0)
  ```
  ----
- WriteAbort(*adapterObj, abort*)  
  作用：写入ABORT寄存器。JTAG模式下写入ABORT扫描链，SWD模式下写入ABORT寄存器  
  参数：
  1. adapterObj Adapter对象
  2. Number：abort 写入ABORT寄存器/扫描链的数据

  返回值：无  
  示例：
  ```lua
  dap.WriteAbort(AdapterObj, 0x01)
  ```
  ----
- FindAP(*adapterObj, apType*)  
  作用：搜索类型为 _apType_ 的AP。  
  参数：  
  1. adapterObj Adapter对象
  2. Enumeration：apType AP类型  
   参考DAP常量 **AP类型** 部分

  返回值：  
  1. Number：该AP的索引 0~255

  示例：  
  ```lua
  -- 选择类型为AMBA AHB的MEM-AP
  local memAP = dap.FindAP(AdapterObj, dap.AP_TYPE_AMBA_AHB)
  -- 选择类型为AMBA APB的MEM-AP
  local debugAP = dap.FindAP(AdapterObj, dap.AP_TYPE_AMBA_APB)
  ```
  ----
- GetAPCapacity(*adapterObj, ...*)  
  作用：获得当前选中AP的Capacity  
  参数：  
  1. adapterObj Adapter对象
  2. 字符串类型的可变参数，可取的值为"PT"、"LD"、"LA"、"BE"、"BT"，且可以任意数量任意顺序组合  
   “PT” 代表探测该AP是否支持“Packed Transfer”  
   “LD” 代表探测该AP是否支持“Large Data”  
   “LA” 代表探测该AP是否支持“Large Address”  
   “BE” 代表探测该AP是否支持“Big-endian”  
   “BT” 代表探测该AP是否支持“Less Word Transfer”  

  返回值：Boolean类型的可变返回值，返回值个数和顺序与函数可变参数的个数和顺序相同  
  示例：  
  ```lua
  -- 选择类型为AMBA AHB的MEM-AP
  local memAP = dap.FindAP(AdapterObj, dap.AP_TYPE_AMBA_AHB)
  do 
  	local PackedTrans, LargeData, LargeAddr, BigEndian, ByteTrans = dap.GetAPCapacity(AdapterObj, "PT","LD","LA","BE","BT")
  	if PackedTrans then print("Support Packed Transfer.") end
  	if LargeData then print("Support Large Data Extention.") end
  	if LargeAddr then print("Support Large Address.") end
  	if BigEndian then print("Big Endian Access.") end
  	if ByteTrans then print("Support Less Word Transfer.") end
  end 
  ```
  ----
- GetROMTable(*adapterObj*)  
  作用：获得当前AP的ROM Table地址。  
  参数：  
  1. adapterObj Adapter对象

  返回值：  
  1. Number：32或64位ROM Table基址

  示例：
  ```lua
  -- 获得当前AP的ROM Table基址
  local romTableBase = dap.GetROMTable(AdapterObj)
  ```
  ----
- Reg(*adapterObj, reg [, data]*)  
  作用：读写DP/AP寄存器。如果第三个参数不存在，则表示读寄存器，否则写寄存器  
  参数：
  1. adapterObj Adapter对象
  2. Enumeration：reg 要读/写的寄存器  
   参考DAP常量 **DAP相关寄存器** 部分
  3. Number：data 写入寄存器的数据（可选参数）

  返回值：无 或者  
  1. Number：寄存器的值

  示例：
  ```lua
  -- 写AP CSW 寄存器
  dap.Reg(AdapterObj, dap.AP_REG_CSW, 0xa2000002)
  -- 读DP CTRL/STAT 寄存器
  local ctrl_stat = dap.Reg(AdapterObj, dap.DP_REG_CTRL_STAT)
  ```
  ----
- Memory8(*adapterObj, addr [, data]*)  
  作用：以字节（8位）的宽度读/写 _addr_ 的数据。如果第三个参数不存在，则表示读，反之为写  
  参数：
  1. adapterObj Adapter对象
  2. Number：addr 要读/写的内存位置
  3. Number：data 要写入的数据，LSB有效（可选参数）

  返回值：无 或者 
  1. Number：_addr_ 位置的数据  

  示例：
  ```lua
  -- 读内存数据：将0x08000000地址的字节数据赋值给局部变量data
  local data = dap.Memory8(AdapterObj, 0x08000000)
  -- 写内存数据：将0xef（0xdeadbeef的最低字节）写入到0x20000000
  dap.Memory8(AdapterObj, 0x20000000， 0xdeadbeef)
  ```
  ----
- Memory16(*adapterObj, addr [, data]*)  
  介绍：以半字（16位）的宽度读/写 _addr_ 的数据。  
  参考 `Memory8`
  ____
- Memory32(*adapterObj, addr [, data]*)  
  介绍：以字（32位）的宽度读/写 _addr_ 的数据。  
  参考 `Memory8`
  ____
- ReadMemBlock(*adapterObj, addr, addrIncMode, dataWidth, readCnt*)  
  介绍：批量读取 _addr_ 内存地址。  
  参数：
  1. adapterObj Adapter对象
  2. Number：addr 要读取的内存地址
  3. Enumeration：addrIncMode 地址自增模式  
   参考DAP常量 **地址自增模式** 部分
  4. Enumeration：dataWidth 单次读取的数据宽度  
   参考DAP常量 **传输数据宽度** 部分
  5. Number：readCnt 读取的次数

  返回值：
  1. String：读取的数据

  示例：
  ```lua
  -- 从0x08000000开始读，读取16次，每次读取32位的数据，每次读取成功后地址自增
  local data = dap.ReadMemBlock(AdapterObj, 0x08000000, dap.ADDRINC_SINGLE, dap.DATA_SIZE_32, 16)
  -- 打印数据
  local pos, word = 1, 0
  while pos < #data do
	word, pos = string.unpack("I4", data, pos)
	print(string.format("0x%08X", word))
  end
  ```
  ----
- WriteMemBlock(*adapterObj, addr, addrIncMode, dataWidth, data*)  
  介绍：批量写入 _addr_ 内存地址。  
  参数：
  1. adapterObj Adapter对象
  2. Number：addr 要读取的内存地址
  3. Enumeration：addrIncMode 地址自增模式  
   参考DAP常量 **地址自增模式** 部分
  4. Enumeration：dataWidth 单次读取的数据宽度  
   参考DAP常量 **传输数据宽度** 部分
  5. String：data 要写入的数据

  返回值：无  
  示例：
  ```lua
  -- 要写入的数据
  local data = string.pack("I4I4I4I4", 0x11223344, 0x55667788, 0x99aabbcc, 0xddeeff00)
  -- 执行写入动作
  dap.WriteMemBlock(AdapterObj, 0x20000000, dap.ADDRINC_SINGLE, dap.DATA_SIZE_32, data)
  ```
  ----
- Get_CID_PID(*adapterObj, base*)  
  作用：获得CoreSight Component的Component ID和Peripheral ID  
  参数：
  1. adapterObj Adapter对象
  2. Number：base Component的基址，4KB对齐
  
  返回值：
  1. Number：cid 32位的Component ID
  2. Number：pid 64位的Peripheral ID

  示例：
  ```lua
  -- 读取ROM Table基址
  local rom_table_base = dap.GetROMTable(AdapterObj)
  -- 读取ROM Table的cid和pid
  local cid,pid = dap.Get_CID_PID(AdapterObj, rom_table_base & 0xFFFFF000)
  ```
----

## 开发人员手册
### 开发计划
### 开发指引
### 核心设计

## 版本历史
