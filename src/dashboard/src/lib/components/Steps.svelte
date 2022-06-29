<script lang="ts">
  import { fade } from "svelte/transition";
  export let selected: number = 0;
</script>

<div class="steps" in:fade={{ duration: 200 }}>
  <div class="step-arrow start" class:active={selected == 0}>Setup</div>
  <div class="step-arrow middle" class:active={selected == 1}>Upload</div>
  <div class="step-arrow end" class:active={selected == 2}>Finalising</div>
</div>

<style>
  .steps {
    width: 100%;
    position: relative;
    /* Centering inside arrow*/
    display: flex;
    justify-content: center;
    align-items: center;
  }
  .step-arrow {
    flex: 1;
    /*width: 200px;*/
    max-width: 200px;
    height: 40px;
    position: relative;
    font-weight: bold;
    /* Centering inside arrow*/
    display: flex;
    justify-content: center;
    align-items: center;
    margin-right: 12px;
    margin-left: 4px;
    background: #f1f1f1;
    color: #949494;
  }
  .step-arrow.start {
    margin-left: 12px;
  }
  .step-arrow.active {
    background: #000;
    color: #fff;
  }
  .step-arrow.start:before {
    content: "";
    border-top: 20px solid transparent;
    border-bottom: 20px solid transparent;
    border-left: 16px solid #f1f1f1;
    position: absolute;
    right: -16px;
  }
  .step-arrow.start.active:before {
    border-left: 16px solid #000;
  }
  .step-arrow.middle:before {
    content: "";
    border-top: 20px solid transparent;
    border-bottom: 20px solid transparent;
    border-left: 16px solid #f1f1f1;
    position: absolute;
    right: -16px;
  }
  .step-arrow.middle:after {
    content: "";
    border-top: 20px solid transparent;
    border-bottom: 20px solid transparent;
    border-left: 16px solid #fff;
    position: absolute;
    left: 0px;
  }
  .step-arrow.middle.active:before {
    border-left: 16px solid #000;
  }
  .step-arrow.end:after {
    content: "";
    border-top: 20px solid transparent;
    border-bottom: 20px solid transparent;
    border-left: 16px solid #fff;
    position: absolute;
    left: 0px;
  }
</style>
