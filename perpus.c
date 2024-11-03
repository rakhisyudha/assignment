/* Anggota Kelompok Alpro 2
1. Achmad Rakhis Yudha 2802521371
2. Iqbal Kevin Kyle 2802523622
3. Marcello Aqeel Pane 2802522166
4. Shevanya Nagita Tesalonika Winowoda 2802522664
5. Zahra Illiyin 2802519745
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxBooks 100
#define maxHistory 100
#define maxString 100
#define maxCode 100
#define database "databuku.txt"

typedef struct {
    char bookCode[maxCode]; //size is defined previously
    char bookTitle[maxString];
    char bookGenre[maxString];
    int bookPrice;
} bookStruct; 

typedef struct {
    char historyCode[maxCode];
    char historyTitle[maxString];
    char historyGenre[maxString];
    int historyPrice;
} historyStruct;

bookStruct books[maxBooks];
historyStruct history[maxHistory];
int bookCount = 0; //counting variables to keep track of entries in each array
int historyCount = 0;

void loadDataBuku() { //Fungsi membaca file databuku.txt
    FILE *file = fopen(database, "r"); //command to read from the .txt
    if (file == NULL) {
        printf("File tidak ditemukan.\n"); //error message if the user wanted to call from non-existent file
        return;
    }
    while (!feof(file) && bookCount < maxBooks) { //scan until the end of the file
        fscanf(file, "%[^|]|%[^|]|%[^|]|%d\n", //each data is separated by | and assigned to respective variables
        books[bookCount].bookCode, 
        books[bookCount].bookTitle,
        books[bookCount].bookGenre,
        &books[bookCount].bookPrice);
        bookCount++; //counter for each entities in the books array
    }
    fclose(file);
}

void saveAndExit() { //Fungsi menulis di file databuku.txt
    FILE *file = fopen(database, "w");
    if (file == NULL) {
        printf("Error, file tidak dapat dibuka.\n");
        return;
    }
    for (int i = 0; i < bookCount; i++) { //counter made to be dynamic as the program can delete and add entry
        fprintf(file, "%s|%s|%s|%d\n", //fprintf to write to the file
        books[i].bookCode,
        books[i].bookTitle,
        books[i].bookGenre,
        books[i].bookPrice);
    }
    fclose(file);
}

void viewHistory() {
    printf("Riwayat transaksi:\n");
    for (int i = 0; i < historyCount; i++) {
        printf("%d. %-10s %-10s %-10d\n", i + 1,
        history[i].historyTitle,
        history[i].historyGenre,
        history[i].historyPrice);
    }
}

void viewBooks() {
    printf("Daftar seluruh buku:\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%d. %-10s %-10s %-10s %-10d\n", i + 1,
        books[i].bookCode,
        books[i].bookTitle,
        books[i].bookGenre,
        books[i].bookPrice);
    }
}

// Fungsi untuk menambah buku baru ke daftar buku
void addBook() {
    if (bookCount >= maxBooks) {
        printf("Buku penuh, tidak bisa menambah buku lagi.\n");
        return;
    }
    printf("Masukkan kode buku: ");
    scanf(" %[^\n]", books[bookCount].bookCode);
    printf("Masukkan judul buku: ");
    scanf(" %[^\n]", books[bookCount].bookTitle);
    printf("Masukkan genre buku: ");
    scanf(" %[^\n]", books[bookCount].bookGenre);
    printf("Masukkan harga buku: ");
    scanf("%d", &books[bookCount].bookPrice);
    bookCount++;
    printf("Buku berhasil ditambahkan.\n");
}

// Fungsi untuk melakukan transaksi (menghapus buku dari daftar dan menambahkannya ke history)
void processTransaction() {
    if (bookCount == 0) {
        printf("Tidak ada buku di database untuk transaksi.\n");
        return;
    }
    viewBooks();
    int nomor;
    do {
        printf("Pilih nomor buku untuk melakukan transaksi: ");
        scanf("%d", &nomor);
    } while (nomor < 1 || nomor > bookCount);
    nomor--; // Sesuaikan dengan indeks array

    // Tambahkan buku yang dipilih ke history
    strcpy(history[historyCount].historyCode, books[nomor].bookCode);
    strcpy(history[historyCount].historyTitle, books[nomor].bookTitle);
    strcpy(history[historyCount].historyGenre, books[nomor].bookGenre);
    history[historyCount].historyPrice = books[nomor].bookPrice;
    historyCount++;

    // Hapus buku yang dipilih dari daftar buku
    for (int i = nomor; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }
    bookCount--;

    printf("Transaksi berhasil. Buku dipindahkan ke riwayat transaksi.\n");
}

void deleteHistory() {
    if (historyCount == 0) {
        printf("Tidak ada riwayat transaksi yang dapat dihapus.\n");
        return;
    }
    viewHistory();
    int nomor;
    do {
        printf("Masukkan nomor yang akan dihapus: ");
        scanf("%d", &nomor);
    } while (nomor < 1 || nomor > historyCount);
    nomor--;

    for (int i = nomor; i < historyCount - 1; i++) {
        history[i] = history[i + 1];
    }
    historyCount--;
    printf("Data berhasil dihapus dari riwayat transaksi.\n");
}

int main() {
    loadDataBuku();
    int menu; 

    do {
        printf("Toko Buku Alpro 2\n");
        printf("1. Input Buku\n");
        printf("2. Proses Transaksi Buku\n");
        printf("3. View History\n");
        printf("4. View Buku\n");
        printf("5. Delete History\n");
        printf("6. Delete Buku\n");
        printf("7. Exit\n");
        printf("Pilihan: ");
        scanf("%d", &menu);

        switch (menu) {
        case 1: addBook(); break;
        case 2: processTransaction(); break;
        case 3: viewHistory(); break;
        case 4: viewBooks(); break;
        case 5: deleteHistory(); break;
        case 6: 
            if (bookCount == 0) {
                printf("Tidak ada buku yang bisa dihapus.\n");
            } else {
                printf("Masukkan nomor buku yang akan dihapus: ");
                int nomor;
                do {
                    scanf("%d", &nomor);
                } while (nomor < 1 || nomor > bookCount);
                for (int i = nomor - 1; i < bookCount - 1; i++) {
                    books[i] = books[i + 1];
                }
                bookCount--;
                printf("Buku berhasil dihapus.\n");
            }
            break;
        case 7: saveAndExit();
            printf("Data berhasil disimpan.\n");
            break;
        default:
            printf("Pilihan tidak valid\n");
            break;
        }
    } while (menu != 7);

    return 0;
}
