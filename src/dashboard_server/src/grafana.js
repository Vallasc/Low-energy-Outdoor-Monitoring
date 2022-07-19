import axios from 'axios'

const GRAFANA_URL = process.env.GRAFANA_URL || 1883
const GRAFANA_PORT = process.env.GRAFANA_PORT || 1883

grafana_url = "ip172-18-0-23-c8vdq1g9jotg00duhte0-3000.direct.labs.play-with-docker.com"
username = "admin"
password = "admin"

axios
  .post('https://whatever.com/todos', {
    todo: 'Buy the milk',
  })
  .then(res => {
    console.log(`statusCode: ${res.status}`);
    console.log(res);
  })
  .catch(error => {
    console.error(error);
  });

import requests
import json


base_url = "http://{}:{}@{}".format(username, password, grafana_url)

data = {
  "name":"User1",
  "email":"user1@graf.com",
  "login":"user1",
  "password":"userpassword",
  "OrgId": 1

}


resp = requests.post(base_url + "/api/admin/users", json=data, verify=False)
data = resp.json()
data