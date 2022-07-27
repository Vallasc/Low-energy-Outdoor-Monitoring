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
    dashboardUrl: String
  }

  Users = mongoose.model('Users', mongoose.Schema({
    _id: String,
    email: String,
    password: String,
    salt: String,
    garfanaId: Number,
    garfanaFolderUid: String,
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

  async createDevice(userId, jsonDevice) {
    return await Users.findById(userId)
  }

  async findDeviceById(userId) {
    return await Users.findById(userId)
  }

}