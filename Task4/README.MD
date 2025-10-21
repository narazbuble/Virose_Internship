# 📁 Proyek CPFT: File Transfer via Serial dan ESP-NOW

## 📋 Deskripsi Proyek

Proyek **CPFT (Cross-Platform File Transfer)** adalah solusi transfer data **JSON** dari lingkungan *desktop* (Program C++) ke dua perangkat *embedded* **ESP32** (Bridge dan Receiver) menggunakan kombinasi Serial dan ESP-NOW.

**Proses Aliran Data:**
1. **Laptop (C++)** → Serial (dengan SOP/EOP) → **ESP32-Bridge**
2. **ESP32-Bridge** → Nirkabel ESP-NOW → **ESP32-Receiver**

Data JSON dipecah (*fragmentasi*), dikirim, disusun kembali (*defragmentasi*), dan diverifikasi keutuhannya menggunakan **CRC32**.

## ✨ Fitur Utama & Nilai Plus

* **Fragmentasi Protokol:** Program C++ memecah *file* JSON menjadi *chunk* dengan ukuran aman (maksimal **220 bytes** *payload*).
* **Protokol Header Kuat:** Setiap *chunk* membawa **Index**, **Total Paket**, dan **CRC32** dari seluruh *file*.
* **Komunikasi Serial Aman (SOP/EOP):** Menggunakan *delimiter* **SOP (`0xAA`)** dan **EOP (`0x55`)** untuk membingkai paket Serial dan mencegah *buffer* korupsi di Bridge.
* **Verifikasi Data (CRC32):** Penerima memverifikasi keutuhan data menggunakan CRC32 setelah *defragmentasi*.
* **OOP di C++:** Menggunakan *class* **`FileSplitter`** dan **`SerialSender`** dengan implementasi PImpl (**Nilai Plus**).
* **Build System (CMake):** Proyek C++ menggunakan **`CMakeLists.txt`** untuk kompilasi yang terstruktur (**Nilai Plus**).

## 🛠️ Persyaratan Sistem

### Hardware
* 1x Laptop/PC.
* 2x Modul ESP32 (Satu sebagai **Bridge**, satu sebagai **Receiver**).
    * (Board yang digunakan: **ESP32 DOIT-DEVKIT-V1** disarankan).

### Software
* **Laptop C++:** CMake (v3.10+), GCC/G++ atau MSVC.
* **ESP32:** PlatformIO (PIO) atau Arduino IDE dengan *framework* **Arduino** dan *library* **ESP-NOW**.

## 🔌 Protokol & Header Paket

Data yang dikirim antara komponen memiliki struktur *header* 12 *byte* yang wajib sinkron di C++ dan ESP32.

### Header Paket (12 Bytes - Little Endian)
| Offset (Bytes) | Ukuran (Bytes) | Deskripsi |
| :---: | :---: | :--- |
| 0 | 2 | `index` (Indeks Paket) |
| 2 | 2 | `total` (Total Paket) |
| 4 | 2 | `filename_len` (Panjang Nama File) |
| 6 | 2 | `payload_len` (Panjang Data JSON di Paket ini) |
| 8 | 4 | `checksum` (CRC32 dari **SELURUH** *file* JSON) |
| 12+ | Variabel | Nama File, diikuti oleh Payload Data JSON |

### Protokol Serial (Laptop → Bridge)
Setiap pengiriman dibingkai dengan *byte* penanda:
`0xAA` (SOP) + [Data Header & Payload] + `0x55` (EOP)

---

## 🚀 Panduan Eksekusi (4 Langkah Kunci)

### Langkah 1: Setup Program C++ (Laptop)

1. **Kompilasi:** Di terminal, navigasi ke `project_root/` dan jalankan:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build . 
    ```
2. **Validasi:** Pastikan `data/myprofile.json` sudah memenuhi syarat (minimal 25 kata di *field* `deskripsi`).
3. **Port:** Catat *Port Serial* (misalnya `COM6`) yang akan digunakan untuk Bridge.

### Langkah 2: Setup ESP32-Receiver (PlatformIO)

1. **Upload Kode:** Unggah kode Receiver (`main.cpp`) ke ESP32 Receiver Anda.
2. **Catat MAC Address:** Buka Serial Monitor (115200 baud) dan catat **MAC Address Receiver**. (Contoh: `08:B6:1F:D5:C3:40`).

### Langkah 3: Setup ESP32-Bridge (PlatformIO)

1. **Update MAC Address:** Buka kode Bridge dan ubah `broadcastAddress[]` sesuai dengan MAC Address Receiver yang dicatat pada Langkah 2.
    ```cpp
    // Contoh penyesuaian:
    uint8_t broadcastAddress[] = {0x08, 0xB6, 0x1F, 0xD5, 0xC3, 0x40}; 
    ```
2. **Upload Kode:** Unggah kode Bridge yang sudah dimodifikasi ini ke ESP32 Bridge Anda.
3. **Persiapan:** **Tutup semua Monitor Serial** (PIO) untuk mengosongkan *port*.

### Langkah 4: Eksekusi Final dan Verifikasi

1. **Jalankan C++:** Jalankan program C++ di terminal Laptop.
    ```bash
    .\build\CPFT.exe 
    ```
    (Atau `./build/CPFT` di Linux/macOS)
2. **Verifikasi:** Setelah C++ selesai, segera buka Serial Monitor **ESP32-Receiver** di PlatformIO (`pio device monitor`).

### ✅ Tolok Ukur Keberhasilan

Output yang sukses di Serial Monitor Receiver harus mencakup:

1.  Log penerimaan *chunk* yang berurutan.
2.  Pesan **`===== TRANSFER SELESAI =====`**.
3.  Pesan verifikasi **CRC32 yang cocok**.
4.  Pencetakan data JSON yang diformat dengan benar:
    ```
    [KONTEN FILE YANG DITERIMA]
    NAMA: [Nama Anda]
    JURUSAN: [Jurusan Anda]
    UMUR: [Umur Anda]
    DESKRIPSI DIRI: [Deskripsi Anda]
    ------------------------------------------
    ```
