<script lang="ts">
  import { signup, showToast } from "../logic"
  import { navigate } from "svelte-navigator"
  import { fade } from "svelte/transition"
  import type { User } from "../types"

  let user: User = {
    email: "",
    password: "",
  }

  let invalidPass: boolean
  let repeatPassword: string
  function validate(): boolean {
    if (user.password.trim().length == 0) return false;
    if (user.password != repeatPassword) {
      showToast("Form compilation error", "Passwords doesn't match")

      invalidPass = true
      return false
    }
    invalidPass = false
    return true
  }
  async function handleSubmit(): Promise<void> {
    if (!validate()) return
    if (await signup(user)) {
      navigate("/signin")
    }
  }

</script>

<div class="form" in:fade={{ duration: 200 }}>
  <h3 class="fw-normal">Sign up</h3>
  <form class="needs-validation" on:submit|preventDefault={handleSubmit}>
    <div class="mb-3">
      <label for="_" class="form-label">Email</label>
      <input
        bind:value={user.email}
        type="email"
        class="form-control"
        placeholder="Your email"
        required
      />
    </div>
    <div class="mb-3">
      <label for="_" class="form-label">Password</label>
      <input
        bind:value={user.password}
        type="password"
        class="form-control"
        placeholder="Your password"
        required
      />
    </div>
    <div class="mb-3">
      <input
        bind:value={repeatPassword}
        type="password"
        class="form-control"
        placeholder="Repeat your password"
        minlength="4"
        maxlength="20"
        required
      />
    </div>
    <button class="mb-3 mt-3 w-100 btn btn-primary" type="submit"
      >Sign up</button
    >
  </form>
</div>

<style>
  .form {
    width: 100%;
    display: flex;
    align-items: center;
    flex-direction: column;
    justify-content: flex-start;
    padding-top: 50px;
  }
  form {
    width: 100%;
    max-width: 350px;
    padding: 30px;
  }
</style>
