import { writable } from 'svelte/store'

export const jwtToken = writable("null")
export const location = writable("/")

export const toast = writable({
    title: "",
    body: "",
    visible: false
})