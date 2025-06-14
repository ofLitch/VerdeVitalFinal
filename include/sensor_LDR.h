#ifndef SENSOR_LDR_H
#define SENSOR_LDR_H

// Librerías necesarias
#include "sensorData.h"
#include <freertos/semphr.h>
#include <Arduino.h>

// Definiciones para el sensor LDR
#define LDR_READ_INTERVAL_MS 1000               ///< Intervalo de muestreo en milisegundos
extern SemaphoreHandle_t mutex;

/**
 * @file sensor_LDR.h
 * @brief Definición de la tarea para leer datos del sensor LDR y actualizar los datos compartidos.
 *
 * @param pvParameters Puntero a los parámetros que incluye el mutex y el pin del sensor LDR.
 */
void taskLDR(void *pvParameters) {    
    // Desempaquetar los parámetros
    int ldrPin = (int)(uintptr_t)pvParameters;

    // Comprobar que el mutex y el pin LDR sean válidos
    if (mutex == NULL) {
        Serial.println("Error: Mutex no inicializado en la tarea LDR.");
        vTaskDelete(NULL);
        return;
    }
    if (ldrPin < 30 || ldrPin > 39) {
        Serial.println("Error: Pin LDR inválido.");
        vTaskDelete(NULL);
        return;
    }

    // Configuración del LDR
    pinMode(ldrPin, INPUT);
    unsigned short int light = 0;
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Comenzar la lectura del LDR
    while (true) {
        light = analogRead(ldrPin);
        if (xSemaphoreTake(mutex, portMAX_DELAY)) {
            data.light = light;
            xSemaphoreGive(mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(LDR_READ_INTERVAL_MS));
    }
}

#endif // SENSOR_LDR_H
