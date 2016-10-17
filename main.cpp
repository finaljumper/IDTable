#include <iostream>
#include <string>



using namespace std;

template <class T>
class Array {
public:
    Array() : size(0), count(0) {}

    ~Array() {}

    void add(const T &element) {
        if (count == size) {
            T * newCollection = new T[size + 10];
            for (int i = 0; i < size; i++) {
                newCollection[i] = collection[i];
            }
            delete[] collection;
            this->collection = newCollection;
        }
        collection[count] = element;
        count++;
    }

    void add(T * element) {
        if (count == size) {
            T * newCollection = new T[size + 10];
            for (int i = 0; i < size; i++) {
                newCollection[i] = collection[i];
            }
            delete[] collection;
            this->collection = newCollection;
        }
        collection[count] = * element;
        count++;
    }

    void remove(int index) {
        int i = index;
        if(index < this->getSize()) {
            while (i != getSize()) {
                collection[i] = collection[i + 1];
                i++;
            }
            count--;
        } else
            count--;
    }

    void erase() {
        collection = NULL;
        count = 0;
    }

    T at(int index) {
        return collection[index];
    }

    int getSize() {
        return count;
    }
private:
    T * collection;
    int size;
    int count;
};

class Statement {
public:
    Statement() {}

    Statement(string ID, string type, string value) {
        int typeID;
        if(type.compare("int") == 0)
            typeID = 1;
        else if(type.compare("float") == 0)
            typeID = 2;
        else if(type.compare("double") == 0)
            typeID = 3;
        else if(type.compare("char") == 0)
            typeID = 4;
        else if(type.compare("bool") == 0)
            typeID = 5;
        else if(type.compare("long") == 0)
            typeID = 6;
        else if(type.compare("short") == 0)
            typeID = 7;
        else
            typeID = 0;
        this->ID.add(ID);
        this->type = typeID;
        this->value = value;
    }

    virtual ~Statement() {
        this->ID.erase();
        this->value.clear();
        this->type = NULL;
    }

    void print() {
        string varType;
        switch(this->type) {
            case 1:
                varType = "int";
                break;
            case 2:
                varType = "float";
                break;
            case 3:
                varType = "double";
                break;
            case 4:
                varType = "char";
                break;
            case 5:
                varType = "bool";
                break;
            case 6:
                varType = "long";
                break;
            case 7:
                varType = "short";
                break;
            default:
                varType = "unknown";
                break;
        }
        cout << varType << " " << this->ID.at(0) << " = " << this->value << endl;
        cout << "Linked: ";
        for (int i = 0; i < this->ID.getSize(); i++) {
            cout << this->ID.at(i);
            if(i != this->ID.getSize() - 1)
                cout << ", ";
        }
        cout << endl;
    }

    string getID() {
        return this->ID.at(0);
    }

    bool hasID(string name) {
        int size = this->ID.getSize();
        for (int i = 0; i < size; i++) {
            if (this->ID.at(i) == name)
                return true;
        }
        size = others.getSize();
        for (int i = 0; i < size; i++) {
            if (this->others.at(i).hasID(name))
                return true;
        }
        return false;
    }

    void setValue(string newValue) {
        this->value = newValue;
    }

    void addLink(string ID) {
        this->ID.add(ID);
    }

    bool hasLinks() {
        return ID.getSize() > 0;
    }

    void deleteLink(string link) {
        for (int i = 0; i <= ID.getSize(); i++) {
            if (ID.at(i) == link)
                ID.remove(i);
        }
    }

    void addToOthers(Statement * temp) {
        others.add(temp);
    }

    void deleteOther(string name) {
        for (int i = 0; i <= others.getSize(); i++) {
            if (others.at(i).hasID(name)) {
                others.remove(i);
                break;
            }
        }
    }

    void addOtherName(string name, string add) {
        for (int i = 0; i <= others.getSize(); i++) {
            if (others.at(i).hasID(name)) {
                others.at(i).addLink(add);
                break;
            }
        }
    }

    void printOther(string name) {
        for (int i = 0; i <= others.getSize(); i++) {
            if (others.at(i).hasID(name)) {
                others.at(i).print();
                break;
            }
        }
    }

    void setOtherValue(string name, string value) {
        for (int i = 0; i <= others.getSize(); i++) {
            if (others.at(i).hasID(name)) {
                others.at(i).setValue(value);
                break;
            }
        }
    }

private:
    Array <string> ID;
    int type;  //types 1:int 2:float 3:double 4:char 5:bool 6:long 7:short
    string value;
    Array<Statement> others;
};

class IDTable {
private:
    int hashcode(string name) {
        int code = name.at(0) * 3;
        code *= name.length() * 8;
        code += name.back() * 17;
        return code;
    }
    Statement * elements[1000000];
public:
    void addElement(string name, string type, string value);
    void editElement(string name, string newValue);
    void deleteElement(string name);
    void searchElement(string name);
};

void IDTable::addElement(string name, string type, string value) {
    int hash = hashcode(name);
    int valueHash = hashcode(value);
    Statement *temp = new Statement(name, type, value);
    if (this->elements[valueHash]) {
        if (this->elements[valueHash]->hasID(value))
            if (this->elements[valueHash]->getID() == value)
                this->elements[valueHash]->addLink(name);
            else
                this->elements[valueHash]->addOtherName(value, name);
    }
    if (this->elements[hash]){
        this->elements[hash]->addToOthers(temp);
    }
    else
        this->elements[hash] = temp;
}

void IDTable::editElement(string name, string newValue) {
    int hash = hashcode(name);
    if (this->elements[hash]) {
        if (this->elements[hash]->hasID(name)) {
            if (this->elements[hash]->getID() == name)
                this->elements[hash]->setValue(newValue);
            else
                this->elements[hash]->setOtherValue(name, newValue);
        }
    } else {
        cout << "Element not found." << endl;
    }
}

void IDTable::deleteElement(string name) {
    int hash = hashcode(name);
    if (this->elements[hash]) {
        if (this->elements[hash]->hasID(name)) {
            if (this->elements[hash]->getID() == name && !this->elements[hash]->hasLinks())
                delete this->elements[hash];
            else if (!this->elements[hash]->hasLinks())
                this->elements[hash]->deleteOther(name);
            else
                this->elements[hash]->deleteLink(name);
            cout << "Element deleted." << endl;
        }
        else
            cout << "Element not found." << endl;
    } else {
        cout << "Element not found." << endl;
    }
}

void IDTable::searchElement(string name) {
    int hash = hashcode(name);
    if (this->elements[hash]) {
        if (this->elements[hash]->hasID(name)) {
            if (this->elements[hash]->getID() == name)
                this->elements[hash]->print();
            else
                this->elements[hash]->printOther(name);
        }
        else
            cout << "Element not found." << endl;
    }
    else
        cout << "Element not found." << endl;

}

int main() {
    IDTable * table = new IDTable();
    while(true) {
        char key;
        cout << "Welcome! Please choose an option: \n"
                "1.Add an element.\n"
                "2.Edit an element.\n"
                "3.Delete an element.\n"
                "4.Search for element.\n"
                "0.Exit program." << endl;
        cin >> key;
        switch(key) {
            case '1': {
                string type, name, value;
                cout << "Enter type: ";
                cin >> type;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter value: ";
                cin >> value;
                table->addElement(name, type, value);
                cout << "Element added." << endl;
                break;
            }
            case '2': {
                string name, newValue;
                cout << "Enter name: " << endl;
                cin >> name;
                cout << "Enter new value: " << endl;
                cin >> newValue;
                table->editElement(name, newValue);
                break;
            }
            case '3': {
                string name;
                cout << "Enter name: ";
                cin >> name;
                table->deleteElement(name);
                break;
            }
            case '4': {
                string name;
                cout << "Enter name: ";
                cin >> name;
                table->searchElement(name);
                break;
            }
            case '0':
                return 0;
            default:
                break;
        }
    }
}