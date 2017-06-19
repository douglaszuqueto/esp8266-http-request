# Realizando requisições HTTP no ESP8266 utilizando a biblioteca HTTPClient

![img](https://raw.githubusercontent.com/douglaszuqueto/esp8266-http-request/master/files/esp8266-request.png)

## HTTP

### Verbos HTTP(mais utilizados - principalmente em API's)

* GET - requisitar registros do servidor
* POST - enviar algo para o servidor
* PUT - atualizar algum registro do servidor
* DELETE - remover algum registro do servidor

## Estrutura simples de uma requisição HTTP

```c
// ############# HTTP REQUEST ################ //

void httpGet(String url)
{
  String payload = makeRequest(url);

  if (!payload) {
    return;
  }

  Serial.println("##[RESULT]## ==> " + payload);

}

String makeRequest(String url)
{
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode < 0) {
    Serial.println("request error - " + httpCode);
    return "";

  }

  if (httpCode != HTTP_CODE_OK) {
    return "";
  }

  return http.getString();

  http.end();
}
```

## Realizando requisições ao Servidor

### GET

```c
  http.begin(url);
  int httpCode = http.GET();
```

### POST

```c
  http.begin(url);
  http.addHeader("content-type", "application/x-www-form-urlencoded");

  String body = "id=7890&name=NTC&value=10";

  int httpCode = http.POST(body);
```

## Referências

* [Biblioteca ESP8266HTTPClient](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient);

## Está gostando dos materiais que vêm sendo produzidos?

Caso tenha gostado deste e de outros materiais que mantenho, no dia 18/06/2017 abri uma campanha de financiamento recorrente com o objetivo de captar recursos para uma dedicação fulltime em projetos open source.
 
Portanto, caso deseja me ajudar, fica aqui o [link do meu Apoia.se](https://apoia.se/douglaszuqueto). Desde já, muito obrigado :D.
