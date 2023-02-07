#include <Arduino.h>
#include <Bounce2.h>
#define BUTTON 27
#define LDR 32
#define GREEN 33
#define YELLOW 25
#define RED 26
TaskHandle_t TaskA = NULL;
TaskHandle_t TaskB = NULL;
int cnt = 0;
Bounce debouncer = Bounce();

void PWM_LED(void *param)
{
  while (1)
  {
    for (int i = 0; i < 255; i++)
    {
      ledcWrite(0, i);
      vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    for (int i = 255; i > 0; i--)
    {
      ledcWrite(0, i);
      vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void Count(void *param)
{
  while (1)
  {
    debouncer.update();
    if (debouncer.fell())
    {
      cnt++;
      Serial.println(cnt);
    }
  }
}

void setup()
{
  ledcSetup(0, 5000, 8);
  ledcAttachPin(GREEN, 0);
  ledcAttachPin(YELLOW, 0);
  ledcAttachPin(RED, 0);
  Serial.begin(115200);
  Serial.println("BUTTON");
  debouncer.attach(BUTTON, INPUT_PULLUP);
  debouncer.interval(25);

  xTaskCreatePinnedToCore(PWM_LED, "PWD_LED", 1000, NULL, 1, &TaskA, 0);
  xTaskCreatePinnedToCore(Count, "count", 1000, NULL, 1, &TaskB, 1);
}


void loop()
{
  // put your main code here, to run repeatedly:
}