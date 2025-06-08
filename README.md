# 🌩️ SysWeather – Sistema de Monitoramento de Eventos Climáticos Extremos

Projeto desenvolvido para a Global Solution 2025/1 da FIAP.

## 📌 Objetivo

Monitorar eventos climáticos extremos em tempo real, como enchentes e tempestades, por meio de sensores IoT integrados a um sistema inteligente com dashboard e emissão de alertas via MQTT.

## 🧩 Tecnologias Utilizadas

- ESP32 (via Wokwi)
- Sensores simulados (temperatura, umidade, chuva)
- MQTT (HiveMQ)
- Node-RED (Dashboard e fluxo)
- JSON (diagrama de montagem)
- GitHub

## 🛠️ Como Executar

### Wokwi

1. Acesse o projeto direto: https://wokwi.com/projects/432861298171813889
2. Ou Acesse [https://wokwi.com](https://wokwi.com)
3. Clique em "New Project", selecione "ESP32".
4. Substitua o conteúdo pelo `sketch.ino` e `diagram.json`.
5. Possivelmente terá que instalar as blibliotecas ArduinoJson e PubSubClient, qualquer coisa estou enviando o arquivo de libraries junto
6. Rode a simulação e observe o envio de dados via MQTT.

### Node-RED

1. Execute no seu ambiente local.
2. Importe o fluxo (arquivo `.json` exportado do Node-RED).
3. Configure o broker HiveMQ (Se necessário).
4. Visualize o painel em tempo real (Dashboard).
5. Caso ao implementar o Flow pela primeira e ele gerar triangulos vermelhos nos arquétipos: Temperatura e Umidade, Alerta de Risco, Nível de Água e Informações da Cidade apenas de dois cliques em cima de cada e clique feito, ou selecione o grupo se nao estiver selecionado

## Arquitetura de Solução
A solução segue uma arquitetura simplificada e eficiente, baseada nos seguintes componentes:

- 🧠 Dispositivo (ESP32)
Lê os dados do sensor DHT22 (temperatura e umidade)

Realiza requisições HTTP para coletar dados de clima via API OpenWeather

Publica os dados em tópicos MQTT

- ☁️ Broker MQTT (Mosquitto)
Utilizado para intermediar a troca de mensagens entre o dispositivo e o cliente via CMD.

Instalado e executado localmente.

💻 Interface (via terminal/CMD)
O usuário pode visualizar as mensagens recebidas usando comandos do Mosquitto CLI:

mosquitto_sub para assinar tópicos

mosquitto_pub para simular publicações, se necessário


## 🔄 Fluxo de Comunicação

ESP32 conecta-se à rede Wi-Fi

Realiza leitura do sensor DHT22

Consulta a API OpenWeather

Constrói um JSON com:

Cidade (zona)

Clima atual (ex: "chuva")

Temperatura e umidade

Tipo de ocorrência (ex: "enchente")

Nível de gravidade

Publica o JSON no tópico MQTT sysweather/ocorrencias

Usuário, via CMD, executa:

bash
Copiar
Editar
mosquitto_sub -h localhost -t "sysweather/ocorrencias"
Os dados são exibidos em tempo real no terminal.

## 📸 Ilustrações

[image](https://github.com/user-attachments/assets/db91991e-3c1c-4c59-9cfc-b9e32c7e8357)
![image1](https://github.com/user-attachments/assets/8c3e75f6-b1b6-49f0-9545-a563d93476a7)
![image2](https://github.com/user-attachments/assets/8e16a1b8-76f6-437f-bca8-14e7746b25cd)

## 🧠 Estrutura do Projeto

```
/sysweather
├── sketch.ino              # Código da ESP32 (simulado no Wokwi)
├── diagram.json            # Diagrama da montagem no Wokwi
├── fluxo-sysweather.json   # Fluxo exportado do Node-RED (opcional)
├── libraries.txt           # Biblioteca do Wokwi com as extensões necessárias do projeto
└── README.md               # Este arquivo
```

---

## 👨‍💻 Nossa equipe
### Gustavo Rangel
💼 Estudante de Análise e Desenvolvimento de Sistemas na FIAP
🔗 linkedin.com/in/gustavoorangel

### David Rapeckman
💼 Estudante de Análise e Desenvolvimento de Sistemas na FIAP
🔗 linkedin.com/in/davidrapeckman

### Luis Felippe Morais
💼 Estudante de Análise e Desenvolvimento de Sistemas na FIAP
🔗 linkedin.com/in/luis-felippe-morais-das-neves-16219b2b9
