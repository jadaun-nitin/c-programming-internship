#include <iostream>
#include <sqlite3.h>
#include <string>
#include <ctime>
#include <limits> // needed for numeric_limits

using namespace std;

sqlite3* db;

void executeSQL(const string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void initDB() {
    if (sqlite3_open("library.db", &db)) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }

    executeSQL("CREATE TABLE IF NOT EXISTS books(id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, author TEXT NOT NULL, isbn TEXT UNIQUE, available INTEGER DEFAULT 1);");
    executeSQL("CREATE TABLE IF NOT EXISTS borrowers(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL);");
    executeSQL("CREATE TABLE IF NOT EXISTS transactions(id INTEGER PRIMARY KEY AUTOINCREMENT, book_id INTEGER, borrower_id INTEGER, borrow_date TEXT, return_date TEXT, FOREIGN KEY(book_id) REFERENCES books(id), FOREIGN KEY(borrower_id) REFERENCES borrowers(id));");
}

void addBook() {
    string title, author, isbn;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter book title: "; getline(cin, title);
    cout << "Enter author: "; getline(cin, author);
    cout << "Enter ISBN: "; getline(cin, isbn);

    string sql = "INSERT INTO books(title, author, isbn) VALUES('" + title + "','" + author + "','" + isbn + "');";
    executeSQL(sql);
    cout << "Book added!\n";
}

void addBorrower() {
    string name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter borrower name: "; getline(cin, name);

    string sql = "INSERT INTO borrowers(name) VALUES('" + name + "');";
    executeSQL(sql);
    cout << "Borrower added!\n";
}

int bookSearchCallback(void* NotUsed, int argc, char** argv, char** azColName){
    for(int i = 0; i < argc; i++)
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
    cout << endl;
    return 0;
}

void searchBooks() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string keyword;
    cout << "Enter title, author, or ISBN to search: ";
    getline(cin, keyword);

    string sql = "SELECT * FROM books WHERE title LIKE '%" + keyword + "%' OR author LIKE '%" + keyword + "%' OR isbn LIKE '%" + keyword + "%';";
    sqlite3_exec(db, sql.c_str(), bookSearchCallback, 0, nullptr);
}

void checkoutBook() {
    int bookId, borrowerId;
    cout << "Enter book ID to checkout: "; cin >> bookId;
    cout << "Enter borrower ID: "; cin >> borrowerId;

    time_t t = time(nullptr);
    string borrowDate = ctime(&t);
    borrowDate.pop_back();

    string sql = "INSERT INTO transactions(book_id, borrower_id, borrow_date) VALUES(" + to_string(bookId) + "," + to_string(borrowerId) + ",'" + borrowDate + "');";
    executeSQL(sql);

    string update = "UPDATE books SET available=0 WHERE id=" + to_string(bookId) + ";";
    executeSQL(update);

    cout << "Book checked out successfully!\n";
}

void returnBook() {
    int transId;
    cout << "Enter transaction ID to return: "; cin >> transId;

    time_t t = time(nullptr);
    string returnDate = ctime(&t);
    returnDate.pop_back();

    string sql = "UPDATE transactions SET return_date='" + returnDate + "' WHERE id=" + to_string(transId) + ";";
    executeSQL(sql);

    string update = "UPDATE books SET available=1 WHERE id=(SELECT book_id FROM transactions WHERE id=" + to_string(transId) + ");";
    executeSQL(update);

    cout << "Book returned successfully!\n";
}

int main() {
    initDB();
    int choice;
    do {
        cout << "\nLibrary System Menu:\n1. Add Book\n2. Add Borrower\n3. Search Books\n4. Checkout Book\n5. Return Book\n6. Exit\nEnter choice: ";
        cin >> choice;

        switch(choice){
            case 1: addBook(); break;
            case 2: addBorrower(); break;
            case 3: searchBooks(); break;
            case 4: checkoutBook(); break;
            case 5: returnBook(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 6);

    sqlite3_close(db);
    return 0;
}
