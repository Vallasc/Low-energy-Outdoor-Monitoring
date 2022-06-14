<script lang="ts">
  import { signin } from "../logic";
  import logo from "../../assets/logo.png";
  import { navigate } from "svelte-navigator";
  import { fade } from "svelte/transition";

  
  let email: string;
  let password: string;
  let remember: boolean = false;
  async function handleSubmit(): Promise<void> {
    if (await signin({email: email, password: password}, remember)) {
      navigate("/");
    }
  }
</script>

<div class="form" on:submit|preventDefault={handleSubmit}>
  <img
    class="mb-3 mt-4"
    src={logo}
    alt="airplane"
    height="120"
    in:fade={{ duration: 200 }}
  />
  <h1 class="h3 fw-normal">Please sign in</h1>
  <form in:fade={{ duration: 200 }}>
    <div class="mb-3 form-floating">
      <input
        bind:value={email}
        type="email"
        class="form-control"
        placeholder="Pippo"
        required
      />
      <label for="floatingInput">Email</label>
    </div>
    <div class="mb-3 form-floating">
      <input
        bind:value={password}
        type="password"
        class="form-control"
        placeholder="Password"
        required
      />
      <label for="floatingPassword">Password</label>
    </div>

    <div class="checkbox mb-3">
      <div class="form-check">
        <input
          bind:checked={remember}
          class="form-check-input"
          type="checkbox"
        />
        <label class="form-check-label" for="gridCheck"> Rimani loggato </label>
      </div>
    </div>
    <button class="w-100 mb-3 btn btn-primary" type="submit">Sign in</button>
    <hr class="dropdown-divider" />
    <a class="w-100 btn btn-light" href="/signup">Sign up</a>
  </form>
</div>

<style>
  .form {
    width: 100%;
    display: flex;
    align-items: center;
    flex-direction: column;
    justify-content: flex-start;
  }
  form {
    width: 100%;
    max-width: 350px;
    padding: 30px;
  }
  .form .checkbox {
    font-weight: 400;
  }
  .form .form-floating:focus-within {
    z-index: 2;
  }
</style>
