
![openwrt-monitor](https://socialify.git.ci/razannnnnn/openwrt-monitor/image?custom_language=PHP&description=1&font=Inter&forks=1&language=1&name=1&owner=1&pattern=Floating+Cogs&stargazers=1&theme=Dark)

Proyek ini menyediakan API ringan berbasis PHP yang berjalan di OpenWRT untuk menampilkan informasi sistem secara real-time, serta menampilkannya ke ESP8266 menggunakan LCD I2C.

---

## 🔧 Fitur

- ✅ Menampilkan suhu perangkat OpenWRT.
- ✅ Informasi board dan model router.
- ✅ Monitoring bandwidth br-lan (upload & download).
- ✅ Speedtest otomatis dengan logging ke file.
- ✅ API endpoint JSON siap diakses dari ESP8266.
- ✅ Menampilkan data ke LCD I2C 16x2 via NodeMCU (ESP8266).
- ✅ Dukungan rotary encoder (opsional).

---

## 📁 Struktur File

```
/www/api/
├── monitor.php
├── speedtest.php
/root/
├── speedtest_log.sh
├── speedtest.log
```

---

## ⚙️ Cara Instalasi API di OpenWRT

### 1. Instalasi PHP dan Modul Web

```sh
opkg update
opkg install php8 php8-cgi uhttpd-mod-php
```

### 2. Konfigurasi uHTTPd

Edit file konfigurasi:
```sh
vi /etc/config/uhttpd
```

Tambahkan atau pastikan baris ini ada:

```
list interpreter ".php=/usr/bin/php-cgi"
option index_page 'index.html index.htm index.php'
```

Restart server:
```sh
/etc/init.d/uhttpd restart
```

### 3. Buat Folder API dan Simpan File

```sh
mkdir -p /www/api
```
Lalu upload `monitor.php` dan `speedtest.php` ke `/www/api/`

---

## 📈 Menjadwalkan Speedtest Otomatis

### 1. Instalasi Python dan Speedtest-CLI

```sh
opkg install python3-pip
pip3 install speedtest-cli
```

### 2. Buat Skrip `/root/speedtest_log.sh`

```sh
touch /root/speedtest_log.sh
chmod +x /root/speedtest_log.sh
```
Isi file:

```sh
#!/bin/sh
timestamp=$(date +"%Y-%m-%d %H:%M:%S")
result=$(speedtest-cli --json 2>/dev/null)

if [ -n "$result" ]; then
    download=$(echo "$result" | jsonfilter -e '@.download')
    upload=$(echo "$result" | jsonfilter -e '@.upload')
    ping=$(echo "$result" | jsonfilter -e '@.ping')
    server=$(echo "$result" | jsonfilter -e '@.server.name')

    echo "$timestamp - DL: $(awk "BEGIN {print $download/1000000}") Mbps, UL: $(awk "BEGIN {print $upload/1000000}") Mbps, Ping: $ping ms, Server: $server" >> /root/speedtest.log
else
    echo "$timestamp - Speedtest gagal" >> /root/speedtest.log
fi
```

### 3. Jadwalkan via Cron

```sh
crontab -e
```

Tambahkan ini untuk menjalankan setiap 1 jam:
```
0 * * * * /root/speedtest_log.sh
```

Aktifkan cron jika belum:
```sh
/etc/init.d/cron enable
/etc/init.d/cron start
```

---

## 🔌 Wiring ESP8266 + LCD I2C

```
LCD SDA → D2 (GPIO4)
LCD SCL → D1 (GPIO5)
VCC     → 3.3V
GND     → GND
```

---

## 🖥️ Kode ESP8266 (Ringkas)

- Gunakan library:
  - `ESP8266WiFi.h`
  - `ESP8266HTTPClient.h`
  - `LiquidCrystal_I2C.h`
  - `ArduinoJson.h`

- Koneksi WiFi dan ambil data dari `http://192.168.1.1/api/monitor.php`

- Tampilkan suhu, RX, TX, bandwidth di LCD

---

## 🧪 Akses API

- Cek di browser:
  - `http://192.168.1.1/api/monitor.php`
  - `http://192.168.1.1/api/speedtest.php`

---

## 📝 Output API

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

## 📚 Lisensi

Proyek ini bersifat open-source dan bebas dimodifikasi untuk keperluan monitoring pribadi.
