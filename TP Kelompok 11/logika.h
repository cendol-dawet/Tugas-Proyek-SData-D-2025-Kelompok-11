#ifndef LOGIKA_H
#define LOGIKA_H

#include "data.h" // Wajib hubungkan dengan file data.h

void saveAntrian();
void saveAdmin();
void saveToko();
void saveNasabah();
void saveHarga();

void dequeueSpecific(NodeAntrian* target);

// ==========================================
// FUNGSI LINKED LIST & TREE
// ==========================================

// --- ADMIN (DLL) ---
void insertAdmin(DataAdmin d, bool autoSave = true) {
    NodeAdmin* baru = new NodeAdmin;
    baru->data = d;
    baru->next = NULL;
    baru->prev = tailAdmin;

    if (headAdmin == NULL) {
        headAdmin = baru;
        tailAdmin = baru;
    } else {
        tailAdmin->next = baru;
        tailAdmin = baru;
    }
    if (autoSave) saveAdmin();
}

void deleteAdmin(string username) {
    NodeAdmin* curr = headAdmin;
    while (curr != NULL) {
        if (curr->data.username == username) {
            if (curr->prev) curr->prev->next = curr->next;
            else headAdmin = curr->next;

            if (curr->next) curr->next->prev = curr->prev;
            else tailAdmin = curr->prev;

            delete curr;
            cout << ">> Admin berhasil dihapus.\n";
            saveAdmin();
            return;
        }
        curr = curr->next;
    }
    cout << ">> Admin tidak ditemukan.\n";
}

int hitungAdminDiToko(string idToko) {
    int count = 0;
    NodeAdmin* curr = headAdmin;
    while (curr != NULL) {
        if (curr->data.isVerified && curr->data.idToko == idToko) {
            count++;
        }
        curr = curr->next;
    }
    return count;
}

// --- TOKO (DLL) ---
void insertToko(DataToko d, bool autoSave = true) {
    NodeToko* baru = new NodeToko;
    baru->data = d;
    baru->next = NULL;
    baru->prev = tailToko;

    if (headToko == NULL) {
        headToko = baru;
        tailToko = baru;
    } else {
        tailToko->next = baru;
        tailToko = baru;
    }
    if (autoSave) saveToko();
}

void tambahStokToko(string idToko, double berat) {
    NodeToko* curr = headToko;
    while (curr != NULL) {
        if (curr->data.idToko == idToko) {
            curr->data.totalSampahTerkumpul += berat;
            saveToko();
            return;
        }
        curr = curr->next;
    }
}

void resetStokToko(string idToko) {
    NodeToko* curr = headToko;
    while (curr != NULL) {
        if (curr->data.idToko == idToko) {
            cout << ">> Mengirim " << curr->data.totalSampahTerkumpul << " kg ke Pusat...\n";
            curr->data.totalSampahTerkumpul = 0;
            saveToko();
            return;
        }
        curr = curr->next;
    }
}

void deleteToko(string idToko) {
    NodeToko* curr = headToko;
    while (curr != NULL) {
        if (curr->data.idToko == idToko) {
            // Hapus semua admin yang terkait dengan toko ini
            NodeAdmin* aCurr = headAdmin;
            while (aCurr != NULL) {
                NodeAdmin* aNext = aCurr->next;
                if (aCurr->data.idToko == idToko) {
                    if (aCurr->prev) aCurr->prev->next = aCurr->next;
                    else headAdmin = aCurr->next;

                    if (aCurr->next) aCurr->next->prev = aCurr->prev;
                    else tailAdmin = aCurr->prev;

                    delete aCurr;
                }
                aCurr = aNext;
            }
            saveAdmin(); // Save admin changes
            cout << ">> Semua admin di toko ini juga telah dihapus.\n";

            if (curr->prev) curr->prev->next = curr->next;
            else headToko = curr->next;

            if (curr->next) curr->next->prev = curr->prev;
            else tailToko = curr->prev;

            delete curr;
            cout << ">> Toko berhasil dihapus.\n";
            saveToko(); // Save toko changes
            return;
        }
        curr = curr->next;
    }
    cout << ">> Toko tidak ditemukan.\n";
}

// --- NASABAH (BST) ---
NodeNasabah* insertNasabahRec(NodeNasabah* node, DataNasabah d) {
    if (node == NULL) {
        NodeNasabah* baru = new NodeNasabah;
        baru->data = d;
        baru->left = baru->right = NULL;
        return baru;
    }
    // Urutkan berdasarkan NIK (7001, 7002, ...)
    if (d.nik < node->data.nik)
        node->left = insertNasabahRec(node->left, d);
    else if (d.nik > node->data.nik)
        node->right = insertNasabahRec(node->right, d);
    return node;
}

// Cari berdasarkan NIK (untuk BST)
NodeNasabah* searchNasabahByNIK(NodeNasabah* node, int nik) {
    if (node == NULL || node->data.nik == nik)
        return node;
    if (nik < node->data.nik)
        return searchNasabahByNIK(node->left, nik);
    return searchNasabahByNIK(node->right, nik);
}

// Cari berdasarkan Username (untuk Login - Traversal PreOrder)
NodeNasabah* searchNasabahByUsername(NodeNasabah* node, string username) {
    if (node == NULL) return NULL;
    
    if (node->data.username == username) return node;
    
    NodeNasabah* leftRes = searchNasabahByUsername(node->left, username);
    if (leftRes != NULL) return leftRes;
    
    return searchNasabahByUsername(node->right, username);
}

// --- TRAVERSAL ---

void inorderNasabah(NodeNasabah* node) {
    if (node == NULL) return;
    inorderNasabah(node->left);
    cout << "NIK: " << node->data.nik << " | User: " << node->data.username 
         << " | Nama: " << node->data.nama << " | Saldo: " << (long)node->data.saldo << endl;
    inorderNasabah(node->right);
}

void preorderNasabah(NodeNasabah* node) {
    if (node == NULL) return;
    cout << "NIK: " << node->data.nik << " | User: " << node->data.username 
         << " | Nama: " << node->data.nama << " | Saldo: " << (long)node->data.saldo << endl;
    preorderNasabah(node->left);
    preorderNasabah(node->right);
}

void postorderNasabah(NodeNasabah* node) {
    if (node == NULL) return;
    postorderNasabah(node->left);
    postorderNasabah(node->right);
    cout << "NIK: " << node->data.nik << " | User: " << node->data.username 
         << " | Nama: " << node->data.nama << " | Saldo: " << (long)node->data.saldo << endl;
}

// Helper untuk mencari nilai minimum di subtree kanan
NodeNasabah* findMin(NodeNasabah* node) {
    while (node->left != NULL) node = node->left;
    return node;
}

// Helper untuk mencari NIK terbesar (untuk auto-increment)
int getMaxNIK(NodeNasabah* node) {
    if (node == NULL) return 7000; // Start dari 7000, jadi next 7001
    while (node->right != NULL) {
        node = node->right;
    }
    return node->data.nik;
}

NodeNasabah* deleteNasabahRec(NodeNasabah* node, int nik) {
    if (node == NULL) return node;

    if (nik < node->data.nik)
        node->left = deleteNasabahRec(node->left, nik);
    else if (nik > node->data.nik)
        node->right = deleteNasabahRec(node->right, nik);
    else {
        // Node ditemukan
        // Case 1: No child
        if (node->left == NULL && node->right == NULL) {
            delete node;
            node = NULL;
        }
        // Case 2: One child
        else if (node->left == NULL) {
            NodeNasabah* temp = node;
            node = node->right;
            delete temp;
        }
        else if (node->right == NULL) {
            NodeNasabah* temp = node;
            node = node->left;
            delete temp;
        }
        // Case 3: Two children
        else {
            NodeNasabah* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteNasabahRec(node->right, temp->data.nik);
        }
    }
    return node;
}

void deleteNasabah(string username) {
    // Cek dulu apakah nasabah ada
    NodeNasabah* target = searchNasabahByUsername(rootNasabah, username);
    if (target == NULL) {
        cout << ">> Data Nasabah " << username << " tidak ditemukan. Gagal menghapus.\n";
        return;
    }

    // 1. Hapus dari Antrian dulu
    NodeAntrian* curr = frontQ;
    while (curr != NULL) {
        NodeAntrian* nextNode = curr->next;
        if (curr->idNasabah == username) {
            dequeueSpecific(curr);
        }
        curr = nextNode;
    }
    
    // 2. Hapus dari BST
    rootNasabah = deleteNasabahRec(rootNasabah, target->data.nik);
    cout << ">> Data Nasabah " << username << " berhasil dihapus.\n";
    saveNasabah();
}

// --- ANTRIAN MANUAL (FIFO) ---
void enqueue(string idNsb, string jenis, double berat, string idToko, bool autoSave = true) {
    NodeAntrian* baru = new NodeAntrian;
    baru->idNasabah = idNsb;
    baru->jenisSampah = jenis;
    baru->beratEstimasi = berat;
    baru->idToko = idToko;
    baru->next = NULL;
    baru->prev = NULL;

    if (frontQ == NULL) {
        frontQ = baru;
        rearQ = baru;
    } else {
        rearQ->next = baru;
        baru->prev = rearQ;
        rearQ = baru;
    }
    if (autoSave) saveAntrian();
}

void dequeueSpecific(NodeAntrian* target) {
    if (frontQ == NULL || target == NULL) return;

    // Jika target adalah Head
    if (frontQ == target) {
        frontQ = frontQ->next;
        if (frontQ != NULL) {
            frontQ->prev = NULL;
        } else {
            rearQ = NULL;
        }
    }
    // Jika target adalah Tail
    else if (target == rearQ) {
        rearQ = target->prev;
        if (rearQ != NULL) {
            rearQ->next = NULL;
        } else {
            frontQ = NULL;
        }
    }
    // Jika di tengah
    else {
        if (target->prev != NULL) target->prev->next = target->next;
        if (target->next != NULL) target->next->prev = target->prev;
    }
    
    delete target;
    saveAntrian();
}

// --- STACK (Doubly Linked List) - UNDO ---
void pushStack(string idNsb, string idToko, double berat, double nominal, string jenis) {
    NodeStack* baru = new NodeStack;
    baru->data.idNasabah = idNsb;
    baru->data.idToko = idToko;
    baru->data.berat = berat;
    baru->data.nominal = nominal;
    baru->data.jenisSampah = jenis;
    
    baru->next = NULL;
    baru->prev = NULL;

    // Push ke Head (Top)
    if (topStack == NULL) {
        topStack = baru;
    } else {
        baru->next = topStack;
        topStack->prev = baru;
        topStack = baru;
    }
}

void popUndo() {
    if (topStack == NULL) {
        cout << ">> Tidak ada riwayat transaksi untuk di-undo.\n";
        return;
    }

    NodeStack* temp = topStack;
    DataRiwayat d = temp->data;

    // 1. Kembalikan Saldo Nasabah
    NodeNasabah* nsb = searchNasabahByUsername(rootNasabah, d.idNasabah);
    if (nsb != NULL) {
        nsb->data.saldo -= d.nominal;
        cout << ">> Undo Saldo Nasabah " << d.idNasabah << " berkurang Rp " << (long)d.nominal << endl;
    }

    // 2. Kembalikan Stok Toko
    NodeToko* t = headToko;
    while (t != NULL) {
        if (t->data.idToko == d.idToko) {
            t->data.totalSampahTerkumpul -= d.berat;
            cout << ">> Undo Stok Toko " << d.idToko << " berkurang " << d.berat << " kg\n";
            break;
        }
        t = t->next;
    }

    // 3. Hapus dari Stack
    topStack = topStack->next;
    if (topStack != NULL) {
        topStack->prev = NULL;
    }
    delete temp;
    cout << ">> Transaksi berhasil dibatalkan.\n";
    saveNasabah();
    saveToko();
}

// ==========================================
// FILE HANDLING (.TXT)
// ==========================================

void saveAdmin() {
    ofstream file("output/data_admin.txt");
    // Header Tabel
    file << left << setw(20) << "USERNAME" << " | " 
         << setw(20) << "PASSWORD" << " | " 
         << setw(25) << "NAMA" << " | " 
         << setw(10) << "TOKO" << " | " 
         << "VERIFIED" << endl;

    NodeAdmin* curr = headAdmin;
    while (curr != NULL) {
        // Hanya simpan admin yang sudah diverifikasi
        if (curr->data.isVerified) {
            file << left << setw(20) << curr->data.username << " | " 
                 << setw(20) << curr->data.password << " | " 
                 << setw(25) << curr->data.nama << " | " 
                 << setw(10) << curr->data.idToko << " | " 
                 << curr->data.isVerified << endl;
        }
        curr = curr->next;
    }
    file.close();
}

void loadAdmin() {
    ifstream file("output/data_admin.txt");
    if (!file.is_open()) return;
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (firstLine && line.find("USERNAME") != string::npos) {
            firstLine = false; continue;
        }
        if (line.empty()) continue;

        DataAdmin d; string ver, segment;
        stringstream ss(line);

        // Cek Format: Apakah ada '|'?
        if (line.find('|') != string::npos) {
            // Format Baru (Tabel)
            getline(ss, segment, '|'); d.username = trim(segment);
            getline(ss, segment, '|'); d.password = trim(segment);
            getline(ss, segment, '|'); d.nama = trim(segment);
            getline(ss, segment, '|'); d.idToko = trim(segment);
            getline(ss, segment, '|'); ver = trim(segment);
        } else {
            // Format Lama (Pagar #)
            getline(ss, d.username, '#'); 
            getline(ss, d.password, '#');
            getline(ss, d.nama, '#'); 
            getline(ss, d.idToko, '#'); 
            getline(ss, ver, '#');
        }
        
        if (d.username.empty()) continue;
        d.isVerified = (ver == "1");
        insertAdmin(d, false);
    }
    file.close();
}

void saveToko() {
    ofstream file("output/data_toko.txt");
    file << left << setw(10) << "ID" << " | " 
         << setw(25) << "NAMA TOKO" << " | " 
         << "TOTAL SAMPAH" << endl;

    NodeToko* curr = headToko;
    while (curr != NULL) {
        file << left << setw(10) << curr->data.idToko << " | " 
             << setw(25) << curr->data.namaToko << " | " 
             << curr->data.totalSampahTerkumpul << endl;
        curr = curr->next;
    }
    file.close();
}

void loadToko() {
    ifstream file("output/data_toko.txt");
    if (!file.is_open()) return;
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (firstLine && line.find("ID") != string::npos && line.find("NAMA TOKO") != string::npos) {
            firstLine = false; continue;
        }
        if (line.empty()) continue;

        DataToko d; string total, segment;
        stringstream ss(line);
        
        if (line.find('|') != string::npos) {
            // Format Baru
            getline(ss, segment, '|'); d.idToko = trim(segment);
            getline(ss, segment, '|'); d.namaToko = trim(segment);
            getline(ss, segment, '|'); total = trim(segment);
        } else {
            // Format Lama
            getline(ss, d.idToko, '#'); 
            getline(ss, d.namaToko, '#');
            getline(ss, total, '#');
        }
        
        if (d.idToko.empty()) continue;
        try { d.totalSampahTerkumpul = stod(total); } catch (...) { d.totalSampahTerkumpul = 0; }
        insertToko(d, false);
    }
    file.close();
}

// --- HARGA (Global) ---
void saveHarga() {
    ofstream file("output/data_harga.txt");
    file << left << setw(15) << "ORGANIK" << " | " 
         << setw(15) << "NON-ORGANIK" << " | " 
         << "BIO-ORGANIK" << endl;
    
    file << left << setw(15) << hargaGlobal.organik << " | " 
         << setw(15) << hargaGlobal.nonOrganik << " | " 
         << hargaGlobal.bioOrganik << endl;
    file.close();
}

void loadHarga() {
    ifstream file("output/data_harga.txt");
    if (!file.is_open()) return;
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (firstLine) { firstLine = false; continue; } // Skip header
        if (line.empty()) continue;

        stringstream ss(line);
        string sOrg, sNon, sBio;
        
        getline(ss, sOrg, '|');
        getline(ss, sNon, '|');
        getline(ss, sBio, '|');

        try {
            hargaGlobal.organik = stod(trim(sOrg));
            hargaGlobal.nonOrganik = stod(trim(sNon));
            hargaGlobal.bioOrganik = stod(trim(sBio));
        } catch (...) {
            // Keep default values if error
        }
    }
    file.close();
}

void saveRiwayat() {
    ofstream file("output/data_riwayat.txt");
    file << left << setw(15) << "NASABAH" << " | " 
         << setw(15) << "TOKO" << " | " 
         << setw(10) << "BERAT" << " | " 
         << setw(15) << "NOMINAL" << " | " 
         << "JENIS" << endl;

    NodeStack* bottom = topStack;
    if (bottom != NULL) {
        while (bottom->next != NULL) {
            bottom = bottom->next;
        }
    }
    
    NodeStack* curr = bottom;
    while (curr != NULL) {
        file << left << setw(15) << curr->data.idNasabah << " | " 
             << setw(15) << curr->data.idToko << " | " 
             << setw(10) << curr->data.berat << " | " 
             << setw(15) << (long)curr->data.nominal << " | " 
             << curr->data.jenisSampah << endl;
        curr = curr->prev;
    }
    file.close();
}

void loadRiwayat() {
    ifstream file("output/data_riwayat.txt");
    if (!file.is_open()) return;
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (firstLine) { firstLine = false; continue; }
        if (line.empty()) continue;

        string idNsb, idTk, sBerat, sNom, jenis;
        stringstream ss(line);
        string segment;
        
        getline(ss, segment, '|'); idNsb = trim(segment);
        getline(ss, segment, '|'); idTk = trim(segment);
        getline(ss, segment, '|'); sBerat = trim(segment);
        getline(ss, segment, '|'); sNom = trim(segment);
        getline(ss, segment, '|'); jenis = trim(segment);

        if (idNsb.empty()) continue;
        
        double berat = 0;
        double nominal = 0;
        try { berat = stod(sBerat); } catch(...) {}
        try { nominal = stod(sNom); } catch(...) {}

        pushStack(idNsb, idTk, berat, nominal, jenis);
    }
    file.close();
}

void saveNasabahHelper(NodeNasabah* node, ofstream& file) {
    if (node == NULL) return;
    
    file << left << setw(10) << node->data.nik << " | " 
         << setw(20) << node->data.username << " | " 
         << setw(25) << node->data.nama << " | " 
         << setw(20) << node->data.password << " | " 
         << (long)node->data.saldo << endl;

    saveNasabahHelper(node->left, file);
    saveNasabahHelper(node->right, file);
}

void saveNasabah() {
    ofstream file("output/data_nasabah.txt");
    file << left << setw(10) << "NIK" << " | " 
         << setw(20) << "USERNAME" << " | " 
         << setw(25) << "NAMA" << " | " 
         << setw(20) << "PASSWORD" << " | " 
         << "SALDO" << endl;
    saveNasabahHelper(rootNasabah, file);
    file.close();
}

void loadNasabah() {
    ifstream file("output/data_nasabah.txt");
    if (!file.is_open()) return;
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (firstLine && line.find("NIK") != string::npos) {
            firstLine = false; continue;
        }
        if (line.empty()) continue;

        DataNasabah d; string sal, nikStr, segment;
        stringstream ss(line);
        
        if (line.find('|') != string::npos) {
            // Format Baru
            getline(ss, segment, '|'); nikStr = trim(segment);
            getline(ss, segment, '|'); d.username = trim(segment);
            getline(ss, segment, '|'); d.nama = trim(segment);
            getline(ss, segment, '|'); d.password = trim(segment);
            getline(ss, segment, '|'); sal = trim(segment);
        } else {
            // Format Lama (Fallback/Migration if needed, or just ignore)
            // Assuming we start fresh or overwrite. 
            // But let's try to map old ID to username if possible, but old ID was string.
            // Let's just support the new format primarily.
            continue; 
        }
        
        if (nikStr.empty()) continue;
        try { 
            d.nik = stoi(nikStr);
            d.saldo = stod(sal); 
        } catch (...) { 
            d.nik = 0; d.saldo = 0; 
        }
        rootNasabah = insertNasabahRec(rootNasabah, d);
    }
    file.close();
}



void saveAntrian() {
    ofstream file("output/data_antrian.txt");
    file << left << setw(10) << "TOKO" << " | " 
         << setw(20) << "USERNAME" << " | " 
         << setw(25) << "NAMA NASABAH" << " | " 
         << setw(15) << "JENIS" << " | " 
         << "BERAT" << endl;

    NodeAntrian* curr = frontQ;
    while (curr != NULL) {
        // Cari nama nasabah untuk disimpan (agar file lebih informatif)
        string namaNsb = "-";
        NodeNasabah* nsb = searchNasabahByUsername(rootNasabah, curr->idNasabah);
        if (nsb != NULL) namaNsb = nsb->data.nama;

        file << left << setw(10) << curr->idToko << " | " 
             << setw(20) << curr->idNasabah << " | " 
             << setw(25) << namaNsb << " | " 
             << setw(15) << curr->jenisSampah << " | " 
             << curr->beratEstimasi << endl;
        curr = curr->next;
    }
    file.close();
}

void loadAntrian() {
    ifstream file("output/data_antrian.txt");
    if (!file.is_open()) return;
    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (firstLine && line.find("TOKO") != string::npos) {
            firstLine = false; continue;
        }
        if (line.empty()) continue;

        string idToko, idNasabah, namaDummy, jenis, beratStr, segment;
        stringstream ss(line);
        
        if (line.find('|') != string::npos) {
            getline(ss, segment, '|'); idToko = trim(segment);
            getline(ss, segment, '|'); idNasabah = trim(segment);
            getline(ss, segment, '|'); namaDummy = trim(segment); // Skip nama, kita pakai ID
            getline(ss, segment, '|'); jenis = trim(segment);
            getline(ss, segment, '|'); beratStr = trim(segment);
            
            if (!idToko.empty() && !idNasabah.empty()) {
                double berat = 0;
                try { berat = stod(beratStr); } catch(...) {}
                enqueue(idNasabah, jenis, berat, idToko, false);
            }
        }
    }
    file.close();
}

// ==========================================
// HELPER FUNCTIONS
// ==========================================

bool areAllShopsFull() {
    if (headToko == NULL) return true; 
    
    NodeToko* curr = headToko;
    while (curr != NULL) {
        if (hitungAdminDiToko(curr->data.idToko) < 1) {
            return false; // Ada toko yang belum penuh
        }
        curr = curr->next;
    }
    return true; // Semua toko penuh
}

bool isSystemFull() {
    if (headToko == NULL) return true; // Belum ada toko -> Tidak bisa daftar

    int totalSlots = 0;
    NodeToko* t = headToko;
    while (t != NULL) {
        totalSlots += 2;
        t = t->next;
    }

    int totalVerified = 0;
    NodeAdmin* a = headAdmin;
    while (a != NULL) {
        if (a->data.isVerified) totalVerified++;
        a = a->next;
    }

    return (totalVerified >= totalSlots);
}

// ==========================================
// HUFFMAN CODING (MANUAL - NO STL)
// ==========================================

// --- MIN HEAP IMPLEMENTATION ---
struct MinHeap {
    int size;
    NodeHuffman* array[256]; // Kapasitas max 256 karakter ASCII
};

MinHeap* createMinHeap() {
    MinHeap* minHeap = new MinHeap;
    minHeap->size = 0;
    return minHeap;
}

NodeHuffman* newNode(char data, int freq) {
    NodeHuffman* temp = new NodeHuffman;
    temp->ch = data;
    temp->freq = freq;
    temp->left = temp->right = NULL;
    return temp;
}

void swapNodes(NodeHuffman** a, NodeHuffman** b) {
    NodeHuffman* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

NodeHuffman* extractMin(MinHeap* minHeap) {
    NodeHuffman* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, NodeHuffman* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

bool isLeaf(NodeHuffman* root) {
    return !(root->left) && !(root->right);
}

// --- HUFFMAN LOGIC ---

void generateCodes(NodeHuffman* root, string str, string codes[]) {
    if (!root) return;

    if (isLeaf(root)) {
        codes[(unsigned char)root->ch] = str;
    }

    generateCodes(root->left, str + "0", codes);
    generateCodes(root->right, str + "1", codes);
}

void compressDatabase(string inputFile, string outputFile) {
    // 1. Baca File
    ifstream in(inputFile);
    if (!in.is_open()) {
        cout << "Gagal membuka file " << inputFile << endl;
        return;
    }
    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    if (text.empty()) {
        cout << "File kosong.\n";
        return;
    }

    // 2. Hitung Frekuensi (Manual Array)
    int freq[256] = {0};
    for (char c : text) {
        freq[(unsigned char)c]++;
    }

    // 3. Build Tree (Manual Heap)
    MinHeap* minHeap = createMinHeap();
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            insertMinHeap(minHeap, newNode((char)i, freq[i]));
        }
    }

    while (minHeap->size != 1) {
        NodeHuffman* left = extractMin(minHeap);
        NodeHuffman* right = extractMin(minHeap);

        NodeHuffman* top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }
    NodeHuffman* root = extractMin(minHeap);

    // 4. Generate Codes
    string codes[256];
    generateCodes(root, "", codes);

    // 5. Simpan File Binary
    ofstream out(outputFile, ios::binary);

    // A. Header: Simpan Frekuensi (Hanya yang > 0)
    int uniqueChars = 0;
    for(int i=0; i<256; i++) if(freq[i] > 0) uniqueChars++;
    
    out.write(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars));
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            char c = (char)i;
            out.write(&c, sizeof(c));
            out.write(reinterpret_cast<char*>(&freq[i]), sizeof(freq[i]));
        }
    }

    // B. Body
    string bitString = "";
    for (char c : text) {
        bitString += codes[(unsigned char)c];
    }

    // Padding
    unsigned char buffer = 0;
    int bitCount = 0;
    for (char bit : bitString) {
        buffer = buffer << 1;
        if (bit == '1') buffer = buffer | 1;
        bitCount++;
        if (bitCount == 8) {
            out.write(reinterpret_cast<char*>(&buffer), sizeof(buffer));
            buffer = 0;
            bitCount = 0;
        }
    }
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        out.write(reinterpret_cast<char*>(&buffer), sizeof(buffer));
    }

    out.close();
    delete minHeap; // Cleanup heap struct (nodes are in tree)
    cout << ">> Backup Berhasil (Manual Huffman)! File: " << outputFile << endl;
}

void deleteTree(NodeNasabah* node) {
    if (node == NULL) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void decompressDatabase(string inputFile, string outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in.is_open()) {
        cout << "File backup tidak ditemukan.\n";
        return;
    }

    // 1. Baca Header
    int freq[256] = {0};
    int uniqueChars;
    in.read(reinterpret_cast<char*>(&uniqueChars), sizeof(uniqueChars));

    int totalChars = 0;
    for (int i = 0; i < uniqueChars; i++) {
        char c;
        int f;
        in.read(&c, sizeof(c));
        in.read(reinterpret_cast<char*>(&f), sizeof(f));
        freq[(unsigned char)c] = f;
        totalChars += f;
    }

    // 2. Rebuild Tree
    MinHeap* minHeap = createMinHeap();
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            insertMinHeap(minHeap, newNode((char)i, freq[i]));
        }
    }

    if (minHeap->size == 0) return;

    NodeHuffman* root = NULL;
    if (minHeap->size == 1) {
        root = extractMin(minHeap);
    } else {
        while (minHeap->size != 1) {
            NodeHuffman* left = extractMin(minHeap);
            NodeHuffman* right = extractMin(minHeap);
            NodeHuffman* top = newNode('$', left->freq + right->freq);
            top->left = left;
            top->right = right;
            insertMinHeap(minHeap, top);
        }
        root = extractMin(minHeap);
    }

    // 3. Decode
    ofstream out(outputFile);
    NodeHuffman* curr = root;
    unsigned char buffer;
    int count = 0;

    while (in.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
        for (int i = 7; i >= 0; i--) {
            if (count >= totalChars) break;
            
            int bit = (buffer >> i) & 1;
            if (bit == 0) curr = curr->left;
            else curr = curr->right;

            if (isLeaf(curr)) {
                out << curr->ch;
                curr = root;
                count++;
            }
        }
    }
    
    in.close();
    out.close();
    delete minHeap;
    cout << ">> Restore Berhasil! Data dikembalikan ke " << outputFile << endl;
}

// ==========================================
// MENU LOGIC
// ==========================================

void menuOwner() {
    int pil;
    do {
        cout << "\n=== MENU OWNER ===\n";
        cout << "1. Lihat Ajuan Admin\n2. Terima Admin (Verifikasi)\n3. Lihat Admin Aktif\n";
        cout << "4. Hapus Admin\n5. Tambah Toko\n6. Lihat Toko\n7. Hapus Toko\n";
        cout << "8. Kembali\n";
        cout << "9. Backup Data Nasabah (Compress)\n";
        cout << "10. Restore Data Nasabah (Decompress)\n";
        cout << "Pilih: "; cin >> pil;

        if (pil == 1) {
            NodeAdmin* curr = headAdmin;
            bool found = false;
            cout << "\n--- DAFTAR AJUAN ADMIN ---\n";
            while (curr != NULL) {
                if (!curr->data.isVerified) {
                    found = true;
                    cout << "User: " << curr->data.username << " | Nama: " << curr->data.nama << endl;
                }
                curr = curr->next;
            }
            if(!found) cout << "Tidak ada ajuan.\n";
        }
        else if (pil == 2) {
            string u;
            cout << "Masukkan Username Admin yang akan diterima: "; cin >> u;
            
            NodeAdmin* curr = headAdmin;
            NodeAdmin* target = NULL;
            while (curr != NULL) {
                if (curr->data.username == u && !curr->data.isVerified) {
                    target = curr;
                    break;
                }
                curr = curr->next;
            }

            if (target == NULL) {
                cout << "Admin tidak ditemukan atau sudah aktif.\n";
            } else {
                cout << "--- PILIH TOKO ---\n";
                NodeToko* t = headToko;
                bool adaToko = false;
                while (t != NULL) {
                    int count = hitungAdminDiToko(t->data.idToko);
                    cout << "ID: " << t->data.idToko << " (" << count << "/2 Admin)\n";
                    adaToko = true;
                    t = t->next;
                }
                
                if (!adaToko) {
                    cout << "Belum ada toko. Silahkan buat toko dulu (Menu 5).\n";
                } else {
                    string idT;
                    cout << "Masukkan ID Toko: "; cin >> idT;
                    
                    bool shopExists = false;
                    t = headToko;
                    while (t != NULL) {
                        if (t->data.idToko == idT) {
                            shopExists = true;
                            break;
                        }
                        t = t->next;
                    }

                    if (!shopExists) {
                        cout << "Toko tidak ditemukan.\n";
                    } else {
                        if (hitungAdminDiToko(idT) >= 2) {
                            cout << "GAGAL: Toko Penuh!\n";
                        } else {
                            target->data.idToko = idT;
                            target->data.isVerified = true;
                            cout << "SUKSES: Admin " << u << " ditempatkan di " << idT << ".\n";
                            saveAdmin();
                        }
                    }
                }
            }
        }
        else if (pil == 3) {
            NodeAdmin* curr = headAdmin;
            bool found = false;
            cout << "\n--- DAFTAR ADMIN AKTIF ---\n";
            while (curr != NULL) {
                if (curr->data.isVerified) {
                    found = true;
                    cout << "User: " << curr->data.username 
                         << " | Nama: " << curr->data.nama 
                         << " | Toko: " << curr->data.idToko << endl;
                }
                curr = curr->next;
            }
            if (!found) cout << "Tidak ada admin yang aktif.\n";
        }
        else if (pil == 5) {
            cout << "\n--- STATUS TOKO SAAT INI ---\n";
            NodeToko* t = headToko;
            if (t == NULL) cout << "(Belum ada toko)\n";
            while (t != NULL) {
                cout << "ID: " << t->data.idToko << " (" << hitungAdminDiToko(t->data.idToko) << "/2 Admin)\n";
                t = t->next;
            }

            if (headToko != NULL && !areAllShopsFull()) {
                cout << "GAGAL: Masih ada toko yang belum terisi (kurang dari 1 admin).\n";
            } else {
                DataToko dt;
                cout << "ID Toko Baru: "; cin >> dt.idToko;
                cin.ignore(); 
                cout << "Nama Toko: "; getline(cin, dt.namaToko);
                dt.totalSampahTerkumpul = 0;
                insertToko(dt);
            }
        }
        else if (pil == 6) {
            NodeToko* curr = headToko;
            bool found = false;
            cout << "\n--- DAFTAR TOKO ---\n";
            while(curr != NULL){
                found = true;
                cout << "ID: " << curr->data.idToko 
                     << " | Nama: " << curr->data.namaToko 
                     << " | Sampah: " << curr->data.totalSampahTerkumpul << "kg\n";
                
                cout << "   Admin: ";
                NodeAdmin* a = headAdmin;
                bool adaAdmin = false;
                while (a != NULL) {
                    if (a->data.isVerified && a->data.idToko == curr->data.idToko) {
                        if (adaAdmin) cout << ", ";
                        cout << a->data.nama << " (" << a->data.username << ")";
                        adaAdmin = true;
                    }
                    a = a->next;
                }
                if (!adaAdmin) cout << "-";
                cout << endl << endl;

                curr = curr->next;
            }
            if (!found) cout << "Tidak ada toko yang terdaftar.\n";
        }
        else if (pil == 4) {
            string u; cout << "Username dihapus: "; cin >> u;
            deleteAdmin(u);
        }
        else if (pil == 7) {
            string id; cout << "ID Toko dihapus: "; cin >> id;
            deleteToko(id);
        }
        else if (pil == 9) {
            compressDatabase("output/data_nasabah.txt", "output/backup_nasabah.bin");
        }
        else if (pil == 10) {
            decompressDatabase("output/backup_nasabah.bin", "output/data_nasabah.txt");
            // Reload data ke memori agar update
            deleteTree(rootNasabah);
            rootNasabah = NULL;
            loadNasabah(); 
            cout << ">> Data Nasabah telah dimuat ulang ke sistem.\n";
        }
    } while (pil != 8);
}

void menuAdmin(string idTokoAdmin) {
    int pil;
    do {
        cout << "\n=== ADMIN (Toko: " << idTokoAdmin << ") ===\n";
        cout << "1. Layani Antrian\n2. Kirim Sampah (Reset Gudang)\n";
        cout << "3. Lihat Daftar Nasabah\n4. Hapus Nasabah\n";
        cout << "5. Kelola Harga Sampah\n6. Undo Transaksi Terakhir\n7. Kembali\n";
        cout << "Pilih: "; cin >> pil;

        if (pil == 1) {
            // Cari antrian untuk toko ini
            NodeAntrian* n = frontQ;
            while (n != NULL) {
                if (n->idToko == idTokoAdmin) break;
                n = n->next;
            }

            if (n == NULL) cout << "Antrian Kosong untuk Toko ini.\n";
            else {
                cout << "Nasabah: " << n->idNasabah << " | Jenis: " << n->jenisSampah << " | Est: " << n->beratEstimasi << "kg\n";
                double beratReal;
                cout << "Berat Real (kg): "; cin >> beratReal;
                
                double harga = (n->jenisSampah=="Organik") ? hargaGlobal.organik : (n->jenisSampah=="Non") ? hargaGlobal.nonOrganik : hargaGlobal.bioOrganik;
                double total = beratReal * harga;
                
                NodeNasabah* nsb = searchNasabahByUsername(rootNasabah, n->idNasabah);
                if (nsb) {
                    nsb->data.saldo += total;
                    tambahStokToko(idTokoAdmin, beratReal);
                    
                    // PUSH KE STACK
                    pushStack(n->idNasabah, idTokoAdmin, beratReal, total, n->jenisSampah);
                    saveRiwayat(); // Simpan riwayat
                    
                    cout << "Saldo bertambah: Rp " << (long)total << endl;
                    saveNasabah();
                }
                dequeueSpecific(n);
            }
        }
        else if (pil == 2) resetStokToko(idTokoAdmin);
        else if (pil == 3) {
            int tPil;
            do {
                cout << "\n=== LIHAT DAFTAR NASABAH (TRAVERSAL) ===\n";
                cout << "1. In-Order (Urut NIK)\n";
                cout << "2. Pre-Order\n";
                cout << "3. Post-Order\n";
                cout << "4. Kembali\n";
                cout << "Pilih: "; cin >> tPil;

                if (rootNasabah == NULL) {
                    cout << "Belum ada nasabah.\n";
                } else {
                    if (tPil == 1) {
                        cout << "\n--- IN-ORDER ---\n";
                        inorderNasabah(rootNasabah);
                    } else if (tPil == 2) {
                        cout << "\n--- PRE-ORDER ---\n";
                        preorderNasabah(rootNasabah);
                    } else if (tPil == 3) {
                        cout << "\n--- POST-ORDER ---\n";
                        postorderNasabah(rootNasabah);
                    }
                }
            } while (tPil != 4);
        }
        else if (pil == 4) {
            string id; cout << "Username Nasabah dihapus: "; cin >> id;
            deleteNasabah(id);
        }
        else if (pil == 5) {
            cout << "\n--- KELOLA HARGA SAMPAH ---\n";
            cout << "Harga Saat Ini:\n";
            cout << "1. Organik     : Rp " << (long)hargaGlobal.organik << "/kg\n";
            cout << "2. Non-Organik : Rp " << (long)hargaGlobal.nonOrganik << "/kg\n";
            cout << "3. Bio-Organik : Rp " << (long)hargaGlobal.bioOrganik << "/kg\n";
            cout << "4. Kembali\n";
            
            int hPil;
            cout << "Ubah harga nomor (1-3): "; cin >> hPil;
            if (hPil >= 1 && hPil <= 3) {
                double baru;
                cout << "Masukkan Harga Baru: Rp "; cin >> baru;
                if (hPil == 1) hargaGlobal.organik = baru;
                else if (hPil == 2) hargaGlobal.nonOrganik = baru;
                else if (hPil == 3) hargaGlobal.bioOrganik = baru;
                cout << "Harga berhasil diupdate.\n";
                saveHarga();
            }
        }
        else if (pil == 6) {
            saveRiwayat(); // Update file setelah undo
            popUndo();
        }
    } while (pil != 7);
}

void menuNasabah(NodeNasabah* user) {
    int pil;
    do {
        cout << "\n=== HALO " << user->data.nama << " ===\n";
        cout << "Saldo: Rp " << (long)user->data.saldo << endl;
        cout << "1. Setor Sampah\n2. Tarik Tunai\n3. Batalkan Antrian\n4. Kembali\n";
        cout << "Pilih: "; cin >> pil;
        
        if (pil == 1) {
            // Tampilkan Toko
            cout << "\n--- PILIH TOKO TUJUAN ---\n";
            NodeToko* t = headToko;
            bool adaToko = false;
            while (t != NULL) {
                cout << "- " << t->data.idToko << " (" << t->data.namaToko << ")\n";
                adaToko = true;
                t = t->next;
            }

            if (!adaToko) {
                cout << "Belum ada toko tersedia. Tidak bisa setor.\n";
            } else {
                string idT;
                cout << "Masukkan ID Toko: "; cin >> idT;

                // Validasi Toko
                bool valid = false;
                t = headToko;
                while (t != NULL) {
                    if (t->data.idToko == idT) { valid = true; break; }
                    t = t->next;
                }

                if (!valid) {
                    cout << "Toko tidak ditemukan.\n";
                } else {
                    string j; double b;
                    cout << "Jenis (Organik/Non/Bio): "; cin >> j;
                    cout << "Berat (kg): "; cin >> b;
                    enqueue(user->data.username, j, b, idT);
                    cout << "Berhasil masuk antrian Toko " << idT << ".\n";
                }
            }
        }
        else if (pil == 2) {
            double t; cout << "Tarik Berapa? "; cin >> t;
            if (t > user->data.saldo) cout << "Saldo kurang.\n";
            else { 
                user->data.saldo -= t; 
                cout << "Berhasil tarik.\n"; 
                saveNasabah();
            }
        }
        else if (pil == 3) {
            cout << "\n--- DAFTAR ANTRIAN ANDA ---\n";
            NodeAntrian* curr = frontQ;
            int count = 0;
            bool found = false;
            
            // Tampilkan antrian milik user ini
            while (curr != NULL) {
                if (curr->idNasabah == user->data.username) {
                    count++;
                    found = true;
                    cout << count << ". Toko: " << curr->idToko 
                         << " | Jenis: " << curr->jenisSampah 
                         << " | Est: " << curr->beratEstimasi << "kg\n";
                }
                curr = curr->next;
            }

            if (!found) {
                cout << "Anda tidak memiliki antrian aktif.\n";
            } else {
                int hapusIdx;
                cout << "Pilih nomor antrian yang ingin dibatalkan (0 batal): "; 
                cin >> hapusIdx;

                if (hapusIdx > 0 && hapusIdx <= count) {
                    // Cari lagi pointer ke node yang dipilih
                    curr = frontQ;
                    int currentIdx = 0;
                    while (curr != NULL) {
                        if (curr->idNasabah == user->data.username) {
                            currentIdx++;
                            if (currentIdx == hapusIdx) {
                                dequeueSpecific(curr);
                                cout << "Antrian berhasil dibatalkan.\n";
                                break;
                            }
                        }
                        curr = curr->next;
                    }
                }
            }
        }
    } while (pil != 4);
}

#endif