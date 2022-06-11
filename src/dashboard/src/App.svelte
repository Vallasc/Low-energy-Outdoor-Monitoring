<script lang="ts">
  //import logo from './assets/svelte.png'
  import Signin from './lib/Signin.svelte'
  import Dashboard from './lib/Dashboard.svelte'
</script>

<Dashboard></Dashboard>

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
</style>
