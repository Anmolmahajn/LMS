#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int quantity;

    Book() : id(0), title(""), author(""), quantity(0) {}

    void inputBookData() {
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Quantity: ";
        cin >> quantity;
    }

    void displayBookData() const {
        cout << left << setw(10) << id
             << left << setw(30) << title
             << left << setw(30) << author
             << left << setw(10) << quantity << endl;
    }
};

class User {
public:
    int id;
    string name;
    string role;  // Can be "Student" or "Staff"

    User() : id(0), name(""), role("") {}

    void inputUserData() {
        cout << "Enter User ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter User Name: ";
        getline(cin, name);
        cout << "Enter Role (Student/Staff): ";
        getline(cin, role);
    }

    void displayUserData() const {
        cout << left << setw(10) << id
             << left << setw(30) << name
             << left << setw(20) << role << endl;
    }
};

class IssueRecord {
public:
    int userId;
    int bookId;
    time_t issueDate;
    bool returned;

    IssueRecord() : userId(0), bookId(0), issueDate(time(0)), returned(false) {}

    void displayIssueRecord() const {
        tm* lt = localtime(&issueDate);
        cout << left << setw(10) << userId
             << left << setw(10) << bookId
             << left << setw(20) << put_time(lt, "%Y-%m-%d %H:%M:%S")
             << left << setw(10) << (returned ? " Yes" : " No") << endl;
    }
};

class LibraryManagementSystem {
private:
    vector<Book> books;
    vector<User> users;
    vector<IssueRecord> issuedBooks;

    void loadBooks() {
        ifstream file("books.dat", ios::binary);
        Book book;
        while (file.read(reinterpret_cast<char*>(&book), sizeof(Book))) {
            books.push_back(book);
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.dat", ios::binary);
        for (const auto& book : books) {
            file.write(reinterpret_cast<const char*>(&book), sizeof(Book));
        }
        file.close();
    }

    void loadUsers() {
        ifstream file("users.dat", ios::binary);
        User user;
        while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
            users.push_back(user);
        }
        file.close();
    }

    void saveUsers() {
        ofstream file("users.dat", ios::binary);
        for (const auto& user : users) {
            file.write(reinterpret_cast<const char*>(&user), sizeof(User));
        }
        file.close();
    }

    void loadIssuedBooks() {
        ifstream file("issuedBooks.dat", ios::binary);
        IssueRecord record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(IssueRecord))) {
            issuedBooks.push_back(record);
        }
        file.close();
    }

    void saveIssuedBooks() {
        ofstream file("issuedBooks.dat", ios::binary);
        for (const auto& record : issuedBooks) {
            file.write(reinterpret_cast<const char*>(&record), sizeof(IssueRecord));
        }
        file.close();
    }

public:
    LibraryManagementSystem() {
        loadBooks();
        loadUsers();
        loadIssuedBooks();
    }

    ~LibraryManagementSystem() {
        saveBooks();
        saveUsers();
        saveIssuedBooks();
    }

    void addBook() {
        Book book;
        book.inputBookData();
        books.push_back(book);
        saveBooks();
        cout << "Book added successfully.\n";
    }

    void displayBooks() const {
        cout << left << setw(10) << "ID"
             << left << setw(30) << "Title"
             << left << setw(30) << "Author"
             << left << setw(10) << "Quantity" << endl;
        for (const auto& book : books) {
            book.displayBookData();
        }
    }

    void searchBook(int id) const {
        for (const auto& book : books) {
            if (book.id == id) {
                book.displayBookData();
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void searchBookByTitle(const string& title) const {
        bool found = false;
        for (const auto& book : books) {
            if (book.title.find(title) != string::npos) {
                book.displayBookData();
                found = true;
            }
        }
        if (!found) {
            cout << "No book found with title containing: " << title << endl;
        }
    }

    void searchBookByAuthor(const string& author) const {
        bool found = false;
        for (const auto& book : books) {
            if (book.author.find(author) != string::npos) {
                book.displayBookData();
                found = true;
            }
        }
        if (!found) {
            cout << "No book found with author name containing: " << author << endl;
        }
    }

    void updateBook(int id) {
        for (auto& book : books) {
            if (book.id == id) {
                cout << "Current details:\n";
                book.displayBookData();
                cout << "Enter new details:\n";
                book.inputBookData();
                saveBooks();
                cout << "Book updated successfully.\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void deleteBook(int id) {
        auto it = remove_if(books.begin(), books.end(), [id](const Book& book) {
            return book.id == id;
        });
        if (it != books.end()) {
            books.erase(it, books.end());
            saveBooks();
            cout << "Book deleted successfully.\n";
        } else {
            cout << "Book not found.\n";
        }
    }

    void addUser() {
        User user;
        user.inputUserData();
        users.push_back(user);
        saveUsers();
        cout << "User added successfully.\n";
    }

    void displayUsers() const {
        cout << left << setw(10) << "ID"
             << left << setw(30) << "Name"
             << left << setw(20) << "Role" << endl;
        for (const auto& user : users) {
            user.displayUserData();
        }
    }

    void searchUser(int id) const {
        for (const auto& user : users) {
            if (user.id == id) {
                user.displayUserData();
                return;
            }
        }
        cout << "User not found.\n";
    }

    void updateUser(int id) {
        for (auto& user : users) {
            if (user.id == id) {
                cout << "Current details:\n";
                user.displayUserData();
                cout << "Enter new details:\n";
                user.inputUserData();
                saveUsers();
                cout << "User updated successfully.\n";
                return;
            }
        }
        cout << "User not found.\n";
    }

    void deleteUser(int id) {
        auto it = remove_if(users.begin(), users.end(), [id](const User& user) {
            return user.id == id;
        });
        if (it != users.end()) {
            users.erase(it, users.end());
            saveUsers();
            cout << "User deleted successfully.\n";
        } else {
            cout << "User not found.\n";
        }
    }

    void issueBook(int userId, int bookId) {
        auto userIt = find_if(users.begin(), users.end(), [userId](const User& user) {
            return user.id == userId;
        });
        auto bookIt = find_if(books.begin(), books.end(), [bookId](const Book& book) {
            return book.id == bookId;
        });

        if (userIt != users.end() && bookIt != books.end()) {
            if (bookIt->quantity > 0) {
                IssueRecord record;
                record.userId = userId;
                record.bookId = bookId;
                record.issueDate = time(0);
                record.returned = false;

                issuedBooks.push_back(record);
                bookIt->quantity--;

                saveBooks();
                saveIssuedBooks();

                cout << "Book issued successfully.\n";
            } else {
                cout << "Sorry, the book is currently out of stock.\n";
            }
        } else {
            cout << "User or Book not found.\n";
        }
    }

    void returnBook(int userId, int bookId) {
        for (auto& record : issuedBooks) {
            if (record.userId == userId && record.bookId == bookId && !record.returned) {
                record.returned = true;
                auto bookIt = find_if(books.begin(), books.end(), [bookId](const Book& book) {
                    return book.id == bookId;
                });
                if (bookIt != books.end()) {
                    bookIt->quantity++;
                    saveBooks();
                    saveIssuedBooks();
                    cout << "Book returned successfully.\n";
                    return;
                }
            }
        }
        cout << "Record not found or book already returned.\n";
    }

    void displayIssuedBooks() const {
        cout << left << setw(10) << "UserID"
             << left << setw(10) << "BookID"
             << left << setw(20) << "IssueDate"
             << left << setw(10) << "Returned" << endl;
        for (const auto& record : issuedBooks) {
            record.displayIssueRecord();
        }
    }
};


int main() {
    LibraryManagementSystem lms;
    int choice, id, userId, bookId;
    string title, author;

    while (true) {
        cout << "\nLibrary Management System\n";
        cout << "1. Add Book\n2. Display Books\n3. Search Book by ID\n";
        cout << "4. Search Book by Title\n5. Search Book by Author\n";
        cout << "6. Update Book\n7. Delete Book\n";
        cout << "8. Add User\n9. Display Users\n10. Search User by ID\n";
        cout << "11. Update User\n12. Delete User\n";
        cout << "13. Issue Book\n14. Return Book\n15. Display Issued Books\n";
        cout << "0. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            lms.addBook();
            break;
        case 2:
            lms.displayBooks();
            break;
        case 3:
            cout << "Enter Book ID to search: ";
            cin >> id;
            lms.searchBook(id);
            break;
        case 4:
            cin.ignore();
            cout << "Enter Book Title to search: ";
            getline(cin, title);
            lms.searchBookByTitle(title);
            break;
        case 5:
            cin.ignore();
            cout << "Enter Author to search: ";
            getline(cin, author);
            lms.searchBookByAuthor(author);
            break;
        case 6:
            cout << "Enter Book ID to update: ";
            cin >> id;
            lms.updateBook(id);
            break;
        case 7:
            cout << "Enter Book ID to delete: ";
            cin >> id;
            lms.deleteBook(id);
            break;
        case 8:
            lms.addUser();
            break;
        case 9:
            lms.displayUsers();
            break;
        case 10:
            cout << "Enter User ID to search: ";
            cin >> id;
            lms.searchUser(id);
            break;
        case 11:
            cout << "Enter User ID to update: ";
            cin >> id;
            lms.updateUser(id);
            break;
        case 12:
            cout << "Enter User ID to delete: ";
            cin >> id;
            lms.deleteUser(id);
            break;
        case 13:
            cout << "Enter User ID: ";
            cin >> userId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            lms.issueBook(userId, bookId);
            break;
        case 14:
            cout << "Enter User ID: ";
            cin >> userId;
            cout << "Enter Book ID: ";
            cin >> bookId;
            lms.returnBook(userId, bookId);
            break;
        case 15:
            lms.displayIssuedBooks();
            break;
        case 0:
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}