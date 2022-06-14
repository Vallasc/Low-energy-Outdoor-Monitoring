const express = require('express')
const path = require('path')
const mongoose = require('mongoose')
const app = express()
let cors = require('cors')
const crypto = require('crypto')
var jwt = require('jsonwebtoken');

const TOKEN_SECRET = process.env.TOKEN_SECRET || "lomolomo"
const USER = process.env.USER || "admin"
const PASS = process.env.PASS || "admin"
const PORT = process.env.PORT || 80
const DB_NAME = process.env.DB_NAME || "lomo"
const HOST_NAME = process.env.HOST_NAME || "localhost"
const MONGODB_URI = `mongodb://${USER}:${PASS}@${HOST_NAME}/${DB_NAME}?authSource=admin`

app.use(express.json())
app.use(express.urlencoded({ extended: true }))
//app.use(cors())
app.use(express.static(path.join(__dirname, '../public')))

//Initiate MongoDB and start server
app.listen(PORT, () => {
  mongoose.connect(MONGODB_URI, { useNewUrlParser: true }).then((response) => {
    console.log(`Connected to MongoDB and server started on PORT ${PORT}`)
  }).catch((err) => {
    console.log(err)
  })
})

const handler = (req, res) => res.sendFile(path.join(__dirname, '../public/index.html'))
const routes = ["/", "/dashboard", "/signin", "/signup", "/signout", "/profile"]
routes.forEach(route => app.get(route, handler))

const User = mongoose.model('User', mongoose.Schema({
  _id: String,
  email: String,
  password: String,
  salt: String
}, { _id: false }))

app.post('/users', async (req, res) => {
  const salt = crypto.randomBytes(20).toString('hex')
  const hash = crypto.createHash('sha256')
    .update(req.body.password + salt)
    .digest('hex')
  try {
    const doc = await User.create({
      _id: req.body.email,
      email: req.body.email,
      password: hash,
      salt: salt
    })
    //console.log(doc)
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
    const user = await verifyToken(req)
    res.status(200).send({
      email : user.email
    })
  } catch (err) {
    res.status(401).send("invalid token")
    return
  }
})

app.post('/auth', async (req, res) => {
  try {
    const doc = await User.findById(req.body.email)
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
    return await User.findById(email.data)
  } else {
    throw new Error("Invalid token")
  }
}