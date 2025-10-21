#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Packet {
    uint16_t index;         // urutan paket
    uint16_t total;         // total paket
    uint16_t filename_len;  // panjang nama file
    uint16_t payload_len;   // panjang data
    uint32_t checksum;      // CRC32 dari file utuh
    std::string filename;   // nama file (UTF-8)
    std::vector<uint8_t> payload; // potongan data

    Packet();
    Packet(uint16_t idx, uint16_t tot, uint32_t crc,
           const std::string &fname, const std::vector<uint8_t> &data);

    // ubah ke byte vector untuk dikirim lewat serial
    std::vector<uint8_t> serialize() const;

    // (opsional) baca balik dari byte vector untuk testing lokal
    static Packet deserialize(const std::vector<uint8_t> &bytes, bool &ok);
};
