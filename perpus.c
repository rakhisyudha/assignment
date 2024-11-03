#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxBooks 100
#define maxHistory 100
#define maxString 100
#define maxCode 100
#define database "databuku.txt"

typedef struct {
    char bookCode[maxCode]; // size is defined previously
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
int bookCount = 0; // counting variables to keep track of entries in each array
int historyCount = 0;

//  Fungsi membaca file databuku.txt
//  This function reads the contents of the databuku.txt file and stores the data into the books array.
void loadDataBuku() { 
    FILE *file = fopen(database, "r"); // command to read from the .txt
    if (file == NULL) {
        printf("File tidak ditemukan.\n"); // error message if the user wanted to call from non-existent file
        return;
    }
    while (!feof(file) && bookCount < maxBooks) { // scan until the end of the file
        fscanf(file, "%[^|]|%[^|]|%[^|]|%d\n", // each data is separated by | and assigned to respective variables
        books[bookCount].bookCode, 
        books[bookCount].bookTitle,
        books[bookCount].bookGenre,
        &books[bookCount].bookPrice);
        bookCount++; // counter for each entity in the books array
    }
    fclose(file);
}

//  Fungsi menulis di file databuku.txt
//  This function writes the contents of the books array back to the databuku.txt file when exiting.
void saveAndExit() { // Fungsi menulis di file databuku.txt
    FILE *file = fopen(database, "w"); // "w" mode will truncate the file to zero length
    if (file == NULL) {
        printf("Error, file tidak dapat dibuka.\n");
        return;
    }

    // Menyimpan data hanya jika ada buku yang valid
    if (bookCount > 0) {
        for (int i = 0; i < bookCount; i++) {
            // Periksa apakah semua field terisi
            if (strlen(books[i].bookCode) == 0 || 
                strlen(books[i].bookTitle) == 0 || 
                strlen(books[i].bookGenre) == 0 || 
                books[i].bookPrice <= 0) {
                printf("Error: Data buku tidak lengkap atau harga tidak valid. Buku dengan kode %s tidak disimpan.\n", books[i].bookCode);
                continue; // Lewati buku yang tidak valid
            }

            fprintf(file, "%s|%s|%s|%d\n", // fprintf to write to the file
            books[i].bookCode,
            books[i].bookTitle,
            books[i].bookGenre,
            books[i].bookPrice);
        }
    } else {
        printf("Tidak ada buku untuk disimpan.\n");
    }
    
    fclose(file);
}

//  Fungsi untuk menampilkan riwayat transaksi
//  This function displays the transaction history stored in the history array.
void viewHistory() {
    printf("Riwayat transaksi:\n");
    for (int i = 0; i < historyCount; i++) {
        printf("%d. %-10s %-10s %-10d\n", i + 1,
        history[i].historyTitle,
        history[i].historyGenre,
        history[i].historyPrice);
    }
    printf("---------\n"); // Pemisah setelah menampilkan riwayat transaksi
}

//  Fungsi untuk menampilkan daftar seluruh buku
//  This function displays all the books currently in the books array.
void viewBooks() {
    printf("Daftar seluruh buku:\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%d. %-10s %-10s %-10s %-10d\n", i + 1,
        books[i].bookCode,
        books[i].bookTitle,
        books[i].bookGenre,
        books[i].bookPrice);
    }
    printf("---------\n"); // Pemisah setelah menampilkan daftar buku
}

//  Fungsi untuk menambah buku baru ke daftar buku
//  This function allows the user to input details for a new book and adds it to the books array.
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
    
    // Memeriksa apakah ada bidang yang kosong atau harga tidak valid
    if (strlen(books[bookCount].bookCode) == 0 || 
        strlen(books[bookCount].bookTitle) == 0 || 
        strlen(books[bookCount].bookGenre) == 0 || 
        books[bookCount].bookPrice <= 0) {
        printf("Error: Semua bidang harus terisi dan harga harus lebih dari 0.\n");
        return; // Batalkan penambahan buku
    }

    bookCount++;
    printf("Jumlah buku saat ini: %d\n", bookCount);
    printf("Buku berhasil ditambahkan.\n");
    printf("---------\n");
}

//  Fungsi untuk melakukan transaksi
//  This function processes a transaction by removing a selected book from the books array 
// and adding it to the history array.
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
    nomor--; // Adjust for zero-based index

    // Add the selected book to history
    strcpy(history[historyCount].historyCode, books[nomor].bookCode);
    strcpy(history[historyCount].historyTitle, books[nomor].bookTitle);
    strcpy(history[historyCount].historyGenre, books[nomor].bookGenre);
    history[historyCount].historyPrice = books[nomor].bookPrice;
    historyCount++;

    // Remove the selected book from the books list
    for (int i = nomor; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }
    bookCount--;

    printf("Transaksi berhasil. Buku dipindahkan ke riwayat transaksi.\n");
    printf("---------\n"); // Pemisah setelah transaksi
}

//  Fungsi untuk menghapus riwayat transaksi
//  This function allows the user to delete a specific entry from the transaction history.
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
    printf("---------\n"); // Pemisah setelah menghapus riwayat
}

/*  Fungsi utama untuk menjalankan program
The main function runs the bookstore application, displaying a menu 
and allowing users to perform various operations until they choose to exit. */
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
                printf("---------\n"); // Pemisah setelah menghapus buku
            }
            break;
        case 7: 
            saveAndExit();
            printf("Data berhasil disimpan.\n");
            printf("---------\n"); // Pemisah setelah menyimpan dan keluar
            break;
        default:
            printf("Pilihan tidak valid\n");
            printf("---------\n"); // Pemisah setelah pilihan tidak valid
            break;
        }
    } while (menu != 7);

    return 0;
}
