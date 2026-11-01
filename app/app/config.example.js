// 复制本文件为 config.js，并填入自己的 OneNET 参数。
// config.js 已在 .gitignore 中忽略，不要提交真实 token。
module.exports = {
  authorization: "your_onenet_token",
  product_id: "your_product_id",
  device_name: "your_device_name",
  getinfo_url: 'https://iot-api.heclouds.com/thingmodel/query-device-property?product_id=your_product_id&device_name=your_device_name',
  setinfo_url: 'https://iot-api.heclouds.com/thingmodel/set-device-property'
}
