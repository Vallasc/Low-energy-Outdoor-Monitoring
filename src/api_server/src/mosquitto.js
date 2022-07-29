import mqtt from 'mqtt'

const MQTT_USERNAME = process.env.MQTT_USERNAME || "admin"
const MQTT_PASSWORD = process.env.MQTT_PASSWORD || "admin"
const MQTT_HOST = process.env.MQTT_HOST || "127.0.0.1"

const client  = mqtt.connect("mqtt://" + MQTT_HOST, {username: MQTT_USERNAME, password: MQTT_PASSWORD})

client.on("connect",function(){	
  console.log("MQTT broker connected "+ client.connected);
})

export function createClient(username, password, rolename) {
  const command = {
    "commands":[
      {
        "command": "createClient",
        "username": username,
        "password": password,
        "roles": [{ "rolename": rolename}]
      }
    ]
  }
  client.publish("$CONTROL/dynamic-security/v1", JSON.stringify(command))
  // console.log("MQTT client created " + username)
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

export function disableClient(username){
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

export function enableClient(username){
  const command = {
    "commands":[
      {
        "command": "enableClient",
        "username": username
      }
    ]
  }
  client.publish("$CONTROL/dynamic-security/v1", JSON.stringify(command))
}

export function createRole(rolename, username){
  const command = {
    "commands":[
      {
        "command": "createRole",
			  "rolename": rolename,
			  "acls": [
				  { "acltype": "publishClientSend", "topic": "devices/" + username, "priority": 1, "allow": true},
          { "acltype": "publishClientReceive", "topic": "devices/" + username, "priority": 1, "allow": true},
          { "acltype": "subscribePattern", "topic": "devices/" + username, "priority": 1, "allow": true},
          { "acltype": "unsubscribePattern", "topic": "devices/" + username, "priority": 1, "allow": true}
			  ] 
      }
    ]
  }
  client.publish("$CONTROL/dynamic-security/v1", JSON.stringify(command))
}

export function deleteRole(rolename){
  const command = {
    "commands":[
      {
        "command": "deleteRole",
			  "rolename": rolename
      }
    ]
  }
  client.publish("$CONTROL/dynamic-security/v1", JSON.stringify(command))
}