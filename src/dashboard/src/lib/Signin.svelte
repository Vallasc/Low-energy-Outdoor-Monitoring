<script lang="ts">
  import logo from '../assets/logo.png'
</script>

<div class="background">
  <div class="body text-center">
    <div class="form-signin w-100 m-auto">
      <form>
        <img
          class="mb-4"
          src={logo}
          alt="logo"
          height="150"
        />
        <h1 class="h3 mb-3 fw-normal">Please sign in</h1>

        <div class="form-floating">
          <input
            type="email"
            class="form-control"
            id="floatingInput"
            placeholder="name@example.com"
          />
          <label for="floatingInput">Email address</label>
        </div>
        <div class="form-floating">
          <input
            type="password"
            class="form-control"
            id="floatingPassword"
            placeholder="Password"
          />
          <label for="floatingPassword">Password</label>
        </div>

        <div class="checkbox mb-3">
          <label>
            <input type="checkbox" value="remember-me" /> Remember me
          </label>
        </div>
        <button class="w-100 btn btn-lg btn-primary" type="submit">Sign in</button>
        <p class="mt-3 mb-3 text-muted">or</p>
        <button class="w-100 btn btn-lg btn-outline-dark" type="submit">Sign up</button>
      </form>
    </div>
  </div>
</div>
<style>
  .background {
    height: 100%;
    overflow: auto;
    background-color: #f5f5f5;
  }

  .body {
    display: flex;
    align-items: center;
    padding-top: 40px;
    padding-bottom: 40px;
  }

  .form-signin {
    max-width: 330px;
    padding: 15px;
  }

  .form-signin .form-floating:focus-within {
    z-index: 2;
  }

  .form-signin input[type="email"] {
    margin-bottom: -1px;
    border-bottom-right-radius: 0;
    border-bottom-left-radius: 0;
  }

  .form-signin input[type="password"] {
    margin-bottom: 10px;
    border-top-left-radius: 0;
    border-top-right-radius: 0;
  }
</style>
