<script lang="ts">
	import { useLocation } from "svelte-navigator"
    import { location } from "../../stores"
	const uselocation = useLocation()
	$: $location = $uselocation.pathname
    // $: console.log($location)
</script>