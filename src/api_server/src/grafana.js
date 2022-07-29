import axios from 'axios'
import { createRequire } from "module";

const require = createRequire(import.meta.url);
const defaultDashboard = require("./defualtDashboard.json");

const GRAFANA_HOST = process.env.GRAFANA_HOST || "127.0.0.1"
const GRAFANA_PORT = process.env.GRAFANA_PORT || 3333
const GRAFANA_USER = process.env.GRAFANA_USER || "admin"
const GRAFANA_PASS = process.env.GRAFANA_PASS || "admin"

const baseUrl = `http://${GRAFANA_USER}:${GRAFANA_PASS}@${GRAFANA_HOST}:${GRAFANA_PORT}`

export async function createUser(email, username, password) {
  let res = await axios.post(baseUrl + "/api/admin/users", {
    name: username,
    email: email,
    login: username,
    password: password
  })
  if (res.status == 200) {
    var folderRes = await createFolder(res.data.id, username)
    if (folderRes.status != 200)
      throw new Error("Couldn't create grafana user")
  } else {
    throw new Error("Couldn't create grafana user")
  }
  res.data.folderUid = folderRes.data.uid
  return res.data
}

export async function deleteUser(userId, folderUid) {
  await deleteFolder(folderUid)
  return await axios.delete(baseUrl + "/api/admin/users/" + userId)
}

export async function changeUserPassword(userId, newPassword) {
  return await axios.put(baseUrl + "/api/admin/users/" + userId + "/password", {
    password: newPassword
  })
}

export async function createFolder(userId, username) {
  const res = await axios.post(baseUrl + "/api/folders", {
    title: username + " dashboards"
  })
  if (res.status == 200) {
    const resPerm = await axios.post(baseUrl + "/api/folders/" + res.data.uid + "/permissions", {
      items: [{
        userId: userId,
        permission: 1
      }]
    })
  }
  return res;
}

export async function deleteFolder(folderUid) {
  return await axios.delete(baseUrl + "/api/folders/" + folderUid)
}

export async function createDashboard(userId, folderUid, deviceId) {
  // const dashboard = {
  //   id: null,
  //   uid: null,
  //   title: "Production Overview",
  //   tags: [],
  //   timezone: "browser",
  //   schemaVersion: 16,
  //   version: 0,
  //   refresh: "25s"
  // }
  let dashboard = JSON.parse(
                    JSON.stringify(defaultDashboard)
                        .replaceAll("${deviceId}", deviceId))
  dashboard.title = "Device-" + deviceId
  let payload = {
    dashboard: dashboard,
    folderUid: folderUid,
    message: "Initial commit",
    overwrite: true
  }
  var res = await axios.post(baseUrl + "/api/dashboards/db", payload)
  if(res.status == 200)
    await axios.post(baseUrl + "/api/dashboards/uid/" + res.data.uid + "/permissions", {
      items: [
        {
          role: "Viewer",
          permission: 1
        },{
          userId: userId,
          permission: 1
      }]
    })
  return res.data
}

export async function deleteDashboard(dashboardUid) {
  var res = await axios.delete(baseUrl + "/api/dashboards/uid/" + dashboardUid)
  return res.data
}