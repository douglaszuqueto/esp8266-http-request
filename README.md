# Realizando requisições HTTP no ESP8266 utilizando a biblioteca HTTPClient

![img](https://raw.githubusercontent.com/douglaszuqueto/esp8266-http-request/master/files/esp8266-request.png)

## Introdução

Certamente você já precisou recuperar algum dado de um servidor, ou até mesmo enviar algum dado para o mesmo a partir do seu embarcado - neste caso estamos nos referindo ao nosso querido ESP8266.

Portanto, neste artigo, será abordado um pequeno cenário de como utilizar o protocolo HTTP para recuperação/envio de dados ao servidor. 

## HTTP(Hypertext Transfer Protocol)

O famoso protocolo HTTP. Quem nunca usou?(não se atreva a dizer que não :P).

Um protocolo baseado na arquitetura cliente/servidor, amplamente utilizado no nosso dia a dia - creio que grande parte das comunicações hoje em dia, usufruem do http.

### Verbos HTTP(mais utilizados - principalmente em API's)

O HTTP disponha de um conjunto de verbos http, então sem muitas delongas, fica abaixo os mais comuns e utilizados hoje nas comunicações:

| Verbo | descrição |
| --- | --- |
| GET | requisitar registros do servidor |
| POST | cadastrar um registro no servidor |
| PUT | atualizar algum registro no servidor |
| DELETE | remover algum registro do servidor |

Nas descrições, penssem em um recurso como manipular um sensor - cadastrá-lo, requisitar, atualizar e remover - famoso **CRUD** (create, read, update, delete).

## WebService para teste

Antes de continuar, segue uma simples estrutura para testes de um simples webservice em nodejs.

```js
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
```

O script se encontra dentro da pasta **server/nodejs**. Para rodar basta ter o nodejs instalado na maquina. Com ele instalado, voce só precisa baixar as depedências do projeto utilizando o NPM ou Yarn.

## Biblioteca HTTPClient

A lib HTTPClient provê de forma simples e descomplicada realizar comunicações http com um servidor. 
 
Possui um conjuntos de métodos simples e abstratos caso seja necessário usar algo mais complexo. A API em si, achei deveras interessante e de fácil entendimento.

Para utilizar, basta incluir a biblioteca em seu sketch.

```c
#include <ESP8266HTTPClient.h>
```

e.. para utilizar, basta criar o objeto da mesma:

```
HTTPClient http;
```

A partir deste momento, você já esta apto a realizar uma comunicação http com o servidor. Será abordado num tópico futuro.

### Métodos

Abaixo, segue uma lista dos metodos que utilizei no decorrer deste artigo:

```c
bool begin(String url); // metodo responsavel por começar a comunicação

void addHeader(const String& name, const String& value, bool first = false, bool replace = true); // método para adicionar cabeçalhos ao header de sua requisicao

int GET(); // responsavel por realizar uma requisição do tipo GET ao servidor

int POST(String payload); // responsavel por realizar uma requisição do tipo POST ao servidor

int sendRequest(const char * type, String payload); // metodo abstrado, caso queira usar outro método que a lib não suporte - DELETE, por exemplo

String getString(void); // recupera a resposta da requisicao, ja convertido em string 

void end(void); // responsavel por encerrar a comunicação

```

## Estrutura simples de uma requisição HTTP

```c
// ############# HTTP REQUEST ################ //

http.begin('URL DO SEU SERVICO'); // inicia a comunicacao com base na sua url
int httpCode = http.GET(); // efetua uma requisicao do tipo get e retorna o código de status da coenxao

if (httpCode < 0) { // caso o status for negativo, mostrará a mensagem no monitor serial
    Serial.println("request error - " + httpCode);
    return;
}
if (httpCode != HTTP_CODE_OK) { // caso o statuso for diferente de 200, reinicia o ciclo da requisicao
    return;
}
String payload = http.getString(); // passando das verificacoes acima, voce tem acesso ao corpo da mensagem retornado pelo servidor

http.end(); // finaliza a conexao

Serial.println("##[RESULT]## ==> " + payload); // imprimi na serial a string retornada pelo servidor

```
### Exemplo contendo todo firmware para o ESP

```c
// ############# LIBRARIES ############### //

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// ############# VARIABLES ############### //

const char* SSID = ""; // rede wifi
const char* PASSWORD = ""; // senha da rede wifi

String BASE_URL = "http://192.168.15.3:3000/";

// ############# PROTOTYPES ############### //

void initSerial();
void initWiFi();
void httpRequest(String path);

// ############### OBJECTS ################# //

WiFiClient client;
HTTPClient http;

// ############## SETUP ################# //

void setup() {
  initSerial();
  initWiFi();
}

// ############### LOOP ################# //

void loop() {
  Serial.println("[GET] /sensors - sending request...");
  Serial.println("");

  httpRequest("sensors");

  Serial.println("");
  delay(1000);

}

// ############# HTTP REQUEST ################ //

void httpRequest(String path)
{
  String payload = makeRequest(path);

  if (!payload) {
    return;
  }

  Serial.println("##[RESULT]## ==> " + payload);

}

String makeRequest(String path)
{
  http.begin(BASE_URL + path);
  int httpCode = http.GET();

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "";
  }

  String response =  http.getString();
  http.end();

  return response;
}

// ###################################### //

// implementacao dos prototypes

void initSerial() {
  Serial.begin(115200);
}

void initWiFi() {
  delay(10);
  Serial.println("Conectando-se em: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na Rede " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());
}
```
## Realizando requisições ao Servidor

Com base no firmware acima, basicamente pouca coisa muda, veja abaixo a principal mudança referente a cada método utilizado.

### GET - recuperando dados do servidor

```c
  http.begin('URL');
  int httpCode = http.GET();
```

### POST - enviando dados para o servidor

```c
  http.begin('URL');
  http.addHeader("content-type", "application/x-www-form-urlencoded");

  String body = "id=7890&name=NTC&value=10";

  int httpCode = http.POST(body);
```

## Finalizando

Todos exemplos que foram testados na construção deste artigo estarão no repositório do Github, segue o link:

* [GET](https://github.com/douglaszuqueto/esp8266-http-request/blob/master/esp8266/get.ino);
* [POST](https://github.com/douglaszuqueto/esp8266-http-request/blob/master/esp8266/post.ino);
* [GET by id - busca por ID](https://github.com/douglaszuqueto/esp8266-http-request/blob/master/esp8266/get-by-id.ino);
* [DELETE](https://github.com/douglaszuqueto/esp8266-http-request/blob/master/esp8266/delete.ino);

Deu para perceber o quão simples fica realizar requisições http a partir do esp? Eu particularmente curti bastante - eu nunca havia utilizado esta lib :P.

Os próximos passos será testar mais a biblioteca e também realizar o parser do JSON retornado pelo servidor utilizando a lib **ArduinoJSON**. Assim, dando alguma utilidade de fato ao contexto de determinado projeto.

## Referências

* [Biblioteca ESP8266HTTPClient](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient);
* [Hypertext Transfer Protocol](https://pt.wikipedia.org/wiki/Hypertext_Transfer_Protocol);

## Está gostando dos materiais que vêm sendo produzidos?

Caso tenha gostado deste e de outros materiais que mantenho, no dia 18/06/2017 abri uma campanha de financiamento recorrente com o objetivo de captar recursos para uma dedicação fulltime em projetos open source.
 
Portanto, caso deseja me ajudar, fica aqui o [link do meu Apoia.se](https://apoia.se/douglaszuqueto). Desde já, muito obrigado :D.
