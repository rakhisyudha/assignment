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

/**
 * Function to read from databuku.txt file.
 * This function reads the contents of the databuku.txt file and stores the data into the books array.
 */
void loadDataBuku() { 
    FILE *file = fopen(database, "r"); // command to read from the .txt
    if (file == NULL) {
        printf("File not found.\n"); // error message if the user wanted to call from non-existent file
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

/**
 * Function to write to databuku.txt file.
 * This function writes the contents of the books array back to the databuku.txt file when exiting.
 */
void saveAndExit() {
    FILE *file = fopen(database, "w"); // "w" mode will truncate the file to zero length
    if (file == NULL) {
        printf("Error, file cannot be opened.\n");
        return;
    }

    // Save data only if there are valid books
    if (bookCount > 0) {
        for (int i = 0; i < bookCount; i++) {
            // Check if all fields are filled
            if (strlen(books[i].bookCode) == 0 || 
                strlen(books[i].bookTitle) == 0 || 
                strlen(books[i].bookGenre) == 0 || 
                books[i].bookPrice <= 0) {
                printf("Error: Incomplete book data or invalid price. Book with code %s not saved.\n", books[i].bookCode);
                continue; // Skip invalid books
            }

            fprintf(file, "%s|%s|%s|%d\n", // fprintf to write to the file
            books[i].bookCode,
            books[i].bookTitle,
            books[i].bookGenre,
            books[i].bookPrice);
        }
    } else {
        printf("No books to save.\n");
    }
    
    fclose(file);
}

/**
 * Function to display transaction history.
 * This function displays the transaction history stored in the history array.
 */
void viewHistory() {
    printf("Transaction history:\n");
    for (int i = 0; i < historyCount; i++) {
        printf("%d. %-10s %-10s %-10d\n", i + 1,
        history[i].historyTitle,
        history[i].historyGenre,
        history[i].historyPrice);
    }
    printf("---------\n"); // Separator after displaying transaction history
}

/**
 * Function to display all books.
 * This function displays all the books currently in the books array.
 */
void viewBooks() {
    printf("List of all books:\n");
    for (int i = 0; i < bookCount; i++) {
        printf("%d. %-10s %-10s %-10s %-10d\n", i + 1,
        books[i].bookCode,
        books[i].bookTitle,
        books[i].bookGenre,
        books[i].bookPrice);
    }
    printf("---------\n"); // Separator after displaying the list of books
}

/**
 * Function to add a new book to the list.
 * This function allows the user to input details for a new book and adds it to the books array.
 */
void addBook() {
    if (bookCount >= maxBooks) {
        printf("Book list is full, cannot add more books.\n");
        return;
    }
    
    printf("Enter book code: ");
    scanf(" %[^\n]", books[bookCount].bookCode);
    printf("Enter book title: ");
    scanf(" %[^\n]", books[bookCount].bookTitle);
    printf("Enter book genre: ");
    scanf(" %[^\n]", books[bookCount].bookGenre);
    printf("Enter book price: ");
    scanf("%d", &books[bookCount].bookPrice);
    
    // Check if any field is empty or price is invalid
    if (strlen(books[bookCount].bookCode) == 0 || 
        strlen(books[bookCount].bookTitle) == 0 || 
        strlen(books[bookCount].bookGenre) == 0 || 
        books[bookCount].bookPrice <= 0) {
        printf("Error: All fields must be filled and price must be greater than 0.\n");
        return; // Cancel book addition
    }

    bookCount++;
    printf("Current number of books: %d\n", bookCount);
    printf("Book successfully added.\n");
    printf("---------\n");
}

/**
 * Function to process a transaction.
 * This function processes a transaction by removing a selected book from the books array 
 * and adding it to the history array.
 */
void processTransaction() {
    if (bookCount == 0) {
        printf("No books in the database for transaction.\n");
        return;
    }
    viewBooks();
    int number;
    do {
        printf("Select book number to perform transaction: ");
        scanf("%d", &number);
    } while (number < 1 || number > bookCount);
    number--; // Adjust for zero-based index

    // Add the selected book to history
    strcpy(history[historyCount].historyCode, books[number].bookCode);
    strcpy(history[historyCount].historyTitle, books[number].bookTitle);
    strcpy(history[historyCount].historyGenre, books[number].bookGenre);
    history[historyCount].historyPrice = books[number].bookPrice;
    historyCount++;

    // Remove the selected book from the books list
    for (int i = number; i < bookCount - 1; i++) {
        books[i] = books[i + 1];
    }
    bookCount--;

    printf("Transaction successful. Book moved to transaction history.\n");
    printf("---------\n"); // Separator after transaction
}

/**
 * Function to delete transaction history.
 * This function allows the user to delete a specific entry from the transaction history.
 */
void deleteHistory() {
    if (historyCount == 0) {
        printf("No transaction history to delete.\n");
        return;
    }
    viewHistory();
    int number;
    do {
        printf("Enter the number to be deleted: ");
        scanf("%d", &number);
    } while (number < 1 || number > historyCount);
    number--;

    for (int i = number; i < historyCount - 1; i++) {
        history[i] = history[i + 1];
    }
    historyCount--;
    printf("Data successfully deleted from transaction history.\n");
    printf("---------\n"); // Separator after deleting history
}

/**
 * Main function to run the program.
 * The main function runs the bookstore application, displaying a menu 
 * and allowing users to perform various operations until they choose to exit.
 */
int main() {
    loadDataBuku();
    int menu; 

    do {
        printf("Bookstore Alpro 2\n");
        printf("1. Add Book\n");
        printf("2. Process Book Transaction\n");
        printf("3. View History\n");
        printf("4. View Books\n");
        printf("5. Delete History\n");
        printf("6. Delete Book\n");
        printf("7. Exit\n");
        printf("Choice: ");
        scanf("%d", &menu);

        switch (menu) {
        case 1: addBook(); break;
        case 2: processTransaction(); break;
        case 3: viewHistory(); break;
        case 4: viewBooks(); break;
        case 5: deleteHistory(); break;
        case 6: 
            if (bookCount == 0) {
                printf("No books to delete.\n");
            } else {
                printf("Enter the number of the book to delete: ");
                int number;
                do {
                    scanf("%d", &number);
                } while (number < 1 || number > bookCount);
                for (int i = number - 1; i < bookCount - 1; i++) {
                    books[i] = books[i + 1];
                }
                bookCount--;
                printf("Book successfully deleted.\n");
                printf("---------\n"); // Separator after deleting book
            }
            break;
        case 7: 
            saveAndExit();
            printf("Data successfully saved.\n");
            printf("---------\n"); // Separator after saving and exiting
            break;
        default: 
            printf("Invalid menu selection.\n");
            printf("---------\n"); // Separator after invalid selection
            break;
        }
    } while (menu != 7); // Repeat the menu until user selects to exit
    return 0;
}
