import React from 'react';
import Col from 'react-bootstrap/Col';
import Row from 'react-bootstrap/Row';
import './stateIndicator.css';

export default function RCStateIndicator({
  flightEnabled,
  motorTest,
  aileronTest,
  elevatorTest,
  rudderTest
}) {

  return (
    <React.Fragment>
      <Row xs={4} className='flight_status'>
        <Col>
          <div className={`indicator_${motorTest}`} />
        </Col>
        <Col>
          <div className={`indicator_${aileronTest}`} />
        </Col>
        <Col>
          <div className={`indicator_${elevatorTest}`} />
        </Col>
        <Col>
          <div className={`indicator_${rudderTest}`} />
        </Col>
        <Row className='flight_name' xs={1}>
          <Col>
            <h2 className={`flight_status_${flightEnabled}`}>{ flightEnabled ? 'ENABLED' : 'DISABLED'}</h2>
          </Col>
        </Row>
      </Row>
    </React.Fragment>
  );
}
