#include <iostream>
#include <string>
#include<vector>
#include<ctime>
#include<cstring>
using namespace std;

class Complaint {
public:
    int complaintID;
    int userID;
    string userType;
    string text;
   

    Complaint(int cID, int uID, const string& uType, const string& desc)
        : complaintID(cID), userID(uID), userType(uType), text(desc) {
       
    }
};

class UserTypePriority {
public:
    static int getPriority(const string& userType) {
        if (userType == "Platinum") return 5;
        if (userType == "Gold") return 4;
        if (userType == "Silver") return 3;
        if (userType == "Regular") return 2;
        return 1; // "New"
    }
};

class CompareComplaints {
public:
    bool operator()(const Complaint& a, const Complaint& b) const {
        int priorityA = UserTypePriority::getPriority(a.userType);
        int priorityB = UserTypePriority::getPriority(b.userType);
        if (priorityA == priorityB) {
            return a.complaintID > b.complaintID; 
        }
        return priorityA < priorityB; 
    }
};

class ComplaintHeap {
private:
    vector<Complaint> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (CompareComplaints()(heap[parent], heap[index])) {
                swap(heap[parent], heap[index]);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (index < size) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && CompareComplaints()(heap[largest], heap[left]))
                largest = left;
            if (right < size && CompareComplaints()(heap[largest], heap[right]))
                largest = right;

            if (largest != index) {
                swap(heap[index], heap[largest]);
                index = largest;
            }
            else {
                break;
            }
        }
    }

public:
    void registerComplaint(const Complaint& complaint) {
        heap.push_back(complaint);
        heapifyUp(heap.size() - 1);
    }

    Complaint serviceComplaint() {
        if (heap.empty()) {
            cout << "No complaints to service." << endl;
        }

        Complaint topComplaint = heap.front();
        swap(heap.front(), heap.back());
        heap.pop_back();
        heapifyDown(0);
        return topComplaint;
    }

    void increasePriority(int complaintID) {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].complaintID == complaintID) {
               
                heap[i].userType = "Platinum";
                heapifyUp(i);
                return;
            }
        }
        throw runtime_error("Complaint not found.");
    }

    vector<Complaint> getComplaintsByUser(int userID) {
        vector<Complaint> userComplaints;
        for (const auto& complaint : heap) {
            if (complaint.userID == userID) {
                userComplaints.push_back(complaint);
            }
        }
        return userComplaints;
    }

    vector<Complaint> getAllComplaints() const {
        return heap;
    }
    void printComplaintsByUser(int userID) {
        bool found = false; 
        for (const auto& complaint : heap) {
            if (complaint.userID == userID) {
                cout << "Complaint ID: " << complaint.complaintID << endl;
                cout << "User  ID: " << complaint.userID << endl;
                cout << "User  Type: " << complaint.userType << endl;
                cout << "Description: " << complaint.text << endl;
                cout << "--------------------------" << endl;
                found = true; 
            }
        }
        if (!found) {
            cout << "No complaints found for User ID: " << userID << endl;
        }
    }
};

// Country Index using Vector
class CountryIndex {
public:
    vector<pair<string, vector<int>>> index;

    void addUser(const string& country, int userID) {
        for (auto& entry : index) {
            if (entry.first == country) {
                entry.second.push_back(userID);
                return;
            }
        }
        index.push_back({ country, {userID} });
    }

    vector<int> getUsersByCountry(const string& country) {
        for (const auto& entry : index) {
            if (entry.first == country) {
                return entry.second;
            }
        }
        return {};
    }
};

// User Index using Vector
class UserIndex {
public:
    vector<pair<int, vector<Complaint>>> index;

    void addComplaint(int userID, const Complaint& complaint) {
        for (auto& entry : index) {
            if (entry.first == userID) {
                entry.second.push_back(complaint);
                return;
            }
        }
        index.push_back({ userID, {complaint} });
    }

    vector<Complaint> getComplaintsByUser(int userID) {
        for (const auto& entry : index) {
            if (entry.first == userID) {
                return entry.second;
            }
        }
        return {};
    }
};

// Function to Print Complaints
void printComplaints(const vector<Complaint>& complaints) {
    for (const auto& complaint : complaints) {
        cout << endl;
        cout << "Complaint ID: " << complaint.complaintID << endl;
        cout<<  "User ID: " << complaint.userID << endl;
       cout << "User Type: " << complaint.userType << endl;
       cout << "Text: " << complaint.text << endl;
       cout << "===========================================" << endl;

    }
}

// Print Complaints by Country
void printComplaintsByCountry(CountryIndex& countryIndex, UserIndex& userIndex, const string& country) {
    vector<int> userIDs = countryIndex.getUsersByCountry(country);
    if (userIDs.empty()) {
        cout << "No users found for the given country.\n";
        return;
    }

    for (int userID : userIDs) {
        vector<Complaint> complaints = userIndex.getComplaintsByUser(userID);
        if (!complaints.empty()) {
            cout << "Complaints for User ID: " << userID << endl;
            printComplaints(complaints);
        }
    }
}


class UserList {
public:
    template <class T>
    friend class index;
    friend class multiplInd;


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
    User* getTail() const {
        return tail;
    }
    void removeUser(int ID) {
        User* temp = head->next;
        while (temp != tail) {
            if (temp->UserID == ID) {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                return;
            }
            temp = temp->next;
        }
    }
    void removeUser(string attribute) {
        User* temp = head->next;
        while (temp != tail) {
            if ((temp->Name == attribute) || (temp->Email == attribute)) {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                return;
            }
            temp = temp->next;
        }
    }

};
class multipleInd {
private:
    class groupNode {
    public:
        string key; 
        UserList group;
        groupNode* left;
        groupNode* right;
        int height;

        groupNode(const string& k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    groupNode* root;

    int height(groupNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(groupNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    groupNode* rightRotate(groupNode* y) {
        groupNode* x = y->left;
        groupNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    groupNode* leftRotate(groupNode* x) {
        groupNode* y = x->right;
        groupNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    groupNode* insert(groupNode* node, const string& key) {
        if (!node) return new groupNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
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

    groupNode* find(groupNode* node, const string& key) {
        if (!node || node->key == key)
            return node;
        if (key < node->key)
            return find(node->left, key);
        return find(node->right, key);
    }
   
    groupNode* deleteGroup(groupNode* node, const string& key) {

        if (!node)
            return node;

        if (key < node->key)
            node->left = deleteGroup(node->left, key);
        else if (key > node->key)
            node->right = deleteGroup(node->right, key);
        else {
            // Node to be deleted found
            if (!node->left || !node->right) {
                groupNode* temp = nullptr;
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

                groupNode* temp = node->right;
                while (temp->left)
                    temp = temp->left;

                node->key = temp->key;
               
                node->right = deleteGroup(node->right, temp->key);
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
    multipleInd() : root(nullptr) {}

    void addGroup(const string& key) {
        root = insert(root, key);
    }

    UserList* getGroup(const string& key) {
        groupNode* node = find(root, key);
        return node ? &(node->group) : nullptr;
    }

    void printGroups(groupNode* node) {
        if (!node) return; 

       
        printGroups(node->left);

        
        cout << "Group: " << node->key << endl;

        
        UserList::User* current = node->group.getHead()->next; 
        while (current != node->group.getTail()) { 
            node->group.print(current); 
            cout << endl;
            current = current->next;
        }

        
        printGroups(node->right);
    }

    void removeGroup(const string& key) {
        root = deleteGroup(root, key);
    }


    void printAllGroups() {
        printGroups(root);
        cout << "========================GROUP END==================================" << endl;
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
    multipleInd groups;

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

    indexNode* insert(indexNode* node, const T key, UserList::User* user) {
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


    indexNode* search(indexNode* node, T key) {
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
                node->userPtr = temp->userPtr; 
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

    void addUser(int id, const string& name, const string& email, const string& country, const string& type, const string& attribute) {
        userList.addUser(id, name, email, country, type);
        UserList::User* user = userList.getUser(id);
        if (attribute == "name")
            root = insert(root, name, user);
        else
            root = insert(root, email, user);

    }
    void searchUser(T value) {
        indexNode* node = search(root, value);
        if (node && node->userPtr) {
            userList.print(node->userPtr);
        }
        else {
            cout << "User not found" << endl;
        }
    }


    void deleteUser(T key) {

        root = deleteNode(root, key);


        userList.removeUser(key);
    }




    void displayIndex() const {
        printInOrder(root);
    }

    void addUsergrp(int id, const string& name, const string& email, const string& country, const string& type, const string& groupAttribute) {
        userList.addUser(id, name, email, country, type);
        UserList::User* user = userList.getUser(id);

        string groupKey;
        if (groupAttribute == "type") {
            groupKey = type;
        }
        else if (groupAttribute == "country") {
            groupKey = country;
        }
        else {
            cout << "Invalid group attribute specified." << endl;
            return;
        }

       
        root = insert(root, groupKey, user);

      
        UserList* group = groups.getGroup(groupKey);
        if (!group) {
            groups.addGroup(groupKey); 
            group = groups.getGroup(groupKey);
        }

       
        group->addUser(id, name, email, country, type);
    }


    void printAllGroups() {
        groups.printAllGroups();
    }

    void removeGroup(const string& key) {
        groups.removeGroup(key);
       
        
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
    userIndex2.searchUser(2);
    cout << "\nSearching for User with ID 8:\n";
    userIndex2.searchUser(8);
    cout << "\nDeleting User with ID 2:\n";
    userIndex2.deleteUser(2);

    cout << "\nAll Users After Deletion:\n";
    userIndex2.displayIndex();

    cout << "=====================================================================================" << endl;



    index<string> userIndex;
    cout << "creating indexes on basis of name" << endl;

    userIndex.addUser(1, "John", "john@example.com", "USA", "Platinum", "name");
    userIndex.addUser(2, "Jane", "jane@example.com", "UK", "Gold", "name");
    userIndex.addUser(3, "Alice", "alice@example.com", "Canada", "Silver", "name");


    userIndex.displayIndex();
    cout << "\nSearching for User with name:John" << endl;
    userIndex.searchUser("John");
    cout << "\nDeleting User with name John\n";
    userIndex.deleteUser("John");
    cout << "\nAll Users After Deletion:\n";
    userIndex.displayIndex();
    cout << "=====================================================================================" << endl;
    index<string> userIndex3;
    cout << "creating indexes on basis of email" << endl;

    userIndex3.addUser(1, "John", "john@example.com", "USA", "Platinum", "email");
    userIndex3.addUser(2, "Jane", "jane@example.com", "UK", "Gold", "email");
    userIndex3.addUser(3, "Alice", "alice@example.com", "Canada", "Silver", "email");
    userIndex3.displayIndex();
    cout << "\nSearching for User with email:john@example.com" << endl;
    userIndex3.searchUser("john@example.com");

    cout << "\nDeleting User with email:john@example.com \n";
    userIndex3.deleteUser("john@example.com");
    cout << "\nAll Users After Deletion:\n";
    userIndex3.displayIndex();

    cout << "============================================================================================" << endl;


    index<string> userIndex4;

    userIndex4.addUsergrp(1, "John", "john@example.com", "USA", "Platinum", "type");
    userIndex4.addUsergrp(2, "Jane", "jane@example.com", "UK", "Gold", "type");
    userIndex4.addUsergrp(3, "Alice", "alice@example.com", "Canada", "Platinum", "type");

    cout << "Grouped by Type:\n";
    userIndex4.printAllGroups();
    cout << "===============================================================================================" << endl;
    index<string> userIndex5;
    userIndex5.addUsergrp(4, "ahmed", "john@example.com", "Pakistan", "Platinum", "country");
    userIndex5.addUsergrp(8, "shima", "jane@example.com", "Pakistan", "Gold", "country");
    userIndex5.addUsergrp(10, "maya", "alice@example.com", "Canada", "Platinum", "country");
    userIndex5.addUsergrp(4, "shumaila", "john@example.com", "USA", "Platinum", "country");
    userIndex5.addUsergrp(8, "adeena", "jane@example.com", "UK", "Gold", "country");
    userIndex5.addUsergrp(10, "aaleen", "alice@example.com", "Finlad", "Platinum", "country");

    cout << "Grouped by Country:\n";
    userIndex5.printAllGroups();
    cout << "Deleting group with key 'Pakistan':\n";
    userIndex5.removeGroup("Pakistan");

    cout << "Groups after deletion:\n";
    userIndex5.printAllGroups();
    cout << "================================================================================================" << endl;


    ComplaintHeap complaintQueue;
    CountryIndex countryIndex;
    UserIndex userIndex6;

    // Adding Complaints
    Complaint c1(1, 101, "Platinum", "Network issue");
    Complaint c2(2, 102, "Gold", "Payment failure");
    Complaint c3(3, 103, "Silver", "Slow speed");

    complaintQueue.registerComplaint(c1);
    complaintQueue.registerComplaint(c2);
    complaintQueue.registerComplaint(c3);

    countryIndex.addUser("USA", 101);
    countryIndex.addUser("USA", 102);
    countryIndex.addUser("Canada", 103);

    userIndex6.addComplaint(101, c1);
    userIndex6.addComplaint(102, c2);
    userIndex6.addComplaint(103, c3);

   
    cout << "All Complaints:\n";
    printComplaints(complaintQueue.getAllComplaints());

  
    cout << "\nServicing Complaint:\n";
    Complaint serviced = complaintQueue.serviceComplaint();
    cout << "Serviced Complaint ID: " << serviced.complaintID << endl;

    
    cout << "\nComplaints by Country (USA):\n";
    printComplaintsByCountry(countryIndex, userIndex6, "USA");

    cout << "\nComplaints for User ID: 102\n";
    vector<Complaint> complaintsForUser = complaintQueue.getComplaintsByUser(102);
    printComplaints(complaintsForUser);

    cout << "\nIncreasing priority for Complaint ID: 2\n";
    complaintQueue.increasePriority(2);

    cout << "All Complaints after priority increase:\n";
    printComplaints(complaintQueue.getAllComplaints());







    return 0;

}