const express = require('express')
  , bodyParser = require('body-parser')
  , cors = require('cors')
  , app = express();

/* Middlewares */
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));
app.use(cors());

const sensors = [
  {
    'id': 1234,
    'name': 'DHT22',
    'type': 'temperature',
    'value': 25,
  },
  {
    'id': 4321,
    'name': 'DHT11',
    'type': 'temperature',
    'value': 25,
  }
];

/* Routes */
app.get('/sensors', (req, res) => res.json(sensors));

app.get('/sensors/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const sensor = sensors.filter((item) => item.id === id);

  if (sensor.length <= 0) {
    return res.json({});
  }
  return res.json(sensor[0]);
});

app.post('/sensors', (req, res) => {
  const sensor = {
    'id': req.body.id,
    'name': req.body.name,
    'type': req.body.type,
    'value': req.body.value,
  };
  sensors.push(sensor);

  res.json(sensor);
});

app.delete('/sensors/:id', (req, res) => {
  console.log(req.params.id);

  return res.json({});
});

/* App listen */
app.listen(3000, () => {
  console.log(`nodejs-backend is running`);
  console.log(`open in http://127.0.0.1:3000/sensors`);
});
