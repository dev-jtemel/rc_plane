import React from 'react';
import './App.css';
import { Overlay, GeoJson, Map, Marker } from 'pigeon-maps';
import { Chart } from "react-google-charts";

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
  const [cs, setCS] = React.useState({
    state: 0,
    motor: 0,
    aileron: 0,
    rudder: 0,
    elevator: 0
  });
  const [history, setHistory] = React.useState([]);
  const [running, setRunning] = React.useState(false);

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
      }, 500);

     return _ => clearInterval(id);
  }, [running]);

   React.useEffect(() => {
    const id = setInterval(() => {
      if (!running) {
        return;
      }
      fetch('http://192.168.0.13:8080/cs', {
        crossDomain: true,
        method: 'GET',
      })
      .then(async response => {
        try {
          let json = await response.json();
          setCS(json);
        } catch (err) {
          // Do nothing on error
        }
      })
      .catch(err => console.error(err));
        // Do nothing on error
      }, 300);

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
      <Map height={windowHeight.current} defaultCenter={config.defaultMapCenter} defaultZoom={17}>
        <div style={{ display: 'inline-flex', position: 'absolute' }}>
        <Chart
          chartType="Gauge"
          data={[['Label', 'Value'], ["m/s", plane.speed || 0]]}
          options={{
            max: 4,
            width: 210,
            height: 210,
            redFrom: 3.5,
            redTo: 4,
            yellowFrom: 2.5,
            yellowTo: 3.5,
            minorTicks: 5,
          }}
        />
        <Chart
          chartType="Gauge"
          data={[['Label', 'Value'], ["motor", cs.motor || 0]]}
          options={{
            max: 255,
            width: 210,
            height: 210,
            redFrom: 240,
            redTo: 255,
            yellowFrom: 200,
            yellowTo: 240,
            minorTicks: 5,
          }}
        />
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
      </div>
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
