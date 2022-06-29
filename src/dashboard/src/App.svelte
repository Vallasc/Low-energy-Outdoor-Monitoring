<script lang="ts">
  import { Router, Route, links, navigate } from "svelte-navigator"
  import Nav from './lib/components/Nav.svelte'
  import NavItem from './lib/components/NavItem.svelte'
  import { jwtToken, selectedDevice } from "./stores"
  import Signin from "./lib/routes/Signin.svelte"
  import Signup from "./lib/routes/Signup.svelte"
  import Page404 from "./lib/routes/404.svelte"
  import Profile from "./lib/routes/Profile.svelte"
  import Devices from "./lib/routes/Devices.svelte"
  import Device from "./lib/routes/Device.svelte"
  import DeviceSettings from "./lib/routes/DeviceSettings.svelte"
  import DeviceDashboard from "./lib/routes/DeviceDashboard.svelte"
  import DeviceMap from "./lib/routes/DeviceMap.svelte"
  import UpdateRoute from "./lib/components/UpdateRoute.svelte"
  import Toast from "./lib/components/Toast.svelte"
  import { init, signout } from "./lib/logic"

  init()

  $: deviceId = $selectedDevice ? $selectedDevice.id : ""
  $: deviceName = $selectedDevice ? $selectedDevice.name : ""

</script>

<div class="main" use:links>
  {#if $jwtToken == null}
    <Router>
      <Route path="/signin">
        <Signin/>
      </Route>
      <Route path="/signup">
          <Signup/>
      </Route>
      <Route>
				{navigate("/signin")}
			</Route>
    </Router>
  {:else}
    <Nav>
      <h2 slot="title" class="title">LOMO</h2>
      <NavItem slot="item-t1" link="/devices">Devices</NavItem>
      <NavItem slot="item-t2" hide={!$selectedDevice} indent={1} link="/devices/{deviceId}">{deviceName}</NavItem>
      <NavItem slot="item-t3" hide={!$selectedDevice} indent={1} link="/devices/{deviceId}/dashboard">- Dashboard</NavItem>
      <NavItem slot="item-t5" hide={!$selectedDevice} indent={1} link="/devices/{deviceId}/map">- Map</NavItem>
      <NavItem slot="item-t4" hide={!$selectedDevice} indent={1} link="/devices/{deviceId}/settings">- Settings</NavItem>
      <NavItem slot="item-b1" link="/telegram">Telegram</NavItem>
      <NavItem slot="item-b2" link="/profile">Profile</NavItem>
      <NavItem slot="item-b3" link="/signout">Sign out</NavItem>
      <Router slot="content">
        <UpdateRoute/>
        <Route path="/">
          {navigate("/devices")}
        </Route>
        <Route path="/devices">
          <Devices/>
        </Route>
        <Route path="/devices/:device/settings">
          <DeviceSettings/>
        </Route>
        <Route path="/devices/:device/dashboard">
          <DeviceDashboard/>
        </Route>
        <Route path="/devices/:device/map">
          <DeviceMap/>
        </Route>
        <Route path="/devices/:device">
          <Device/>
        </Route>
        <Route path="/profile">
          <Profile/>
        </Route>
        <Route path="/signout">
          {signout()}
          {navigate("/")}
        </Route>
        <Route>
          <Page404></Page404>
        </Route>
      </Router>
    </Nav>
  {/if}
</div>

<Toast/>

<style>
  /*:global(p.dark-mode) {
      color: white;
  }*/
  :global(html, body, #app) {
    height: 100%;
  }
  :global(.nav-item) {
    margin-right: 0px;
  }

  :global(h1:focus, h2:focus, h3:focus) {
    outline-color: transparent;
  }

  .main {
		display: flex;
		flex-direction: column;
		align-items: stretch;
		background-color: #ffffff;
		height: 100%;
		width: 100%;
	}

  .title {
    margin-bottom: 0px;
  }
</style>
