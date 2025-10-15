# DOKUMENTASI TASK 3 SAPA MENYAPA (ESP NOW) ><

---

## Tujuan
membuat sistem komunikasi antar ESP32 menggunakan metode ESP-now communication,
di mana setiap ESP bisa menerima perintah dari serial (esp ke laptop, melalui usb), lalu memproses perintah komunikasi HALO, CEK, dan JAWAB, serta mengirim atau membalas pesan ke ESP lain.

---

## Struktur Folder `src`
```
src/
 ├─ SapaMenyapa.cpp    # Program utama (setup & loop)
 ├─ utility.cpp        # Fungsi logika ESP-NOW, parsing serial, dan pemrosesan perintah
 └─ main.h             # Header yang berisi definisi, enum, dan deklarasi global
```

---

## Penjelasan fungsi file

### 1. SapaMenyapa.cpp
SapaMenyapa.cpp merupakan file inti yang dijalankan paling awal saat ESP tersambung. 

**Setup:**
```cpp
Serial.begin(115200);    // mengaktifkan komunikasi serial antara ESP dan komputer. (115200 bit per detik)
EEPROM.begin(128);       // declare penyimpanan di memori sementara (128 byte) .
Serial.printf("Halo, aku %s\n", mac_names[mac_index_ku]);    // Untuk menampilkan identitas ESP yang sudah di declare diawal yaitu 9 
mulai_esp_now(mac_index_ku); // Inisialisasi ESP-NOW, daftarkan peer ESP lain
```
mulai_esp_now(mac_index_ku)
memanggil fungsi dari utility.cpp sembari mengecek apakah file utility.cpp berhasil dibuka.
Bagian ini, bertujuan untuk mengatur mode WiFi jadi station, memberi alamat MAC ke ESP, dan mendaftarkan semua ESP teman ke dalam peer list ESP-NOW.


**Loop:**

```cpp
void loop() {
    if (Serial.available()) {
        baca_serial(callback_data_serial);
    }
}
```
 cek apakah ada input dari monitor juka ada panggil `baca_serial()`  
 setelah itu `callback_data_serial()` akan meneruskan data ke `process_perintah()`  

---

### 2. utility.cpp
File pusat seluruh logika komunikasi ESP yang menjalankan Inisialisasi dan pengaturan ESP-NOW, Pembacaan data dari Serial Monitor/laptop, serta pemrosesan perintah (HALO, CEK, JAWAB)

#### A. Inisialisasi ESP-NOW
```cpp
esp_err_t mulai_esp_now(int index_mac_address)
```
Fungsi ini melakukan:  
- Mengatur WiFi mode ke Station  
- Menjalankan ESP-NOW  
- Menetapkan alamat MAC ESP sesuai index  
- Menambahkan semua ESP teman ke daftar peer  

#### B. Pencarian dan Identitas MAC
```cpp
int cari_mac_index(const uint8_t *mac) // Cari index ESP berdasarkan MAC
mac_index_to_names()                  // Konversi index ke nama (contoh: 9 -> "Dhanishara Zaschya Putri Syamsudin")
```

#### C. Callback ESP-NOW
```cpp
void callback_data_esp_now(const uint8_t *mac, const uint8_t *data, int len)
```
- Dipanggil setiap kali menerima data dari ESP lain → meneruskan ke `process_perintah()`  

```cpp
void callback_pengiriman_esp_now()
```
callback_data_esp_now()
 dipanggil setiap kali ESP menerima data dari ESP lain.
 kode akan otomatis memanggil process_perintah().


callback_pengiriman_esp_now()
 cuma buat menampilkan status sukses/gagal kirim data ke Serial Monitor

#### D. Fungsi Pembaca Serial
Fungsi ini membaca data byte per byte dari Serial Monitor dengan format paket:
```
HEADER : 0xFF 0xFF 0x00
LENGTH : 1 byte
DATA   : N byte
```
Jika paket lengkap → kirim ke `callback_data_serial()` sehingga ESP bisa mengerti input dari laptop.  

---

### 3. Fungsi process_perintah()
```cpp
void process_perintah(const uint8_t *data, int len, int index_mac_address_asal)
```
- Identifikasi tipe data: `command` (`HALO`, `CEK`, `JAWAB`) dan `target_index`  
- `index_mac_address_asal == -1` → data dari Serial Monitor  
- `index_mac_address_asal != -1` → data dari ESP-NOW  

**Perintah:**
1. `HALO`  
   - Dari Serial → kirim pesan sapaan ke ESP lain  
   - Dari ESP lain → balas dengan `JAWAB`  

2. `CEK`  
   - Dari Serial → tanya "[target] ini [sender] apakah kamu disana? " ke ESP lain  
   - Dari ESP lain → balas "iya, aku [target] disini - [sender]"  

3. `JAWAB`  
   - Tampilkan di Serial Monitor, tidak dikirim balik lagi  

**Workflow keseluruhan:**
```
Serial Monitor → baca_serial() → callback_data_serial() → process_perintah() 
→ esp_now_send() → ESP lain → callback_data_esp_now() → process_perintah()
```

---

### 4. main.h
Header file yang berisi:  
- Definisi global  
- Enum  
- Deklarasi fungsi  

---

## Kesimpulan
Sistem ini memungkinkan antar ESP32 saling berkomunikasi, sambil tetap bisa dikontrol melalui Serial Monitor dari laptop. Semua alur komunikasi dan perintah (`HALO`, `CEK`, `JAWAB`) terpusat di `utility.cpp`.
