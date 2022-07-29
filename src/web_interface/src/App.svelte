<script lang="ts">
  import { Router, Route, links, navigate } from "svelte-navigator"
  import Nav from './lib/components/Nav.svelte'
  import NavItem from './lib/components/NavItem.svelte'
  import { jwtToken, selectedDevice, user } from "./stores"
  import Signin from "./lib/routes/Signin.svelte"
  import Signup from "./lib/routes/Signup.svelte"
  import Page404 from "./lib/routes/404.svelte"
  import Profile from "./lib/routes/Profile.svelte"
  import Devices from "./lib/routes/Devices.svelte"
  import Device from "./lib/routes/Device.svelte"
  import DeviceSettings from "./lib/routes/DeviceSettings.svelte"
  import UpdateRoute from "./lib/components/UpdateRoute.svelte"
  import Toast from "./lib/components/Toast.svelte"
  import { init, signout, getUser } from "./lib/logic"

  init()
  $user = {email: ""}
  getUser()

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
      <div slot="title">
        <h2 class="title">LOMO</h2>
        <div class="subtitle">{$user.email}</div>
      </div>
      <hr slot="item-t0"/>
      <NavItem slot="item-t1" link="/devices">Devices</NavItem>
      <hr slot="item-t2" class:hidden = {!$selectedDevice}/>
      <NavItem slot="item-t3" hide={!$selectedDevice} selectable={false} link="/devices/{deviceId}"><h5 class="device-name">{deviceName}</h5></NavItem>
      <NavItem slot="item-t4" hide={!$selectedDevice} link="/devices/{deviceId}">Dashboard</NavItem>
      <NavItem slot="item-t5" hide={!$selectedDevice} link="/devices/{deviceId}/settings">Settings</NavItem>
      <hr slot="item-t6"/>
      <NavItem slot="item-t7" link="/telegram">Telegram</NavItem>
      <NavItem slot="item-t8" link="/profile">Profile</NavItem>
      <NavItem slot="item-t9" link="/signout">Sign out</NavItem>
      <Router slot="content">
        <UpdateRoute/>
        <Route path="/">
          {navigate("/devices")}
        </Route>
        <Route path="/devices">
          <Devices/>
        </Route>
        <Route path="/devices/:device/settings" let:params>
          <DeviceSettings deviceId={params.device}/>
        </Route>
        <Route path="/devices/:device" let:params>
          <Device deviceId={params.device}/>
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
  <div class="footer">Made with ❤️ by Vallasc and Barba</div>
</div>

<Toast/>

<style>

  :global(html, body, #app) {
    height: 100%;
  }
  :global(.nav-item) {
    margin-right: 0px;
  }

  :global(h1:focus, h2:focus, h3:focus) {
    outline-color: transparent;
  }

  :global(input) {
    border: 1px solid #dcdcdc !important;
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
    margin-bottom: 5px;
  }
  @media (min-width: 768px) {
    .subtitle {
      text-overflow: ellipsis;
      display: block;
      overflow: hidden;
      max-width: 15vw;
    }

    .device-name {
      text-overflow: ellipsis;
      display: block;
      overflow: hidden;
      max-width: 11vw;
    }
  }
  
  .footer {
    position: fixed;
    bottom: 0;
    left: 50%;
    transform: translateX(-50%);
    margin: 5px;
    font-size: 14px;
  }

  .hidden {
    display: none;
  }
</style>
