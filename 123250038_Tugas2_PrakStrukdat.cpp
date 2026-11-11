#include <iostream>
using namespace std;

struct Menu {
    string nama;
    string kategori;
    double harga;
    bool tersedia;
};

struct NodeAntrian {
    int nomor;
    string namaPelanggan;
    NodeAntrian* next;
};

struct Queue{
    NodeAntrian* depan;
    NodeAntrian* belakang;
    int count;
};

struct Node{
    Menu data;
    Queue antrian;
    Node* left;
    Node* right;
};

enum JenisAksi{
    Pesanan,
    Status_Menu
};

struct Aksi{
    JenisAksi jenis;
    string namaMenu;
    string namaPelanggan;
    int nomorAntrian;
    bool sedangMenunggu;
    bool statusSebelumnya;
    bool pelangganDilayani;
    string namaPelangganDilayani;
    int nomorAntrianDilayani;
    Aksi* next;
};

struct Stack{
    Aksi* top;
};

Node* root = nullptr;
Stack undo;
int nomorAntrianBerikutnya = 1;

//  Inisialisasi void dan node
void init(Queue& q);
bool isEmpty(const Queue& q);
bool enqueue(Queue& q, const string& namaPelanggan, int nomor);
bool enqueueDepan(Queue& q, const string& namaPelanggan, int nomor);
NodeAntrian* dequeue(Queue& q);
void clearQueue(Queue& q);
void initStack(Stack& s);
bool isEmpty(const Stack& s);
void push(Stack& s, Aksi* aksi);
Aksi* pop(Stack& s);
void clearStack(Stack& s);
Node* buatNode(const Menu& data);
void tambahMenu(Node*& root, const Menu& data);
Node* cariMenu(Node* root, const string& nama);
void tampilMenu(const Node* root);
void tukarData(Node* pertama, Node* kedua);
bool hapusMenuNode(Node*& root, const string& nama);
void clearMenu(Node* root);
void tambahMenuBaru();
void lihatDaftarMenu();
void tandaiMenuHabis();
void catatPesanan();
void tandaiMenuTersedia();
void hapusMenu();
void undoAksiTerakhir();
void tampilkanAntrian();
void tampilMenuUtama();

int main() {
    int pilihan;
    initStack(undo);

    while (true) {
        tampilMenuUtama();
        cout << "Pilih menu: ";
        if (!(cin >> pilihan)) {
            break;
        }
        cin.ignore(1000, '\n');

        switch (pilihan) {
            case 1:
                tambahMenuBaru();
                break;
            case 2:
                lihatDaftarMenu();
                break;
            case 3:
                tandaiMenuHabis();
                break;
            case 4:
                catatPesanan();
                break;
            case 5:
                tandaiMenuTersedia();
                break;
            case 6:
                hapusMenu();
                break;
            case 7:
                undoAksiTerakhir();
                break;
            case 8:
                tampilkanAntrian();
                break;
            case 0:
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
                break;
        }
        if (pilihan == 0) {
            break;
        }
    }

    clearMenu(root);
    clearStack(undo);
    return 0;
}

void init(Queue& q){
    q.depan = nullptr;
    q.belakang = nullptr;
    q.count = 0;
}

bool isEmpty(const Queue& q){
    return q.depan == nullptr;
}

bool enqueue(Queue& q, const string& namaPelanggan, int nomor) {
    NodeAntrian* baru = new NodeAntrian;
    baru->nomor = nomor;
    baru->namaPelanggan = namaPelanggan;
    baru->next = nullptr;

    if (isEmpty(q)) {
        q.depan = baru;
        q.belakang = baru;
    } else {
        q.belakang->next = baru;
        q.belakang = baru;
    }
    q.count++;
    return true;
}

bool enqueueDepan(Queue& q, const string& namaPelanggan, int nomor) {
    NodeAntrian* baru = new NodeAntrian;
    baru->nomor = nomor;
    baru->namaPelanggan = namaPelanggan;
    baru->next = q.depan;
    q.depan = baru;
    if (q.belakang == nullptr) {
        q.belakang = baru;
    }
    q.count++;
    return true;
}

NodeAntrian* dequeue(Queue& q) {
    if (isEmpty(q)) {
        return nullptr;
    }

    NodeAntrian* bantu = q.depan;
    q.depan = q.depan->next;
    if (q.depan == nullptr) {
        q.belakang = nullptr;
    }

    bantu->next = nullptr;
    q.count--;
    return bantu;
}

void clearQueue(Queue& q) {
    while (!isEmpty(q)) {
        NodeAntrian* hapus = q.depan;
        q.depan = q.depan->next;
        delete hapus;
    }
    init(q);
}

void initStack(Stack& s) {
    s.top = nullptr;
}

bool isEmpty(const Stack& s) {
    return s.top == nullptr;
}

void push(Stack& s, Aksi* aksi) {
    aksi->next = s.top;
    s.top = aksi;
}

Aksi* pop(Stack& s) {
    if (isEmpty(s)) {
        return nullptr;
    }

    Aksi* bantu = s.top;
    s.top = s.top->next;
    bantu->next = nullptr;
    return bantu;
}

void clearStack(Stack& s) {
    while (!isEmpty(s)) {
        delete pop(s);
    }
}

Node* buatNode(const Menu& data) {
    Node* baru = new Node;
    baru->data = data;
    init(baru->antrian);
    baru->left = nullptr;
    baru->right = nullptr;
    return baru;
}

void tambahMenu(Node*& root, const Menu& data) {
    if (root == nullptr) {
        root = buatNode(data);
        return;
    }

    if (data.nama < root->data.nama) {
        tambahMenu(root->left, data);
    } else if (data.nama > root->data.nama) {
        tambahMenu(root->right, data);
    }
}

Node* cariMenu(Node* root, const string& nama) {
    if (root == nullptr || root->data.nama == nama) {
        return root;
    }
    if (nama < root->data.nama) {
        return cariMenu(root->left, nama);
    }
    return cariMenu(root->right, nama);
}

void tampilMenu(const Node* root) {
    if (root == nullptr) {
        return;
    }
    tampilMenu(root->left);
    cout << "Nama: " << root->data.nama
         << " | Kategori: " << root->data.kategori
         << " | Harga: Rp " << root->data.harga
         << " | Status: " << (root->data.tersedia ? "Tersedia" : "Habis")
         << endl;
    tampilMenu(root->right);
}

void tukarData(Node* pertama, Node* kedua) {
    Menu bantu = pertama->data;
    pertama->data = kedua->data;
    kedua->data = bantu;

    Queue antrian = pertama->antrian;
    pertama->antrian = kedua->antrian;
    kedua->antrian = antrian;
}

bool hapusMenuNode(Node*& root, const string& nama) {
    if (root == nullptr) {
        return false;
    }
    if (nama < root->data.nama) {
        return hapusMenuNode(root->left, nama);
    }
    if (nama > root->data.nama) {
        return hapusMenuNode(root->right, nama);
    }

    if (root->left == nullptr || root->right == nullptr) {
        Node* hapus = root;
        root = root->left != nullptr ? root->left : root->right;
        clearQueue(hapus->antrian);
        delete hapus;
        return true;
    }

    Node* penerus = root->right;
    while (penerus->left != nullptr) {
        penerus = penerus->left;
    }
    string namaDihapus = root->data.nama;
    tukarData(root, penerus);
    return hapusMenuNode(root->right, namaDihapus);
}

void clearMenu(Node* root) {
    if (root == nullptr) {
        return;
    }
    clearMenu(root->left);
    clearMenu(root->right);
    clearQueue(root->antrian);
    delete root;
}

void tambahMenuBaru() {
    Menu menuBaru;
    cout << "Masukkan nama menu: ";
    getline(cin, menuBaru.nama);
    cout << "Masukkan kategori makanan: ";
    getline(cin, menuBaru.kategori);
    cout << "Masukkan harga: ";
    if (!(cin >> menuBaru.harga) || menuBaru.harga < 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Harga tidak valid!\n";
        return;
    }
    cin.ignore(1000, '\n');

    if (menuBaru.nama.empty() || menuBaru.kategori.empty()) {
        cout << "Nama dan kategori menu tidak boleh kosong.\n";
        return;
    }
    if (cariMenu(root, menuBaru.nama) != nullptr) {
        cout << "Menu dengan nama tersebut sudah ada.\n";
        return;
    }

    menuBaru.tersedia = true;
    tambahMenu(root, menuBaru);
    cout << "Menu berhasil ditambahkan\n";
}

void lihatDaftarMenu() {
    if (root == nullptr) {
        cout << "Daftar menu kosong.\n";
        return;
    }
    tampilMenu(root);
}

void tandaiMenuHabis() {
    string namaMenu;
    cout << "Masukkan nama menu: ";
    getline(cin, namaMenu);

    Node* menu = cariMenu(root, namaMenu);
    if (menu == nullptr) {
        cout << "Menu tidak ditemukan.\n";
        return;
    }
    if (!menu->data.tersedia) {
        cout << "Menu sudah habis.\n";
        return;
    }

    Aksi* aksi = new Aksi;
    aksi->jenis = Status_Menu;
    aksi->namaMenu = namaMenu;
    aksi->namaPelanggan = "";
    aksi->nomorAntrian = 0;
    aksi->sedangMenunggu = false;
    aksi->statusSebelumnya = menu->data.tersedia;
    aksi->pelangganDilayani = false;
    aksi->namaPelangganDilayani = "";
    aksi->nomorAntrianDilayani = 0;
    aksi->next = nullptr;

    menu->data.tersedia = false;
    push(undo, aksi);
    cout << "Menu ditandai habis\n";
}

void catatPesanan() {
    string namaPelanggan;
    string namaMenu;
    cout << "Masukkan nama pelanggan: ";
    getline(cin, namaPelanggan);
    cout << "Masukkan nama menu: ";
    getline(cin, namaMenu);

    Node* menu = cariMenu(root, namaMenu);
    if (menu == nullptr) {
        cout << "Menu tidak ditemukan.\n";
        return;
    }
    if (namaPelanggan.empty()) {
        cout << "Nama pelanggan tidak boleh kosong.\n";
        return;
    }

    Aksi* aksi = new Aksi;
    aksi->jenis = Pesanan;
    aksi->namaMenu = namaMenu;
    aksi->namaPelanggan = namaPelanggan;
    aksi->nomorAntrian = 0;
    aksi->sedangMenunggu = !menu->data.tersedia;
    aksi->statusSebelumnya = menu->data.tersedia;
    aksi->pelangganDilayani = false;
    aksi->namaPelangganDilayani = "";
    aksi->nomorAntrianDilayani = 0;
    aksi->next = nullptr;

    if (menu->data.tersedia) {
        cout << "Pesanan " << namaPelanggan << " berhasil dibuat\n";
    } else {
        aksi->nomorAntrian = nomorAntrianBerikutnya++;
        enqueue(menu->antrian, namaPelanggan, aksi->nomorAntrian);
        cout << "Menu sedang habis, " << namaPelanggan
             << " masuk ke dalam waiting list\n";
    }
    push(undo, aksi);
}

void tandaiMenuTersedia() {
    string namaMenu;
    cout << "Masukkan nama menu: ";
    getline(cin, namaMenu);

    Node* menu = cariMenu(root, namaMenu);
    if (menu == nullptr) {
        cout << "Menu tidak ditemukan.\n";
        return;
    }
    if (menu->data.tersedia) {
        cout << "Menu tersedia kembali\n";
        return;
    }

    Aksi* aksi = new Aksi;
    aksi->jenis = Status_Menu;
    aksi->namaMenu = namaMenu;
    aksi->namaPelanggan = "";
    aksi->nomorAntrian = 0;
    aksi->sedangMenunggu = false;
    aksi->statusSebelumnya = menu->data.tersedia;
    aksi->pelangganDilayani = false;
    aksi->namaPelangganDilayani = "";
    aksi->nomorAntrianDilayani = 0;
    aksi->next = nullptr;

    menu->data.tersedia = true;
    NodeAntrian* pelangganMenunggu = dequeue(menu->antrian);
    if (pelangganMenunggu != nullptr) {
        aksi->pelangganDilayani = true;
        aksi->namaPelangganDilayani = pelangganMenunggu->namaPelanggan;
        aksi->nomorAntrianDilayani = pelangganMenunggu->nomor;
    }
    push(undo, aksi);

    cout << "Menu tersedia kembali\n";
    if (pelangganMenunggu != nullptr) {
        cout << "Pesanan langsung diberikan kepada "
             << pelangganMenunggu->namaPelanggan << " dari waiting list\n";
        delete pelangganMenunggu;
    }
}

void hapusMenu() {
    string namaMenu;
    cout << "Masukkan nama menu yang ingin dihapus: ";
    getline(cin, namaMenu);

    if (!hapusMenuNode(root, namaMenu)) {
        cout << "Menu tidak ditemukan.\n";
        return;
    }
    cout << "Menu berhasil dihapus\n";
}

void undoAksiTerakhir() {
    Aksi* aksi = pop(undo);
    if (aksi == nullptr) {
        cout << "Tidak ada aksi untuk di-undo\n";
        return;
    }

    Node* menu = cariMenu(root, aksi->namaMenu);
    if (aksi->jenis == Pesanan) {
        if (aksi->sedangMenunggu && menu != nullptr) {
            Queue sementara;
            init(sementara);
            while (!isEmpty(menu->antrian)) {
                NodeAntrian* pelanggan = dequeue(menu->antrian);
                if (pelanggan->nomor == aksi->nomorAntrian) {
                    delete pelanggan;
                } else {
                    enqueue(sementara, pelanggan->namaPelanggan,
                            pelanggan->nomor);
                    delete pelanggan;
                }
            }
            menu->antrian = sementara;
        }
        cout << "Undo: Pesanan menu " << aksi->namaMenu << " dibatalkan\n";
    } else {
        if (menu != nullptr) {
            menu->data.tersedia = aksi->statusSebelumnya;
            if (aksi->pelangganDilayani) {
                enqueueDepan(menu->antrian, aksi->namaPelangganDilayani,
                             aksi->nomorAntrianDilayani);
            }
        }
        if (aksi->statusSebelumnya) {
            cout << "Undo: Menu " << aksi->namaMenu
                 << " kembali tersedia\n";
        } else {
            cout << "Undo: Menu " << aksi->namaMenu << " kembali habis\n";
        }
    }
    delete aksi;
}

void tampilkanAntrian() {
    string namaMenu;
    cout << "Masukkan nama menu: ";
    getline(cin, namaMenu);

    Node* menu = cariMenu(root, namaMenu);
    if (menu == nullptr) {
        cout << "Menu tidak ditemukan.\n";
        return;
    }
    if (isEmpty(menu->antrian)) {
        cout << "Antrian kosong\n";
        return;
    }

    NodeAntrian* temp = menu->antrian.depan;
    while (temp != nullptr) {
        cout << temp->namaPelanggan << " menunggu menu "
             << menu->data.nama << endl;
        temp = temp->next;
    }
}

void tampilMenuUtama() {
    cout << "\n=== Sistem Pesanan Restoran ===\n";
    cout << "1. Tambah menu\n";
    cout << "2. Lihat daftar menu\n";
    cout << "3. Tandai menu habis\n";
    cout << "4. Catat pesanan pelanggan\n";
    cout << "5. Catat menu tersedia kembali\n";
    cout << "6. Hapus menu\n";
    cout << "7. Undo pesanan/pembaruan menu\n";
    cout << "8. Tampilkan waiting list\n";
    cout << "0. Keluar\n";
}