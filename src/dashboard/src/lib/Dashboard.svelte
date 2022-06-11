<script lang="ts">
  import Content from './Content.svelte'
  import Sidemenu from './Sidemenu.svelte'
</script>

<div class="container-fluid">
  <div class="row">
    <nav class="col-md-3 col-lg-3 bg-white">
      <Sidemenu></Sidemenu>
    </nav>
    <div class="col-md-9 col-lg-9 px-md-4 main">
      <Content></Content>
    </div>
  </div>
</div>

<style>
  .container-fluid{
    font-size: 0.875rem;
  }
  .container-fluid, .row{
    height: 100%;
  }
  .main {
    height: 100%;
    overflow: auto;
  }

</style>
