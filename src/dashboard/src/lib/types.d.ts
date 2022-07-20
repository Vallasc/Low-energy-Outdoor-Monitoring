export type { User, Device, Position, Protocol, Connection }

interface User {
    id?: string
    email: string
    password?: string
    devices?: Array<Device>
}

interface Device {
    id: string
    name: string,
    userId?: string
    //position: Position
    protocol: string
    sampleFrequency: number
    configUpdateFrequency: number
    minGasValue: number
    maxGasValue: number
    lastSeen?: number
    proxyPort?: string
    mqttPort?: string
    host?: string
    wifiSsid?: string
    wifiPassword?: string
    token?: string
    latitude: number
    longitude: number
}
