const express = require('express');
const app = express();
const port = 3000;

let sensorData = {
  humidity: 0,
  temperature: 0,
  gas: 0,
  light: 0,
  motion: 0
};

app.use(express.static('public'));

app.get('/update', (req, res) => {
  sensorData.humidity = req.query.humidity;
  sensorData.temperature = req.query.temperature;
  sensorData.gas = req.query.gas;
  sensorData.light = req.query.light;
  sensorData.motion = req.query.motion;
  res.send('Data updated');
});

app.get('/data', (req, res) => {
  res.json(sensorData);
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}/`);
});
