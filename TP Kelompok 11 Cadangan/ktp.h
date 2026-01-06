#ifndef KTPDATA_H
#define KTPDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Struktur tanggal lahir
struct Date {
    int hari, bulan, tahun;
};

// Struktur node data KTP
struct NodeKTP {
    long long int nik;
    string nama;
    string alamat;
    Date tanggalLahir;
    bool jenisKelamin;  // true = LAKI-LAKI, false = PEREMPUAN
    string agama;
    bool statusPerkawinan;
    string pekerjaan;
    string kewarganegaraan;
    string password;    // Untuk login
    string status;      // "Aktif" atau "Nonaktif"

    NodeKTP* next;  // Untuk linked list
    NodeKTP* prev;  // Untuk doubly linked list

    // Constructor
    NodeKTP(long long int nik_, string nama_, string alamat_, Date tgl,
            bool jk_, string agama_, bool status_, string kerja_, string kewn_, 
            string pass_) 
    {
        nik = nik_;
        nama = nama_;
        alamat = alamat_;
        tanggalLahir = tgl;
        jenisKelamin = jk_;
        agama = agama_;
        statusPerkawinan = status_;
        pekerjaan = kerja_;
        kewarganegaraan = kewn_;
        password = pass_;
        status = "Aktif";

        next = nullptr;
        prev = nullptr;
    }

    NodeKTP() : nik(0), nama(""), alamat(""), jenisKelamin(true), 
                agama(""), statusPerkawinan(false), pekerjaan(""), 
                kewarganegaraan(""), password(""), status("Aktif"), 
                next(nullptr), prev(nullptr) {}
};

// Fungsi konversi bool ke string
inline string boolToString(bool b) {
    return b ? "1" : "0";
}

// Fungsi konversi string ke bool
inline bool stringToBool(string s) {
    return s == "1";
}

// Global variables untuk KTP
inline NodeKTP* headKTP = NULL;
inline NodeKTP* tailKTP = NULL;

// ==========================================
// FUNGSI LINKED LIST KTP (Doubly Linked List)
// ==========================================

// Insert KTP ke akhir list
void insertKTP(NodeKTP* data) {
    if (data == NULL) return;
    
    data->next = NULL;
    data->prev = tailKTP;

    if (headKTP == NULL) {
        headKTP = data;
        tailKTP = data;
    } else {
        tailKTP->next = data;
        tailKTP = data;
    }
}

// Search KTP berdasarkan NIK
NodeKTP* searchKTPByNIK(long long int nik) {
    NodeKTP* curr = headKTP;
    while (curr != NULL) {
        if (curr->nik == nik) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// Search KTP berdasarkan Nama
NodeKTP* searchKTPByName(string nama) {
    NodeKTP* curr = headKTP;
    while (curr != NULL) {
        if (curr->nama == nama) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// Tampilkan semua data KTP
void displayAllKTP() {
    NodeKTP* curr = headKTP;
    int nomor = 1;
    
    if (curr == NULL) {
        cout << "Belum ada data KTP terdaftar.\n";
        return;
    }

    cout << "\n===== DAFTAR PENDAFTAR KTP =====\n";
    while (curr != NULL) {
        cout << "\n[" << nomor << "] NIK: " << curr->nik << endl;
        cout << "    Nama: " << curr->nama << endl;
        cout << "    Alamat: " << curr->alamat << endl;
        cout << "    TTL: " << curr->tanggalLahir.hari << "/" 
             << curr->tanggalLahir.bulan << "/" 
             << curr->tanggalLahir.tahun << endl;
        cout << "    JK: " << (curr->jenisKelamin ? "Laki-laki" : "Perempuan") << endl;
        cout << "    Agama: " << curr->agama << endl;
        cout << "    Status: " << (curr->statusPerkawinan ? "Kawin" : "Belum") << endl;
        cout << "    Pekerjaan: " << curr->pekerjaan << endl;
        cout << "    Kewarganegaraan: " << curr->kewarganegaraan << endl;
        cout << "    Status Akun: " << curr->status << endl;
        nomor++;
        curr = curr->next;
    }
    cout << "===============================\n";
}

// Delete KTP berdasarkan NIK
bool deleteKTPByNIK(long long int nik) {
    NodeKTP* curr = headKTP;
    while (curr != NULL) {
        if (curr->nik == nik) {
            if (curr->prev) curr->prev->next = curr->next;
            else headKTP = curr->next;

            if (curr->next) curr->next->prev = curr->prev;
            else tailKTP = curr->prev;

            delete curr;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// ==========================================
// FILE HANDLING KTP
// ==========================================

// Format penyimpanan file:
// NIK#Nama#Alamat#Hari#Bulan#Tahun#JK#Agama#Status#Pekerjaan#Kewarganegaraan#Password#StatusAkun

void saveKTP() {
    ofstream file("data_ktp.txt");
    if (!file.is_open()) {
        cerr << "GAGAL: Tidak bisa membuka data_ktp.txt untuk menulis.\n";
        return;
    }

    NodeKTP* curr = headKTP;
    while (curr != NULL) {
        file << curr->nik << "#"
             << curr->nama << "#"
             << curr->alamat << "#"
             << curr->tanggalLahir.hari << "#"
             << curr->tanggalLahir.bulan << "#"
             << curr->tanggalLahir.tahun << "#"
             << boolToString(curr->jenisKelamin) << "#"
             << curr->agama << "#"
             << boolToString(curr->statusPerkawinan) << "#"
             << curr->pekerjaan << "#"
             << curr->kewarganegaraan << "#"
             << curr->password << "#"
             << curr->status << endl;
        curr = curr->next;
    }
    file.close();
    cout << ">> Data KTP berhasil disimpan ke data_ktp.txt\n";
}

void loadKTP() {
    ifstream file("data_ktp.txt");
    if (!file.is_open()) {
        // File tidak ada = normal (program pertama kali)
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string nik_str, nama, alamat, hari_str, bulan_str, tahun_str, jk_str,
                agama, status_str, pekerjaan, kewarganegaraan, password, status_akun;

        getline(ss, nik_str, '#');
        getline(ss, nama, '#');
        getline(ss, alamat, '#');
        getline(ss, hari_str, '#');
        getline(ss, bulan_str, '#');
        getline(ss, tahun_str, '#');
        getline(ss, jk_str, '#');
        getline(ss, agama, '#');
        getline(ss, status_str, '#');
        getline(ss, pekerjaan, '#');
        getline(ss, kewarganegaraan, '#');
        getline(ss, password, '#');
        getline(ss, status_akun, '#');

        // Convert string ke tipe data yang sesuai
        long long int nik = stoll(nik_str);
        int hari = stoi(hari_str);
        int bulan = stoi(bulan_str);
        int tahun = stoi(tahun_str);
        bool jk = stringToBool(jk_str);
        bool status = stringToBool(status_str);

        Date tgl;
        tgl.hari = hari;
        tgl.bulan = bulan;
        tgl.tahun = tahun;

        NodeKTP* newNode = new NodeKTP(nik, nama, alamat, tgl, jk, agama, 
                                       status, pekerjaan, kewarganegaraan, password);
        newNode->status = status_akun;

        insertKTP(newNode);
    }
    file.close();
    cout << ">> Data KTP berhasil dimuat dari data_ktp.txt\n";
}

// Fungsi untuk login menggunakan KTP (berdasarkan NIK + Password)
NodeKTP* loginKTP(long long int nik, string password) {
    NodeKTP* user = searchKTPByNIK(nik);
    if (user && user->password == password && user->status == "Aktif") {
        return user;
    }
    return NULL;
}

// Fungsi untuk clear semua data KTP (untuk testing)
void clearAllKTP() {
    while (headKTP != NULL) {
        NodeKTP* temp = headKTP;
        headKTP = headKTP->next;
        delete temp;
    }
    tailKTP = NULL;
}

#endif
