v3.3.4
1. feature:
  1、支持通过UART升级固件功能，升级使用HB-M_DownloadTool工具
2. bugfix:
3. perf:
4. requirement:

v3.3.3
1. feature:
2. bugfix:
  2.1 修复gpio pulse timer未初始化问题
3. perf:
4. requirement:

v3.3.2
1. feature:
  1.1 解析识别文本时，仅去除中文之间的空格，以兼容中英缩写
2. bugfix:
3. perf:
4. requirement:

v3.3.1
1. feature:
  1.1 内存优化 
  1.2 新增power sleep demo
  1.3 增加I2C从机模式有效地址范围说明
2. bugfix:
  2.1 修复一个pwm平台配置错误问题
  2.2 修复user_play_stop接口会导致无法识别的问题
  2.3 去除hb_housekeeper示例中首次唤醒特殊逻辑
3. perf:
4. requirement:

v3.3.0
1. feature:
  1.1 支持UDP平台自动化GPIO配置功能
  1.2 支持UDP平台红外品类功能
2. bugfix:
3. perf:
4. requirement:

v3.2.1
1. feature:
2. bugfix:
  2.1 解决user demo拷贝导致资源错乱的问题
3. perf:
4. requirement:

v3.2.0
1. feature:
2. bugfix:
  2.1 解决udp编译出的image和产品配置不一致的问题
  2.2 音量设置播报音与用户设置一直
3. perf:
4. requirement:

v3.1.9
1. feature:
  1.1 增加顺序播放列表中所有音频的功能，用于音频拼接
2. bugfix:
3. perf:
4. requirement:

v3.1.8
1. feature:
  1.1 增加简易版串口协议，默认串口通过简易协议输出识别信息
  1.2 增加简易串口协议测试demo
2. bugfix:
3. perf:
4. requirement:

v3.1.7
1. feature:
  1.1 重写hb_housekeeper示例程序，支持标准化红外遥控产品
  1.2 增加demo资源自动切换功能，修改user_config.h中demo的选择后自动复制对应demo的资源
2. bugfix:
3. perf:
4. requirement:

v3.1.6
1. feature:
  1.1 支持更新识别阈值表
2. bugfix:
3. perf:
4. requirement:

v3.1.5
1. feature:
  1.1 增加低内存时的异常恢复机制
2. bugfix:
  2.1 解决一个极端情况下会导致内存泄漏的问题
3. perf:
  3.1 升级AIS库版本到v2.0.3
4. requirement:

v3.1.4
1. feature:
2. bugfix:
  2.1 解决配置没有唤醒词灵敏度时，不解析唤醒灵敏度的问题
3. perf:
4. requirement:

v3.1.3
1. feature:
  1.1 支持语言配置
  1.2 支持识别灵敏度配置
  1.3 支持近场、远场配置
2. bugfix:
3. perf:
4. requirement:

v3.0.6
1. feature:
  1.1 调整MIC增益默认为0，增加上层设置接口
  1.2 更新烧录工具使用说明
2. bugfix:
3. perf:
  3.1 升级AIS库版本到v2.0.0
  3.2 增加ADC buffer到8KB，配合AIS版本需求
  3.3 调整AIK配置文件FA阈值为0.2 - 0.4
4. requirement:

v3.0.5
1. feature:
  1.1 B8串口波特率改为57600，解决串口输出出现乱码的问题
  1.2 增加系统组提供产测功能库
2. bugfix:
  2.1 修正UDP平台配置无开机播报音时无法识别的问题
  2.2 修正音量调节到最小后增加音量错误的问题
  2.3 解决了Timer4无法正常工作的问题
3. perf:
4. requirement:

v3.0.4
1. feature:
2. bugfix:
3. perf:
  3.1 更新ais版本为ais100 v2.1.9，aik log方式由系统实现，恢复aik debug版本下的log输出。
4. requirement:

v3.0.3
1. feature:
2. bugfix:
  2.1 修复崩溃问题
3. perf:
4. requirement:

v3.0.2
1. feature:
  1.1 更新AIK2.1.4
2. bugfix:
3. perf:
  3.1 内存优化
4. requirement:

v3.0.1
1. feature:
  1.1 增加USER层封装，提供常用IO及功能模块驱动及示例程序
2. bugfix:
3. perf:
4. requirement:

v2.3.4
1. feature:
2. bugfix:
3. perf:
  1.1 关闭aik debug输出，防止多线程打log重启死机问题
4. requirement:

v2.3.3
1. feature:
  1.1 更新aik版本为ais100 v2.1.4，kws版本为6.1.1
2. bugfix:
  1.1 解决上版本上电开机引擎初始化崩溃问题
3. perf:
4. requirement:


v2.3.2
1. feature:
  1.1 更新aik版本为ais100 v2.1.3，支持多唤醒词时分别配置阈值
2. bugfix:
3. perf:
4. requirement:

v2.3.1
1. feature:
  1.1 更新IC加密算法，不再兼容旧版芯片加密方式，必须对demo板芯片升级加密固件
2. bugfix:
  2.1 修正烧录工具内说明书合并时数据错误为空白的问题
3. perf:
4. requirement:

v2.2.1
1. feature:
  1.1 支持hal层PWM控制接口
  1.2 支持hal层flash读写操作（固定最后64K的前8K空间）
2. bugfix:
3. perf:
4. requirement:

v2.1.1
1. feature:
  1.1 支持开机无播报音需求
  1.2 支持命令词无回复语需求
2. bugfix:
3. perf:
  3.1 正常识别FA=0.4次/分钟；正常唤醒FA=0.1次/小时；深度唤醒FA=0.02次/小时
  3.2 使用v6.0.1版本引擎，通用模型
4. requirement:

v2.1.0
1. feature:
  1.1 支持芯片ID加密验证，未授权烧录芯片将不能启动
  1.2 所有hal层以下接口全部隐藏，不再暴露底层实现
2. bugfix:
3. perf:
  3.1 正常识别FA=0.4次/分钟；正常唤醒FA=0.1次/小时；深度唤醒FA=0.02次/小时
  3.2 使用v6.0.1版本引擎，通用模型
4. requirement:

v2.0.0
1. feature:
  1.1 智能风扇命令词demo，支持24Pin量产芯片
  1.2 支持通用串口协议(B6-Rx\B7-Tx)
  1.3 log通过Software UART输出(B8-Tx)
2. bugfix:
  2.1 解决AIK start时频繁死机问题
  2.2 解决主动退出播报错误问题
3. perf:
  3.1 正常识别FA=0.4次/分钟；正常唤醒FA=0.1次/小时；深度唤醒FA=0.02次/小时
  3.2 使用v6.0.1版本引擎，通用模型
4. requirement:

v1.0.0
1. feature:
  1.1 智能风扇命令词demo
2. bugfix:
3. perf:
  3.1 正常识别FA=0.4次/分钟；正常唤醒FA=0.1次/小时；深度唤醒FA=0.02次/小时
  3.2 使用v5.1.6版本引擎，通用模型
4. requirement:
