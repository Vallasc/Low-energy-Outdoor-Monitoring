export type { User, Device, Position, Protocol, Connection }

interface User {
    id?: string
    email: string
    password?: string
}

interface Device {
    id: string
    userId?: string
    position: Position
    connectionType: Connection
    sampleFrequency: number
    minGasValue: number
    maxGasValue: number
    airQuality: number
    averageDelay: number
    packetDeliveryRatio: number
    telegramUser: number
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