import mqtt from 'mqtt'

const MQTT_USER = process.env.MQTT_USER || "admin"
const MQTT_PASS = process.env.MQTT_PASS || "admin"
const MQTT_URI = process.env.MQTT_URI || "mqtt://localhost"

const client  = mqtt.connect(MQTT_URI, {username: MQTT_USER, password: MQTT_PASS})

client.on("connect",function(){	
  console.log("connected  "+ client.connected);
})

export function createClient(username, password) {
  const command = {
    "commands":[
      {
        "command": "createClient",
        "username": username,
        "password": password,
        "roles": [{ "rolename": "device"}]
      }
    ]
  }
  client.publish("$CONTROL/dynamic-security/v1", JSON.stringify(command))
}

export function deleteClient(username) {
  const command = {
    "commands":[
      {
        "command": "deleteClient",
        "username": username
      }
    ]
  }
  client.publish("$CONTROL/dynamic-security/v1", JSON.stringify(command))
}

export function disableClient(){
  const command = {
    "commands":[
      {
        "command": "disableClient",
        "username": username
      }
    ]
  }
  client.publish("$CONTROL/dynamic-security/v1", JSON.stringify(command))
}