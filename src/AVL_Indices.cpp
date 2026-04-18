#include <iostream>
#include <string>
#include <map>
using namespace std;

class UserList {
public:
    template <class T>
    friend class index;

private:
    class User {
    public:
        int UserID;
        string Name;
        string Email;
        string Country;
        string Type;
        User* next;
        User* prev;

        User(int id = 0, string nm = " ", string mail = " ", string cntry = " ", string tp = " ",
            User* nxt = nullptr, User* prv = nullptr)
            : UserID(id), Name(nm), Email(mail), Country(cntry), Type(tp), next(nxt), prev(prv) {}
    };

    User* head;
    User* tail;

public:
    UserList() {
        head = new User();
        tail = new User();
        head->next = tail;
        tail->prev = head;
    }

    ~UserList() {
        User* current = head;
        while (current) {
            User* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void addUser(int id, const string& name, const string& email, const string& country, const string& type) {
        User* newUser = new User(id, name, email, country, type);
        User* temp = tail->prev;

        temp->next = newUser;
        newUser->prev = temp;
        newUser->next = tail;
        tail->prev = newUser;
    }

    User* getUserByEmail(const string& email) const {
        User* temp = head->next;
        while (temp != tail) {
            if (temp->Email == email) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    User* getUserByName(const string& name) const {
        User* temp = head->next;
        while (temp != tail) {
            if (temp->Name == name) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void print(User* user) const {
        if (user) {
            cout << "ID: " << user->UserID << endl;
            cout << "Name: " << user->Name << endl;
            cout << "Email: " << user->Email << endl;
            cout << "Country: " << user->Country << endl;
            cout << "Type: " << user->Type << endl;
        }
    }
    User* getUser(int id) {
        User* temp = head->next;
        while (temp != tail) {
            if (temp->UserID == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }
    User* getHead() const {
        return head;
    }
    void removeUser(int id) {
        User* temp = head->next;
        while (temp != tail) {
            if (temp->UserID == id) {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                return;
            }
            temp = temp->next;
        }
    }

};

template <class T>
class index {
private:
    class indexNode {
    public:
        T key;
        UserList::User* userPtr;
        indexNode* left;
        indexNode* right;
        int height;

        indexNode(T k, UserList::User* u) : key(k), userPtr(u), left(nullptr), right(nullptr), height(1) {}
    };

    indexNode* root;
    UserList userList;
    map<string, bool> indicesCreated;
    map<string, indexNode*> countryIndex;
    map<string, indexNode*> typeIndex;

    int height(indexNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(indexNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    indexNode* rightRotate(indexNode* y) {
        indexNode* x = y->left;
        indexNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    indexNode* leftRotate(indexNode* x) {
        indexNode* y = x->right;
        indexNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    indexNode* insert(indexNode* node, T key, UserList::User* user) {
        if (!node) return new indexNode(key, user);

        if (key < node->key)
            node->left = insert(node->left, key, user);
        else if (key > node->key)
            node->right = insert(node->right, key, user);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }



    indexNode* search(indexNode* node, T key) const {
        if (!node || node->key == key) return node;

        if (key < node->key)
            return search(node->left, key);
        return search(node->right, key);
    }

    void printInOrder(indexNode* node) const {
        if (!node) return;
        printInOrder(node->left);
        userList.print(node->userPtr);
        cout << endl;
        printInOrder(node->right);
    }
    indexNode* deleteNode(indexNode* node, T key) {

        if (!node)
            return node;

        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else {
            // Node to be deleted found
            if (!node->left || !node->right) {
                indexNode* temp = nullptr;
                if (node->left) {
                    temp = node->left;
                }
                else {
                    temp = node->right;
                }

                if (!temp) { // No child
                    temp = node;
                    node = nullptr;
                }
                else { // One child 
                    *node = *temp;
                }

                delete temp;
            }
            else {

                indexNode* temp = node->right;
                while (temp->left)
                    temp = temp->left;

                node->key = temp->key;
                node->userPtr = temp->userPtr; // Update user pointer
                node->right = deleteNode(node->right, temp->key);
            }
        }


        if (!node)
            return node;


        node->height = 1 + max(height(node->left), height(node->right));


        int balance = getBalance(node);

        // Step 4: Balance the tree
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

public:
    index() : root(nullptr) {}
    void addUserbyID(int id, const string& name, const string& email, const string& country, const string& type) {
        userList.addUser(id, name, email, country, type);
        UserList::User* user = userList.getUser(id);
        root = insert(root, id, user);
    }

    void addUser(int id, const string& name, const string& email, const string& country, const string& type) {
        userList.addUser(id, name, email, country, type);
    }
    void searchUserbyID(int id) const {
        indexNode* node = search(root, id);
        if (node && node->userPtr) {
            userList.print(node->userPtr);
        }
        else {
            cout << "User with ID " << id << " not found.\n";
        }
    }
    void createIndex(const string& field) {
        if (indicesCreated[field]) {
            cout << "Index on " << field << " already exists.\n";
            return;
        }

        UserList::User* current = userList.getHead()->next;
        while (current != nullptr && current->next != nullptr) {
            if (field == "Name") root = insert(root, current->Name, current);
            if (field == "Email") root = insert(root, current->Email, current);
            current = current->next;
        }
        indicesCreated[field] = true;
        cout << "Index on " << field << " created successfully.\n";
    }
    void deleteUser(T key) {

        root = deleteNode(root, key);


        userList.removeUser(key);
    }

    void deleteIndex(const string& field) {
        if (!indicesCreated[field]) {
            cout << "Index on " << field << " does not exist.\n";
            return;
        }
        root = nullptr;
        indicesCreated[field] = false;
        cout << "Index on " << field << " deleted successfully.\n";
    }

    void searchUser(const string& field, const string& value) {
        if (indicesCreated[field]) {
            indexNode* node = search(root, value);
            if (node) userList.print(node->userPtr);
            else cout << "User not found.\n";
        }
        else {
            cout << "Searching in Linked List...\n";
            if (field == "Name") userList.print(userList.getUserByName(value));
            else if (field == "Email") userList.print(userList.getUserByEmail(value));
        }
    }
    void displayIndex() const {
        printInOrder(root);
    }
    void createGroupIndexOnCountry() {
        UserList::User* current = userList.getHead()->next;
        while (current != nullptr && current->next != nullptr) {
            countryIndex[current->Country] = insert2(countryIndex[current->Country], current->Country, current);
            current = current->next;
        }
        cout << "Group Index on Country created successfully.\n";
    }
    void createGroupIndexOnType() {
        UserList::User* current = userList.getHead()->next;
        while (current != nullptr && current->next != nullptr) {
            typeIndex[current->Type] = insert(typeIndex[current->Type], current->Type, current);
            current = current->next;
        }
        cout << "Group Index on Type created successfully.\n";
    }
    void deleteGroupIndexOnCountry(const string& country) {
        if (countryIndex.find(country) == countryIndex.end()) {
            cout << "No group index found for Country: " << country << endl;
        }
        else {
            countryIndex.erase(country);
            cout << "Group index for Country " << country << " deleted.\n";
        }
    }

    void deleteGroupIndexOnType(const string& type) {
        if (typeIndex.find(type) == typeIndex.end()) {
            cout << "No group index found for Type: " << type << endl;
        }
        else {
            typeIndex.erase(type);
            cout << "Group index for Type " << type << " deleted.\n";
        }
    }

    void printGroupIndexOnCountry() {
        for (auto& group : countryIndex) {
            cout << "Group for Country " << group.first << ":\n";
            printInOrder(group.second);
        }
    }

    void printGroupIndexOnType() {
        for (auto& group : typeIndex) {
            cout << "Group for Type " << group.first << ":\n";
            printInOrder(group.second);
        }
    }

};

int main() {
    index<int> userIndex2;
    userIndex2.addUserbyID(6, "maya", "mayapasha081@gmail.com", "USA", "platinum");
    userIndex2.addUserbyID(2, "arfa", "arfa573@gmail.com", "Pakistan", "gold");
    userIndex2.addUserbyID(3, "ahmed", "ahmed758@gamil.com", "Japan", "new");
    userIndex2.addUserbyID(4, "ahmed", "ahmed758@gamil.com", "Pakistan", "new");

    cout << "All Users:\n";
    userIndex2.displayIndex();

    cout << "\nSearching for User with ID 2:\n";
    userIndex2.searchUserbyID(2);
    cout << "\nSearching for User with ID 8:\n";
    userIndex2.searchUserbyID(8);
    cout << "\nDeleting User with ID 2:\n";
    userIndex2.deleteUser(2);

    cout << "\nAll Users After Deletion:\n";
    userIndex2.displayIndex();

    cout << "=====================================================================================" << endl;



    index<string> userIndex;

    userIndex.addUser(1, "John", "john@example.com", "USA", "Platinum");
    userIndex.addUser(2, "Jane", "jane@example.com", "UK", "Gold");
    userIndex.addUser(3, "Alice", "alice@example.com", "Canada", "Silver");

    cout << "Creating Name Index...\n";
    userIndex.createIndex("Name");

    cout << "\nSearching for User by Name 'Jane':\n";
    userIndex.searchUser("Name", "Jane");

    cout << "\nDeleting Name Index...\n";
    userIndex.deleteIndex("Name");

    cout << "\nSearching for User by Name 'Jane' without Index:\n";
    userIndex.searchUser("Name", "Jane");

    cout << "\nCreating Email Index...\n";
    userIndex.createIndex("Email");

    cout << "\nSearching for User by Email 'alice@example.com':\n";
    userIndex.searchUser("Email", "alice@example.com");

    cout << "\nDeleting Email Index...\n";
    userIndex.deleteIndex("Email");
    cout << "=====================================================================================" << endl;
    /*index<int> idIndex3;
    idIndex3.addUserbyID(1, "Alice", "alice@mail.com", "USA", "Student");
    idIndex3.addUserbyID(2, "Bob", "bob@mail.com", "Canada", "Teacher");
    idIndex3.createIndex("Name");
    idIndex3.createGroupIndexOnCountry();
    idIndex3.createGroupIndexOnType();
    idIndex3.printGroupIndexOnCountry();
    idIndex3.printGroupIndexOnType();*/


    return 0;
}