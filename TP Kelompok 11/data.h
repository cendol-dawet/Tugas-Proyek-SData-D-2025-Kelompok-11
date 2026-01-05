#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Helper untuk Trim spasi (agar load data tabel aman)
inline string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// ==========================================
// 1. STRUKTUR DATA (STRUCT)
// ==========================================

// HUFFMAN NODE (Manual Implementation)
struct NodeHuffman {
    char ch;
    int freq;
    NodeHuffman *left;
    NodeHuffman *right;
};

// Config Harga (Global)
struct HargaSampah {
    double organik = 5000;
    double nonOrganik = 10000;
    double bioOrganik = 50000;
};

// TOKO (Doubly Linked List)
struct DataToko {
    string idToko;
    string namaToko;
    double totalSampahTerkumpul;
};

struct NodeToko {
    DataToko data;
    NodeToko* next;
    NodeToko* prev;
};

// ADMIN (Doubly Linked List)
struct DataAdmin {
    string username;
    string password;
    string nama;
    string idToko;
    bool isVerified;
};

struct NodeAdmin {
    DataAdmin data;
    NodeAdmin* next;
    NodeAdmin* prev;
};

// NASABAH (Binary Search Tree)
struct DataNasabah {
    int nik; // Format: 7001, 7002, ...
    string username; // Unik
    string nama;
    string password;
    double saldo;
};

struct NodeNasabah {
    DataNasabah data;
    NodeNasabah* left;
    NodeNasabah* right;
};

// ANTRIAN (Doubly Linked List - Manual FIFO)
struct NodeAntrian {
    string idNasabah;
    string jenisSampah;
    double beratEstimasi;
    string idToko; // Tambahan: Tujuan Toko
    NodeAntrian* next;
    NodeAntrian* prev;
};

// STACK (Doubly Linked List) - Riwayat Transaksi untuk Undo
struct DataRiwayat {
    string idNasabah;
    string idToko;
    double berat;
    double nominal;
    string jenisSampah;
};

struct NodeStack {
    DataRiwayat data;
    NodeStack* next;
    NodeStack* prev;
};

// ==========================================
// 2. VARIABEL GLOBAL
// ==========================================
// Kita deklarasikan static/inline agar bisa diakses semua file
inline HargaSampah hargaGlobal;

inline NodeAdmin* headAdmin = NULL;
inline NodeAdmin* tailAdmin = NULL;

inline NodeToko* headToko = NULL;
inline NodeToko* tailToko = NULL;

inline NodeNasabah* rootNasabah = NULL;

inline NodeAntrian* frontQ = NULL;
inline NodeAntrian* rearQ = NULL;

inline NodeStack* topStack = NULL; // Top of Stack

#endif