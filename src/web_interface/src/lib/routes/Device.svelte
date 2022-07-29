<script lang="ts">
  import { selectedDevice, user } from "../../stores";
  import { fade } from 'svelte/transition'
  import { getUser } from '../logic'

  export let deviceId = ''

  // let url = "#"
  // src="http://192.168.1.229:3333{$selectedDevice.dashboardUrl}?orgId=1&refresh=5s&theme=light&kiosk"
  // TODO sistema iframe che carica prima di avere l'id della dashboard

  async function init() {
    console.log("init")
    $selectedDevice = {}
    $user = await getUser()
    for(let d of $user.devices)
      if(d.id == deviceId){
        $selectedDevice = d
        return
      }
  }

  if(!$selectedDevice)
    init()
</script>

<div class="mb-3 mt-3" in:fade = {{duration: 200}}>
  <h3>Grafana dashboard</h3>
</div>
<iframe in:fade = {{delay: 1000, duration: 200}} title="Grafana dashboard" src="http://192.168.1.229:3333{$selectedDevice.dashboardUrl}?orgId=1&refresh=5s&theme=light&kiosk">
</iframe>

<style>
  iframe {
    width: 100%;
    height: calc(100% - 85px);
  }
</style>