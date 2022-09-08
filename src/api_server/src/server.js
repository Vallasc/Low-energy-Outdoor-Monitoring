import {
  MIN_GAS_VALUE,
  MAX_GAS_VALUE,
  SAMPLE_FREQUENCY,
  PROTOCOL,
  PROXY_PORT,
  CONFIG_UPDATE_FREQUENCY,
  UDP_PORT,
  MQTT_PORT,
  ENABLE_PERFORMANCE } from './deviceConfig.js'

import {
  createClient, 
  deleteClient,
  disableClient,
  enableClient,
  createRole,
  deleteRole } from './mosquitto.js'
import {
  createUser, 
  createDashboard, 
  deleteUser, 
  changeUserPassword,
  deleteDashboard,
  setupAlertConfigReceivers,
  createAlert,
  deleteAlert,
  setDatasource } from './grafana.js'
import {MongoManager} from './mongo.js'

import express from 'express'
import path from 'path'
import cors from 'cors'
import crypto from 'crypto'
import jwt from 'jsonwebtoken'
import axios from 'axios'

const app = express()

const TELEGRAM_HOST = process.env.TELEGRAM_HOST || "127.0.0.1:8087"
const TOKEN_SECRET = process.env.TOKEN_SECRET || "lomolomo"
const PORT = process.env.PORT || 8888

const __dirname = path.resolve();

app.use(express.json())
app.use(express.urlencoded({ extended: true }))
app.use(cors())

app.use(express.static(path.join(__dirname, './public')))

let mongoManager

//Initiate MongoDB and start server
app.listen(PORT, '0.0.0.0', () => {
  mongoManager = new MongoManager()
  setDatasource()
  setupAlertConfigReceivers("http://lomoapi:" + PORT + "/alerts")
  console.log(`HTTP server started on PORT ${PORT}`)
})

const handler = (req, res) => res.sendFile(path.join(__dirname, './public/index.html'))
const routes = ["/", "/dashboard", "/signin", "/signup", "/signout", "/profile", "/telegram", "/devices",  "/devices/*"]
routes.forEach(route => app.get(route, handler))


app.post('/users', async (req, res) => {
  const salt = crypto.randomBytes(20).toString('hex')
  const hash = crypto.createHash('sha256')
    .update(req.body.password + salt)
    .digest('hex')
  const telegramToken = crypto.randomBytes(8).toString('hex')
  try {
    const doc = await mongoManager.createUser({
      _id: req.body.email,
      email: req.body.email,
      password: hash,
      salt: salt,
      telegramToken: telegramToken,
      devices: []
    })
    let grafanaUserRes = await createUser(req.body.email, req.body.email, req.body.password)
    doc.garfanaId = grafanaUserRes.id
    doc.garfanaFolderUid = grafanaUserRes.folderUid

    await axios.post("http://" + TELEGRAM_HOST + "/users", {
      token: telegramToken,
      userId: req.body.email,
    })
    await doc.save();
    res.status(201).send()
  } catch (err) {
    console.log(err)
    if (err && err.code && err.code === 11000)
      res.status(409).send("duplicate email")
    else
      res.status(400).send()
    res.status(500).send()
  }
})

app.get('/users/api/telegram', async (req, res) => {
  try {
    if(req.query.telegramToken && req.query.userId) {
      let user = await mongoManager.findUserById(req.query.userId)
      if(user.telegramToken !== req.query.telegramToken){
        res.status(401).send("invalid token")
        return
      }
      user.devices.forEach((device) => device.token = undefined)
      res.status(200).send({
        email : user.email,
        devices: user.devices,
        telegramToken: user.telegramToken
      })
    }
  } catch (err) {
    console.log(err)
    res.status(401).send("invalid token")
    return
  }
})

app.get('/users/me', async (req, res) => {
  try {
    let user = await verifyToken(req)
    user.devices.forEach((device) => device.token = undefined)
    res.status(200).send({
      email : user.email,
      devices: user.devices,
      telegramToken: user.telegramToken
    })
  } catch (err) {
    res.status(401).send("invalid token")
    return
  }
})

app.put('/users/me', async (req, res) => {
  const doc = await mongoManager.findUserById(req.body.email)
  if(!doc.email || !doc.password || !doc.salt){
    res.status(400).send()
    return
  }
  const hash = crypto.createHash('sha256')
                      .update(req.body.password + doc.salt)
                      .digest('hex')
  if(hash !== doc.password){
    res.status(400).send()
    return
  }
  const newHash = crypto.createHash('sha256')
    .update(req.body.newPassword + doc.salt)
    .digest('hex')
  doc.password = newHash
  doc.save()
  await changeUserPassword(doc.garfanaId, req.body.newPassword)
  res.status(200).send()
})

app.delete('/users/me', async (req, res) => {
  const doc = await mongoManager.findUserById(req.body.email)
  if(!doc.email || !doc.password || !doc.salt){
    res.status(400).send()
    return
  }
  const hash = crypto.createHash('sha256')
                      .update(req.body.password + doc.salt)
                      .digest('hex')
  if(hash !== doc.password){
    res.status(400).send()
    return
  }
  try {
    for(let device of doc.devices){
      deleteClient(device.id)
      deleteRole("role_" + device.id)
    }
    await deleteUser(doc.garfanaId, doc.garfanaFolderUid)
    await mongoManager.deleteUser(doc.email)
    res.status(200).send()
  } catch (err) {
    console.log(err)
    res.status(400).send()
  }
})

app.post('/auth', async (req, res) => {
  try {
    const doc = await mongoManager.findUserById(req.body.email)
    if(!doc.email || !doc.password || !doc.salt){
      res.status(400).send()
      return
    } 
    const hash = crypto.createHash('sha256')
                      .update(req.body.password + doc.salt)
                      .digest('hex')
    if(hash !== doc.password){
      res.status(400).send()
      return
    }

    let token = jwt.sign({
      data: doc.email
    }, TOKEN_SECRET, { expiresIn: '12h' })
    res.status(200).send({
      token: token
    })
  } catch (err) {
    console.log(err)
    res.status(400).send()
  }
})

app.put('/auth/refresh', async (req, res) => {
  try {
    const user = await verifyToken(req)
    let token = jwt.sign({
      data: user.email
    }, TOKEN_SECRET, { expiresIn: '12h' })
    res.status(200).send({
      token: token
    })
  } catch (err) {
    res.status(400).send()
  }
})

async function verifyToken(req){
  const bearerHeader = req.headers['authorization']
  //check if bearer is undefined
  if(typeof bearerHeader !== 'undefined'){
    //split the space at the bearer
    const bearer = bearerHeader.split(' ')
    //Get token from string
    const bearerToken = bearer[1]
    const email = jwt.verify(bearerToken, TOKEN_SECRET)
    return await mongoManager.findUserById(email.data)
  } else {
    throw new Error("Invalid token")
  }
}

app.post('/devices', async (req, res) => {
  try {
    const user = await verifyToken(req)
    const token = crypto.randomBytes(20).toString('hex')
    //console.log(req.body)
    let device = {
      userId: user._id,
      name: req.body.name,
      protocol: PROTOCOL,
      sampleFrequency: SAMPLE_FREQUENCY,
      configUpdateFrequency: CONFIG_UPDATE_FREQUENCY,
      minGasValue: MIN_GAS_VALUE,
      maxGasValue: MAX_GAS_VALUE,
      lastSeen: -1,
      proxyPort: parseInt(PROXY_PORT),
      mqttPort: parseInt(MQTT_PORT),
      udpPort: parseInt(UDP_PORT),
      token: token,
      latitude: req.body.latitude,
      longitude: req.body.longitude,
      host: "",
      wifiSsid: "",
      enablePerformanceMonitoring: ENABLE_PERFORMANCE,
      totalMqttPacketCount: 0,
      receivedMqttPacketCount: 0,
      totalHttpPacketCount: 0,
      receivedHttpPacketCount: 0,
      mqttMeanTime: 0,
      httpMeanTime: 0,
      alertEnable: false,
      lastAlertTime: -1,
      lastTempValue: -1,
      lastHumidityValue: -1,
      lastGasValue: -1,
      lastSoilValue: -1,
      lastAqiValue: -1,
      lastRssiValue: -1,
      enablePrediction: false,
      trainingTime: 30,
      predictionTime: 10
    }
    const id = await mongoManager.createDevice(user._id, device)
    const grafanaDashboardRes = await createDashboard(user.garfanaId, user.garfanaFolderUid, id)
    await mongoManager.updateDevice(user._id, {
      id: id,
      dashboardUid: grafanaDashboardRes.uid,
      dashboardUrl: grafanaDashboardRes.url
    })
    createRole("role_" + id, id)
    createClient(id, token, "role_" + id)
    res.status(200).send(device)
  } catch (err) {
    console.log(err)
    res.status(401).send("invalid token")
    return
  }
})

app.put('/devices/:id', async (req, res) => {
  try {
    const user = await verifyToken(req)
    const deviceId = req.params.id
    let device = undefined
    try {
      for( let d of user.devices )
        if(d.id === deviceId){
          device = d
          break;
        }
    } catch (err) {}
    let alertRes = {uid: undefined}
    if(device && req.body.alertEnable !== undefined) {
      if(req.body.alertEnable && !device.alertEnable)
        alertRes = await createAlert(deviceId, user.garfanaFolderUid)
      if(!req.body.alertEnable)
        await deleteAlert(device.alertUid)
    }
    await mongoManager.updateDevice(user._id, {
      id: deviceId,
      protocol: req.body.protocol,
      sampleFrequency: req.body.configUpdateFrequency,
      sampleFrequency: req.body.sampleFrequency,
      minGasValue: req.body.minGasValue,
      maxGasValue: req.body.maxGasValue,
      enablePerformanceMonitoring: req.body.enablePerformanceMonitoring,
      alertEnable: req.body.alertEnable,
      alertUid: alertRes.uid,
      enablePrediction: req.body.enablePrediction,
      trainingTime: req.body.trainingTime,
      predictionTime: req.body.predictionTime
    })
    // console.log(req.body)
    if(req.body.protocol !== "MQTT")
      disableClient(deviceId)
    else
      enableClient(deviceId)
    
    res.status(200).send()
  } catch (err) {
    console.log(err)
    res.status(401).send("invalid token")
  }
})

app.delete('/devices/:id', async (req, res) => {
  try {
    const user = await verifyToken(req)
    const deviceId = req.params.id
    try {
      for( let d of user.devices )
        if(d.id === deviceId){
          await deleteDashboard(d.dashboardUid)
          await deleteAlert(d.alertUid)
          break;
        }
    } catch (err) {}
    try {
      await axios.delete("http://" + TELEGRAM_HOST + "/users/" + user._id)
    } catch (err) {}
    mongoManager.deleteDevice(user._id, deviceId)
    res.status(200).send()
  } catch (err) {
    console.log(err)
    res.status(401).send("invalid token")
  }
})

app.post('/alerts', async (req, res) => {
  try {
    for(let alert of req.body.alerts){
      console.log("New alert of device: " + alert.labels.deviceId)
      const deviceId = alert.labels.deviceId
      // TODO mettere device name
      // console.log(alert)
      // console.log(deviceId)
      const userId = await mongoManager.findUserIdByDeviceId(deviceId)
      const secondsSinceEpoch = Math.round(Date.now() / 1000)
      const user = await mongoManager.findUserById(userId)
      await mongoManager.updateDevice(userId, {
        id: deviceId,
        lastAlertTime: secondsSinceEpoch,
        lastAlert: alert.valueString
      })
      await axios.post("http://" + TELEGRAM_HOST + "/users/" + userId + "/messages", {
        telegramToken: user.telegramToken,
        message: "Alert fired for device " + deviceId + "\n AQI >= 2"
      })
    }
    res.status(200).send()
  } catch (err) {
    console.log(err)
    res.status(500).send()
  }
})
