<script lang="ts">
  import Steps from "./Steps.svelte"
  import Map from "./Map.svelte"
  import { showToast, initHost, pairDevice } from "../logic"
  import type { Device } from "../types"

  export let show = true
  let state = 0
  let name = ""
  let ssid = ""
  let password = ""
  let latitude = 0
  let longitude = 0

  let address = ""
  let deviceInit: Device
  let timeout
  const RETRY_MSEC = 2000

  function clear(){
    clearTimeout(timeout)
    state = 0
    name = ""
    ssid = ""
    password = ""
    latitude = 0
    longitude = 0
    address = ""
  }

  async function next() {
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
      console.log(deviceInit)
      state = 1
      await initDevice()
    } catch (e){
      console.error(e)
    }
  }

  async function initDevice() {
    if (state != 1) return
    console.log("Device initializing")
    deviceInit.wifiSsid = ssid
    deviceInit.wifiPassword = password
    deviceInit.host = window.location.hostname
    deviceInit.latitude = latitude
    deviceInit.longitude = longitude
    console.log(deviceInit)
    try{
      let res = await initHost(deviceInit)
      console.log(res)
      if(res){
        state = 2
        clearTimeout(timeout)
      } else {
        timeout = setTimeout(initDevice, RETRY_MSEC)
      }
    } catch (e){
      console.error(e)
      timeout = setTimeout(initDevice, RETRY_MSEC)
    }
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
              autodetectPosition = {true}
              selectPosition = {true}
              isVisible = {show}
            ></Map>
          </div>
        </div>
      {:else if state == 1}
        <div class="modal-body">
          <div class="mb-3 mt-3">
            <h3 class="text-danger">DO NOT RELOAD THIS PAGE!</h3>
            <br />
            <ul class="list-group">
              <li class="list-group-item">
                1. Press the pair button on the device
              </li>
              <li class="list-group-item">
                2. Connect to network called LOMO_DEVICE
                <span class="badge bg-primary rounded-pill"
                  >Waiting <div class="spinner-border text-light" /></span>
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
                Id: {deviceInit.id}
              </li>
              <li class="list-group-item">
                Name: {deviceInit.name}
              </li>
              <li class="list-group-item">
                Protocol: {deviceInit.protocol}
              </li>
              <li class="list-group-item">
                Server: {deviceInit.host}
              </li>
              <li class="list-group-item">
                Sample frequency: {deviceInit.sampleFrequency} minutes
              </li>
              <li class="list-group-item">
                Config frequency: {deviceInit.configUpdateFrequency} minutes
              </li>
              <li class="list-group-item">
                Wifi: {deviceInit.wifiSsid}
              </li>
              <li class="list-group-item">
                Min gas value: {deviceInit.minGasValue}
              </li>
              <li class="list-group-item">
                Max gas value: {deviceInit.maxGasValue}
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
            disabled = {state == 1}
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
  .badge {
    margin-left: 5px;
  }
  .spinner-border {
    width: 10px;
    height: 10px;
  }
</style>
