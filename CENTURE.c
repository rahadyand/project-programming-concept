#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ========================= KONSTANTA ========================= */
#define MAKS_TEKS           300
#define MAKS_OPSI           150

#define JUMLAH_DUNIA        10
#define SOAL_PER_DUNIA      5
#define SOAL_DIUJIKAN       3

#define NYAWA_AWAL          3
#define POIN_BENAR          10

/* ========================= STRUCT ========================= */
typedef struct {
    int dunia; /* 0..9 */
    char pertanyaan[MAKS_TEKS];
    char opsi[4][MAKS_OPSI];
    char jawabanBenar; /* A/B/C/D */
} Soal;

typedef struct NodeSoal {
    Soal data;
    struct NodeSoal *next;
} NodeSoal;

/* ========================= NAMA & DESKRIPSI DUNIA ========================= */

static const char *NAMA_DUNIA[JUMLAH_DUNIA] = {
    "Monumen Nasional (Monas)",
    "Lawang Sewu",
    "Keraton Yogyakarta",
    "Pabrik Gula Colomadu",
    "Kepulauan Raja Ampat",
    "Gunung Semeru",
    "Candi Borobudur",
    "Benteng Vredeburg",
    "Hutan Kalimantan",
    "Pelabuhan Tanjung Priok"
};

static const char *DESKRIPSI_DUNIA[JUMLAH_DUNIA] = {
    "Perjalanan CENTURE dimulai di Monumen Nasional (Monas). Dari titik awal ini kamu membangun fondasi "
    "pemrograman C: mengenal struktur program, peran fungsi main, cara program dijalankan, serta aturan dasar "
    "penulisan yang rapi. Dunia ini menjadi pijakan awal agar langkahmu di dunia-dunia berikutnya lebih terarah.",

    "Dari Monas, kamu memasuki Lawang Sewu yang terkenal dengan banyak pintu dan pilihan jalur. Di dunia ini kamu "
    "mempelajari pemilihan dan perulangan: if/else, switch, dan loop. Logika yang tepat menentukan jalannya program, "
    "seperti memilih pintu yang benar agar tidak kembali ke lorong yang sama.",

    "Perjalanan berlanjut ke Keraton Yogyakarta, tempat budaya dan bahasa terasa kuat. Di dunia ini kamu mempelajari "
    "character dan string. Kamu memahami bagaimana teks disimpan, dibaca, dan diproses sehingga program bisa berinteraksi "
    "dengan pengguna secara jelas dan benar.",

    "Selanjutnya kamu tiba di Pabrik Gula Colomadu, gambaran proses yang tertata. Dunia ini membahas function: membagi program "
    "menjadi bagian-bagian kecil, memakai parameter dan return value, memanfaatkan fungsi library dasar, serta memahami scope "
    "agar program rapi dan mudah dipelihara.",

    "Dari daratan, kamu berlayar menuju Kepulauan Raja Ampat. Banyak pulau yang tersusun rapi menggambarkan konsep array. "
    "Dunia ini menguji array satu dimensi dan dua dimensi, serta cara mengakses indeks dengan tepat. Ketelitian sangat penting "
    "agar data tidak tertukar atau keluar batas.",

    "Perjalanan semakin menantang saat mendaki Gunung Semeru. Dunia ini membahas pointer dan alamat memori. Kamu belajar bahwa "
    "data memiliki lokasi, dan pointer membantu mengakses serta mengubah data secara efektif. Materi ini penting untuk memahami "
    "hubungan data dan memori dalam C.",

    "Setelah melewati puncak, kamu tiba di Candi Borobudur yang memiliki pola berulang bertingkat. Dunia ini menggambarkan rekursi: "
    "fungsi yang memanggil dirinya sendiri. Kamu dilatih memahami base case agar rekursi berhenti dengan benar dan tidak berjalan "
    "tanpa akhir.",

    "Perjalanan berlanjut ke Benteng Vredeburg yang kokoh dan terstruktur. Dunia ini membahas struct untuk mengelompokkan data, "
    "union untuk penggunaan memori, enum untuk konstanta bernama, serta operasi bitwise dasar. Tujuannya agar kamu bisa membentuk "
    "data yang lebih terorganisir.",

    "Dari benteng, kamu memasuki Hutan Kalimantan yang memiliki banyak cabang dan jalur. Dunia ini mengenalkan struktur data seperti "
    "linked list, stack, queue, dan tree. Kamu belajar bahwa data bisa dikelola secara dinamis, tidak selalu statis seperti array.",

    "Perjalanan berakhir di Pelabuhan Tanjung Priok. Dunia terakhir membahas file processing: menyimpan data secara permanen, membaca "
    "dan menulis file, serta mengenal perbedaan akses berurutan (sequential) dan akses acak (random access) sebagai bekal pemrograman nyata."
};

/* ========================= UTILITAS TEKS ========================= */

static void trim_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

static void trim_spaces(char *s) {
    /* trim kiri */
    size_t start = 0;
    while (s[start] && isspace((unsigned char)s[start])) start++;

    if (start > 0) memmove(s, s + start, strlen(s + start) + 1);

    /* trim kanan */
    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) {
        s[n - 1] = '\0';
        n--;
    }
}

static void bersihkanInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void tekanEnter(void) {
    printf("\nTekan ENTER untuk melanjutkan...");
    fflush(stdout);
    /* tunggu enter */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* ========================= ANIMASI SEDERHANA ========================= */
/* Aman semester 1: hanya loading titik, pakai clock() */
static void tunda_ms(int ms) {
    clock_t mulai = clock();
    clock_t durasi = (clock_t)((ms / 1000.0) * CLOCKS_PER_SEC);
    while ((clock() - mulai) < durasi) {
        /* tunggu */
    }
}

static void animasi_loading(const char *teks) {
    printf("%s", teks);
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        tunda_ms(350);
    }
    printf("\n");
}

/* ========================= TAMPILAN ========================= */

static void tampilkanJudul(void) {
    printf("============================================\n");
    printf("                 C E N T U R E\n");
    printf("               (SI-ADVENTURE)\n");
    printf("============================================\n\n");
}

static void tampilkanPanelMenu(void) {
    printf("+------------------------------------------+\n");
    printf("|  1. Mulai Petualangan                     |\n");
    printf("|  2. Panduan Dunia (Bab)                   |\n");
    printf("|  3. Tentang Game                          |\n");
    printf("|  4. Keluar                                |\n");
    printf("+------------------------------------------+\n");
}

static void garis(void) {
    printf("--------------------------------------------\n");
}

/* ========================= LINKED LIST ========================= */

static NodeSoal* buatNode(const Soal *data) {
    NodeSoal *node = (NodeSoal*)malloc(sizeof(NodeSoal));
    if (!node) return NULL;
    node->data = *data;
    node->next = NULL;
    return node;
}

static void tambahBelakang(NodeSoal **kepala, NodeSoal **ekor, const Soal *data) {
    NodeSoal *baru = buatNode(data);
    if (!baru) return;

    if (*kepala == NULL) {
        *kepala = baru;
        *ekor = baru;
    } else {
        (*ekor)->next = baru;
        *ekor = baru;
    }
}

static void bebasList(NodeSoal **kepala) {
    NodeSoal *cur = *kepala;
    while (cur) {
        NodeSoal *hapus = cur;
        cur = cur->next;
        free(hapus);
    }
    *kepala = NULL;
}

/* ========================= FILE HANDLING ========================= */
/*
Format per soal:
Baris 1: pertanyaan
Baris 2: opsi1;opsi2;opsi3;opsi4
Baris 3: jawaban benar (A/B/C/D)
Baris 4: baris kosong pemisah (disarankan, tapi program tetap aman jika tidak ada)
*/
static int bacaSoalKeLinkedList(const char *namaFile, NodeSoal **kepala, NodeSoal **ekor) {
    FILE *fp = fopen(namaFile, "r");
    if (!fp) return 0;

    char barisPertanyaan[MAKS_TEKS];
    char barisOpsi[4 * MAKS_OPSI];
    char barisJawaban[64];

    int jumlah = 0;

    while (fgets(barisPertanyaan, sizeof(barisPertanyaan), fp)) {
        trim_newline(barisPertanyaan);
        trim_spaces(barisPertanyaan);

        /* Lewati baris kosong */
        if (strlen(barisPertanyaan) == 0) continue;

        if (!fgets(barisOpsi, sizeof(barisOpsi), fp)) break;
        trim_newline(barisOpsi);
        trim_spaces(barisOpsi);

        if (!fgets(barisJawaban, sizeof(barisJawaban), fp)) break;
        trim_newline(barisJawaban);
        trim_spaces(barisJawaban);

        /* Baris pemisah (boleh tidak ada di akhir file) */
        long pos = ftell(fp);
        char pemisah[MAKS_TEKS];
        if (fgets(pemisah, sizeof(pemisah), fp)) {
            /* jika pemisah bukan baris kosong, kembalikan posisi supaya tidak “memakan” soal berikutnya */
            char cek[MAKS_TEKS];
            strncpy(cek, pemisah, sizeof(cek) - 1);
            cek[sizeof(cek) - 1] = '\0';
            trim_newline(cek);
            trim_spaces(cek);

            if (strlen(cek) != 0) {
                /* bukan pemisah kosong -> balik */
                fseek(fp, pos, SEEK_SET);
            }
        }

        Soal s;
        memset(&s, 0, sizeof(s));

        /* Dunia ditentukan dari urutan: 5 soal per dunia */
        s.dunia = jumlah / SOAL_PER_DUNIA;
        if (s.dunia >= JUMLAH_DUNIA) {
            /* kalau file lebih dari 50 soal, sisanya diabaikan agar sesuai 10 dunia */
            break;
        }

        strncpy(s.pertanyaan, barisPertanyaan, MAKS_TEKS - 1);
        s.pertanyaan[MAKS_TEKS - 1] = '\0';

        /* parse opsi */
        int idx = 0;
        char opsiCopy[4 * MAKS_OPSI];
        strncpy(opsiCopy, barisOpsi, sizeof(opsiCopy) - 1);
        opsiCopy[sizeof(opsiCopy) - 1] = '\0';

        char *token = strtok(opsiCopy, ";");
        while (token && idx < 4) {
            trim_spaces(token);
            strncpy(s.opsi[idx], token, MAKS_OPSI - 1);
            s.opsi[idx][MAKS_OPSI - 1] = '\0';
            idx++;
            token = strtok(NULL, ";");
        }
        while (idx < 4) {
            strcpy(s.opsi[idx], "-");
            idx++;
        }

        /* jawaban benar: ambil huruf pertama non-spasi */
        char jb = 'A';
        for (size_t k = 0; k < strlen(barisJawaban); k++) {
            if (!isspace((unsigned char)barisJawaban[k])) {
                jb = (char)toupper((unsigned char)barisJawaban[k]);
                break;
            }
        }
        if (jb < 'A' || jb > 'D') jb = 'A';
        s.jawabanBenar = jb;

        tambahBelakang(kepala, ekor, &s);
        jumlah++;
    }

    fclose(fp);
    return jumlah;
}

/* Ambil 5 soal untuk 1 dunia dari linked list -> simpan pointer node ke array */
static int kumpulkanSoalDunia(NodeSoal *kepala, int dunia, NodeSoal *hasil[SOAL_PER_DUNIA]) {
    int count = 0;
    NodeSoal *cur = kepala;
    while (cur) {
        if (cur->data.dunia == dunia) {
            if (count < SOAL_PER_DUNIA) {
                hasil[count] = cur;
                count++;
            }
        }
        cur = cur->next;
    }
    return count;
}

/* ========================= RANDOM ========================= */
static void acakIndeks(int indeks[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indeks[i];
        indeks[i] = indeks[j];
        indeks[j] = tmp;
    }
}

/* ========================= NILAI & PREDIKAT ========================= */
static const char* predikatDariNilai(int nilai) {
    /* Tanpa A+ dan tanpa C- */
    if (nilai >= 85) return "A";
    if (nilai >= 80) return "A-";
    if (nilai >= 75) return "B+";
    if (nilai >= 70) return "B";
    if (nilai >= 65) return "B-";
    if (nilai >= 60) return "C+";
    if (nilai >= 55) return "C";
    if (nilai >= 40) return "D";
    return "E";
}

/* ========================= PANDUAN & TENTANG ========================= */

static void tampilkanPanduanDunia(void) {
    tampilkanJudul();
    printf("PANDUAN DUNIA (BAB)\n");
    garis();
    printf("CENTURE dibagi menjadi 10 dunia. Setiap dunia mewakili materi pemrograman C semester 1.\n");
    printf("Pada permainan, setiap dunia menampilkan %d soal acak dari total %d soal di dunia tersebut.\n\n",
           SOAL_DIUJIKAN, SOAL_PER_DUNIA);

    for (int i = 0; i < JUMLAH_DUNIA; i++) {
        printf("DUNIA %d : %s\n", i + 1, NAMA_DUNIA[i]);
        printf("%s\n", DESKRIPSI_DUNIA[i]);

        printf("Fokus materi: ");
        switch (i) {
            case 0:  printf("Pengantar C (struktur program, main, konsep awal)\n"); break;
            case 1:  printf("Pemilihan & Perulangan (if/else, switch, loop)\n"); break;
            case 2:  printf("Character & String (char, ASCII, string)\n"); break;
            case 3:  printf("Function, library dasar, scope\n"); break;
            case 4:  printf("Array 1D/2D dan array sebagai parameter fungsi\n"); break;
            case 5:  printf("Pointer, alamat memori, pass by reference dasar\n"); break;
            case 6:  printf("Rekursi dan base case\n"); break;
            case 7:  printf("Struct, union, enum, bitwise dasar\n"); break;
            case 8:  printf("Struktur data (linked list/stack/queue/tree - pengantar)\n"); break;
            case 9:  printf("File processing (read/write, sequential/random access)\n"); break;
        }
        garis();
        printf("\n");
    }

    printf("Catatan:\n");
    printf("- Bank soal disimpan di file: soal.txt (wajib satu folder dengan program).\n");
    printf("- Format soal: 1 pertanyaan + 1 baris opsi (pisah ;) + 1 baris jawaban + (opsional) baris kosong.\n");
    printf("- Untuk pembagian dunia yang rapi, sediakan minimal %d soal (10 dunia x 5 soal).\n",
           JUMLAH_DUNIA * SOAL_PER_DUNIA);

    tekanEnter();
}

static void tampilkanTentangGame(void) {
    tampilkanJudul();
    printf("TENTANG GAME\n");
    garis();
    printf("CENTURE (SI-ADVENTURE) adalah game kuis petualangan untuk latihan materi pemrograman C semester 1.\n");
    printf("Game ini menggunakan konsep dasar C seperti if, loop, array, struct, file handling, serta dynamic memory\n");
    printf("(malloc/free) melalui penerapan linked list untuk menyimpan bank soal.\n\n");

    printf("ATURAN PERMAINAN:\n");
    printf("1) Total dunia: %d\n", JUMLAH_DUNIA);
    printf("2) Soal per dunia: %d (diambil acak dari %d soal)\n", SOAL_DIUJIKAN, SOAL_PER_DUNIA);
    printf("3) Nyawa awal: %d\n", NYAWA_AWAL);
    printf("4) Jawaban benar: +%d poin\n", POIN_BENAR);
    printf("5) Jawaban salah: -1 nyawa\n");
    printf("6) Game berakhir jika nyawa habis atau semua dunia selesai.\n\n");

    printf("SKOR, NILAI, DAN PREDIKAT:\n");
    int skorMaks = JUMLAH_DUNIA * SOAL_DIUJIKAN * POIN_BENAR; /* 10*3*10=300 */
    printf("- Skor maksimum = %d (=%d dunia x %d soal x %d poin)\n",
           skorMaks, JUMLAH_DUNIA, SOAL_DIUJIKAN, POIN_BENAR);
    printf("- Nilai akhir dihitung dari skor ke skala 0-100.\n");
    printf("- Predikat (tanpa A+ dan tanpa C-):\n");
    printf("  A  : 85-100\n");
    printf("  A- : 80-84\n");
    printf("  B+ : 75-79\n");
    printf("  B  : 70-74\n");
    printf("  B- : 65-69\n");
    printf("  C+ : 60-64\n");
    printf("  C  : 55-59\n");
    printf("  D  : 40-54\n");
    printf("  E  : 0-39\n\n");

    printf("FORMAT FILE soal.txt:\n");
    printf("- Baris 1: Pertanyaan\n");
    printf("- Baris 2: Opsi dipisah ';' => opsi1;opsi2;opsi3;opsi4\n");
    printf("- Baris 3: Jawaban benar (A/B/C/D)\n");
    printf("- Baris 4: Baris kosong (opsional, sebagai pemisah)\n\n");

    printf("SARAN:\n");
    printf("- Susun soal per bab: 5 soal untuk dunia 1, lalu 5 soal untuk dunia 2, dan seterusnya.\n");
    printf("- Ini membantu pembagian dunia tetap konsisten.\n");

    tekanEnter();
}

/* ========================= GAMEPLAY ========================= */

static int tanyaSatuSoal(const Soal *s) {
    printf("%s\n\n", s->pertanyaan);
    printf("A. %s\n", s->opsi[0]);
    printf("B. %s\n", s->opsi[1]);
    printf("C. %s\n", s->opsi[2]);
    printf("D. %s\n", s->opsi[3]);

    char jawab;
    printf("\nJawaban (A/B/C/D): ");
    if (scanf(" %c", &jawab) != 1) {
        bersihkanInput();
        return -1;
    }
    bersihkanInput();
    jawab = (char)toupper((unsigned char)jawab);

    if (jawab == s->jawabanBenar) {
        printf("\nJawaban kamu BENAR.\n");
        printf("Skor bertambah +%d poin.\n", POIN_BENAR);
        return POIN_BENAR;
    } else {
        printf("\nJawaban kamu SALAH.\n");
        printf("Jawaban yang benar adalah: %c\n", s->jawabanBenar);
        printf("Saran: baca lagi konsep inti bab ini, lalu lanjutkan.\n");
        return 0;
    }
}

static void jalankanSatuDunia(NodeSoal *kepala, int dunia, int *nyawa, int *skor) {
    tampilkanJudul();
    printf("DUNIA %d : %s\n", dunia + 1, NAMA_DUNIA[dunia]);
    garis();
    printf("%s\n", DESKRIPSI_DUNIA[dunia]);
    garis();
    printf("Nyawa: %d | Skor: %d\n\n", *nyawa, *skor);

    NodeSoal *kumpulan[SOAL_PER_DUNIA];
    int jumlah = kumpulkanSoalDunia(kepala, dunia, kumpulan);

    if (jumlah < SOAL_PER_DUNIA) {
        printf("Soal untuk dunia ini tidak lengkap.\n");
        printf("Pastikan minimal ada %d soal untuk dunia %d.\n", SOAL_PER_DUNIA, dunia + 1);
        *nyawa = 0;
        return;
    }

    int indeks[SOAL_PER_DUNIA];
    for (int i = 0; i < SOAL_PER_DUNIA; i++) indeks[i] = i;
    acakIndeks(indeks, SOAL_PER_DUNIA);

    animasi_loading("Memulai sesi soal");

    for (int i = 0; i < SOAL_DIUJIKAN && *nyawa > 0; i++) {
        printf("\nSoal %d/%d\n", i + 1, SOAL_DIUJIKAN);
        garis();

        Soal *s = &kumpulan[indeks[i]]->data;
        int hasil = tanyaSatuSoal(s);

        if (hasil == POIN_BENAR) {
            *skor += hasil;
        } else if (hasil == 0) {
            (*nyawa)--;
            printf("Nyawa berkurang. Sisa nyawa: %d\n", *nyawa);
        } else {
            printf("Input tidak terbaca. Nyawa tidak dikurangi, lanjut.\n");
        }

        garis();
    }

    if (*nyawa > 0) {
        printf("\nKamu menyelesaikan dunia ini.\n");
        tekanEnter();
    }
}

static void mulaiPetualangan(NodeSoal *kepala) {
    int nyawa = NYAWA_AWAL;
    int skor = 0;

    tampilkanJudul();
    animasi_loading("Menyiapkan petualangan");

    printf("ATURAN SINGKAT:\n");
    printf("- %d dunia, tiap dunia %d soal acak.\n", JUMLAH_DUNIA, SOAL_DIUJIKAN);
    printf("- Nyawa awal: %d\n", NYAWA_AWAL);
    printf("- Benar: +%d poin | Salah: -1 nyawa\n\n", POIN_BENAR);
    tekanEnter();

    for (int d = 0; d < JUMLAH_DUNIA && nyawa > 0; d++) {
        jalankanSatuDunia(kepala, d, &nyawa, &skor);
    }

    tampilkanJudul();
    printf("HASIL AKHIR\n");
    garis();
    printf("Skor total : %d\n", skor);
    printf("Nyawa      : %d\n", nyawa);

    animasi_loading("Menghitung nilai");

    int skorMaks = JUMLAH_DUNIA * SOAL_DIUJIKAN * POIN_BENAR; /* 300 */
    int nilai = (skor * 100) / skorMaks;
    const char *predikat = predikatDariNilai(nilai);

    printf("Nilai akhir : %d\n", nilai);
    printf("Predikat    : %s\n", predikat);

    if (nyawa <= 0) {
        printf("\nStatus      : GAME OVER (nyawa habis)\n");
    } else {
        printf("\nStatus      : SELESAI (petualangan tamat)\n");
    }

    garis();
    tekanEnter();
}

/* ========================= MENU ========================= */

static int menuUtama(void) {
    tampilkanJudul();
    printf("Selamat datang, Petualang.\n\n");
    tampilkanPanelMenu();

    int pilihan;
    printf("\nInput pilihan (1-4): ");
    if (scanf("%d", &pilihan) != 1) {
        bersihkanInput();
        return 0;
    }
    bersihkanInput();
    return pilihan;
}

/* ========================= MAIN ========================= */

int main(void) {
    srand((unsigned int)time(NULL));

    NodeSoal *kepala = NULL;
    NodeSoal *ekor = NULL;

    int jumlahSoal = bacaSoalKeLinkedList("soal.txt", &kepala, &ekor);

    if (jumlahSoal <= 0) {
        printf("File soal.txt tidak ditemukan / format salah.\n");
        printf("Pastikan soal.txt ada di folder yang sama.\n");
        return 0;
    }

    int minimal = JUMLAH_DUNIA * SOAL_PER_DUNIA;
    if (jumlahSoal < minimal) {
        printf("Jumlah soal kurang.\n");
        printf("Minimal %d soal (10 dunia x 5 soal). Sekarang terbaca: %d\n", minimal, jumlahSoal);
        bebasList(&kepala);
        return 0;
    }

    while (1) {
        int pilihan = menuUtama();

        if (pilihan == 1) {
            mulaiPetualangan(kepala);
        } else if (pilihan == 2) {
            tampilkanPanduanDunia();
        } else if (pilihan == 3) {
            tampilkanTentangGame();
        } else if (pilihan == 4) {
            break;
        } else {
            printf("\nPilihan tidak valid.\n");
            tunda_ms(500);
        }
    }

    tampilkanJudul();
    printf("Terima kasih telah bermain CENTURE (SI-ADVENTURE).\n");

    bebasList(&kepala);
    return 0;
}
