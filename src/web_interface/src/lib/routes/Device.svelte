<script lang="ts">
  import { selectedDevice, user, location } from "../../stores";
  import { fade } from 'svelte/transition'
  import { getUser } from '../logic'
  import { onMount } from "svelte";

  export let deviceId = ''
  let url = window.location.protocol + "//" + window.location.hostname

  async function init() {
    console.log("init")
    if(!$selectedDevice) {
      $selectedDevice = {}
      $user = await getUser()
      for(let d of $user.devices)
        if(d.id == deviceId) {
          $selectedDevice = d
          return
        }
    }
    let iframe: any = document.getElementById("dashboard-iframe")
    if(window.location.protocol === "https:")
      iframe.src = url + $selectedDevice.dashboardUrl + "?orgId=1&refresh=5s&theme=light&kiosk=tv"
    else
      iframe.src = url + ":3333" + $selectedDevice.dashboardUrl + "?orgId=1&refresh=5s&theme=light&kiosk=tv"
    console.log(iframe.src)
  }

  onMount(async () => {
		await init()
	})
</script>

<div class="mb-3 mt-3" in:fade = {{duration: 200}}>
  <h3>Grafana dashboard</h3>
</div>
  <iframe 
    id="dashboard-iframe" in:fade = {{delay: 1000, duration: 200}} 
    title="Grafana dashboard"></iframe>

<style>
  iframe {
    width: 100%;
    height: calc(100% - 85px);
  }

  h3:focus {
    outline: none;
  }
</style>