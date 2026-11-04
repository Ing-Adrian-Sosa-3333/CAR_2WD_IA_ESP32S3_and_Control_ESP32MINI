/*
  Control remoto para el robot XiaoZhi (ESP32-S3) — ESP32-C3 Super Mini
  Envía los botones por ESP-NOW al carro. BUSCA EL CANAL SOLO (no necesita datos del router),
  así que si cambias de router sigue funcionando.

  Arduino IDE:
    - Paquete "esp32" de Espressif (2.0.x o 3.x)
    - Placa: "ESP32C3 Dev Module"  (o "Nologo ESP32C3 Super Mini" si aparece)
    - Herramientas -> "USB CDC On Boot": Enabled   (para ver el Monitor Serie)
  No necesita librerías extra.

  BOTONES (cada uno entre el pin y GND; usa la pull-up interna):
    GPIO 3 -> HABLAR   (mantener presionado mientras hablas)
    GPIO 4 -> VOLUMEN +
    GPIO 5 -> VOLUMEN -
  (No uses GPIO 2, 8 ni 9: intervienen en el arranque; el 8 es el LED)
  LED azul (GPIO 8): 3 parpadeos = carro encontrado; destello corto = enviado OK;
                     parpadeo rápido = no encuentra el carro.
*/
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include <esp_idf_version.h>
#include <Preferences.h>

// ===== MAC del carro (confirmada en su monitor: E8:F6:0A:8C:BD:34) =====
uint8_t carMac[] = {0xE8, 0xF6, 0x0A, 0x8C, 0xBD, 0x34};

// ===== Diagnóstico: 1 = PING cada 3 s y muestra OK/FALLA en el Monitor Serie =====
#define DEBUG_PING 1

// ===== Arreglo de antena de algunas C3 Super Mini (1 = bajar potencia, mejora el alcance) =====
#define FIX_TX_POWER 1

// ===== Pines =====
const uint8_t PIN_TALK  = 3;
const uint8_t PIN_VOLUP = 4;
const uint8_t PIN_VOLDN = 5;
const uint8_t PIN_LED   = 8;   // LED azul integrado, se enciende con LOW

// ===== Comandos (deben coincidir con remote_receiver.h del carro) =====
enum : uint8_t { CMD_TALK_DOWN = 1, CMD_TALK_UP = 2, CMD_VOL_UP = 3, CMD_VOL_DOWN = 4, CMD_PING = 9 };

struct __attribute__((packed)) Msg {
  char magic[4];   // "XZRC"
  uint8_t cmd;
  uint8_t seq;
};

volatile int sendStatus = -1;   // -1 esperando, 0 = el carro confirmó, 1 = sin confirmación
uint8_t seq = 0;
uint8_t channel = 1;
int failCount = 0;
Preferences prefs;

// La firma del callback cambió en ESP-IDF 5.5 (paquete esp32 3.3+): soportamos ambas
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)
void onSent(const esp_now_send_info_t *info, esp_now_send_status_t status) {
#else
void onSent(const uint8_t *mac, esp_now_send_status_t status) {
#endif
  sendStatus = (status == ESP_NOW_SEND_SUCCESS) ? 0 : 1;
}

void led(bool on) { digitalWrite(PIN_LED, on ? LOW : HIGH); }
void blink(int n, int ms) { for (int i = 0; i < n; i++) { led(true); delay(ms); led(false); delay(ms); } }

uint8_t radioChannel() {
  uint8_t primary = 0;
  wifi_second_chan_t second;
  esp_wifi_get_channel(&primary, &second);
  return primary;
}

void setChannel(uint8_t ch) {
  channel = ch;
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
}

// Envía y espera la confirmación (ACK) del carro, reintentando hasta timeout_ms
bool sendMsg(uint8_t cmd, uint16_t timeout_ms) {
  if (radioChannel() != channel) setChannel(channel);
  Msg m = {{'X', 'Z', 'R', 'C'}, cmd, ++seq};   // mismo seq en los reintentos
  unsigned long t0 = millis();
  while (millis() - t0 < timeout_ms) {
    sendStatus = -1;
    esp_now_send(carMac, (uint8_t *)&m, sizeof(m));
    unsigned long t1 = millis();
    while (sendStatus < 0 && millis() - t1 < 80) delay(1);
    if (sendStatus == 0) return true;
    delay(20);
  }
  return false;
}

// El carro usa el canal de su router: probamos primero el último conocido y luego 1..13
bool tryChannel(uint8_t ch) {
  setChannel(ch);
  return sendMsg(CMD_PING, 400);
}

bool findChannel() {
  Serial.println("Buscando el canal del carro...");
  uint8_t last = prefs.getUChar("ch", 1);
  if (last < 1 || last > 13) last = 1;
  bool found = tryChannel(last);
  for (uint8_t ch = 1; ch <= 13 && !found; ch++) {
    if (ch == last) continue;
    found = tryChannel(ch);
  }
  if (found) {
    if (prefs.getUChar("ch", 0) != channel) prefs.putUChar("ch", channel);
    Serial.printf("Carro encontrado en el canal %d\n", channel);
    blink(3, 80);
    return true;
  }
  Serial.println("No se encontró el carro (¿está encendido, con ojos y hora?)");
  return false;
}

void send(uint8_t cmd) {
  led(true);
  bool ok = sendMsg(cmd, 1500);
  led(false);
  if (ok) {
    failCount = 0;
    Serial.printf("Enviado cmd %d (canal %d)\n", cmd, channel);
  } else {
    Serial.printf("FALLÓ cmd %d\n", cmd);
    blink(5, 40);
    if (++failCount >= 2) {          // quizá cambiaste de router / canal
      failCount = 0;
      if (findChannel()) sendMsg(cmd, 1500);
    }
  }
}

// ----- Botones con antirrebote -----
struct Button {
  uint8_t pin;
  bool stable, last;
  unsigned long changedAt;
  Button(uint8_t p) : pin(p), stable(HIGH), last(HIGH), changedAt(0) {}
  int update() {   // +1 al presionar, -1 al soltar, 0 sin cambio
    bool r = digitalRead(pin);
    if (r != last) { last = r; changedAt = millis(); }
    if (millis() - changedAt > 25 && r != stable) {
      stable = r;
      return stable == LOW ? +1 : -1;
    }
    return 0;
  }
};

Button bTalk(PIN_TALK), bUp(PIN_VOLUP), bDn(PIN_VOLDN);

void setup() {
  Serial.begin(115200);
  delay(1500);   // tiempo para que abra el Monitor Serie por USB
  pinMode(PIN_TALK, INPUT_PULLUP);
  pinMode(PIN_VOLUP, INPUT_PULLUP);
  pinMode(PIN_VOLDN, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  led(false);
  prefs.begin("control", false);

  // Radio en modo estación, SIN conectarse a ningún router
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_ps(WIFI_PS_NONE);
#if FIX_TX_POWER
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
#endif

  Serial.print("\nMAC del control: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error iniciando ESP-NOW");
    while (true) blink(1, 100);
  }
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, carMac, 6);
  peer.channel = 0;            // 0 = el canal actual de la radio (lo vamos cambiando)
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = false;
  if (esp_now_add_peer(&peer) != ESP_OK) Serial.println("Error agregando el carro como peer");

  while (!findChannel()) {     // reintenta hasta que el carro esté listo
    blink(10, 50);
    delay(2000);
  }
}

void loop() {
  int t = bTalk.update();
  if (t == +1) send(CMD_TALK_DOWN);
  if (t == -1) send(CMD_TALK_UP);
  if (bUp.update() == +1) send(CMD_VOL_UP);
  if (bDn.update() == +1) send(CMD_VOL_DOWN);

#if DEBUG_PING
  static unsigned long tPing = 0;
  if (millis() - tPing > 3000) {
    tPing = millis();
    uint8_t real = radioChannel();
    bool ok = sendMsg(CMD_PING, 1000);
    Serial.printf("PING canal %d (radio en %d): %s\n", channel, real, ok ? "OK" : "FALLA");
  }
#endif

  delay(5);
}
