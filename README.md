# CAR_2WD_IA_ESP32S3_and_Control_ESP32MINI

# Botones del Control Remoto
<img width="3600" height="1020" alt="botones-robot" src="https://github.com/user-attachments/assets/3af8fe41-224c-4057-a825-8e73b4827e82" />

# Comandos del carro 🚗

# Dices	#                                        # Hace #
- “Avanza”	                                     - Adelante 1 s
- “Retrocede”	                                   - Atrás 1 s
- “Izquierda” / “Gira a la izquierda”	           - Gira sobre su eje
- “Derecha” / “Gira a la derecha”	               - Gira sobre su eje
- “Pare” / “Detente”	                           - Se detiene ya
  
# Comando	Ojos
<img width="575" height="519" alt="image" src="https://github.com/user-attachments/assets/05789abd-0fb3-4f91-8fe4-651180f4745b" />

Avanza:       	EYES_WIDE (bien abiertos)
Retrocede:    	EYES_DISTRESSED (preocupados)
Izquierda:    	EYES_LEFT (miran a la izquierda)
Derecha:      	EYES_RIGHT (miran a la derecha)
Pare:         	EYES_MAD (enojados) y después de 1,5 s vuelven a la normalidad 

# Puedes agregar detalles y la IA los entiende:

“Avanza 3 segundos”: ajusta la duración (de 0,2 a 10 s).
“Avanza despacio” o “Retrocede rápido”: ajusta la velocidad.
“Avanza a velocidad 50”: velocidad exacta (de 30 a 100 %).
“Gira un poquito a la derecha”: hace un movimiento corto.
“Avanza y después gira a la izquierda”: encadena las dos órdenes.

" Siempre se detiene solo al terminar el tiempo, por seguridad. "

# MAC del control: 24:A1:60:20:92:B9
# MAC del carro: E8:F6:0A:8C:BD:34

# Esquema de pines de cada módulo por separado.

# 🎤 Micrófono INMP441
# INMP441	               # ESP32-S3
VDD: 	                     3V3
GND:	                     GND
L/R:	                     GND (canal izquierdo)
WS:	                       GPIO 4
SCK:                       GPIO 5
SD:	                       GPIO 6

# 🔊 Amplificador MAX98357A + parlante
# MAX98357A	              # ESP32-S3
VIN:	                      5V
GND:	                      GND
DIN:	                      GPIO 7
BCLK:	                      GPIO 15
LRC:	                      GPIO 16
GAIN / SD:	                Sin conectar
Salida + / −:	              Parlante 4 Ω 2 W

# ⚙️ Puente H L298N + 2 motores
# L298N:	                 # Conexión
IN1:	                       GPIO 8
IN2:	                       GPIO 9
ENA:	                       GPIO 12 (quitar el jumper)
IN3:	                       GPIO 10
IN4:	                       GPIO 11
ENB:	                       GPIO 13 (quitar el jumper)
OUT1 / OUT2:	               Motor izquierdo
OUT3 / OUT4:	               Motor derecho
12V:	                       + de las baterías (2 × 18650 = 7,4 V)
GND:                         − de las baterías y GND del ESP32 (común)
5V:	                         ESP32 5V

# 🔘 Botones (Control inalámbrico - ESPNOW)
# Botón	  # GPIO	   # Función
BOOT	        0	       Ya viene en la placa: iniciar o terminar la conversación
Hablar	      47	     Mantener presionado para hablar
Volumen +	    40	     Subir volumen
Volumen −	    39	     Bajar volumen

# Paso 1: Copiar el proyecto XiaoZhi a tu cuenta
- Entra a https://github.com/78/xiaozhi-esp32
- Arriba a la derecha pulsa Fork y luego Create fork. Ahora tienes tu propia copia en github.com/TU_USUARIO/xiaozhi-esp32.
# Paso 2: Activar la compilación automática
- En tu copia, abre la pestaña Actions (arriba).
- Pulsa el botón verde “I understand my workflows, go ahead and enable them”.
# Paso 3: Subir tus archivos
- Vuelve a la pestaña Code y entra a las carpetas main, luego boards, luego bread-compact-wifi.
- Pulsa Add file y después Upload files.
- Arrastra los 4 archivos: los 3 que tienes más el config.json que te pasé antes.
- Abajo, pulsa Commit changes. GitHub reemplaza los originales por los tuyos.
# Paso 4: Esperar la compilación
- Ve a Actions. Verás “Build Boards” con un círculo amarillo girando.
- Espera de 10 a 20 minutos hasta que cambie a ✅ verde. Si sale ❌ rojo, mándame una captura del error.
# Paso 5: Descargar el .bin
- Haz clic en esa ejecución con ✅ y baja hasta Artifacts.
- Descarga xiaozhi_bread-compact-wifi-128x64_...: es un .zip.
- Descomprímelo. Adentro está merged-binary.bin.
  
# Paso 6: Grabarlo en el ESP32
- Con ese archivo, vuelve a la página Herramienta ESP de tu captura y sigue los pasos que te di: Conectar, Borrar flash, dirección 0x0, elegir el .bin y Programa.

- Ve a tu repo → Code (Código) → main → boards → bread-compact-wifi.
- Add file → Upload files.
- Arrastra los 6 archivos sueltos (descomprime el .zip antes, no subas el zip).
- Comprueba que la lista muestre los 6 nombres exactos, sin (1) ni similares.

# En Artifacts descarga el primero de la lista:

<img width="1921" height="712" alt="image" src="https://github.com/user-attachments/assets/c4342f00-b968-4f91-ba26-1e1a5b67fecd" />


# ✅ xiaozhi_bread-compact-wifi-128x64_cd1ab2d... (1.81 MB)

Haz clic en el ícono de descarga ⬇ que está a la derecha de esa fila, junto al tacho de basura (no toques el tacho). También puedes hacer clic directamente en el nombre.

Después:
- Se descarga un .zip. Haz clic derecho sobre él y elige Extraer todo.
- Adentro está merged-binary.bin. Pesa más de 1.81 MB, porque el zip va comprimido.
- Ve a:
 # https://espressif.github.io/esptool-js/

 # Todo está bien configurado y listo para grabar:
<img width="1747" height="845" alt="image" src="https://github.com/user-attachments/assets/2c02a355-ec55-4d77-958f-67e024e042d9" />

- Flash ID 184068: confirma 16 MB de flash.
- PSRAM 8MB: confirma que es el N16R8 ✅
- Dirección 0x0 con merged-binary.bin ✅
- Borrar todo está marcado ✅, así que borra la memoria antes de grabar. No necesitas pulsar “Borrar Flash” aparte.


- Sigue estos pasos: Conectar → Borrar flash → dirección 0x0 → elegir merged-binary.bin → Programa → al terminar, pulsa RST.

# Ahora:

- Pulsa Programa.
- Espera a que la consola llegue al 100 % y diga “Leaving...” o “Hard resetting”. Tarda entre 1 y 2 minutos, porque primero borra todo.
- Pulsa Desconectar y luego el botón RST de la placa.
- Si se queda trabado o da error, baja la velocidad a 460800 o 115200 y vuelve a conectar, como sugiere la línea “consider using a lower baud rate”.
- Después del RST, mira la OLED: debería mostrar que está en modo de configuración WiFi. Busca desde el celular o PC la red Xiaozhi-XXXX.
- Configura la red que va a conectarse el carro

  
# Qué tan lejos te escucha el micrófono

# Situación	                        # Distancia aproximada
Habitación silenciosa:              voz normal	1–2 m
Habitación silenciosa:              voz fuerte	3–4 m
Con ruido (gente, TV, ventilador): 	menos de 1 m
Con los motores girando:           	Casi nada
