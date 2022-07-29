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
  deleteRole, } from './mosquitto.js'
import {
  createUser, 
  createDashboard, 
  deleteUser, 
  changeUserPassword,
  deleteDashboard } from './grafana.js'
import {MongoManager} from './mongo.js'
import mongoose from 'mongoose'

import express from 'express'
import path from 'path'
import cors from 'cors'
import crypto from 'crypto'
import jwt from 'jsonwebtoken'

const app = express()

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
  console.log(`HTTP server started on PORT ${PORT}`)
})

const handler = (req, res) => res.sendFile(path.join(__dirname, './public/index.html'))
const routes = ["/", "/dashboard", "/signin", "/signup", "/signout", "/profile", "/devices",  "/devices/*"]
routes.forEach(route => app.get(route, handler))


app.post('/users', async (req, res) => {
  const salt = crypto.randomBytes(20).toString('hex')
  const hash = crypto.createHash('sha256')
    .update(req.body.password + salt)
    .digest('hex')

  try {
      const doc = await mongoManager.createUser({
        _id: req.body.email,
        email: req.body.email,
        password: hash,
        salt: salt,
        devices: []
      })
      let grafanaUserRes = await createUser(req.body.email, req.body.email, req.body.password)
      doc.garfanaId = grafanaUserRes.id
      doc.garfanaFolderUid = grafanaUserRes.folderUid
      await doc.save();
      res.status(201).send()
  } catch (err) {
    //console.log(err)
    if (err && err.code && err.code === 11000)
      res.status(409).send("duplicate email")
    else
      res.status(400).send()
    res.status(500).send()
  }
})

app.get('/users/me', async (req, res) => {
  try {
    let user = await verifyToken(req)
    user.devices.forEach((device) => device.token = undefined)
    res.status(200).send({
      email : user.email,
      devices: user.devices
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
      httpMeanTime: 0
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
    await mongoManager.updateDevice(user._id, {
      id: deviceId,
      protocol: req.body.protocol,
      sampleFrequency: req.body.configUpdateFrequency,
      sampleFrequency: req.body.sampleFrequency,
      minGasValue: req.body.minGasValue,
      maxGasValue: req.body.maxGasValue,
      enablePerformanceMonitoring: req.body.enablePerformanceMonitoring
    })
    // console.log(req.body)
    if(req.body.protocol === "HTTP")
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
          break;
        }
    } catch (err) {}
    mongoManager.deleteDevice(user._id, deviceId)
    res.status(200).send()
  } catch (err) {
    console.log(err)
    res.status(401).send("invalid token")
  }
})