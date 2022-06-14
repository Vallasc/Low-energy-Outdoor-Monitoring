import { jwtToken, toast } from "../stores"
import type { User } from "./types"

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
    let response = await fetch(path, {
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
  const response = await fetch("/auth",
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
  const response = await fetch("/users",
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
  const response = await Fetch("/auth/refresh", "PUT")
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
  const response = await Fetch("/users/me", "GET")
  if (response.status == 200) {
    return await response.json()
  }
  return Promise.resolve(null)
}

export function signOut() {
  localStorage.removeItem("jwtToken")
  jwtToken.set(null)
}

/*export async function updateUser(name: string, surname: string, email: string, password: string, 
                    newPassword: string, newProntogramUsername: string) : Promise<User | null> {
    const response = await Fetch(ACMESKY_HOST + "/users/me", "PUT", {
        name: name,
        surname: surname,
        email: email,
        password: password,
        newPassword: newPassword,
        newProntogramUsername: newProntogramUsername
    })
    if (response.status == 200) {
        showToast("Profilo aggiornato", true)
        return await response.json()
    } else if (response.status == 400) {
        showToast("Credenziali errate", true)
        return Promise.resolve(null)
    }
    showToast("Errore interno"+ response.status, true)
    return Promise.resolve(null)
}

export async function deleteUser(email: string, password: string) : Promise<boolean> {
    const response = await Fetch(ACMESKY_HOST + "/users/me", "DELETE", {
        email: email,
        password: password,
    })
    if (response.status == 200) {
        showToast("Profilo eliminato", true)
        signout()
        return true
    } else if (response.status == 400) {
        showToast("Credenziali errate", true)
        return false
    }
    showToast("Errore interno"+ response.status, true)
    return false
}*/