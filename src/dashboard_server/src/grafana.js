import axios from 'axios'

const GRAFANA_URL = process.env.GRAFANA_URL || "127.0.0.1"
const GRAFANA_PORT = process.env.GRAFANA_PORT || 3333
const GRAFANA_USER = process.env.GRAFANA_USER || "admin"
const GRAFANA_PASS = process.env.GRAFANA_PASS || "admin"

const baseUrl = `http://${GRAFANA_USER}:${GRAFANA_PASS}@${GRAFANA_URL}:${GRAFANA_PORT}/api/admin/users`

export async function createUser(email, username, password) {
    try{
        const res = axios.post(baseUrl, {
        name : username,
        email: email,
        login: username,
        password: password,
        OrgId: 1
        })
        return res.status == 200
    }catch(e){
        return false
    }
}