<script lang="ts">
  import { selectedDevice, user } from "../../stores";
  import { getUser, putDevice, deleteDevice } from '../logic'
  import { navigate } from "svelte-navigator"

  export let deviceId = ''

  const PROTOCOL_TYPES = ["MQTT", "HTTP"]

  let protocol = ''
  let sampleFrequency = -1
  let configUpdateFrequency = -1
  let minGasValue = -1
  let maxGasValue = -1
  let enableMonitoring = false
  let enableAlert = false
  let grafanaHost = window.location.protocol + "//" + window.location.hostname + ":3333"

  async function saveButton() {
    await putDevice(deviceId, {
      protocol: protocol,
      sampleFrequency: sampleFrequency,
      configUpdateFrequency: configUpdateFrequency,
      minGasValue: minGasValue,
      maxGasValue: maxGasValue,
      enablePerformanceMonitoring: enableMonitoring,
      alertEnable: enableAlert
    })
    await init()
  }

  async function deleteButton() {
    await deleteDevice(deviceId)
    navigate("/devices")
  }

  function fillVars() {
    protocol = $selectedDevice.protocol
    sampleFrequency = $selectedDevice.sampleFrequency
    configUpdateFrequency = $selectedDevice.configUpdateFrequency
    minGasValue = $selectedDevice.minGasValue
    maxGasValue = $selectedDevice.maxGasValue
    enableMonitoring = $selectedDevice.enablePerformanceMonitoring
    enableAlert = $selectedDevice.alertEnable
  }

  function updateSampleFrequencyParams(){
    let sampleEl = document.getElementById('range') as HTMLInputElement
    sampleEl.min = '' + configUpdateFrequency
    sampleEl.step = '' + configUpdateFrequency
    sampleFrequency = sampleFrequency < configUpdateFrequency? configUpdateFrequency : sampleFrequency
    sampleFrequency = sampleFrequency % configUpdateFrequency != 0? configUpdateFrequency : sampleFrequency
  }

  async function init() {
    console.log("init")
    $selectedDevice = {}
    $user = await getUser()
    for(let d of $user.devices)
      if(d.id == deviceId){
        $selectedDevice = d
        fillVars()
        return
      }
  }

  if(!$selectedDevice)
    init()
  else
    fillVars()
</script>

<div class="mb-3 mt-3">
  <h3>Device settings</h3>
</div>
<div class="row">
  <div class="column">
    <div class="block">
      <h5>Configuration</h5>
      <table class="table align-middle table-striped">
        <thead>
          <tr>
            <th>Key</th>
            <th>Value</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <th>Id</th>
            <td>{$selectedDevice.id}</td>
          </tr>
          <tr>
            <th>Name</th>
            <td>{$selectedDevice.name}</td>
          </tr>
          <tr>
            <th>Last seen</th>
            <td>{$selectedDevice.lastSeen}</td>
          </tr>
          <tr>
            <th>Latitude</th>
            <td>{$selectedDevice.latitude}</td>
          </tr>
          <tr>
            <th>Longitude</th>
            <td>{$selectedDevice.longitude}</td>
          </tr>
          <tr>
            <th>Wifi SSID</th>
            <td>{$selectedDevice.wifiSsid}</td>
          </tr>
          <tr>
            <th>Server host</th>
            <td>{$selectedDevice.host}</td>
          </tr>
          <tr>
            <th>MQTT port</th>
            <td>{$selectedDevice.mqttPort}</td>
          </tr>
          <tr>
            <th>HTTP port</th>
            <td>{$selectedDevice.proxyPort}</td>
          </tr>
          <tr>
            <th>Grafana dashboard</th>
            <td><a href="{grafanaHost}{$selectedDevice.dashboardUrl}" target="_blank">link</a></td>
          </tr>
          <tr class="divider"></tr>
          <tr class="divider"></tr>
          <tr>
            <th>Protocol</th>
            <td>
              <select class="form-select" bind:value={protocol}>
                {#each PROTOCOL_TYPES as p}
                  <option value={p}>
                    {p}
                  </option>
                {/each}
              </select>
            </td>
          </tr>
          <tr>
            <th>Config update frequency</th>
            <td><input class="form-control" bind:value={configUpdateFrequency}  type="number" step="1" min="1" max="60" on:change={updateSampleFrequencyParams}></td>
          </tr>
          <tr>
            <th>Sample frequency</th>
            <td><input id="range" class="form-control" bind:value={sampleFrequency}  type="number" step="{configUpdateFrequency}" min="{configUpdateFrequency}" max="120"></td>
          </tr>
          <tr>
            <th>Min gas value</th>
            <td><input id="range" class="form-control" bind:value={minGasValue}  type="number" step="1" min="0" max="100"></td>
          </tr>
          <tr>
            <th>Max gas value</th>
            <td><input id="range" class="form-control" bind:value={maxGasValue}  type="number" step="1" min="0" max="100"></td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
  <div class="column">
    <div class="block">
      <h5>Performance</h5>
      <div class="form-check">
        <input class="form-check-input" type="checkbox" value="" id="check-perf" bind:checked = {enableMonitoring}>
        <label class="form-check-label" for="check-perf">
          Enable performance monitoring
        </label>
      </div>
      {#if enableMonitoring}
        <table class="table align-middle table-striped">
          <thead>
            <tr>
              <th>Protocol</th>
              <th>Received/Sent packets</th>
              <th>Latency</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <th>MQTT</th>
              <td>{$selectedDevice.receivedMqttPacketCount}/{$selectedDevice.totalMqttPacketCount}</td>
              <td>{$selectedDevice.mqttMeanTime}ms</td>
            </tr>
            <tr>
              <th>HTTP</th>
              <td>{$selectedDevice.receivedHttpPacketCount}/{$selectedDevice.totalHttpPacketCount}</td>
              <td>{$selectedDevice.httpMeanTime}ms</td>
            </tr>
          </tbody>
        </table>
      {/if}
    </div>
    <div class="block">
      <h5>Alert</h5>
      <div class="form-check">
        <input class="form-check-input" type="checkbox" value="" id="check-perf" bind:checked = {enableAlert}>
        <label class="form-check-label" for="check-perf">
          Enable alerting messages when AQI >= 2
        </label>
      </div>
      {#if enableAlert}
        <table class="table align-middle table-striped">
          <thead>
            <tr>
              <th>Last alert</th>
              <th>Info</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>{$selectedDevice.lastAlertTime}</td>
              <td>{$selectedDevice.lastAlert}</td>
            </tr>
          </tbody>
        </table>
      {/if}
    </div>
    <div class="block">
      <h5>Prediction</h5>
      <table class="table align-middle table-striped">
        <thead>
          <tr>
            <th>Bo</th>
            <th>Bo</th>
            <th>Bo</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <th> Mean Square Error (MSE)</th>
            <td>0</td>
            <td>0</td>
          </tr>
          <tr>
            <th>Bo</th>
            <td>0</td>
            <td>0</td>
          </tr>
        </tbody>
      </table>
    </div>
  </div>
</div>
<div class="row">
  <button class="w-100 mb-3 btn btn-primary" type="submit" on:click={saveButton}>Save settings</button>
  <button class="w-100 mb-3 btn btn-danger" type="submit" on:click={deleteButton}>DELETE DEVICE</button>
</div>
<style>
  .table td, .table th {
    padding: 10px;
    margin: 16px;
  }
  .table {
    background-color: #fff;
    background-clip: border-box;
    border: 1px solid rgba(0,0,0,.125);
  }

  .column {
    max-width: 700px;
  }

  .block {
    padding-top: 24px;
  }

  .row {
    justify-content: center;
  }
  .divider {
    border-color: #000;
    border-style: solid;
    border-width: 2px;
  }

  .form-select, .form-control {
    padding: 0px;
    background-color: transparent;
    border-color: transparent !important;
    font-size: 0.875rem;
    font-weight: 200;
    letter-spacing: 1px;
  }

  button {
    max-width: 300px;
    margin: 50px;
  }
</style>