
# 📡 OpenWRT Realtime API Monitor + ESP8266 LCD

Proyek ini menyediakan API ringan berbasis PHP yang berjalan di OpenWRT untuk menampilkan informasi sistem secara real-time, serta menampilkannya ke ESP8266 menggunakan LCD I2C.

## 🔧 Fitur

- ✅ Menampilkan suhu perangkat OpenWRT.
- ✅ Informasi board dan model router.
- ✅ Monitoring bandwidth br-lan (upload & download).
- ✅ Speedtest otomatis dengan logging ke file.
- ✅ API endpoint JSON siap diakses dari ESP8266.
- ✅ Menampilkan data ke LCD I2C 16x2 via NodeMCU (ESP8266).
- ✅ Dukungan rotary encoder (opsional).

---

## 🖥️ API Endpoint

### `monitor.php`
```json
{
  "board_name": "tplink,tl-wr841n-v13",
  "model": "TP-Link TL-WR841N v13",
  "temperature_celsius": 58.3,
  "br_lan": {
    "rx_megabytes": 17.64,
    "tx_megabytes": 7.45
  },
  "bandwidth_used": "25.09 MB"
}
```

### `speedtest.php`
```json
{
  "download_mbps": 23.45,
  "upload_mbps": 8.91,
  "ping_ms": 26.45,
  "server": "Jakarta"
}
```

---

## ⚙️ Instalasi di OpenWRT

### 1. Instalasi PHP & Modul Web
```sh
opkg update
opkg install php8 php8-cgi uhttpd-mod-php
```

Tambahkan ke `/etc/config/uhttpd`:
```
list interpreter ".php=/usr/bin/php-cgi"
option index_page 'index.html index.htm index.php'
```

Restart server:
```sh
/etc/init.d/uhttpd restart
```

### 2. Tambahkan Script Cron untuk Speedtest Otomatis
```sh
crontab -e
```

Tambahkan:
```
0 * * * * /root/speedtest_log.sh
```

---

## 📲 ESP8266 Setup

- Komponen:
  - NodeMCU ESP8266
  - LCD I2C 16x2
  - (Opsional) Rotary Encoder
- Library:
  - `ESP8266WiFi.h`
  - `ESP8266HTTPClient.h`
  - `LiquidCrystal_I2C.h`
  - `ArduinoJson.h`

LCD menampilkan suhu, RX/ TX data, total bandwidth, dan hasil speedtest secara real-time dari API OpenWRT.

---

## 💾 Logging

Speedtest log disimpan di:
```
/root/speedtest.log
```

Format:
```
2025-06-29 10:00:00 - DL: 23.45 Mbps, UL: 8.91 Mbps, Ping: 26.45 ms, Server: Jakarta
```

---

## 📦 Struktur File
```
/www/api/
├── monitor.php
├── speedtest.php
/root/
├── speedtest_log.sh
├── speedtest.log
```

