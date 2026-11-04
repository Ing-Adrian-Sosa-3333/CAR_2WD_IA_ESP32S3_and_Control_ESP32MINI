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

# Pasos en GitHub para generar el .bin

- Ve a tu repo → Code (Código) → main → boards → bread-compact-wifi.
- Add file → Upload files.
- Arrastra los 6 archivos sueltos (descomprime el .zip antes, no subas el zip).
- Comprueba que la lista muestre los 6 nombres exactos, sin (1) ni similares.
- 


# Todo está bien configurado y listo para grabar:
<img width="1747" height="845" alt="image" src="https://github.com/user-attachments/assets/2c02a355-ec55-4d77-958f-67e024e042d9" />

- Flash ID 184068: confirma 16 MB de flash.
- PSRAM 8MB: confirma que es el N16R8 ✅
- Dirección 0x0 con merged-binary.bin ✅
- Borrar todo está marcado ✅, así que borra la memoria antes de grabar. No necesitas pulsar “Borrar Flash” aparte.

# En Artifacts descarga el primero de la lista:

# ✅ xiaozhi_bread-compact-wifi-128x64_cd1ab2d... (1.81 MB)

Haz clic en el ícono de descarga ⬇ que está a la derecha de esa fila, junto al tacho de basura (no toques el tacho). También puedes hacer clic directamente en el nombre.

Después:
- Se descarga un .zip. Haz clic derecho sobre él y elige Extraer todo.
- Adentro está merged-binary.bin. Pesa más de 1.81 MB, porque el zip va comprimido.
- Ve a:
 # https://espressif.github.io/esptool-js/
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
