# Mini Sistema Operacional para IoT 

Este projeto representa um **mini sistema operacional embarcado para IoT**, implementado com **FreeRTOS** sobre um **ESP32**, usando tarefas concorrentes, semáforos, filas e exibição em **display OLED SSD1306**. Ele simula uma arquitetura de IoT com sensores, atuadores e interface visual.

---

## 🧠 Arquitetura do Sistema

O "sistema operacional" possui 4 tarefas simultâneas:

- 🧼 **Sensor**: Monitora o botão  
- 🧠 **Processador**: Decide qual comando enviar ao atuador
- ⚡ **Atuador**: Liga/desliga o LED
- 📺 **Display**: Atualiza o estado do sistema

As tarefas se comunicam usando:
- **Filas** para envio de comandos
- **Semáforos** para notificação de eventos

---

## 🛠️ Requisitos

- **Placa:** ESP32 DevKit (ou simulador Wokwi)
- **Display:** OLED SSD1306 
- **Botão** físico (ou simulado)
- **LED** 

### 📦 Bibliotecas Arduino

- [`Adafruit_GFX`](https://github.com/adafruit/Adafruit-GFX-Library)
- [`Adafruit_SSD1306`](https://github.com/adafruit/Adafruit_SSD1306)

Instale pela **Arduino IDE** > Gerenciar Bibliotecas.

---

## 🚀 Como rodar

### ✅ Localmente (ESP32 físico)

1. Conecte os seguintes pinos:

| Componente | Pino ESP32 |
|------------|------------|
| OLED SDA   | GPIO21     |
| OLED SCL   | GPIO22     |
| Botão      | GPIO12     |
| LED        | GPIO13     |

2. Instale as bibliotecas mencionadas acima.
3. Selecione **Placa: ESP32 Dev Module** na Arduino IDE.
4. Compile e envie o código.
5. Abra o monitor serial (baud 115200) para ver mensagens (opcional).

---

### 🌐 Online no [Wokwi Simulator](https://wokwi.com)

Clique no botão abaixo para abrir e executar o projeto diretamente no navegador:

👉 **[Rodar no Wokwi](https://wokwi.com/projects/438045289005235201)** 

Caso queira criar seu próprio projeto Wokwi:

1. Acesse [https://wokwi.com](https://wokwi.com)
2. Crie um novo projeto com ESP32
3. Substitua o `sketch.ino` pelo código-fonte deste repositório
4. Cole o conteúdo do `diagram.json` para configurar o circuito
5. Execute

---
## 📁 Estrutura do Projeto

```cpp
setup()         // Inicializa display, semáforo, fila e tarefas
loop()          // Não usado 
taskSensor()    // Lê o botão
taskProcessador() // Interpreta evento e envia comando
taskAtuador()   // Liga/desliga o LED
taskDisplay()   // Mostra os estados no OLED
```
---

## 🎓 Sobre

Este projeto foi desenvolvido como parte da disciplina  
**DCC062 – Sistemas Operacionais** da **Universidade Federal de Juiz de Fora (UFJF)**.

---
