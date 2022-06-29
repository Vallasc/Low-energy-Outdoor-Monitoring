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
    minGasValue: number
    maxGasValue: number
    lastSeen?: number
    proxyPort?: string
    mqttPort?: string
    host?: string
    wifiSsid?: string
    wifiPassword?: string
    token?: string
    //airQuality: number
    //averageDelay: number
    //packetDeliveryRatio: number
}

interface Position {
    latitude: number
    longitude: number
}

interface Protocol {
    isMQTT: boolean
    isHTTP: boolean
}

interface Connection {
    isWIFI: boolean
    isSIM: boolean
}