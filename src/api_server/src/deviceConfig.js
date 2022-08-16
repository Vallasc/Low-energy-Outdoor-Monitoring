export const MIN_GAS_VALUE = process.env.MIN_GAS_VALUE || 30
export const MAX_GAS_VALUE = process.env.MAX_GAS_VALUE || 200
export const SAMPLE_FREQUENCY = process.env.SAMPLE_FREQUENCY || 1
export const CONFIG_UPDATE_FREQUENCY = process.env.CONFIG_UPDATE_FREQUENCY || 1
export const PROTOCOL = process.env.PROTOCOL || "MQTT"
export const PROXY_PORT = process.env.PROXY_PORT || 8082
export const MQTT_PORT = process.env.MQTT_PORT || 1883
export const UDP_PORT = process.env.UDP_PORT || 9090

export const ENABLE_PERFORMANCE = process.env.ENABLE_PERFORMANCE || false