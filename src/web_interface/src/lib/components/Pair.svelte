<script lang="ts">
  import Steps from "./Steps.svelte"
  import Map from "./Map.svelte"
  import { showToast, pairDevice } from "../logic"
  import type { Device as DeviceType } from "../types"

  export let show = true
  let state = 0
  let name = ""
  let ssid = ""
  let password = ""
  let latitude = 0
  let longitude = 0
  let timeout
  let address = ""
  let deviceInit: DeviceType
  let configResponse: any = {}
  let configuration: string

  if (!navigator.geolocation) {
    console.log('Geolocation is not supported by your browser')
  } else {
    console.log('Locating…')
    navigator.geolocation.getCurrentPosition((position) => {
      latitude = position.coords.latitude
      longitude = position.coords.longitude
    })
  }

  function clear(){
    state = 0
    name = ""
    ssid = ""
    password = ""
    latitude = 0
    longitude = 0
    address = ""
  }

  async function next() {
    switch(state) {
      case 0:
        if(state != 0) return
        name = name.trim()
        ssid = ssid.trim()
        password = password.trim()
        if (name == "" || ssid == "" || password == "") {
          showToast("Device error", "Fill all the fields")
          return;
        }
        try {
          deviceInit = await pairDevice(name, latitude, longitude)
          configuration = JSON.stringify(initDevice())
          console.log(configuration)
          state = 1
        } catch (e){
          console.error(e)
        }
        break
      case 1:
        state = 2
        break;
    }
  }

  function initDevice() {
    configResponse = {}
    configResponse.id = deviceInit.id
    configResponse.userId = deviceInit.userId
    configResponse.name = deviceInit.name
    configResponse.protocol = deviceInit.protocol
    configResponse.sampleFrequency = deviceInit.sampleFrequency
    configResponse.configUpdateFrequency = deviceInit.configUpdateFrequency
    configResponse.minGasValue = deviceInit.minGasValue
    configResponse.maxGasValue = deviceInit.maxGasValue
    configResponse.proxyPort = deviceInit.proxyPort
    configResponse.mqttPort = deviceInit.mqttPort
    configResponse.udpPort = deviceInit.udpPort
    configResponse.token = deviceInit.token
    configResponse.enablePerformanceMonitoring = deviceInit.enablePerformanceMonitoring
    configResponse.wifiSsid = ssid
    configResponse.wifiPassword = password
    configResponse.host = window.location.hostname
    configResponse.latitude = latitude
    configResponse.longitude = longitude
    return configResponse
  }

  $: {
    if(!show){
      clear()
    }
  }

  async function fetchLatLong(address: string){
    clearTimeout(timeout)
    let result = await fetch("https://nominatim.openstreetmap.org/search?q="+ encodeURIComponent(address) +"&format=json&polygon=1&addressdetails=1")
    let json = await result.json()
    if(json.length > 0){
      let first = json[0]
      console.log(first)
      longitude = parseFloat(first.lon)
      latitude = parseFloat(first.lat)
    }
  }
</script>

<div class="modal fade" class:show>
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">Pair a Device</h5>
        <button
          type="button"
          disabled = {state == 1}
          class="btn-close"
          aria-label="Close"
          on:click={() => (show = false)}
        />
      </div>
      <div class="modal-header">
        <Steps selected={state} />
      </div>
      {#if state == 0}
        <div class="modal-body">
          <div class="mb-3">
            <label for="_" class="form-label">Device name</label>
            <input
              type="text"
              class="form-control"
              placeholder="Name"
              bind:value={name}
              required
            />
          </div>
          <div class="mb-3">
            <label for="_" class="form-label">Wifi network</label>
            <input
              type="text"
              class="form-control"
              placeholder="WiFi SSID"
              bind:value={ssid}
              required
            />
          </div>
          <div class="mb-3">
            <input
              type="password"
              class="form-control"
              placeholder="WiFi Password"
              bind:value={password}
              required
            />
          </div>
          <div class="mb-3">
            <label for="_" class="form-label">Device coordinates</label>
            <input
              type="text"
              class="form-control"
              placeholder="Insert address or select on map"
              style="margin-bottom: 5px;"
              bind:value={address}
              on:input={() => {
                clearTimeout(timeout)
                timeout = setTimeout(()=>{
                  fetchLatLong(address)
                }, 1000)
              }}
            />
            <Map
              bind:latitude = {latitude}
              bind:longitude = {longitude}
              selectPosition = {true}
              isVisible = {show}
            ></Map>
          </div>
        </div>
      {:else if state == 1}
        <div class="modal-body">
          <div class="mb-3 mt-3">
            <ul class="list-group">
              <li class="list-group-item">
                1. Press the pair button on the device
              </li>
              <li class="list-group-item">
                2. Connect to network called LOMO_DEVICE
              </li>
              <li class="list-group-item">
                3. Open this <a href="http://192.168.1.10" target="_blank">page</a> and copy/paste the following configuration:
                <input bind:value={configuration} readonly/>
              </li>
            </ul>
          </div>
        </div>
      {:else}
        <div class="modal-body">
          <div class="mb-3 mt-3">
            <h4>Device initialization completed</h4>
            <div class="mb-3">You can now reconnect to your default network</div>
            <ul class="list-group">
              <li class="list-group-item">
                Id: {configResponse.id}
              </li>
              <li class="list-group-item">
                Name: {configResponse.name}
              </li>
              <li class="list-group-item">
                Wifi: {configResponse.wifiSsid}
              </li>
              <li class="list-group-item">
                Protocol: {configResponse.protocol}
              </li>
              <li class="list-group-item">
                Server: {configResponse.host}
              </li>
              <li class="list-group-item">
                MQTT port: {configResponse.proxyPort}
              </li>
              <li class="list-group-item">
                HTTP port: {configResponse.mqttPort}
              </li>
              <li class="list-group-item">
                UDP port: {configResponse.udpPort}
              </li>
              <li class="list-group-item">
                Sample frequency: {configResponse.sampleFrequency} minutes
              </li>
              <li class="list-group-item">
                Config frequency: {configResponse.configUpdateFrequency} minutes
              </li>
              <li class="list-group-item">
                Min gas value: {configResponse.minGasValue}
              </li>
              <li class="list-group-item">
                Max gas value: {configResponse.maxGasValue}
              </li>
            </ul>
          </div>
        </div>
      {/if}
      {#if state == 2}
        <div class="modal-footer">
          <button 
            type="button" 
            class="btn btn-primary"
            on:click={() => (show = false)}
          >Done</button
        ></div>
      {:else}
        <div class="modal-footer">
          <button 
            type="button" 
            class="btn btn-primary"
            on:click={next}>
            Next
          </button>
        </div>
      {/if}
    </div>
  </div>
</div>

<style>
  .show {
    display: block;
    background-color: #000000a6;
  }
</style>
