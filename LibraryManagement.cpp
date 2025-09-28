#include <iostream>
#include <ctime>
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

// Hàm in tiêu đề
void printCentered(string t, int width) {
    int len = t.length();
    int left = (width - len) / 2;
    int right = width - len - left;
    for (int i = 0; i < left; i++) {
        cout << "=";
    }
    cout << t;
    for (int i = 0; i < right; i++) {
        cout << "=";
    }
    cout << endl;
}

class Book {
private:
    string bookName;
    string authorName;
    unsigned int numberOfBooks;
    tm publicDate;

public:
    // Hàm khởi tạo 0 tham số
    Book() {
        bookName = "";
        authorName = "";
        numberOfBooks = 0;
    }

    // Hàm thành phần nhập dữ liệu cho sách
    void insertBook() {
        cout << "Nhap ten sach:";
        getline(cin, bookName);

        cout << "Nhap ten tac gia:";
        getline(cin, authorName);

        cout << "Nhap ngay thang nam san xuat (dd/mm/yy):";
        cin >> publicDate.tm_mday >> publicDate.tm_mon >> publicDate.tm_year;
        cin.ignore();

        publicDate.tm_mon -= 1;
        publicDate.tm_year -= 1900;
        if (publicDate.tm_mday > 31 || publicDate.tm_mday < 1) {
            cout << "Ngay khong hop le, chuong trinh se dong";
            return;
        }
        if (publicDate.tm_mon > 11 || publicDate.tm_mon < 0) {
            cout << "Thang khong hop le, chuong trinh se dong";
            return;
        }
        cout << "Nhap so luong sach:";
        cin >> numberOfBooks;
    }

    // Hàm hiển thị dữ liệu sách
    void display() {
        cout << "Sach: " << bookName
                << " | Tac gia: " << authorName
                << " | So luong: " << numberOfBooks
                << " | Ngay xuat ban: "
                << publicDate.tm_mday << "/"
                << (publicDate.tm_mon + 1) << "/"
                << (publicDate.tm_year + 1900)
                << endl;
    }

    int getPublicYear() {
        return publicDate.tm_year + 1900;
    }

    string getBookName() {
        return bookName;
    }

    void setBookName(string bookName) {
        this->bookName = bookName;
    }

    void setNumberOfBooks(int numberOfBooks) {
        this->numberOfBooks = numberOfBooks;
    }

    int getNumberOfBooks() {
        return numberOfBooks;
    }
};

struct Node {
    Book data;
    Node *prev;
    Node *next;

    Node(Book value) {
        data = value;
        prev = nullptr;
        next = nullptr;
    }
};

class doublyLinkedList {
private:
    Node *head;
    // sắp xếp sách theo năm xuất bản ( sử dụng merge sort)
    // Merge Sort cho DLL
    Node *mergeSort(Node *head) {
        if (!head || !head->next)
            return head; // danh sách rỗng hoặc chỉ có 1 node thì coi như là đã sort

        // sử dụng kỹ thuật fast-slow pointer để chia double linkedlist thành 2 nửa
        Node *slow = head;
        Node *fast = head;
        while (fast && fast->next) {
            fast = fast->next->next;
            if (fast) slow = slow->next;
        }

        Node *second = slow->next; // nhánh phải
        slow->next = nullptr; // cắt đôi
        if (second) second->prev = nullptr;

        // Đệ quy cho 2 nửa của danh sách
        Node *left = mergeSort(head);
        Node *right = mergeSort(second);

        // Merge hai nửa đã sort
        return merge(left, right);
    }

    // Gộp 2 danh sách đã sắp xếp
    Node *merge(Node *first, Node *second) {
        if (!first) return second;
        if (!second) return first;
        if (first->data.getPublicYear() > second->data.getPublicYear()) {
            first->next = merge(first->next, second);
            if (first->next) first->next->prev = first;
            first->prev = nullptr;
            return first;
        } else {
            second->next = merge(first, second->next);
            if (second->next) second->next->prev = second;
            second->prev = nullptr;
            return second;
        }
    }

public:
    doublyLinkedList() {
        head = nullptr;
    }

    // Hàm thêm sách vào danh sách liên kết
    void insertBookIntoDbLinkedList(Book value) {
        Node *newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            return;
        } else {
            Node *temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->prev = temp;
        }
    }

    void sortByPublicYear() {
        head = mergeSort(head);
    }
};

class User {
    string fullName, nationID;
    vector<string> borrowBookNames;
    int numberOfBorrowedBooks;

public:
    User() {
        fullName = "";
        nationID = "";
    }


    string getNationID() {
        return nationID;
    }

    string getBookName(int index) {
        if (index >= 0 && index < borrowBookNames.size()) {
            return borrowBookNames[index];
        }
        return "";
    }

    int getNumberOfBorrowedBooks() {
        return numberOfBorrowedBooks;
    }

    vector<string> getBorrowBookNames() {
        return borrowBookNames;
    }

    string getFullName() {
        return fullName;
    }
};

class libraryManagement {
    unordered_map<string, Book*> hashBook;
    unordered_map<string, vector<string> > hashBorrowBooks;

public:
    void addBook(Book* b) {
        hashBook[b->getBookName()] = b;
    }

    Book *findBook(string bookName) {
        if (hashBook.count(bookName)) {
            return hashBook[bookName];
        }
        return nullptr;
    }

    // chức năng mượn sách
    void borrowBook(string bookName, string nationID) {
        if (!hashBook.count(bookName)) {
            cout << "Thu vien hien khong co sach nay!\n";
            return;
        }
        if (!hashBook[bookName]->getNumberOfBooks()) {
            cout << "Sach da het!";
            return;
        }
        hashBook[bookName]->setNumberOfBooks(hashBook[bookName]->getNumberOfBooks() - 1);
        hashBorrowBooks[nationID].push_back(bookName);
        cout <<"Muon sach thanh cong!\n";
    }

    // chức năng trả sách
    void returnBook(string bookName, string nationID) {
        // Kiểm tra xem có người dùng này trong danh sách mượn không
        if (!hashBorrowBooks.count(nationID)) {
            cout << "Nguoi dung nay chua muon bat ky sach nao!\n";
            return;
        }
        auto &books = hashBorrowBooks[nationID];
        auto it = find(books.begin(), books.end(), bookName);

        if (it != books.end()) {
            books.erase(it);
            hashBook[bookName]->setNumberOfBooks(hashBook[bookName]->getNumberOfBooks() + 1);
            cout << "Tra sach thanh cong!\n";
        } else {
            cout << "Nguoi nay khong muon sach: " << bookName << endl;
        }
    }
    // in ra danh sách mượn sách
    void displayBorrowBooks() {
        if (hashBorrowBooks.empty()) {
            cout << "Chua co nguoi dung nao muon sach.\n";
            return;
        }

        printCentered("Danh sach muon sach", 50);
        for (auto &pair : hashBorrowBooks) {
            string nationID = pair.first;
            vector<string> &books = pair.second;

            cout << "CCCD: " << nationID << " | Sach da muon: ";
            if (books.empty()) {
                cout << "Chua muon sach nao";
            } else {
                for (int i = 0; i < books.size(); i++) {
                    cout << books[i];
                    if (i != books.size() - 1) cout << ", ";
                }
            }
            cout << endl;
        }
        for (int i = 0; i < 50; i++) cout << "=";
        cout << endl;
    }
    // in ra danh sách sách trong thư viện
    void displayLibraryBooks() {
        printCentered("Danh sach sach trong thu vien", 50);
        for (auto &pair : hashBook) {
            pair.second->display(); // in Book từ hashBook
        }
        for (int i = 0; i < 50; i++) cout << "=";
        cout << endl;
    }
};

int main() {
    int select;
    doublyLinkedList thuVien;
    libraryManagement lib;
    do {
        printCentered("Danh sach chuc nang", 50);
        cout << "0: Exit. \n"
                << "1: Insert book. \n"
                << "2: Borrow book. \n"
                << "3: Display borrow book. \n"
                << "4: Return book. \n"
                << "5. Searching: \n +)searchByAuthorName. \n +)searchByBookName.\n"
                << "6. Sorting. \n"
                << "7. Display. \n";
        for (int i = 0; i < 50; i++) {
            cout << "=";
        }
        cout << endl << "->";
        cin >> select;
        switch (select) {
            case 0: {
                return 0;
            }
            case 1: {
                cin.ignore();
                Book* book = new Book();
                book->insertBook();
                thuVien.insertBookIntoDbLinkedList(*book);
                lib.addBook(book);
                break;
            }
            case 2: {
                cin.ignore();
                cout <<"Nhap CCCD: ";
                string nationID;
                getline(cin, nationID);
                cout <<"Nhap so luong sach can muon: ";
                int numberOfBooks;
                cin >> numberOfBooks;
                cin.ignore();
                for (int i = 0; i < numberOfBooks; i++) {
                    string bookName;
                    cout <<"Nhap ten sach can muon: ";
                    getline(cin, bookName);
                    lib.borrowBook(bookName, nationID);
                }
                break;
            }
            case 3: {
                lib.displayBorrowBooks();
                break;
            }
            case 4: {
                cin.ignore();
                cout <<"Nhap CCCD: ";
                string nationID;
                getline(cin, nationID);
                cout <<"Nhap so luong sach can tra: ";
                int numberOfBooks;
                cin >> numberOfBooks;
                cin.ignore();
                for (int i = 0; i < numberOfBooks; i++) {
                    string bookName;
                    cout <<"Nhap ten sach can tra: ";
                    getline(cin, bookName);
                    lib.returnBook(bookName, nationID);
                }
                break;
            }

            case 6: {
                thuVien.sortByPublicYear();
                break;
            }
            case 7: {
                lib.displayLibraryBooks();
                cout << endl;
                break;
            }
            default:
                return 0;
        }
    } while (select != 0);
}
