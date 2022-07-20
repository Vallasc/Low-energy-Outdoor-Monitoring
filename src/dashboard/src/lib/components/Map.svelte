<script lang="ts">
  import { onMount } from "svelte"
  import Map from "ol/Map"
  import OSM from "ol/source/OSM"
  import TileLayer from "ol/layer/Tile"
  import View from "ol/View"
  import { Vector as LayerVector } from "ol/layer"
  import { Vector as SourceVector } from "ol/source"
  import { defaults as defaultControls } from "ol/control"
  import { Feature } from "ol"
  import { Point } from "ol/geom"
  import { Icon, Style } from "ol/style"
  import { fromLonLat, toLonLat } from "ol/proj"
  import marker from "../../assets/red-blank.png"
  import "ol/ol.css"

  export let latitude = 0;
  export let longitude = 0;
  export let autodetectPosition = false;
  export let selectPosition = false;
  export let initialZoom = 3;
  export let isVisible = true;

  let icon = new Feature({
      type: "icon",
      geometry: new Point(fromLonLat([longitude, latitude])),
  })

  icon.setStyle(
    new Style({
      image: new Icon({
        src: marker,
        anchor: [0.5, 1],
        scale: 0.5,
      }),
    })
  )

  if (autodetectPosition && navigator.geolocation) {
    navigator.geolocation.getCurrentPosition((position) => {
      console.log(position)
      setCoordinates(position.coords.longitude, position.coords.latitude)
    })
  } else {
    console.log("Geolocation is not supported by this browser.")
  }

  function setCoordinates(longitude: number, latitude: number) {
    let coordinates = [longitude, latitude]
    icon.setGeometry(new Point(fromLonLat(coordinates)))
    view.setCenter(fromLonLat(coordinates))
  }

  let view = new View({
      center: [0, 0],
      //zoom: 14,
      zoom: initialZoom,
  })

  $: {
    setCoordinates(longitude, latitude)
  }

  let mapElement

  onMount(()=>{
    mapElement = document.getElementById("map")
  })

  $: {
    try {
      mapElement.innerHTML = ''
    } catch (e) {}

    if(isVisible){
      const map = new Map({
        controls: defaultControls(),
        layers: [
          new TileLayer({
            source: new OSM(),
          }),
          new LayerVector({
            source: new SourceVector({
              features: [icon],
            }),
          }),
        ],
        target: "map",
        view: view,
      })

      if(selectPosition){
        map.on("singleclick", function (evt) {
          const coordinate = evt.coordinate
          const lon_lat = toLonLat(coordinate)
          longitude = lon_lat[0]
          latitude = lon_lat[1]
          //icon.setGeometry(new Point(coordinate))
        })
      }
    }
  }

</script>

<div id="map" class="map"/>

<style>
  .map {
    width: 100%;
    height: 400px;
  }
</style>
