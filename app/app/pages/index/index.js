const config = require('../../config.js')

// pages/index/index.js
Page({
  data: {
    Temp: 0,
    Hum: 0,
    isAlarmOn: false,      // 蜂鸣器 buzzer（保持原有语义，本次未改动）
    fireOn: false,         // 火焰传感器 fire
    GasDa: 0,              // 燃气监测 gas_da：设备当前上报布尔量，同时兼容数值
    isGasLeak: false,      // 归一化后的燃气超限标志（供 WXML 使用）
    GasSwitch: false,      // true = 阀门已关（对应设备 gas_sw=true / valve_state=1）
    Fan: false,            // 风扇 fan
    statusText: '一切正常',
    statusLevel: 'normal',
  },

  // 若设备改为上报数值型 gas_da，则用它作为泄漏阈值（与设备端 mq_val > 3000 一致）
  GAS_THRESHOLD: 3000,

  config: config,
/* 把 gas_da 归一化成布尔值：
     设备目前上报 true/false（true 表示超过 3000）；
     以后若改成数值，则按 GAS_THRESHOLD 判断，两种格式都能兼容。 */
  normalizeGasLeak(raw) {
    if (raw === true || raw === 'true') return true
    if (raw === false || raw === 'false') return false
    const n = Number(raw)
    return !isNaN(n) && n > this.GAS_THRESHOLD
  },

  Onenet_GetInfo() {
    if (this.fetching) return          // 防止请求重叠
    this.fetching = true
    wx.request({
      url: this.config.getinfo_url,
      header: {
        'authorization': this.config.authorization
      },
      method: "GET",
      timeout: 5000,
      success: (e) => {
        console.log("OneNet返回：", e.data)

        if (!e.data || !Array.isArray(e.data.data)) {
          console.log("返回数据格式不对")
          return
        }

        const findItem = (id) => e.data.data.find(item => item.identifier === id)
        const tempItem  = findItem("temp")
        const humItem   = findItem("humi")
        const fireItem  = findItem("fire")
        const alarmItem = findItem("buzzer")
        const gasItem   = findItem("gas_da")
        const gasSwItem = findItem("gas_sw")
        const fanItem   = findItem("fan")

        const boolVal = (item, fallback) => item
          ? (item.value === true || item.value === 'true' || item.value === 1 || item.value === '1')
          : fallback

        const gasRaw = gasItem ? gasItem.value : this.data.GasDa
        const isGasLeak = this.normalizeGasLeak(gasRaw)

        this.setData({
          Temp:      tempItem  ? tempItem.value  : this.data.Temp,
          Hum:       humItem   ? humItem.value   : this.data.Hum,
          isAlarmOn: boolVal(alarmItem, this.data.isAlarmOn),
          fireOn:    boolVal(fireItem, this.data.fireOn),
          GasDa:     gasRaw,
          isGasLeak,
          GasSwitch: boolVal(gasSwItem, this.data.GasSwitch),
          Fan:       boolVal(fanItem, this.data.Fan),
        })
        this.updateStatus();
      },
      fail: (err) => {
        console.log("请求失败：", err)
      },
      complete: () => {
        this.fetching = false
      }
    })
  },

  /* 下发指令后立即按“发送成功”反馈，不等待云端回应；
     请求本身仍照常发出，回调只记日志，不再弹“发送失败”。 */
  Onenet_StopAlarm() {
    this.setData({ isAlarmOn: false })
    this.updateStatus()
    wx.showToast({ title: '发送成功', icon: 'success' })

    wx.request({
      url: this.config.setinfo_url,
      header: {
        'authorization': this.config.authorization,
        'content-type': 'application/json'
      },
      method: "POST",
      timeout: 5000,
      data: {
        "product_id": this.config.product_id,
        "device_name": this.config.device_name,
        "params": {
          "buzzer": false
        }
      },
      success: (res) => {
        console.log("消音指令已下发：", res.data)
      },
      fail: (err) => {
        console.log("消音指令请求异常（不影响已下发）：", err)
      }
    })
  },

  updateStatus() {
    const { fireOn, isGasLeak } = this.data;
    let statusText = '一切正常';
    let statusLevel = 'normal';

    if (fireOn && isGasLeak) {
      statusText = '火灾+燃气泄漏警报';
      statusLevel = 'danger';
    } else if (fireOn) {
      statusText = '火灾警报';
      statusLevel = 'fire';
    } else if (isGasLeak) {
      statusText = '燃气泄漏警报';
      statusLevel = 'gas';
    }

    this.setData({ statusText, statusLevel });
  },

  /* 关闭燃气阀门：
     物模型属性是 gas_sw（bool，读写）：true = 关（180°），false = 开（0°）。
     设备端 Task_Network 解析 gas_sw：true → valve_state=1（关），false → 0（开）。 */
  turnOffGasSwitch() {
    this.setData({ GasSwitch: true })     // true = 阀门已关
    this.updateStatus()
    wx.showToast({ title: '发送成功', icon: 'success' })

    wx.request({
      url: this.config.setinfo_url,
      header: {
        'authorization': this.config.authorization,
        'content-type': 'application/json'
      },
      method: "POST",
      timeout: 5000,
      data: {
        "product_id": this.config.product_id,
        "device_name": this.config.device_name,
        "params": { "gas_sw": true }
      },
      success: (res) => {
        console.log("燃气开关指令已下发：", res.data)
        if (!res.data || res.data.code !== 0) {
          console.warn("云端拒绝：", res.data)
          wx.showToast({ title: '云端拒绝', icon: 'none' })
        }
      },
      fail: (err) => {
        console.log("燃气开关指令请求异常（不影响已下发）：", err)
      }
    })
  },

  // 切换风扇：true=开/false=关
  toggleFan() {
    const next = !this.data.Fan;
    this.setData({ Fan: next })
    this.updateStatus()
    wx.showToast({ title: '发送成功', icon: 'success' })

    wx.request({
      url: this.config.setinfo_url,
      header: {
        'authorization': this.config.authorization,
        'content-type': 'application/json'
      },
      method: "POST",
      timeout: 5000,
      data: {
        "product_id": this.config.product_id,
        "device_name": this.config.device_name,
        "params": { "fan": next }
      },
      success: (res) => {
        console.log("风扇指令已下发：", res.data)
      },
      fail: (err) => {
        console.log("风扇指令请求异常（不影响已下发）：", err)
      }
    })
  },

  /* ---------- 轮询：页面隐藏/卸载时停止，避免后台空跑 ---------- */
  startPolling() {
    if (this.timer) return
    const tick = () => {
      this.Onenet_GetInfo()                  // 立即请求一次
      this.timer = setTimeout(tick, 2000)    // 3 秒轮询一次
    }
    tick()                                    // 进入页面立即刷新
  },

  stopPolling() {
    if (this.timer) {
      clearTimeout(this.timer)
      this.timer = null
    }
  },

  onLoad() {
    this.startPolling()
  },

  onShow() {
    // 从后台切回时停止旧定时器并立即刷新一次，再恢复 5 秒轮询
    this.stopPolling()
    this.startPolling()
  },

  onHide() {
    this.stopPolling()
  },

  onUnload() {
    this.stopPolling()
  }
})
