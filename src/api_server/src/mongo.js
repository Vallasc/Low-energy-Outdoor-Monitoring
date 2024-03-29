import mongoose from 'mongoose'

const USER = process.env.MONGO_USERNAME || "admin"
const PASS = process.env.MONGO_PASSWORD || "admin"
const DB_NAME = process.env.MONGO_DB_NAME || "lomo"
const HOST_NAME = process.env.MONGO_HOST || "127.0.0.1"

const MONGODB_URI = `mongodb://${USER}:${PASS}@${HOST_NAME}/${DB_NAME}?authSource=admin`

export class MongoManager {
  Device = {
    id: String,
    name: String,
    protocol: String,
    sampleFrequency: Number,
    configUpdateFrequency: Number,
    minGasValue: Number,
    maxGasValue: Number,
    lastSeen: Number,
    userId: String,
    proxyPort: Number,
    mqttPort: Number,
    udpPort: Number,
    token: String,
    latitude: Number,
    longitude: Number,
    dashboardUid: String,
    dashboardUrl: String,
    alertUid: String,
    host: String,
    wifiSsid: String,
    enablePerformanceMonitoring: Boolean,
    totalMqttPacketCount: Number,
    receivedMqttPacketCount: Number,
    totalHttpPacketCount: Number,
    receivedHttpPacketCount: Number,
    mqttMeanTime: Number,
    httpMeanTime: Number,
    alertEnable: Boolean,
    lastAlertTime: Number,
    lastAlert: String,
    lastTempValue: Number,
    lastHumidityValue: Number,
    lastGasValue: Number,
    lastSoilValue: Number,
    lastAqiValue: Number,
    lastRssiValue: Number,
    enablePrediction: Boolean,
    trainingTime: Number,
    predictionTime: Number
  }

  Users = mongoose.model('Users', mongoose.Schema({
    _id: String,
    email: String,
    password: String,
    salt: String,
    garfanaId: Number,
    garfanaFolderUid: String,
    telegramToken: String,
    devices: [this.Device]
  }, { _id: false }))

  DevicesUsers = mongoose.model('DevicesUsers', mongoose.Schema({
    _id: String,
    token: String,
    userId: String
  }, { _id: false }))

  constructor() {
    mongoose.connect(MONGODB_URI, { useNewUrlParser: true }).then((response) => {
      console.log(`Connected to MongoDB`)
    }).catch((err) => {
      console.log(err)
    })
  }

  async createUser(jsonUser) {
    return await this.Users.create(jsonUser)
  }

  async deleteUser(userId) {
    return await this.Users.findByIdAndRemove(userId)
  }

  async findUserById(userId) {
    return await this.Users.findById(userId)
  }

  async updateDevice(userId, device) {
    await this.Users.updateOne(
      { _id: userId, "devices.id": device.id },
      {
        $set: {
            "devices.$.protocol": device.protocol,
            "devices.$.configUpdateFrequency": device.configUpdateFrequency,
            "devices.$.sampleFrequency": device.sampleFrequency,
            "devices.$.minGasValue": device.minGasValue,
            "devices.$.maxGasValue": device.maxGasValue,
            "devices.$.dashboardUid": device.dashboardUid,
            "devices.$.dashboardUrl": device.dashboardUrl,
            "devices.$.enablePerformanceMonitoring": device.enablePerformanceMonitoring,
            "devices.$.alertEnable": device.alertEnable,
            "devices.$.alertUid": device.alertUid,
            "devices.$.lastAlertTime": device.lastAlertTime,
            "devices.$.lastAlert": device.lastAlert,
            "devices.$.enablePrediction": device.enablePrediction,
            "devices.$.trainingTime": device.trainingTime,
            "devices.$.predictionTime": device.predictionTime
        }
      }
    )
  }

  async deleteDevice(userId, deviceId) {
    await this.DevicesUsers.findByIdAndRemove(deviceId)
    return await this.Users.updateOne(
      { _id: userId },
      { $pull: { 'devices': { id: deviceId } }}
    )
  }

  async createDevice(userId, jsonDevice) {
    const id = new mongoose.Types.ObjectId().toString()
    jsonDevice.id = id
    const deviceUser= {
      _id: jsonDevice.id,
      token: jsonDevice.token,
      userId: userId,
    }
    await this.DevicesUsers.create(deviceUser)
    await this.Users.updateOne(
      { _id: userId },
      { $push: { 'devices': jsonDevice }}
    )
    return id
  }

  async findDeviceById(userId, deviceId) {
    const user = await this.Users.findById(userId)
    for(let device of user.devices) {
      if(device.id === deviceId)
        return device
    }
    throw new Error("Device not found")
  }

  async findUserIdByDeviceId(deviceId) {
    const deviceUser = await this.DevicesUsers.findById(deviceId)
    return deviceUser.userId
  }

}