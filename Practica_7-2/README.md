
# **Práctica 7-2 - Buses de comunicación III (I2S) - Reproducción de archivo WAV desde SD**  

## **1. Descripción**  
Esta práctica utiliza el protocolo **I2S** para reproducir un archivo de audio WAV almacenado en una tarjeta SD, mediante el **ESP32-S3** y el amplificador **MAX98357**. Se demuestra el flujo completo de lectura de datos, decodificación y transmisión digital-analógica de audio.

---

## **2. Objetivos**  
- Configurar el bus **I2S** para reproducción de audio de alta calidad.  
- Leer archivos WAV desde una tarjeta SD usando SPI.  
- Controlar el volumen y la gestión de archivos con la librería `ESP32-audioI2S`.  

---

## **3. Materiales**  
- **ESP32-S3**  
- **Tarjeta SD** (formateada en FAT32)  
- **Amplificador MAX98357 I2S**  
- **Altavoz 4Ω/8Ω**  
- **Lector de tarjetas SD** (SPI)  

---

## **4. Desarrollo**  

### **4.1 Configuración del entorno**  
1. Crear un nuevo proyecto en PlatformIO.  

2. Configurar el archivo `platformio.ini`con el siguiente contenido: 
   ```ini
   [env:esp32-s3-devkitm-1]
   platform = espressif32
   board = esp32-s3-devkitm-1
   framework = arduino
   monitor_speed = 115200
   lib_deps =
     earlephilhower/ESP8266Audio @ ^1.9.7
     schreibfaul1/ESP32-audioI2S @ ^1.0.4
   ```  

### **4.2 Programación**  
Código en `main.cpp`:  
```cpp
#include <Arduino.h>
#include <Audio.h>
#include <SD.h>
#include <FS.h>

// Pines SPI para SD
#define SD_CS   5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK  18

// Pines I2S para MAX98357
#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC  26

Audio audio;  // Objeto de audio

void setup() {
  // Inicializar SPI para SD
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  SD.begin(SD_CS);

  // Configurar I2S
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.setVolume(10);  // Volumen 0-21

  // Reproducir archivo WAV
  audio.connecttoFS(SD, "/Ensoniq-ZR-76-01-Dope-77.wav");
  Serial.println("Reproduciendo archivo...");
}

void loop() {
  audio.loop();  // Gestión continua del audio
}
```  

**Explicación del código**:  
- **Inicialización SPI**:  
  - Configura los pines para la comunicación con la tarjeta SD (`CS=GPIO5`, `MOSI=23`, `MISO=19`, `SCK=18`).  
- **Configuración I2S**:  
  - `setPinout()` define los pines I2S: **BCLK=27**, **LRC=26**, **DIN=25**.  
- **Gestión de audio**:  
  - `connecttoFS()` carga el archivo WAV desde la SD.  
  - `audio.loop()` procesa los datos en tiempo real.  

### **4.3 Conexión física**  
| Componente  | ESP32-S3       |  
|-------------|----------------|  
| **SD (SPI)** | CS=GPIO5       |  
|             | MOSI=GPIO23    |  
|             | MISO=GPIO19    |  
|             | SCK=GPIO18     |  
| **MAX98357** | DIN=GPIO25     |  
|             | BCLK=GPIO27    |  
|             | LRC=GPIO26     |  

---

## **5. Resultados**  
- **Salida del monitor serie**:  
  ```plaintext
  Reproduciendo archivo...
  ```  
  (Mensaje inicial; la librería puede mostrar metadatos del WAV si está habilitado).  
- **Salida de audio**:  
  - El archivo WAV se reproduce en el altavoz con calidad estéreo (si el archivo lo soporta).  
  - El volumen se ajusta con `setVolume()` (rango 0-21).  

---
