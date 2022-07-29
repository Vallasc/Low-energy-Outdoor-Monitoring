<script lang="ts">
  import { location } from "../../stores"
  export let link: string = "/"
  export let indent: number = 0
  export let hide: boolean = false
  export let selectable: boolean = true
  let active: boolean

    $: {
      active = $location === link && selectable
    }
</script>

<li class="nav-item" class:hide style="margin-left:{indent*10}px">
  <a class="nav-link" class:active  aria-current="page" href={link}>
    <slot></slot>
  </a>
</li>


<style>
  .hide {
    display: none;
  }
</style>
