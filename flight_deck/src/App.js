import 'bootstrap/dist/css/bootstrap.min.css';
import React from 'react';
import './App.css';
import { Chart } from "react-google-charts";
import RCPlaneMap from './components/RCPlaneMap';
import RCStateIndicator from './components/RCStateIndicator';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import {
  Airspeed,
  Altimeter,
  AttitudeIndicator,
  HeadingIndicator,
  TurnCoordinator,
  Variometer
} from 'react-typescript-flight-indicators'

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
  const [plane, setPlane] = React.useState(config.defaultPlane);
  const [gyro, setGyro] = React.useState({
    pitch: 0,
    roll: 0,
    yaw: 0
  });
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
      fetch('http://192.168.0.22:8080/gps', {
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
      fetch('http://192.168.0.22:8080/gyro', {
        crossDomain: true,
        method: 'GET',
      })
      .then(async response => {
        try {
          let json = await response.json();
          setGyro(json);
        } catch (err) {
          // Do nothing on error
        }
      })
      .catch(err => console.error(err));
        // Do nothing on error
      }, 100);

     return _ => clearInterval(id);
  }, [running]);

   React.useEffect(() => {
    const id = setInterval(() => {
      if (!running) {
        return;
      }
      fetch('http://192.168.0.22:8080/cs', {
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
      }, 100);

     return _ => clearInterval(id);
  }, [running]);

  return (
    <div className="App">
        <Container fluid>
          <Row>
            <Col>
              <RCPlaneMap plane={plane} history={history} />
            </Col>
            <Col>
              <Row xs={5} style={{ height: 150 }}>
                <Col>
                  <RCStateIndicator 
                    flightEnabled={Boolean(cs.state & 1)}
                    motorTest={Boolean(cs.state & 2)}
                    aileronTest={Boolean(cs.state & 4)}
                    elevatorTest={Boolean(cs.state & 8)}
                    rudderTest={Boolean(cs.state & 16)}
                  />
                </Col>
                <Col>
                  <Chart
                    chartType="Gauge"
                    data={[['Label', 'Value'], ["motor", cs.motor || 0]]}
                    options={{
                      animation: {
                        duration: 200,
                      },
                      max: 255,
                      width: 150,
                      height: 150,
                      redFrom: 240,
                      redTo: 255,
                      yellowFrom: 200,
                      yellowTo: 240,
                      minorTicks: 5,
                    }}
                  />
                </Col>
                <Col>
                  <Chart
                    chartType="Gauge"
                    data={[['Label', 'Value'], ["aileron", cs.aileron || 0]]}
                    options={{
                      animation: {
                        duration: 200,
                      },
                      min: -50,
                      max: 50,
                      width: 150,
                      height: 150,
                      minorTicks: 5,
                    }}
                  />
                </Col>
                <Col>
                  <Chart
                    chartType="Gauge"
                    data={[['Label', 'Value'], ["elevator", cs.elevator || 0]]}
                    options={{
                      animation: {
                        duration: 200,
                      },
                      min: -30,
                      max: 30,
                      width: 150,
                      height: 150,
                      minorTicks: 5,
                    }}
                  />
                </Col>
                <Col>
                  <Chart
                    chartType="Gauge"
                    data={[['Label', 'Value'], ["rudder", cs.rudder || 0]]}
                    options={{
                      animation: {
                        duration: 200,
                      },
                      min: -50,
                      max: 50,
                      width: 150,
                      height: 150,
                      minorTicks: 5,
                    }}
                  />
                </Col>
              </Row>
              <Row xs={3} style={{ height: 150 }}>
                <Col>
                  <Airspeed speed={plane.speed * 50} showBox={false} />
                </Col>
                <Col>
                  <HeadingIndicator heading={plane.track} showBox={false} />
                </Col>
                <Col>
                  <AttitudeIndicator roll={-1 *gyro.roll} pitch={gyro.pitch} showBox={false} />
                </Col>
              </Row>
            </Col>

            <div onClick={_ => setRunning(!running)}><p>{running ? "STOP" : "START"}</p></div>
        </Row>
      </Container>
    </div>
  );
}

export default App;

/*
                <Col>
                  <Chart
                    chartType="Gauge"
                    data={[['Label', 'Value'], ["m/s", plane.speed || 0]]}
                    options={{
                      max: 4,
                      redFrom: 3.5,
                      redTo: 4,
                      yellowFrom: 2.5,
                      yellowTo: 3.5,
                      minorTicks: 5,
                    }}
                  />
                </Col>
                <Col>
                </Col>
                */
