import React from 'react';
import './App.css';
import { Overlay, GeoJson, Map, Marker } from 'pigeon-maps';

const config = {
  defaultMapCenter: [
    43.92353815,
    -79.44267511
  ],
  defaultPlane: {
    latitude: 43.92353815,
    longitude: -79.44267511,
    track: 0,
  },
  pollingMs: 100,
};

function App() {
  const windowHeight = React.useRef(window.innerHeight);
  const [plane, setPlane] = React.useState(config.defaultPlane);
  const [history, setHistory] = React.useState([]);
  const [running, setRunning] = React.useState(false);

  console.log(history);
   React.useEffect(() => {
    const id = setInterval(() => {
      if (!running) {
        return;
      }
      fetch('http://192.168.0.13:8080/gps', {
        crossDomain: true,
        method: 'GET',
      })
      .then(async response => {
        try {
          let json = await response.json();
          setPlane(json);
          setHistory(prev => {
            return [...prev, [json.longitude, json.latitude]];
          });
        } catch (err) {
          // Do nothing on error
        }
      })
      .catch(err => console.error(err));
        // Do nothing on error
      }, config.pollingMs);

     return _ => clearInterval(id);
  }, [running]);

  const geoJson = {
    type: "FeatureCollection",
    features: [{
      "type": "Feature",
      "color": "#FF00FF",
      "properties": {},
        "geometry": {
          "coordinates": history.slice(-100),
          "type": "LineString"
        }
      }
    ]
  };

  return (
    <div className="App">
      <div onClick={_ => setRunning(!running)}><p>{running ? "STOP" : "START"}</p></div>
      <div
        style={{
          display: 'flex',
          height: '200px',
          width: '200px',
          transform: `rotate(${plane.track}deg)`,
          backgroundImage: 'url(/compass.png)',
          backgroundRepeat: 'no-repeat',
          backgroundSize: '200px 200px',
          alignItems: 'center',
          justifyContent: 'center',
        }}
      >
        <img src='/compass_plane.png' width={80} height={80} alt='' 
          style={{
            transform: `rotate(-${plane.track}deg)`,
           }}
        />
      </div>
      <Map height={windowHeight.current} defaultCenter={config.defaultMapCenter} defaultZoom={17}>
        {/*<Marker width={30} anchor={[plane.latitude, plane.longitude]} color='black' />*/}
        <GeoJson
          data={geoJson}
          styleCallback={(feature, hover) => {
            return {
              stroke: `${feature.color}DD`,
              strokeWidth: '3'
            };
          }}
        />
        <Overlay anchor={[plane.latitude, plane.longitude]} offset={[25, 23]}>
          <img src='/plane.png' width={50} height={45} alt='' 
            style={{
               transform: `rotate(${plane.track}deg)`,
             }}
          />
        </Overlay>
      </Map>
    </div>
  );
}

export default App;
