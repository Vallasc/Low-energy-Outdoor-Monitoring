<script lang="ts">
  import { toast } from "../../stores";
  const TOAST_SEC = 3;
  let timeout = null

  $: {
      if ($toast.visible)
        timeout = setTimeout(() => ($toast.visible = false), TOAST_SEC * 1000)
    }
</script>

<div
  class="toast main-toast"
  class:show={$toast.visible}
  role="alert"
  aria-live="assertive"
  aria-atomic="true"
>
  <div class="toast-header">
    <strong class="me-auto">{$toast.title}</strong>
    <button
      type="button"
      class="btn-close"
      data-bs-dismiss="toast"
      aria-label="Close"
      on:click={()=>{
        $toast.visible = false
        clearTimeout(timeout)
      }}
    />
  </div>
  <div class="toast-body">
    {$toast.body}
  </div>
</div>

<style>
  .toast {
    position: fixed;
    bottom: 40px;
    left: 50%;
    transform: translateX(-50%);
  }
</style>
