export type { User, Device, Position, Protocol, Connection }

interface User {
    id?: string
    email: string
    password?: string
    devices?: Array<Device>
    telegramToken?: string
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
    udpPort?: number
    host?: string
    wifiSsid?: string
    wifiPassword?: string
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
    httpMeanTime?: number,
    alertEnable?: boolean,
    lastAlertTime?: number,
    lastAlert?: string,
    lastTempValue: number,
    lastHumidityValue: number,
    lastGasValue: number,
    lastSoilValue: number,
    lastAqiValue: number,
    lastRssiValue: number,
    enablePrediction: boolean,
    trainingTime: number,
    predictionTime: number
}
