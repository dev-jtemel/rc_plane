import React from 'react';
import { Overlay, GeoJson, Map } from 'pigeon-maps';

export default function RCPlaneMap({
  plane,
  history
}) {
  const windowHeight = React.useRef(window.innerHeight);
  return (
    <Map height={windowHeight.current} width={'100%'} center={[plane.latitude, plane.longitude]} defaultZoom={17}>
      <GeoJson
        data={{
          type: "FeatureCollection",
          features: [{
            "type": "Feature",
            "color": "#FF80FF",
            "properties": {},
              "geometry": {
                "coordinates": history.slice(-50),
                "type": "LineString"
              }
            }
          ]
        }}
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
  );
};
