<script lang="ts">
  let navhide = true
</script>

<div class="row">
  <nav class="col-md-3 col-lg-2 bg-white pt-3">
    <div
      class="mb-3 d-flex justify-content-between flex-wrap align-items-center"
    >
      <slot name="title" />
      <button
        on:click={() => {navhide = !navhide}}
        class="navbar-toggler bg-light nav-btn-hide"
        type="button"
        aria-controls="navbarToggleExternalContent"
        aria-label="Toggle navigation"
      >
        <svg
          xmlns="http://www.w3.org/2000/svg"
          width="24"
          height="24"
          fill="currentColor"
          class="bi bi-list"
          viewBox="0 0 16 16"
        >
          <path
            fill-rule="evenodd"
            d="M2.5 12a.5.5 0 0 1 .5-.5h10a.5.5 0 0 1 0 1H3a.5.5 0 0 1-.5-.5zm0-4a.5.5 0 0 1 .5-.5h10a.5.5 0 0 1 0 1H3a.5.5 0 0 1-.5-.5zm0-4a.5.5 0 0 1 .5-.5h10a.5.5 0 0 1 0 1H3a.5.5 0 0 1-.5-.5z"
          />
        </svg>
      </button>
    </div>
    <div class:navhide>
      <ul class="nav nav-pills flex-column mb-auto">
        <slot name="item-t0" />
        <slot name="item-t1" />
        <slot name="item-t2" />
        <slot name="item-t3" />
        <slot name="item-t4" />
        <slot name="item-t5" />
        <slot name="item-t6" />
        <slot name="item-t7" />
        <slot name="item-t8" />
        <slot name="item-t9" />
        <slot name="item-t10" />
        <slot name="item-t11" />
        <slot name="item-t12" />
      </ul>
    </div>
  </nav>
  <div class="col-md-9 col-lg-10 px-md-4 main">
    <slot name="content" />
  </div>
</div>

<style>
  .row {
    font-size: 0.875rem;
    height: 100%;
    margin: 0;
  }

  .main {
    height: 100%;
    overflow: auto;
    background-color: #f4f5f5;
    padding-bottom: 30px;
  }
  nav {
    border-right: 1px solid var(--bs-gray-400);
  }

  @media (min-width: 768px) {
    .nav-btn-hide {
      display: none;
    }
  }

  @media (max-width: 768px) {
    .navhide {
      display: none;
    }

    nav {
      border-right: none;
    }
  }
</style>
