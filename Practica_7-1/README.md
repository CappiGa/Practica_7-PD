# **Práctica 7-1 - Buses de comunicación III (I2S) - Reproducción de audio desde memoria interna**  

## **1. Descripción**  
Esta práctica implementa el protocolo **I2S** en el **ESP32-S3** para reproducir audio almacenado en la memoria interna (PROGMEM) utilizando un decodificador AAC y el amplificador **MAX98357**. Se explora el flujo de datos digitales a analógicos y la configuración del bus I2S para aplicaciones de audio.  

---

## **2. Objetivos**  
- Comprender el protocolo **I2S** y su aplicación en sistemas de audio digital.  
- Configurar el ESP32-S3 para reproducir audio desde memoria interna.  
- Utilizar la librería `ESP8266Audio` para decodificar archivos AAC.  

## **3. Resumen teórico**  
#### **Protocolo I2S**  
- **Propósito**: Transmisión de audio digital con 3 líneas:  
  - **BCLK** (Bit Clock): Sincronización (ej. 1.411 MHz para 44.1 kHz estéreo 16-bit).  
  - **LRC** (Left/Right Clock): Selección de canal (0=izquierdo, 1=derecho).  
  - **DIN** (Data In): Datos de audio (MSB first).  
- **Ventajas**:  
  - Calidad sin pérdidas (hasta 32-bit/96 kHz).  
  - Compatibilidad con DACs externos (ej. MAX98357).  

#### **MAX98357**  
- **Funciones**:  
  - Decodifica I2S a señal analógica.  
  - Amplificador Clase D integrado (3.2W @ 4Ω).  
  - Configurable en modo mono/estéreo.  

---

## **4. Materiales**  
- **ESP32-S3**  
- **Amplificador MAX98357 I2S**  
- **Altavoz 4Ω/8Ω**  
- **Cables dupont**  

---

## **5. Desarrollo**  

### *5.1 Configuración del entorno**  
1. Crear un nuevo proyecto en PlatformIO.  

2. Configurar el archivo `platformio.ini`con el siguiente contenido:   
   ```ini
   [env:esp32-s3-devkitc-1]
   platform = espressif32
   board = esp32-s3-devkitc-1
   framework = arduino
   monitor_speed = 115200
   lib_deps =
     earlephilhower/ESP8266Audio @ ^1.9.7
     schreibfaul1/ESP32-audioI2S @ ^1.0.4
   ```  

### **5.2 Programación**  
Código en `main.cpp`:  
```cpp
#include <Arduino.h>
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"  // Array con datos AAC

AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;

void setup() {
  Serial.begin(115200);
  
  // Inicializar fuentes de audio
  in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
  aac = new AudioGeneratorAAC();
  out = new AudioOutputI2S();
  
  // Configurar I2S (DIN=GPIO26, BCLK=GPIO25, LRC=GPIO22)
  out->SetPinout(26, 25, 22);
  out->SetGain(0.125);  // Ajuste de ganancia (12.5%)
  
  aac->begin(in, out);  // Iniciar decodificación
}

void loop() {
  if (aac->isRunning()) {
    aac->loop();  // Procesar audio
  } else {
    Serial.println("Reproducción finalizada");
    delay(1000);
  }
}
```  

**Explicación del código**:  
- **`AudioFileSourcePROGMEM`**: Carga el audio desde memoria flash (array `sampleaac`).  
- **`AudioOutputI2S`**: Configura el bus I2S con los pines:  
  - **DIN** → GPIO26  
  - **BCLK** → GPIO25  
  - **LRC** → GPIO22  
- **Decodificación**: `AudioGeneratorAAC` convierte el AAC a PCM para I2S.  

### **5.3 Conexión física**  
| MAX98357 | ESP32-S3 |  
|----------|----------|  
| DIN      | GPIO26   |  
| BCLK     | GPIO25   |  
| LRC      | GPIO22   |  
| GND      | GND      |  
| VIN      | 3.3V     |  

---

## **6. Resultados**  
- **Salida del monitor serie**:  
  ```plaintext
  Reproducción finalizada
  ```  
  (El mensaje aparece cuando termina el audio).  
- **Salida de audio**:  
  - El archivo AAC almacenado en `sampleaac.h` se reproduce en el altavoz conectado al MAX98357.  
  - La ganancia se ajusta al 12.5% (`SetGain(0.125)`).  

---
