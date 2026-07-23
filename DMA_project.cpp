#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <algorithm>

#define NOT_FOUND 0
#define FOUND 1

using namespace std;

// Global variables
int nalp = 0; // number of all processes
int len_alp = 0;
struct Process;

// Linked list namespace
namespace link_list {
    enum var_type { none, c, i, l, f, d, ll, p } t;

    typedef union _var {
        char c;
        int i;
        long l;
        float f;
        double d;
        long long ll;
        void* p;
    } var;

    typedef struct _vtype {
        var data;
        char type;
    } vtype;

    struct node {
        vtype data;
        node* next;
    };

    class List {
    private:
        node* now = nullptr;
        node* start = nullptr;
        int len = 0;

    public:
        void add_nodep(void* pp) {
            node* tmp = new node;
            tmp->data.data.p = pp;
            tmp->data.type = p;
            tmp->next = nullptr;
            
            if (start == nullptr) {
                start = tmp;
                now = tmp;
            } else {
                now->next = tmp;
                now = tmp;
            }
            len++;
        }

        void* get(int index) {
            if (index < 0 || index >= len) return nullptr;
            node* tmp = start;
            for (int i = 0; i < index; i++) {
                if (tmp == nullptr) return nullptr;
                tmp = tmp->next;
            }
            return tmp->data.data.p;
        }

        void remove(int index) {
            if (index < 0 || index >= len) return;
            
            node* before = nullptr;
            node* current = start;
            
            if (index == 0) {
                start = start->next;
                delete current;
                len--;
                if (len == 0) now = nullptr;
                return;
            }
            
            for (int i = 0; i < index; i++) {
                before = current;
                current = current->next;
            }
            
            before->next = current->next;
            if (current == now) now = before;
            delete current;
            len--;
        }
        
        int length() { return len; }
        
        ~List() {
            node* current = start;
            while (current != nullptr) {
                node* next = current->next;
                delete current;
                current = next;
            }
        }
    };
}

// Mix union
typedef union _mix {
    char c;
    int i;
    float f;
    char* s;
} mix;

// Helper functions
int convert_size_to_num(int m, int type) {
    if (type == 1) return m;
    if (type == 2 || type == 3) return m / sizeof(int);
    if (type == 4 || type == 5) return m / sizeof(mix);
    return -1;
}

void print_type(int x) {
    switch (x) {
        case 1: cout << "char\n"; break;
        case 2: cout << "int\n"; break;
        case 3: cout << "float\n"; break;
        case 4: cout << "string\n"; break;
        case 5: cout << "mix\n"; break;
        default: cout << "unknown type\n";
    }
}

// Block class
class Block {
private:
    int real_size = 0;
    
int forward_allocation(int m, char type) {
    if (type == 'c') {
        return m * 2;
    }
    else if (type == 'i' || type == 'f') {
        return (m % (2 * sizeof(int)) == 0)
            ? m * 1.5
            : ((m * 3 / (2 * sizeof(int)) + 1) * sizeof(int));
    }
    else if (type == 'm' || type == 's') {
        return (m % (4 * sizeof(mix)) == 0)
            ? m * 1.25
            : ((m * 5 / (4 * sizeof(mix)) + 1) * sizeof(mix));
    }

    return m;
}
    void free_resources() {
        if (type == 4) {
            for (int i = 0; i < convert_size_to_num(real_size, type); i++) {
                if (fwrite[i] == 1 || fwrite[i] == 11) {
                    free(((char**)add)[i]);
                }
            }
        } else if (type == 5) {
            for (int i = 0; i < convert_size_to_num(real_size, type); i++) {
                if (fwrite[i] == 5 || fwrite[i] == 15) {
                    free(((mix*)add)[i].s);
                }
            }
        }
    }

public:
    char* bname = nullptr;
    int size = 0;
    int type = 0;
    char* fwrite = nullptr;
    void* add = nullptr;
    char* fhist = nullptr;

    ~Block() {
        if (add) bfree();
    }

    void bmalloc() {  
        if (add != nullptr) {
            cout << "Block already allocated\n";
            return;
        } 
        
        cout << "Enter size of block: ";
        cin >> size;
        if (size <= 0) {
            cout << "Invalid size\n";
            return;
        }
        
        int n;
        char alloc_type;
        switch (type) {
            case 1:
                alloc_type = 'c';
                real_size = n = forward_allocation(size, 'c');
                add = malloc(n);
                break;
            case 2: case 3:
                alloc_type = 'i';
                real_size = n = forward_allocation(size, 'i');
                add = malloc(n);
                n = n / sizeof(int);
                break;
            case 4:
                alloc_type = 's';
                real_size = n = forward_allocation(size, 's');
                add = malloc(n);
                n = n / sizeof(char*);
                break;
            case 5:
                alloc_type = 'm';
                real_size = n = forward_allocation(size, 'm');
                add = malloc(n);
                n = n / sizeof(mix);
                break;
            default:
                cout << "Invalid type\n";
                return;
        }
        
        memset(add, 0, real_size);
        fwrite = new char[n](); // Zero-initialized
        fhist = new char[2 * n]();
        
        ofstream fout("history.txt", ios::app);
        fout << "malloc block: " << bname << ", type: " << type << ", size: " << size << "\n";
    }

    void brealloc() {
        if (add == nullptr) {
            cout << "Block not allocated\n";
            return;
        }
        
        cout << "Enter new size: ";
        int nsize;
        cin >> nsize;
        if (nsize <= 0) {
            cout << "Invalid size\n";
            return;
        }
        
        char ch;
        switch (type) {
            case 1: ch = 'c'; break;
            case 2: case 3: ch = 'i'; break;
            case 4: ch = 's'; break;
            case 5: ch = 'm'; break;
            default: ch = 'c';
        }
        
        ofstream fout("history.txt", ios::app);
        fout << "realloc block: " << bname << ", from " << size << " to " << nsize << "\n";
        
        if (nsize > real_size) {
            int new_real_size = forward_allocation(nsize, ch);
            int old_count = convert_size_to_num(real_size, type);
            int new_count = convert_size_to_num(new_real_size, type);
            
            void* new_add = realloc(add, new_real_size);
            if (!new_add) {
                cout << "Realloc failed\n";
                return;
            }
            
            char* new_fwrite = new char[new_count]();
            char* new_fhist = new char[2 * new_count]();
            
            memcpy(new_fwrite, fwrite, old_count);
            memcpy(new_fhist, fhist, 2 * old_count);
            
            if (new_real_size > real_size) {
                memset((char*)new_add + real_size, 0, new_real_size - real_size);
            }
            
            delete[] fwrite;
            delete[] fhist;
            
            add = new_add;
            fwrite = new_fwrite;
            fhist = new_fhist;
            size = nsize;
            real_size = new_real_size;
        } 
        else if (nsize < size) {
            cout << "Data may be lost. Continue? (Y/N): ";
            char selection;
            cin >> selection;
            
            if (toupper(selection) == 'Y') {
                size = nsize;
            }
        }
    }

    void bfree() {
        if (add == nullptr) return;
        
        free_resources();
        
        ofstream fout("history.txt", ios::app);
        fout << "free block: " << bname << ", type: " << type << ", size: " << size << "\n";
        
        free(add);
        delete[] bname;
        delete[] fwrite;
        delete[] fhist;
        
        add = nullptr;
        bname = nullptr;
        fwrite = nullptr;
        fhist = nullptr;
        size = 0;
        real_size = 0;
    }

    void print_mix(int index) {
        if (index < 0 || index >= convert_size_to_num(size, type)) {
            cout << "Invalid index\n";
            return;
        }
        
        switch (fwrite[index]) {
            case 2: case 12:
                cout << ((mix*)add)[index].c << endl;
                break;
            case 3: case 13:
                cout << ((mix*)add)[index].i << endl;
                break;
            case 4: case 14:
                cout << ((mix*)add)[index].f << endl;
                break;
            case 5: case 15:
                cout << ( ((mix*)add)[index].s ? ((mix*)add)[index].s : "(null)" ) << endl;
                break;
            default:
                cout << "Uninitialized\n";
        }
    }

    void bread() {
        if (add == nullptr) {
            cout << "Block not allocated\n";
            return;
        }
        
        cout << "Block: " << bname << " - Type: ";
        print_type(type);
        cout << "Size: " << size << endl;
        
        int choice;
        do {
            cout << "1. Read all indexes\n"
                 << "2. Read one index\n"
                 << "Choice: ";
            cin >> choice;
        } while (choice != 1 && choice != 2);
        
        int num = convert_size_to_num(size, type);
        ofstream fout("history.txt", ios::app);
        
        if (choice == 2) {
            int index;
            do {
                cout << "Enter index (0-" << num-1 << "): ";
                cin >> index;
            } while (index < 0 || index >= num);
            
            fout << "Read index " << index << " of block " << bname << "\n";
            
            switch (type) {
                case 1:
                    if (fwrite[index]) cout << ((char*)add)[index] << endl;
                    else cout << "Uninitialized\n";
                    break;
                case 2:
                    if (fwrite[index]) cout << ((int*)add)[index] << endl;
                    else cout << "Uninitialized\n";
                    break;
                case 3:
                    if (fwrite[index]) cout << ((float*)add)[index] << endl;
                    else cout << "Uninitialized\n";
                    break;
                case 4:
                    if (fwrite[index]) 
                        cout << ( ((char**)add)[index] ? ((char**)add)[index] : "(null)" ) << endl;
                    else cout << "Uninitialized\n";
                    break;
                case 5:
                    if (fwrite[index]) print_mix(index);
                    else cout << "Uninitialized\n";
                    break;
            }
        } else {
            fout << "Read entire block " << bname << "\n";
            
            for (int i = 0; i < num; i++) {
                if (!fwrite[i]) continue;
                
                cout << i << ": ";
                switch (type) {
                    case 1: cout << ((char*)add)[i]; break;
                    case 2: cout << ((int*)add)[i]; break;
                    case 3: cout << ((float*)add)[i]; break;
                    case 4: 
                        cout << ( ((char**)add)[i] ? ((char**)add)[i] : "(null)" ); 
                        break;
                    case 5: print_mix(i); continue;
                }
                cout << endl;
            }
        }
    }

    void bwrite() {
        if (add == nullptr) {
            cout << "Block not allocated\n";
            return;
        }
        
        if (fwrite[0] == 6) {
            cout << "Block is read-only\n";
            return;
        }
        
        cout << "Block: " << bname << " - Type: ";
        print_type(type);
        cout << "Size: " << size << endl;
        
        int choice;
        do {
            cout << "1. Set entire block read-only\n"
                 << "2. Set one index read-only\n"
                 << "3. Write to one index\n"
                 << "Choice: ";
            cin >> choice;
        } while (choice < 1 || choice > 3);
        
        int num = convert_size_to_num(size, type);
        ofstream fout("history.txt", ios::app);
        
        if (choice == 1) {
            fwrite[0] = 6;
            fout << "Set entire block " << bname << " as read-only\n";
            cout << "Block set to read-only\n";
            return;
        }
        
        int index;
        do {
            cout << "Enter index (0-" << num-1 << "): ";
            cin >> index;
        } while (index < 0 || index >= num);
        
        if (choice == 2) {
            if (fwrite[index] < 10) fwrite[index] += 10;
            fout << "Set index " << index << " of block " << bname << " as read-only\n";
            cout << "Index set to read-only\n";
            return;
        }
        
        // Actual writing
        if (fwrite[index] >= 11) {
            cout << "Index is read-only\n";
            return;
        }
        
        fout << "Write to index " << index << " of block " << bname << "\n";
        
        switch (type) {
            case 1:
                cout << "Enter char: ";
                cin >> ((char*)add)[index];
                fwrite[index] = 1;
                break;
            case 2:
                cout << "Enter int: ";
                cin >> ((int*)add)[index];
                fwrite[index] = 1;
                break;
            case 3:
                cout << "Enter float: ";
                cin >> ((float*)add)[index];
                fwrite[index] = 1;
                break;
            case 4: {
                cout << "Enter string length: ";
                int len;
                cin >> len;
                cin.ignore();
                
                if (fwrite[index] == 1) {
                    free(((char**)add)[index]);
                }
                
                char* str = new char[len + 1];
                cout << "Enter string: ";
                cin.getline(str, len + 1);
                ((char**)add)[index] = str;
                fwrite[index] = 1;
                break;
            }
            case 5: {
                int choice;
                do {
                    cout << "Choose type:\n"
                         << "1. char\n"
                         << "2. int\n"
                         << "3. float\n"
                         << "4. string\n"
                         << "Choice: ";
                    cin >> choice;
                } while (choice < 1 || choice > 4);
                
                cin.ignore();
                switch (choice) {
                    case 1:
                        cout << "Enter char: ";
                        cin >> ((mix*)add)[index].c;
                        fwrite[index] = 2;
                        break;
                    case 2:
                        cout << "Enter int: ";
                        cin >> ((mix*)add)[index].i;
                        fwrite[index] = 3;
                        break;
                    case 3:
                        cout << "Enter float: ";
                        cin >> ((mix*)add)[index].f;
                        fwrite[index] = 4;
                        break;
                    case 4: {
                        cout << "Enter string length: ";
                        int len;
                        cin >> len;
                        cin.ignore();
                        
                        if (fwrite[index] == 5) {
                            free(((mix*)add)[index].s);
                        }
                        
                        char* str = new char[len + 1];
                        cout << "Enter string: ";
                        cin.getline(str, len + 1);
                        ((mix*)add)[index].s = str;
                        fwrite[index] = 5;
                        break;
                    }
                }
                break;
            }
        }
        fhist[2 * index]++; // Increment write count
    }
};

// Process class
class Process {
public:
    char* pname = nullptr;
    int bnum = 0;
    link_list::List* allb = nullptr;

    ~Process() {
        delete[] pname;
        if (allb) {
            for (int i = 0; i < allb->length(); i++) {
                Block* blk = (Block*)allb->get(i);
                delete blk;
            }
            delete allb;
        }
    }

    void bcreator() {
        if (!allb) {
            allb = new link_list::List();
        }
        
        Block* blk = new Block();
        cout << "Enter block name: ";
        char name[256];
        cin >> name;
        blk->bname = new char[strlen(name) + 1];
        strcpy(blk->bname, name);
        
        do {
            cout << "Choose type:\n"
                 << "1. char\n"
                 << "2. int\n"
                 << "3. float\n"
                 << "4. string\n"
                 << "5. mix\n"
                 << "Choice: ";
            cin >> blk->type;
        } while (blk->type < 1 || blk->type > 5);
        
        allb->add_nodep(blk);
        bnum++;
    }

    int bsearch() {
        char name[256];
        int choice;
        
        do {
            cout << "Enter block name: ";
            cin >> name;
            
            for (int i = 0; i < bnum; i++) {
                Block* blk = (Block*)allb->get(i);
                if (strcmp(blk->bname, name) == 0) {
                    return i;
                }
            }
            
            cout << "Block not found. Search again?\n"
                 << "1. Yes\n"
                 << "2. No\n"
                 << "Choice: ";
            cin >> choice;
        } while (choice == 1);
        
        cout << "Create new block? (Y/N): ";
        char create;
        cin >> create;
        
        if (toupper(create) == 'Y') {
            bcreator();
            return bnum - 1;
        }
        
        return -1;
    }

    void remove_block() {
        int index = bsearch();
        if (index == -1) {
            cout << "Block not found\n";
            return;
        }
        
        Block* blk = (Block*)allb->get(index);
        ofstream fout("history.txt", ios::app);
        fout << "Process " << pname << ": remove block " << blk->bname << "\n";
        
        blk->bfree();
        allb->remove(index);
        bnum--;
    }
};

// Global functions
Process* alp = nullptr;

int pcreator() {
    if (nalp >= len_alp) {
        len_alp += 5;
        Process* new_alp = new Process[len_alp];
        for (int i = 0; i < nalp; i++) {
            new_alp[i] = alp[i];
        }
        delete[] alp;
        alp = new_alp;
    }
    
    cout << "Enter process name: ";
    char name[256];
    cin >> name;
    
    alp[nalp].pname = new char[strlen(name) + 1];
    strcpy(alp[nalp].pname, name);
    alp[nalp].bnum = 0;
    alp[nalp].allb = nullptr;
    
    return nalp++;
}

int psearch() {
    char name[256];
    int choice;
    
    do {
        cout << "Enter process name: ";
        cin >> name;
        
        for (int i = 0; i < nalp; i++) {
            if (strcmp(alp[i].pname, name) == 0) {
                cout << "Process: " << alp[i].pname 
                     << "\nBlocks: " << alp[i].bnum << endl;
                for (int j = 0; j < alp[i].bnum; j++) {
                    Block* blk = (Block*)alp[i].allb->get(j);
                    cout << "  " << j << ": " << blk->bname 
                         << " (Type: " << blk->type 
                         << ", Size: " << blk->size << ")\n";
                }
                return i;
            }
        }
        
        cout << "Process not found. Search again?\n"
             << "1. Yes\n"
             << "2. No\n"
             << "Choice: ";
        cin >> choice;
    } while (choice == 1);
    
    cout << "Create new process? (Y/N): ";
    char create;
    cin >> create;
    
    if (toupper(create) == 'Y') {
        return pcreator();
    }
    
    return -1;
}

void big_bang() {
    for (int i = 0; i < nalp; i++) {
        delete &alp[i];
    }
    delete[] alp;
    alp = nullptr;
    nalp = 0;
    len_alp = 0;
    
    ofstream fout("history.txt", ios::trunc);
}

// Main function
int main() {
    const char* commands[] = {
        "allocate", "deallocate", "reallocate", "write", "read", 
        "lam", "history", "clear", "exit"
    };
    
    while (true) {
        cout << "\nOptions:\n"
             << "allocate\ndeallocate\nreallocate\nwrite\nread\n"
             << "lam (List Allocated Memory)\nhistory\nclear\nexit\n"
             << "Enter command: ";
        
        string command;
        cin >> command;
        
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        
        if (command == commands[0]) { // allocate
            int pi = psearch();
            if (pi == -1) continue;
            
            if (alp[pi].allb == nullptr) {
                alp[pi].allb = new link_list::List();
            }
            
            alp[pi].bcreator();
            int bi = alp[pi].bnum - 1;
            Block* blk = (Block*)alp[pi].allb->get(bi);
            blk->bmalloc();
        }
        else if (command == commands[1]) { // deallocate
            int pi = psearch();
            if (pi == -1) continue;
            alp[pi].remove_block();
        }
        else if (command == commands[2]) { // reallocate
            int pi = psearch();
            if (pi == -1) continue;
            
            int bi = alp[pi].bsearch();
            if (bi == -1) continue;
            
            Block* blk = (Block*)alp[pi].allb->get(bi);
            blk->brealloc();
        }
        else if (command == commands[3]) { // write
            int pi = psearch();
            if (pi == -1) continue;
            
            int bi = alp[pi].bsearch();
            if (bi == -1) continue;
            
            Block* blk = (Block*)alp[pi].allb->get(bi);
            blk->bwrite();
        }
        else if (command == commands[4]) { // read
            int pi = psearch();
            if (pi == -1) continue;
            
            int bi = alp[pi].bsearch();
            if (bi == -1) continue;
            
            Block* blk = (Block*)alp[pi].allb->get(bi);
            blk->bread();
        }
        else if (command == commands[5]) { // lam
            int pi = psearch();
            if (pi == -1) continue;
            
            ofstream fout("history.txt", ios::app);
            fout << "List Allocated Memory for process: " << alp[pi].pname << "\n";
            
            cout << "\nAllocated blocks:\n";
            for (int i = 0; i < alp[pi].bnum; i++) {
                Block* blk = (Block*)alp[pi].allb->get(i);
                if (blk->add) {
                    cout << "  " << blk->bname << ": Type " << blk->type 
                         << ", Size " << blk->size << endl;
                }
            }
        }
        else if (command == commands[6]) { // history
            ifstream fin("history.txt");
            string line;
            
            cout << "\nHistory:\n";
            while (getline(fin, line)) {
                cout << line << endl;
            }
        }
        else if (command == commands[7]) { // clear
            system("clear || cls");
            ofstream fout("history.txt", ios::app);
            fout << "Terminal cleared\n";
        }
        else if (command == commands[8]) { // exit
            break;
        }
        else {
            cout << "Invalid command\n";
        }
    }

    big_bang();
    return 0;
}