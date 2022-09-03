<script lang="ts">
  import telegram from "../../assets/telegram.png";
  import { fade } from "svelte/transition";
  import { user } from "../../stores";
</script>

<div class="form">
  <div class="block">
    <img
      class="mb-3 mt-4"
      src={telegram}
      alt="telegram"
      height="120"
      in:fade={{ duration: 200 }}
    />
    <h1 class="h3 fw-normal">Telegram BOT</h1>
    <a
      href="https://t.me/LOMO_API_BOT?start={$user.telegramToken}"
      class="w-100 mb-3 mt-3 btn btn-primary"
      target="_blank">Clck here</a
    >
  </div>
</div>

<style>
  .form {
    width: 100%;
    display: flex;
    align-items: center;
    flex-direction: column;
    justify-content: flex-start;
  }
  .block {
    width: 100%;
    max-width: 350px;
    padding: 30px;
    display: flex;
    align-items: center;
    flex-direction: column;
  }
</style>
