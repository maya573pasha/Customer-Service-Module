#include<iostream>
using namespace std;
#include<forward_list>
//=====================================================USER=====================================================================================
class ServiceModule;
class user {
public:
    int userid;
    string username;
    string Email;
    string country;
    string type;
    user* next;
    user* prev;
    user()
    {
        next = NULL;
        prev = NULL;
    }

    user(int id, string uname, string email, string Country, string Type, user* nxt = nullptr, user* pre = nullptr)
    {
        userid = id;
        username = uname;
        Email = email;
        country = Country;
        type = Type;
        next = nxt;
        prev = pre;
    }


        void print()
    {
        cout << "User ID: " << userid << endl;
        cout << "Name: " << username << endl;
        cout << "Email Address: " << Email << endl;
        cout << "Country: " << country << endl;
        cout << "Customer Type: " << type << endl;
    }
};
//===================================================USERLIST===================================================================================
class UserList {
private:
    user* head;
    user* tail;

public:

    UserList()
    {
        head = new user();
        tail = new user();
        head->next = tail;
        tail->prev = head;
        head->prev = nullptr;
        tail->next = nullptr;
    }
    void addAfter(user* newUser, user* existingUser)
    {
        if (head->next == tail)
        {
            newUser->prev = head;
            newUser->next = tail;
            head->next = newUser;
            tail->prev = newUser;
            return;
        }
        newUser->prev = existingUser;
        newUser->next = existingUser->next;
        existingUser->next->prev = newUser;
        existingUser->next = newUser;
    }

    void addBefore(user* newUser, user* existingUser)
    {
        if (head->next == tail)
        {
            newUser->prev = head;
            newUser->next = tail;
            head->next = newUser;
            tail->prev = newUser;
            return;
        }
        newUser->next = existingUser;
        newUser->prev = existingUser->prev;
        existingUser->prev->next = newUser;
        existingUser->prev = newUser;
    }

    void addUserSorted(user* newUser)
    {
        if (head->next == tail)
        {
            newUser->prev = head;
            newUser->next = tail;
            head->next = newUser;
            tail->prev = newUser;
            return;
        }
        user* current = head->next;
        while (current != tail && current->userid < newUser->userid)
        {
            current = current->next;
        }
        addBefore(newUser, current);
    }


    void removeUser(user* userToRemove)
    {
        userToRemove->prev->next = userToRemove->next;
        userToRemove->next->prev = userToRemove->prev;
        delete userToRemove;
        userToRemove = nullptr;
    }


    void Print()
    {
        user* current = head->next;
        while (current != tail)
        {
            current->print();
            cout << endl;
            current = current->next;
        }
    }
};
//================================================HASHUSER======================================================================================
class UserHashTable {
private:
    class HashNode {
    public:
        user* userNode;
        string userKey;
        HashNode* nextNode;

        HashNode(user* user, const string& key) {
            userNode = user;
            userKey = key;
            nextNode = nullptr;
        }

        HashNode() {
            userNode = nullptr;
            userKey = "";
            nextNode = nullptr;
        }
    };

    HashNode** table;
    int capacity;
    int currentSize;

    int computeHash(const string& key) {
        int hashValue = 0;
        int power = 1;
        for (char ch : key) {
            hashValue = (hashValue + ch * power) % capacity;
            power = (power * 33) % capacity;
        }
        return (hashValue + capacity) % capacity;
    }

    void resizeTable() {
        int oldCapacity = capacity;
        capacity *= 2;
        HashNode** oldTable = table;
        table = new HashNode * [capacity] {nullptr};
        currentSize = 0;

        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i] != nullptr) {
                HashNode* current = oldTable[i];
                while (current != nullptr) {
                    insertUser(current->userKey, current->userNode);
                    current = current->nextNode;
                }
            }
        }
        delete[] oldTable;
    }

public:
    UserHashTable() {
        capacity = 11;
        table = new HashNode * [capacity] {nullptr};
        currentSize = 0;
    }

    ~UserHashTable() {
        clearAllUsers();
        delete[] table;
    }

    void insertUser(const string& key, user* newUser) {
        if (static_cast<double>(currentSize) / capacity > 0.5) {
            resizeTable();
        }
        int index = computeHash(key);
        HashNode* newNode = new HashNode(newUser, key);

        if (table[index] == nullptr) {
            table[index] = newNode;
            currentSize++;
        }
        else {
            HashNode* current = table[index];
            while (current->nextNode != nullptr) {
                current = current->nextNode;
            }
            current->nextNode = newNode;
            currentSize++;
        }
    }

    void removeUser(const string& key) {
        int index = computeHash(key);
        HashNode* current = table[index];
        HashNode* previous = nullptr;

        while (current != nullptr && current->userKey != key) {
            previous = current;
            current = current->nextNode;
        }

        if (current == nullptr) {
            cout << "User not found with the given key." << endl;
            return;
        }

        if (previous == nullptr) {
            table[index] = current->nextNode;
        }
        else {
            previous->nextNode = current->nextNode;
        }

        delete current;
        currentSize--;
    }

    user* getUser(const string& key) {
        int index = computeHash(key);
        HashNode* current = table[index];
        while (current != nullptr) {
            if (current->userKey == key) {
                return current->userNode;
            }
            current = current->nextNode;
        }
        return nullptr;
    }

    void clearAllUsers() {
        for (int i = 0; i < capacity; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->nextNode;
                delete temp;
            }
            table[i] = nullptr;
        }
        currentSize = 0;
    }

    void printAllUsers() const {
        for (int i = 0; i < capacity; i++) {
            if (table[i] != nullptr) {
                HashNode* current = table[i];
                while (current != nullptr) {
                    current->userNode->print();
                    current = current->nextNode;
                    cout << endl;
                }
            }
        }
    }
};
//================================================HASHID======================================================================================
template<typename T>
class HashId {
private:
    class HashNode {
    public:
        T* data;
        int key;

        HashNode(T* Data, int Key) {
            data = Data;
            key = Key;
        }

        HashNode() {
            data = nullptr;
            key = -1;
        }
    };

    int capacity;
    HashNode* table;
    int numElements;

    int primaryHash(int key) const {
        return key % capacity;
    }

    int secondaryHash(int key) const {
        return 1 + (key % (capacity - 1));
    }

    int probeIndex(int baseIndex, int attempt, int key) const {
        return (baseIndex + attempt * secondaryHash(key)) % capacity;
    }

    void resize() {
        int oldCapacity = capacity;
        capacity *= 2;
        HashNode* oldTable = table;
        table = new HashNode[capacity];
        numElements = 0;

        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i].key != -1) {
                add(oldTable[i].key, oldTable[i].data);
            }
        }
        delete[] oldTable;
    }

public:
    HashId() {
        capacity = 11;
        table = new HashNode[capacity];
        numElements = 0;
    }

    ~HashId() {
        delete[] table;
    }

    void add(int key, T* value) {
        if (static_cast<double>(numElements) / capacity > 0.5) {
            resize();
        }

        int baseIndex = primaryHash(key);
        if (table[baseIndex].key == -1) {
            table[baseIndex].key = key;
            table[baseIndex].data = value;
            numElements++;
            return;
        }

        for (int attempt = 0;; attempt++) {
            int index = probeIndex(baseIndex, attempt, key);
            if (table[index].key == -1) {
                table[index].key = key;
                table[index].data = value;
                numElements++;
                break;
            }
        }
    }

    T* remove(int key) {
        int baseIndex = primaryHash(key);
        if (table[baseIndex].key == key) {
            table[baseIndex].key = -1;
            T* removedData = table[baseIndex].data;
            table[baseIndex].data = nullptr;
            numElements--;
            return removedData;
        }

        for (int attempt = 0;; attempt++) {
            int index = probeIndex(baseIndex, attempt, key);
            if (table[index].key == -1) {
                std::cout << "Key not found." << std::endl;
                return nullptr;
            }
            if (table[index].key == key) {
                table[index].key = -1;
                T* removedData = table[index].data;
                table[index].data = nullptr;
                numElements--;
                return removedData;
            }
        }
    }

    T* get(int key) const {
        int baseIndex = primaryHash(key);
        if (table[baseIndex].key == key) {
            return table[baseIndex].data;
        }

        for (int attempt = 0;; attempt++) {
            int index = probeIndex(baseIndex, attempt, key);
            if (table[index].key == -1) {
                return nullptr;
            }
            if (table[index].key == key) {
                return table[index].data;
            }
        }
    }
};
//==============================================GROUPHASH=====================================================================================
template<typename U1, typename U2>
class GroupHashing {
private:
    class Node {
    public:
        U1* data;
        U2 identifier;
        Node* link;

        Node(U1* Data, U2 Identifier) {
            data = Data;
            identifier = Identifier;
            link = nullptr;
        }

        Node() {
            data = nullptr;
            identifier = U2();
            link = nullptr;
        }

        void displayChain() {
            Node* temp = this;
            while (temp != nullptr) {
                temp->data->print();
                temp = temp->link;
                std::cout << std::endl;
            }
        }
    };

    Node** table;
    int capacity;
    int elementCount;
    int probeStrategy;

    int computeHash(const std::string& key) {
        int hashVal = 0;
        int factor = 1;
        for (char c : key) {
            hashVal = (hashVal + c * factor) % capacity;
            factor = (factor * 33) % capacity;
        }
        return (hashVal + capacity) % capacity;
    }

    int computeHash(const int& key) {
        return key % capacity;
    }

    void expand() {
        int oldCapacity = capacity;
        capacity *= 2;
        Node** oldTable = table;
        table = new Node * [capacity] {nullptr};
        elementCount = 0;

        for (int i = 0; i < oldCapacity; i++) {
            if (oldTable[i] != nullptr) {
                Node* current = oldTable[i];
                while (current != nullptr) {
                    addItem(current->identifier, current->data);
                    current = current->link;
                }
            }
        }
        delete[] oldTable;
    }

    int linearSearch(int hashVal, int step) {
        return (hashVal + step) % capacity;
    }

    int quadraticSearch(int hashVal, int step) {
        return (hashVal + step * step) % capacity;
    }

public:
    GroupHashing(const int strategy) {
        capacity = 11;
        table = new Node * [11] {nullptr};
        elementCount = 0;
        probeStrategy = strategy;
    }

    void addItem(U2 key, U1* value) {
        if ((double)elementCount / capacity > 0.5) {
            expand();
        }

        int index = computeHash(key);
        Node* newNode = new Node(value, key);

        if (table[index] == nullptr) {
            table[index] = newNode;
            elementCount++;
            return;
        }

        if (table[index]->identifier == key) {
            Node* temp = table[index];
            while (temp->link != nullptr) {
                temp = temp->link;
            }
            temp->link = newNode;
            elementCount++;
            return;
        }

        for (int i = 0;; i++) {
            int newIndex = (probeStrategy == 1) ? linearSearch(index, i) : quadraticSearch(index, i);
            if (table[newIndex] == nullptr) {
                table[newIndex] = newNode;
                elementCount++;
                break;
            }
            else if (table[newIndex]->identifier == key) {
                Node* temp = table[newIndex];
                while (temp->link != nullptr) {
                    temp = temp->link;
                }
                temp->link = newNode;
                elementCount++;
                break;
            }
        }
    }

    void removeByKey(int uniqueID) {
        for (int i = 0; i < capacity; i++) {
            Node* temp = table[i];
            Node* prev = nullptr;
            while (temp) {
                if (temp->data->getComplaintID() == uniqueID) {
                    if (prev) {
                        prev->link = temp->link;
                    }
                    else {
                        table[i] = temp->link;
                    }
                    delete temp->data;
                    delete temp;
                    elementCount--;
                    return;
                }
                prev = temp;
                temp = temp->link;
            }
        }
        std::cout << "No entry found for ID: " << uniqueID << std::endl;
    }

    void removeItem(const std::string& key, int userID) {
        int hashVal = computeHash(key);
        int step = 0;

        while (true) {
            int probeIndex = (probeStrategy == 1) ? linearSearch(hashVal, step) : quadraticSearch(hashVal, step);
            Node* temp = table[probeIndex];
            Node* prev = nullptr;

            while (temp != nullptr) {
                if (temp->identifier == key && temp->data->userid == userID) {
                    if (prev == nullptr) {
                        table[probeIndex] = temp->link;
                    }
                    else {
                        prev->link = temp->link;
                    }
                    delete temp;
                    elementCount--;
                    return;
                }
                prev = temp;
                temp = temp->link;
            }

            if (table[probeIndex] == nullptr) {
                std::cout << "No matching key or user ID found." << std::endl;
                return;
            }

            step++;
        }
    }

    Node* fetch(const U2& key) {
        int hashVal = computeHash(key);
        int step = 0;

        while (true) {
            int probeIndex = (probeStrategy == 1) ? linearSearch(hashVal, step) : quadraticSearch(hashVal, step);
            Node* temp = table[probeIndex];

            while (temp != nullptr) {
                if (temp->identifier == key) {
                    return temp;
                }
                temp = temp->link;
            }

            if (table[probeIndex] == nullptr) {
                return nullptr;
            }

            step++;
        }
    }

    void displayAll() {
        for (int i = 0; i < capacity; i++) {
            if (table[i] != nullptr) {
                Node* temp = table[i];
                while (temp != nullptr) {
                    temp->data->print();
                    temp = temp->link;
                    std::cout << "\n";
                }
            }
        }
    }

    void clearTable() {
        for (int i = 0; i < capacity; i++) {
            Node* temp = table[i];
            while (temp != nullptr) {
                Node* toDelete = temp;
                temp = temp->link;
                delete toDelete;
            }
            table[i] = nullptr;
        }
        elementCount = 0;
    }
};
//================================================service=====================================================================================
class ServiceModule {
    UserList userDirectory;
    HashId<user> idMap;
    HashId usernameMap;
    HashId emailMap;
    GroupHashing<user, string>* regionMap;
    GroupHashing<user, string>* membershipMap;
    ComplaintHashTable complaintTable;
    HashGroup<complains, int>* complaintIdMap;

public:
    ServiceModule() {
        regionMap = new HashGroup<user, string>(1);
        membershipMap = new HashGroup<user, string>(2);
        complaintIdMap = new HashGroup<complains, int>(1);
    }

    void addServiceUser(int id, string name, string email, string region, string membership) {
        user* newUser = new user(id, name, email, region, membership);
        idMap.Insert(id, newUser);
        usernameMap.Insert(name, newUser);
        emailMap.Insert(email, newUser);
        regionMap->Insert(region, newUser);
        membershipMap->Insert(membership, newUser);
        userDirectory.insertSorted(newUser);
    }

    void listAllUsers() {
        userDirectory.display();
    }

    void findUserById(int id) {
        user* foundUser = idMap.Access(id);
        if (foundUser != nullptr) {
            foundUser->display();
        }
        else {
            cout << "User not found in the system.\n";
        }
    }

    void removeUserById(int id) {
        user* userToRemove = idMap.Delete(id);
        usernameMap.Delete(userToRemove->username);
        emailMap.Delete(userToRemove->Email);
        regionMap->Delete(userToRemove->region, id);
        membershipMap->Delete(userToRemove->membership, id);
        userDirectory.removeNode(userToRemove);
    }

    void findUserByName(const string name) {
        user* foundUser = usernameMap.Access(name);
        if (foundUser != nullptr) {
            foundUser->display();
        }
        else {
            cout << "No user found with this name.\n";
        }
    }

    void findUserByEmail(const string email) {
        user* foundUser = emailMap.Access(email);
        if (foundUser != nullptr) {
            foundUser->display();
        }
        else {
            cout << "No user found with this email.\n";
        }
    }

    void showUsernamesIndex() {
        usernameMap.print();
    }

    void showEmailsIndex() {
        emailMap.print();
    }

    void showRegionsIndex() {
        regionMap->print();
    }

    void showMembershipsIndex() {
        membershipMap->print();
    }

    void findUsersByRegion(const string region) {
        HashGroup<user, string>::hashnode* foundList = regionMap->Access(region);
        if (foundList != nullptr) {
            foundList->displayAll();
        }
        else {
            cout << "No users found in this region.\n";
        }
    }

    void findUsersByMembership(const string membership) {
        HashGroup<user, string>::hashnode* foundList = membershipMap->Access(membership);
        if (foundList != nullptr) {
            foundList->displayAll();
        }
        else {
            cout << "No users found with this membership.\n";
        }
    }

    void clearRegionIndex() {
        regionMap->clearEntries();
    }

    void clearMembershipIndex() {
        membershipMap->clearEntries();
    }

    void clearEmailIndex() {
        emailMap.clearEntries();
    }

    void clearUsernameIndex() {
        usernameMap.clearEntries();
    }

    void registerComplaint(int userId, string description) {
        user* foundUser = idMap.Access(userId);
        if (foundUser != nullptr) {
            int priority = 0;
            if (foundUser->membership == "Platinum") {
                priority = 1;
            }
            else if (foundUser->membership == "Gold") {
                priority = 2;
            }
            else if (foundUser->membership == "Silver") {
                priority = 3;
            }
            else if (foundUser->membership == "Regular") {
                priority = 4;
            }
            else if (foundUser->membership == "New") {
                priority = 5;
            }
            complains* newComplaint = new complains(userId, priority, description);
            complaintTable.Insert(priority, newComplaint);
            complaintIdMap->Insert(userId, newComplaint);
        }
        else {
            cout << "User not found.\n";
        }
    }

    void displayAllComplaints() {
        complaintTable.print();
    }

    void resolveComplaint() {
        complaintIdMap->Delete(complaintTable.removeHighestPriority());
    }

    void escalateComplaintPriority(int complaintId) {
        complaintTable.increasePriority(complaintId);
    }

    void listComplaintsByUserId(int userId) {
        HashGroup<complains, int>::hashnode* foundList = complaintIdMap->Access(userId);
        if (foundList != nullptr) {
            foundList->displayAll();
        }
    }

    void listComplaintsByRegion(const string region) {
        HashGroup<user, string>::hashnode* regionComplaints = regionMap->Access(region);
        while (regionComplaints != nullptr) {
            listComplaintsByUserId(regionComplaints->node->userId);
            regionComplaints = regionComplaints->next;
        }
    }
};
//=============================================complaint================================================================================
class ComplaintModule {
private:
    ComplaintList complaintRecords; // List to manage complaints
    ComplaintHashTable complaintRegistry; // Hash table for complaints
    HashTable<int, UserComplaint>* complaintIdRegistry; // Map user IDs to complaints
    RegionGroupHashTable* regionGroups; // Group complaints by region
    MembershipGroupHashTable* membershipGroups; // Group complaints by membership type
    ComplaintPriorityQueue complaintQueue; // Priority queue for handling complaints

    int computeHash(int key) {
        return key % complaintQueue.getSize();
    }

    void resizeHashTable() {
        int oldSize = complaintQueue.getSize();
        complaintQueue.resize(oldSize * 2);
        complaintQueue.clearAllEntries();
        for (int i = 0; i < oldSize; i++) {
            complaintRecords.rehashEntries(i);
        }
    }

    int probeIndex(int baseIndex, int offset) {
        return (baseIndex + offset) % complaintQueue.getSize();
    }

public:
    ComplaintModule() {
        regionGroups = new RegionGroupHashTable(1);
        membershipGroups = new MembershipGroupHashTable(2);
        complaintIdRegistry = new HashTable<int, UserComplaint>(1);
    }

    void addComplaint(int userId, const string& details) {
        UserComplaint* userRecord = complaintRecords.retrieveUser(userId);
        if (userRecord) {
            int priority = determinePriority(userRecord->membershipType);
            Complaint* newComplaint = new Complaint(userId, priority, details);
            complaintQueue.insert(priority, newComplaint);
            complaintIdRegistry->Insert(userId, newComplaint);
        }
        else {
            cout << "Error: User not found. Cannot register complaint.\n";
        }
    }

    void displayAllComplaints() {
        complaintQueue.printAll();
    }

    void prioritizeComplaint(int complaintId) {
        complaintQueue.increasePriority(complaintId);
    }

    void resolveTopPriorityComplaint() {
        int resolvedId = complaintQueue.removeTop();
        if (resolvedId != -1) {
            complaintIdRegistry->Remove(resolvedId);
        }
    }

    void searchComplaintByUser(int userId) {
        auto* userComplaints = complaintIdRegistry->Access(userId);
        if (userComplaints) {
            userComplaints->printList();
        }
        else {
            cout << "No complaints found for User ID: " << userId << "\n";
        }
    }

    void groupComplaintsByRegion(const string& region) {
        auto* regionalComplaints = regionGroups->Access(region);
        if (regionalComplaints) {
            while (regionalComplaints) {
                searchComplaintByUser(regionalComplaints->node->userId);
                regionalComplaints = regionalComplaints->next;
            }
        }
        else {
            cout << "No complaints found for region: " << region << "\n";
        }
    }

    void groupComplaintsByMembership(const string& membershipType) {
        auto* membershipComplaints = membershipGroups->Access(membershipType);
        if (membershipComplaints) {
            membershipComplaints->printList();
        }
        else {
            cout << "No complaints found for membership type: " << membershipType << "\n";
        }
    }

    void clearComplaintRegistry() {
        complaintQueue.clearAllEntries();
        complaintIdRegistry->clearAll();
    }

    ~ComplaintModule() {
        delete regionGroups;
        delete membershipGroups;
        delete complaintIdRegistry;
    }
};
int main() {
    ServiceModule serviceModule;

    // Adding users
    serviceModule.addServiceUser(1, "Alice Smith", "alice@example.com", "USA", "Gold");
    serviceModule.addServiceUser(2, "Bob Johnson", "bob@example.com", "Canada", "Silver");
    serviceModule.addServiceUser(3, "Charlie Brown", "charlie@example.com", "UK", "Platinum");
    serviceModule.addServiceUser(4, "David Wilson", "david@example.com", "USA", "Regular");

    // Listing all users
    cout << "All Users:" << endl;
    serviceModule.listAllUsers();

    // Finding users by ID
    cout << "\nFinding User by ID 1:" << endl;
    serviceModule.findUser ById(1);

    // Finding users by Name
    cout << "\nFinding User by Name 'Bob Johnson':" << endl;
    serviceModule.findUser ByName("Bob Johnson");

    // Finding users by Email
    cout << "\nFinding User by Email 'charlie@example.com':" << endl;
    serviceModule.findUser ByEmail("charlie@example.com");

    // Finding users by Region
    cout << "\nFinding Users by Region 'USA':" << endl;
    serviceModule.findUsersByRegion("USA");

    // Finding users by Membership
    cout << "\nFinding Users by Membership 'Gold':" << endl;
    serviceModule.findUsersByMembership("Gold");

    // Removing a user by ID
    cout << "\nRemoving User by ID 2:" << endl;
    serviceModule.removeUserById(2);
    serviceModule.listAllUsers();

    // Registering complaints
    cout << "\nRegistering Complaints:" << endl;
    serviceModule.registerComplaint(1, "Issue with account access.");
    serviceModule.registerComplaint(3, "Payment not processed.");

    // Displaying all complaints
    cout << "\nAll Complaints:" << endl;
    serviceModule.displayAllComplaints();

    // Resolving a complaint
    cout << "\nResolving top priority complaint:" << endl;
    serviceModule.resolveComplaint();
    serviceModule.displayAllComplaints();

    // Escalating a complaint's priority
    cout << "\nEscalating complaint priority:" << endl;
    serviceModule.escalateComplaintPriority(1); // Assuming 1 is a valid complaint ID
    serviceModule.displayAllComplaints();

    // Listing complaints by user ID
    cout << "\nListing complaints for User ID 1:" << endl;
    serviceModule.listComplaintsByUserId(1);

    // Listing complaints by region
    cout << "\nListing complaints by Region 'USA':" << endl;
    serviceModule.listComplaintsByRegion("USA");

    // Clear all indexes
    cout << "\nClearing all indexes:" << endl;
    serviceModule.clearRegionIndex();
    serviceModule.clearMembershipIndex();
    serviceModule.clearEmailIndex();
    serviceModule.clearUsernameIndex();

    return 0;
}