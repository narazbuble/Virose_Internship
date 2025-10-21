#include "packet.hpp"
#include <cstring>

Packet::Packet() : index(0), total(0), filename_len(0),
                   payload_len(0), checksum(0) {}

Packet::Packet(uint16_t idx, uint16_t tot, uint32_t crc,
               const std::string &fname, const std::vector<uint8_t> &data)
    : index(idx), total(tot), checksum(crc),
      filename(fname), payload(data) {
    filename_len = static_cast<uint16_t>(fname.size());
    payload_len  = static_cast<uint16_t>(data.size());
}

// ubah ke vector<uint8_t>
std::vector<uint8_t> Packet::serialize() const {
    std::vector<uint8_t> buf;

    auto append16 = [&](uint16_t v) {
        buf.push_back(v & 0xFF);
        buf.push_back((v >> 8) & 0xFF);
    };
    auto append32 = [&](uint32_t v) {
        buf.push_back(v & 0xFF);
        buf.push_back((v >> 8) & 0xFF);
        buf.push_back((v >> 16) & 0xFF);
        buf.push_back((v >> 24) & 0xFF);
    };

    append16(index);
    append16(total);
    append16(filename_len);
    append16(payload_len);
    append32(checksum);

    buf.insert(buf.end(), filename.begin(), filename.end());
    buf.insert(buf.end(), payload.begin(), payload.end());
    return buf;
}

// parsing balik (u/ test)
Packet Packet::deserialize(const std::vector<uint8_t> &bytes, bool &ok) {
    Packet p;
    ok = false;
    if (bytes.size() < 12) return p;

    auto read16 = [&](size_t pos) {
        return (uint16_t)(bytes[pos] | (bytes[pos + 1] << 8));
    };
    auto read32 = [&](size_t pos) {
        return (uint32_t)(bytes[pos] | (bytes[pos + 1] << 8) |
                          (bytes[pos + 2] << 16) | (bytes[pos + 3] << 24));
    };

    p.index = read16(0);
    p.total = read16(2);
    p.filename_len = read16(4);
    p.payload_len  = read16(6);
    p.checksum     = read32(8);

    size_t expect = 12 + p.filename_len + p.payload_len;
    if (bytes.size() < expect) return p;

    p.filename.assign(bytes.begin() + 12, bytes.begin() + 12 + p.filename_len);
    p.payload.assign(bytes.begin() + 12 + p.filename_len,
                     bytes.begin() + 12 + p.filename_len + p.payload_len);
    ok = true;
    return p;
}
