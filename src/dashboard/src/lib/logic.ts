import { jwtToken, toast } from "../stores"
import type { User, Device } from "./types"

const LOMO_HOST = "http://localhost:80"
const DEVICE_HOST = "http://192.168.1.10:80"

let token: string | null = null;
jwtToken.subscribe(value => {
  token = value;
})

export function showToast(title: string, body: string) {
  toast.set({
    title: title,
    body: body,
    visible: true
  })
}

export function init() {
  let token = localStorage.getItem("jwtToken")
  if (token != null) {
    jwtToken.set(token)
    refreshToken()
  }
}

export async function Fetch(path: string, method: string, body: any = null): Promise<Response> {
  console.log(path)
  if (token != null) {
    const response = await fetch(path, {
      method: method,
      cache: 'no-cache',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': 'Bearer ' + token
      },
      body: body != null ? JSON.stringify(body) : null
    })
    if (response.status == 401) {
      jwtToken.set(null)
      localStorage.removeItem("jwtToken")
    }
    return response
  }
  return Promise.resolve(new Response(new Blob(), { "status": 401 }))
}

// Login
export async function signin(user: User, remainSignin: boolean): Promise<boolean> {
  const response = await fetch(LOMO_HOST + "/auth",
    {
      method: 'POST',
      cache: 'no-cache',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(user)
    })
  console.log(response)
  if (response.status == 200) {
    let token: string = (await response.json()).token
    jwtToken.set(token);
    if (remainSignin) {
      localStorage.setItem("jwtToken", token)
    }
    return true
  } else {
    showToast("Auth error", "Wrong credentials")
    return false
  }
}

export async function signup(user: User): Promise<boolean> {
  const response = await fetch(LOMO_HOST + "/users",
    {
      method: 'POST',
      cache: 'no-cache',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(user)
    })
  console.log(response)
  if (response.status == 201) {
    showToast("Sign up status", "Registration successful")
    return true
  } if (response.status == 409) {
    showToast("Sign up status", "Email not available")
    return false
  } else {
    showToast("Sign up status", "Internal server error")
    return false
  }
}

export function signout(): void {
  jwtToken.set(null)
  localStorage.removeItem("jwtToken")
}

export async function refreshToken(): Promise<any> {
  console.log("Refreshing token")
  const response = await Fetch(LOMO_HOST + "/auth/refresh", "PUT")
  if (response.status == 200) {
    let token: string = (await response.json()).token
    jwtToken.set(token);
    if (localStorage.getItem("jwtToken") != null) {
      localStorage.setItem("jwtToken", token)
    }
  }
  return null
}

export async function getUser(): Promise<User | null> {
  const response = await Fetch(LOMO_HOST + "/users/me", "GET")
  if (response.status == 200) {
    return await response.json()
  }
  return Promise.resolve(null)
}

export async function pairDevice(deviceName: string): Promise<Device | null> {
  const response = await Fetch(LOMO_HOST + "/devices", "POST", {name: deviceName})
  if(response.status == 200)
    return await response.json()
  return Promise.resolve(null)
}

export async function initHost(deviceInit: Device): Promise<boolean> {
  const response = await fetch(DEVICE_HOST + "/init", {
    method: "POST",
    cache: 'no-cache',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(deviceInit)
  })
  return response.status == 200
}