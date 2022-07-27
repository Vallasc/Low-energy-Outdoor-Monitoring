import { writable } from 'svelte/store'

export const jwtToken = writable(null)
export const location = writable("/")
export const selectedDevice = writable(null)

export const toast = writable({
    title: "",
    body: "",
    visible: false
})