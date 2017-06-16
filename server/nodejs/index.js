const express = require('express')
  , bodyParser = require('body-parser')
  , cors = require('cors')
  , app = express();

/* Middlewares */
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));
app.use(cors());

const temperatures = [
  {
    'id': 1234,
    'name': 'DHT22',
    'value': 25,
  },
  {
    'id': 4321,
    'name': 'DHT11',
    'value': 25,
  }
];

/* Routes */
app.get('/temperatures', (req, res) => res.json(temperatures));

app.get('/temperatures/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const temperature = temperatures.filter((item) => item.id === id);

  if (temperature.length <= 0) {
    return res.json({});
  }
  return res.json(temperature[0]);
});

app.post('/temperatures', (req, res) => {
  const temperature = {
    'id': req.body.id,
    'name': req.body.name,
    'value': req.body.value,
  };
  temperatures.push(temperature);

  res.json(temperature);
});

/* App listen */
app.listen(3000, () => {
  console.log(`nodejs-backend is running`);
  console.log(`open in http://127.0.0.1:3000/temperatures`);
});
