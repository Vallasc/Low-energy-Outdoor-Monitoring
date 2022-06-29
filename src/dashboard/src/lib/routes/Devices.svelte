<script lang="ts">
  import DeviceCard from "../components/DeviceCard.svelte"
  import Pair from "../components/Pair.svelte"
  import {getUser} from "../logic"
  import { navigate } from "svelte-navigator"
  import { selectedDevice } from "../../stores"

  let showPairScreen = false

  let devices = []
  async function get(){
    let user = await getUser()
    devices = user.devices
    console.log(devices)
  }

  get()
  $selectedDevice = null

  function openDevice(device){
    $selectedDevice = device
    navigate("/devices/" + device.id)
  }

</script>

<div class="row row-cols-auto justify-content-center">
  <div class="col">
    <button class="btn btn-outline-primary btn-pair" on:click={() => showPairScreen = true}
      >Pair a device</button>
  </div>
  {#each devices as device}
    <div class="col">
      <DeviceCard>
        <div slot="title">{device.name}</div>
        <div slot="body">
          Active: True<br>
          Last seen: 10 min ago<br>
          Protocol: {device.protocol}<br>
          <button type="button" class="btn mt-3 btn-outline-primary w-100" on:click={() => openDevice(device)}>
            View
          </button>
        </div>
      </DeviceCard>
    </div>
  {/each}
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
</div>
<Pair bind:show={showPairScreen}></Pair>

<style>
  .row {
    margin-top: 15px;
  }
  .btn-pair {
    width: 200px;
    height: 200px;
    margin: 15px;
  }
  .hidden {
    width: 200px;
    margin: 15px;
  }
</style>