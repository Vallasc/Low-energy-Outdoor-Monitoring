<script lang="ts">
  import DeviceCard from "../components/DeviceCard.svelte"
</script>

<div class="row row-cols-auto justify-content-center">
  <div class="col">
    <button class="btn btn-outline-primary btn-pair"
      >Pair a device</button>
  </div>
  <div class="col">
    <DeviceCard>
      <div slot="title">Giardino 1</div>
      <div slot="body">
        Active: True<br>
        Last seen: 10 min ago<br>
        Protocol: MQTT<br>
        <button type="button" class="btn mt-3 btn-outline-primary w-100">
          View
        </button>
      </div>
    </DeviceCard>
  </div>
  <div class="col">
    <DeviceCard>
      <div slot="title">Giardino 2</div>
      <div slot="body">
        Active: True<br>
        Last seen: 10 min ago<br>
        Protocol: MQTT<br>
        <button type="button" class="btn mt-3 btn-outline-primary w-100">
          View
        </button>
      </div>
    </DeviceCard>
  </div>
  <div class="col">
    <DeviceCard></DeviceCard>
  </div>
  <div class="col">
    <DeviceCard></DeviceCard>
  </div>
  <div class="col">
    <DeviceCard></DeviceCard>
  </div>
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
  <div class="col"><div class="hidden"></div></div>
</div>

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