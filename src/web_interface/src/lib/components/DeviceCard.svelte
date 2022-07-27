<div class="card">
  <div class="card-body">
    <h5 class="card-title"><slot name="title"></slot></h5>
    <slot name="body"></slot>
  </div>
</div>

<style>
  .card {
    width: 200px;
    height: 200px;
    margin: 15px;
  }

  .card-title {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }
</style>
