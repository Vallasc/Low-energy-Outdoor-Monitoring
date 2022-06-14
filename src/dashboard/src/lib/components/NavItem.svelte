<script lang="ts">
  import { location } from "../../stores"
  export let link: string = "/"
  export let indent: number = 0
  let active: boolean
  let alternativeActive: boolean

    $: {
      active = $location === link
    }
</script>

<li class="nav-item" style="margin-left:{indent*10}px">
  <a class="nav-link" class:active class:alternativeActive  aria-current="page" href={link}>
    <slot></slot>
  </a>
</li>
