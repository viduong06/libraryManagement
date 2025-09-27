#include <iostream>
#include <ctime>
using namespace std;

class Book {
private:
    string bookName;
    string authorName;
    int numberOfBooks;
    int count = 0;
public:
    tm publicDate;
    // Hàm khởi tạo 0 tham số
    Book() {
        bookName = "";
        authorName = "";
        numberOfBooks = 0;
    }

    // Hàm thành phần nhập dữ liệu cho sách
    void insertBook() {
        count++;
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
        if (publicDate.tm_mon > 12 || publicDate.tm_mon < 1) {
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
                << " | Ngay san xuat: "
                << publicDate.tm_mday << "/"
                << (publicDate.tm_mon + 1) << "/"
                << (publicDate.tm_year + 1900)
                << endl;
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

class doublyLinkedList {
private:
    Node *head;
    // sắp xếp sách theo năm xuất bản ( sử dụng merge sort)
    // Merge Sort cho DLL
    Node* mergeSort(Node* head) {
        if (!head || !head->next)
            return head;  // TH cơ sở: danh sách rỗng hoặc chỉ có 1 node thì đã "sort"

        // sử dụng kỹ thuật fast-slow pointer để chia double linkedlist thành 2 nửa
        Node* slow = head;
        Node* fast = head;
        while (fast && fast->next) {
            fast = fast->next->next;
            if (fast) slow = slow->next;
        }

        Node* second = slow->next;  // nhánh phải
        slow->next = nullptr;       // cắt đôi
        if (second) second->prev = nullptr;

        // 2. Đệ quy cho 2 nửa của danh sách
        Node* left  = mergeSort(head);
        Node* right = mergeSort(second);

        // 3. Merge hai nửa đã sort
        return merge(left, right);
    }
    // Gộp 2 danh sách đã sắp xếp
    Node* merge(Node* first, Node* second) {
        if (!first) return second;
        if (!second) return first;
        if (first->data.publicDate.tm_year > second->data.publicDate.tm_year) {
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
    int count() {
        Node *temp = head;
        int count = 0;
        while (temp->next != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // In danh sách liên kết

    void display() {
        Node *temp = head;
        printCentered("Thu vien", 50);
        while (temp != nullptr) {

            temp->data.display();
            temp = temp->next;
        }
        for (int i = 0; i < 50; i++) {
            cout << "=";
        }
        cout << endl;
    }
    void sortByPublicYear() {
        head = mergeSort(head);
    }
};

int main() {
    int select;
    Book book;
    doublyLinkedList Thuvien;
    do {
        printCentered("Danh sach chuc nang", 50);
        cout << "0: Exit. \n"
                << "1: Insert book. \n"
                << "2: Borrow book. \n"
                << "3: Return book. \n"
                << "4. Searching: \n +)searchByAuthorName. \n +)searchByBookName.\n"
                << "5. Sorting. \n"
                << "6. Display. \n";
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
                book.insertBook();
                Thuvien.insertBookIntoDbLinkedList(book);
                break;
            }
            case 5: {
                Thuvien.sortByPublicYear();
                break;
            }
            case 6: {
                Thuvien.display();
                cout << endl;
                break;
            }
            default:
                return 0;
        }
    } while (select != 0);
}
