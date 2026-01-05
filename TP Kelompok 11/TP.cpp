#include <iostream>
#include <string>
#include "logika.h"
#include "ktp.h"

using namespace std;

int main() {
    // 1. Load Data TXT
    loadToko();
    loadAdmin();
    loadNasabah();
    loadHarga();
    loadAntrian();

    int menuUtama;
    do {
        cout << "\n==============================\n";
        cout << "    APLIKASI BANK SAMPAH\n";
        cout << "==============================\n";
        cout << "1. Login Owner\n";
        cout << "2. Login Admin\n";
        cout << "3. Login Nasabah\n";
        cout << "4. Daftar Admin Baru\n";
        cout << "5. Daftar Nasabah Baru\n";
        cout << "6. Keluar & Simpan\n";
        cout << "Pilih: "; cin >> menuUtama;
        cin.ignore();

        if (menuUtama == 1) {
            string p;
            cout << "Pass Owner: "; cin >> p;
            if (p == "owner123") menuOwner();
        }
        else if (menuUtama == 2) {
            string u, p;
            cout << "Username: "; cin >> u; cout << "Pass: "; cin >> p;
            NodeAdmin* curr = headAdmin;
            bool login = false;
            while (curr != NULL) {
                if (curr->data.username == u && curr->data.password == p) {
                    if (curr->data.isVerified) { menuAdmin(curr->data.idToko); login = true; }
                    else cout << "Akun belum diverifikasi Owner!\n";
                    break;
                }
                curr = curr->next;
            }
            if (!login && curr == NULL) cout << "Gagal Login.\n";
        }
        else if (menuUtama == 3) {
            string id, p;
            cout << "Username: "; cin >> id; cout << "Pass: "; cin >> p;
            NodeNasabah* user = searchNasabahByUsername(rootNasabah, id);
            if (user && user->data.password == p) menuNasabah(user);
            else cout << "Username atau password salah!.\n";
        }
        else if (menuUtama == 4) {
            if (isSystemFull()) {
                cout << "Pendaftaran DITUTUP. Belum ada toko atau kuota admin penuh.\n";
            } else {
                DataAdmin d;
                cout << "Username: "; cin >> d.username;

                // Cek apakah username sudah ada
                bool exists = false;
                NodeAdmin* curr = headAdmin;
                while (curr != NULL) {
                    if (curr->data.username == d.username) {
                        exists = true;
                        break;
                    }
                    curr = curr->next;
                }

                if (exists) {
                    cout << "Username sudah digunakan! Silakan gunakan username lain.\n";
                } else {
                    cout << "Password: "; cin >> d.password;
                    cout << "Nama: "; cin >> d.nama;
                    d.idToko = "-"; d.isVerified = false;
                    insertAdmin(d);
                    cout << "Berhasil daftar.\n";
                }
            }
        }
        else if (menuUtama == 5) {
            // Daftar Nasabah - Kombinasi Daftar KTP + Username/Password Nasabah
            cout << "\n===== DAFTAR NASABAH BARU =====\n";
            cout << "Silakan isi data KTP terlebih dahulu.\n\n";
            
            // --- STEP 1: INPUT DATA KTP ---
            long long int nik;
            string nama, alamat, agama, pekerjaan, kewarganegaraan, passwordKTP;
            int hari, bulan, tahun;
            int jkChoice, statusChoice;

            cout << "=== DATA KTP ===\n";
            cout << "NIK: "; cin >> nik;
            cin.ignore();
            
            // Check apakah NIK sudah terdaftar
            if (searchKTPByNIK(nik) != NULL) {
                cout << "GAGAL: NIK sudah terdaftar!\n";
            } else {
                cout << "Nama Lengkap: "; getline(cin, nama);
                cout << "Alamat: "; getline(cin, alamat);
                cout << "Tanggal Lahir (DD MM YYYY): "; cin >> hari >> bulan >> tahun;
                cin.ignore();

                cout << "Jenis Kelamin (1=Laki-laki, 0=Perempuan): "; cin >> jkChoice;
                cin.ignore();

                cout << "Agama: "; getline(cin, agama);
                cout << "Status Perkawinan (1=Kawin, 0=Belum): "; cin >> statusChoice;
                cin.ignore();

                cout << "Pekerjaan: "; getline(cin, pekerjaan);
                cout << "Kewarganegaraan: "; getline(cin, kewarganegaraan);
                passwordKTP = "default123"; // Password default untuk KTP

                // Validasi tanggal
                if (hari < 1 || hari > 31 || bulan < 1 || bulan > 12) {
                    cout << "GAGAL: Tanggal tidak valid!\n";
                } else {
                    // --- STEP 2: INPUT USERNAME & PASSWORD NASABAH ---
                    string usernameNasabah, passwordNasabah;
                    cout << "\n=== DATA NASABAH ===\n";
                    
                    // Auto-generate NIK (7001, 7002, ...)
                    int newNIK = getMaxNIK(rootNasabah) + 1;
                    cout << "NIK Nasabah (Auto): " << newNIK << endl;

                    cout << "Username (ID) Nasabah: "; cin >> usernameNasabah;
                    cout << "Password Nasabah: "; cin >> passwordNasabah;
                    cin.ignore();
                    
                    // Validasi username belum ada
                    NodeNasabah* cekUser = searchNasabahByUsername(rootNasabah, usernameNasabah);
                    if (cekUser != NULL) {
                        cout << "GAGAL: Username sudah digunakan!\n";
                    } else {
                        // --- STEP 3: BUAT KTP ---
                        Date tglLahir;
                        tglLahir.hari = hari;
                        tglLahir.bulan = bulan;
                        tglLahir.tahun = tahun;

                        NodeKTP* newKTP = new NodeKTP(nik, nama, alamat, tglLahir,
                        (jkChoice == 1), agama, (statusChoice == 1),
                        pekerjaan, kewarganegaraan, passwordKTP);
                        insertKTP(newKTP);

                        // --- STEP 4: BUAT AKUN NASABAH ---
                        DataNasabah d;
                        d.nik = newNIK;
                        d.username = usernameNasabah;
                        d.nama = nama;
                        d.password = passwordNasabah;
                        d.saldo = 0;
                        
                        rootNasabah = insertNasabahRec(rootNasabah, d);
                        saveNasabah(); // Simpan data nasabah ke txt
                        
                        cout << "\n=== Pendaftaran Berhasil! ===\n";
                        cout << "- NIK KTP: " << nik << endl;
                        cout << "- NIK Nasabah: " << newNIK << endl;
                        cout << "- Nama: " << nama << endl;
                        cout << "- Username Nasabah: " << usernameNasabah << endl;
                        cout << "- Saldo awal: Rp 0\n";
                    }
                }
            }
        }

    } while (menuUtama != 6);

    // 2. Save Data TXT
    saveToko();
    saveAdmin();
    saveNasabah();
    saveHarga();
    saveAntrian();
    cout << "Data tersimpan. Bye!\n";

    return 0;
}
