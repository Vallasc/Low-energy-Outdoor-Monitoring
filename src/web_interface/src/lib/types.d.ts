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
    protocol: string
    sampleFrequency: number
    configUpdateFrequency: number
    minGasValue: number
    maxGasValue: number
    lastSeen?: number
    proxyPort?: number
    mqttPort?: number
    host?: string
    wifiSsid?: string
    token?: string
    latitude: number
    longitude: number
    dashboardUid?: string,
    dashboardUrl?: string,
    enablePerformanceMonitoring?: boolean,
    totalMqttPacketCount?: number,
    receivedMqttPacketCount?: number,
    totalHttpPacketCount?: number,
    receivedHttpPacketCount?: number,
    mqttMeanTime?: number,
    httpMeanTime?: number
}
