#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIN_BUTTON 12
#define PIN_LED    13
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

QueueHandle_t comandoQueue;
SemaphoreHandle_t sensorSem;
volatile int valorSensorAtual = 1; // começa solto

// Tarefa Sensor
void taskSensor(void *pvParameters) {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  int valorAnterior = digitalRead(PIN_BUTTON);
  while (1) {
    int valorAtual = digitalRead(PIN_BUTTON);
    if (valorAtual != valorAnterior) {
      valorSensorAtual = valorAtual;
      xSemaphoreGive(sensorSem);
      valorAnterior = valorAtual;
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// Tarefa Processador
void taskProcessador(void *pvParameters) {
  int comando;
  while (1) {
    if (xSemaphoreTake(sensorSem, portMAX_DELAY)) {
      comando = (valorSensorAtual == 0) ? 1 : 0;
      xQueueSend(comandoQueue, &comando, portMAX_DELAY);
    }
  }
}

// Tarefa Atuador
void taskAtuador(void *pvParameters) {
  pinMode(PIN_LED, OUTPUT);
  int comando;
  while (1) {
    if (xQueueReceive(comandoQueue, &comando, portMAX_DELAY)) {
      digitalWrite(PIN_LED, comando ? HIGH : LOW);
    }
  }
}

// Tarefa Display
void taskDisplay(void *pvParameters) {
  while (1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("====== STATUS ======");

    display.setCursor(0, 15);
    display.print("LED: ");
    display.println(digitalRead(PIN_LED) ? "LIGADO " : "APAGADO");

    display.setCursor(0, 28);
    display.print("BOTAO: ");
    display.println(valorSensorAtual == 0 ? "PRESSIONADO" : "SOLTO");

    display.setCursor(0, 60);
    display.println("--------------------");
    display.display(); 
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar o display SSD1306"));
    for (;;); // trava se falhar
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Display pronto!");
  display.display();

  comandoQueue = xQueueCreate(5, sizeof(int));
  sensorSem = xSemaphoreCreateBinary();

  // Tarefas com prioridades
  xTaskCreate(taskSensor, "Sensor", 1024, NULL, 1, NULL);
  xTaskCreate(taskProcessador, "Processador", 1024, NULL, 2, NULL);
  xTaskCreate(taskAtuador, "Atuador", 1024, NULL, 3, NULL);
  xTaskCreate(taskDisplay, "Display", 1024, NULL, 1, NULL);
}


void loop() {
  // Sem código aqui
}

