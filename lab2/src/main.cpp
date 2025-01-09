#include <iostream>
#include <cstring>
#include <stdbool.h>
#include <fstream>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <sstream>

const int BIT_COUNT = 5;

typedef class Patricia {
private:
    struct Node {
        std::string key;
        unsigned long long value;
        size_t index;
        Node *right, *left;

        Node(const std::string& key, const unsigned long long& value, const int& index)
            : key(key), value(value), index(index), left(nullptr), right(nullptr) { }

        ~Node(){}
    };

    Node *root = nullptr;
    Node* search(const std::string& finding) const;
    Node** Search_Parent(const std::string& finding) const;
    void Insert(const std::string& key, const unsigned long long& value, const size_t& index);

    void SaveData(const Patricia::Node *node, std::ofstream &stream);
    void RecursiveSave(const Patricia::Node *node, std::ofstream &stream);
    Node* LoadData(std::ifstream &stream);
public:

    void Add(const std::string& key, unsigned long long value);
    unsigned long long  Get_Value(const std::string& finding);
    void Remove(const std::string& key);
    void DeleteNode(Node* node);
    void DeletePatricia();

    void SaveToFile(const std::string &path);
    void LoadFromFile(const std::string& path);
} patr;


std::string ToLowercase(std::string str) {
    for (char& c : str) {
        c = std::tolower(c);
    }
    return str;
}

Patricia::Node* Patricia::search(const std::string& finding) const {
    Node *currentNode = root->left, *prevNode = root; //создадим два указателя для отслеживания текущей вершины и выхода из функции поиска
    while (currentNode->index > prevNode->index) {
        size_t charIdx = (currentNode->index - 1) / BIT_COUNT; // получаем индекс символа, который надо проверить из исходной строки
        if (charIdx >= finding.size()) { //проверяем выход индекса за пределы нужной строки
            prevNode = currentNode;
            currentNode = currentNode->left;  //если индекс вышел за пределы, то получаем нулевой бит и перемещаемся по левому поддереву
            continue;
        }
        char currentChar = finding[charIdx]; //берем текущей символ из нужной строки
        int offset = (BIT_COUNT - 1 - ((currentNode->index - 1) % BIT_COUNT)); //высчитываем смещение так, чтобы нужный бит остался в крайнем правом положении
        bool currentBit = (currentChar >> offset) & 1; //если в конце битовой цепочки ненулевой бит, то получаем true, иначе - false
        prevNode = currentNode; // запоминаем предыдущую вершину

        if (currentBit) {
            currentNode = currentNode->right;
        }
        else {
            currentNode = currentNode->left;
        }
    }
    return currentNode; //возвращаем currentNode, когда условие цикла перестанет выполняться
}

// предположим следующую ситуацию:

// надо найти по ключу узел дерева:

//    |--- 0, a ------------|
//    |    |                |
//    |   1,y-----------    |
//    |   |            |    |
//    |  2,i--|        2,t--|
//    |  |  | |      |-| |
//    |_4,b_ 5,m_   _3,q-|
//        |_|  |_| |_|

// Есть следующее дерево В узлах записан ключ и индекс. Будем искать вершину с ключом t.
// 1) Итерация:
//     curNode = y,1
//     prevNode = 0,a
//     charIdx = 0 / 5 = 0

//     currentChar = finding[0] = 't'
//     offset = 4 - 0 % 5 = 4 - 0 = 4, значит смещение вправо на 4 бита.
    
//     Смещение:
//     №ит. | битовая маска ключа
//     0    | 11001    
//     1    | 01100
//     2    | 00110
//     3    | 00011
//     4    | 00001

//     Последний бит = 1 => идем в правое поддерево

// 2) Итерация:
//     curNode = t,2
//     prevNode = 1,y
//     charIdx = 1 / 5 = 0

//     currentChar = finding[0] = 't'
//     offset = 4 - 1 % 5 = 4 - 1 = 3, значит смещение вправо на 3 бита.
    
//     Смещение:
//     №ит. | битовая маска ключа
//     0    | 10100
//     1    | 01010
//     2    | 00101
//     3    | 00010

//     Последний бит = 0 => идем в левое поддерево

// 3) Итерация:
//     curNode = q,3
//     prevNode = t,2
//     charIdx = 2 / 5 = 0

//     currentChar = finding[0] = 't'
//     offset = 4 - 2 % 5 = 4 - 2 = 2, значит смещение вправо на 2 бита.
    
//     Смещение:
//     №ит. | битовая маска ключа
//     0    | 10100
//     1    | 01010
//     2    | 00101

//     Последний бит = 1 => идем в правое поддерево 

// 4) Итерация:
//     curNode = t,2
//     prevNode = q,3

// Цикл не будет продолжаться, потому что curNode->index < prevNode->index, значит возвращаем curNode - это и есть нужная вершина t. Поиск закончен.

void Patricia::Insert(const std::string& key, const unsigned long long& value, const size_t& index){
    Node *currentNode = root->left, *prevNode = root;

    while(currentNode->index > prevNode->index){
        if(currentNode->index > index) //выходим из цикла, как только индекс текущей веришны станет больше, чем индекс вставляемой
            break;

        size_t charIndex = (currentNode->index - 1) / BIT_COUNT;
        if(charIndex >= key.size()){
            prevNode = currentNode;
            currentNode = currentNode->left;
            continue;
        }
        char currentChar = key[charIndex];
        int offset = (BIT_COUNT - 1 - ((currentNode->index - 1) % BIT_COUNT));
        bool currentBit = (currentChar >> offset) & 1;

        prevNode = currentNode;
        currentBit  ? currentNode = currentNode->right
                    : currentNode = currentNode->left;
    }       // имеем дело с той же самой функцией поиска и на этом этапе получаем currentNode - вершина с индексом больше, чем у вставляемой
            // prevNode - вершина с индексом меньше, чем у вставляемой curnode - m, 5; prevnode - l,2

    char getCharFromKey = key[(index - 1) / BIT_COUNT];  //n
    bool getBit = getCharFromKey >> (BIT_COUNT - 1 - (index - 1) % BIT_COUNT) & 1; //4 - 3 = 1, have true
    Node *newNode = new Node(key, value, index);

    if(prevNode->left == currentNode) // Если у родителя вершина, которая была на месте добавленной, была левым ребенком, то на ее место встанет новая
        prevNode->left = newNode;
    else
        prevNode->right = newNode; // Если у родителя вершина, которая была на месте добавленной, была правым ребенком, то на ее место встанет новая

    getBit  ? (newNode->right = newNode, newNode->left = currentNode) // если бит единичка, то правая ссылка на себя, левая на старого "потомка" нашего родителя
            : (newNode->left = newNode, newNode->right = currentNode); // если бит нулевой, правая на ссылка на старого "потомка" нашего родителя, левая на себя
}

void Patricia::Add(const std::string& key, unsigned long long value){
    if(!root){
        root = new Node(key, value, 0);   // если нет корня, то создаем из добавляемой вершины корень
        root->left = root;
        // std::cout << "OK" << std::endl;
        return;
    }

    Node *foundNode = search(key);
    if(foundNode->key == key)
        throw std::runtime_error("Exist\n");   // Проверяем существует ли добавялемая вершина
    
    bool run = true;
    size_t charIndex = 0; //индекс символа который мы берем из ключа
    while(run){
        char foundedKey = (foundNode->key.size() > charIndex ? foundNode->key[charIndex] : '\0');
        char inputKey   = (key.size() > charIndex ? key[charIndex] : '\0'); //проверка на то, выходит ли индекс за пределы искомого ключа и за пределы найденного ключа
        for(size_t i = 0; i < BIT_COUNT; ++i){ //если нет, берем по индексу, иначе - нулевой символ
            bool foundedKeyBit = foundedKey >> (BIT_COUNT - 1 - i) & 1; 
            bool inputKeyBit = inputKey >> (BIT_COUNT - 1 - i) & 1; //сдвигаем биты вправо и сравниваем их друг с другом
            if(foundedKeyBit != inputKeyBit){ //как только нашли отличие в битах, добавляем искомый элемент в дерево
                Insert(key, value, charIndex * BIT_COUNT + i + 1); // индексом элемента будет служить индекс несовпадающего бита
                // std::cout << "OK" << std::endl;
                run = false;
                break;
            }
        }
        ++charIndex; //увеличиваем номер индекса для проверки следующего символа ключа
    }
}

unsigned long long Patricia::Get_Value(const std::string& finding) { // Получение значения из вершины по ключу
    if (!root) {
        throw std::runtime_error("NoSuchWord\n"); // если нет корня, то значение мы не получим
    }

    Node* get = search(finding); //пробуем найти его в дереве, если нашли, то возвращаем значение
    if (get->key == finding) {
        return get->value;
    }
    throw std::runtime_error("NoSuchWord\n"); // если не нашли, то значение мы не получим
}

Patricia::Node** Patricia::Search_Parent(const std::string& finding) const { //тот же самый поиск вершины, но теперь возвращается массив, содержащий так же указатель на родителя
    Node** arr = new Node*[3];
    Node *currentNode = root->left, *prevNode = root, *prevPrevNode = root;
    while(currentNode->index > prevNode->index) {
        size_t charIndex = (currentNode->index - 1) / BIT_COUNT;

        if(charIndex >= finding.size()){
            prevPrevNode = prevNode;
            prevNode = currentNode;
            currentNode = currentNode->left;
            continue;
        }

        char currentChar = finding[charIndex];
        int offset = (BIT_COUNT - 1 - ((currentNode->index - 1) % BIT_COUNT));
        bool currentBit = (currentChar >> offset) & 1;

        prevPrevNode = prevNode;
        prevNode = currentNode;
        currentBit  ? currentNode = currentNode->right
                    : currentNode = currentNode->left;
    }

    arr[0] = currentNode;
    arr[1] = prevNode;
    arr[2] = prevPrevNode;

    return arr;
}

void Patricia::Remove(const std::string& key) {
    if (!root) {
        throw std::runtime_error("NoSuchWord\n");
    }

    Node** arr = Search_Parent(key); // записываем в массив указатель на удаляемую вершину, ее владельца и владельца владельца
    Node* deleteNode = arr[0]; // удаляемая
    Node* ownerDeleteNode = arr[1]; // владелец удаляемой
    Node* parentOwnerDeleteNode = arr[2]; // владелец владельца удаляемой

    if (deleteNode->key != key) {
        throw std::runtime_error("NoSuchWord\n");
    }

    if (deleteNode == root && root->left == root) { // первый кейс, когда удаляемый элемент - корень
        delete root;
        root = nullptr;
        std::cout << "OK" << std::endl;
        return;
    }

    if (ownerDeleteNode == deleteNode) { // второй кейс, когда удаляемый элемент - лист (одна его ссылка указывает на самого себя, другая - на потомка)
        if (parentOwnerDeleteNode->right == deleteNode) { // если удаляемый узел - правый потомок
            if (deleteNode->right == deleteNode) { //если правая ссылка удаляемого узла указывает на удаляемый узел
                parentOwnerDeleteNode->right = deleteNode->left; // то правым потомком владельца владельца удаляемого узла становится левый потомок удаляемого узла
            }
            else {   //если левая ссылка удаляемого узла указывает на удаляемый узел
                parentOwnerDeleteNode->right = deleteNode->right; // то правым потомком владельца владельца удаляемого узла становится правый потомок удаляемого узла
            }
        }
        else {    //если удаляемый узел - левый потомок
            if (deleteNode->right == deleteNode) { //если правая ссылка удаляемого узла указывает на удаляемый узел
                parentOwnerDeleteNode->left = deleteNode->left; // то левым потомком владельца владельца удаляемого становится левый потомок удаляемого узла
            }
            else {   //если левая ссылка удаляемого узла указывает на удаляемый узел
                parentOwnerDeleteNode->left = deleteNode->right; // то левым потомком владельца владельца удаляемого узла становится правый потомок удаляемого узла
            }
        }
        delete deleteNode;
        std::cout << "OK" << std::endl;
        return;
    }


    //третий кейс, когда удаляемый элемент имеет двух детей
    Node** arr1 = Search_Parent(ownerDeleteNode->key);  // ищем владельца удаляемого элемента (эл-т, с обратной ссылкой на удаляемый) вместе с владельцем этого владельца (эл-т с обратной ссылкой на владельца удаляемого)
    Node* OwnerOwnerDeleteNode = arr1[1]; //достаем ссылку на владельца владельца удаляемого эл-та

    // std::cout << parentOwnerDeleteNode->value << std::endl;
    // std::cout << ownerDeleteNode->value << std::endl;
    // std::cout << OwnerOwnerDeleteNode->value << std::endl;

    deleteNode->key = ownerDeleteNode->key;  //запихиваем ключ и значение владельца удаляемого эл-та в удаляемый эл-т
    deleteNode->value = ownerDeleteNode->value;

    if(OwnerOwnerDeleteNode == ownerDeleteNode){ // если владелец удаляемого элемента имеет обратную ссылку на себя, то (он также является владельцем владельца)
        if (parentOwnerDeleteNode->right == ownerDeleteNode) { // если владелец является правым ребенком
            parentOwnerDeleteNode->right = deleteNode; // то правым ребенком у родителя владельца станет удаляемый узел
        }
        else {
            parentOwnerDeleteNode->left = deleteNode; // иначе левым ребенком станет удаляемый узел
        }
    }
    else { // иначе
        if (parentOwnerDeleteNode->right == ownerDeleteNode) { //если владелец удаляемого эл-та яв-ся правым ребенком своего родителя
            if (ownerDeleteNode->right == deleteNode) { //если удалемый узел яв-ся правым потомком владельца
                parentOwnerDeleteNode->right = ownerDeleteNode->left; // то правым ребенком родителя владельца удаляемого узла становится левый потомок владельца удаляемого узла
            }
            else { //иначе правым ребенком родителя владельца удаляемого узла становится правый ребенок владельца удаляемого эл-та
                parentOwnerDeleteNode->right = ownerDeleteNode->right;
            }
        }
        else { //иначе, если владелец удаляемого эл-та яв-ся левым ребенком своего родитителя
            if (ownerDeleteNode->right == deleteNode) {
                parentOwnerDeleteNode->left = ownerDeleteNode->left; // то левым ребенком родителя владельца удаляемого эл-та становится левый потомок владельца удаляемого эл-та
            }
            else { // иначе левым ребенком становится правый потомок владельца удаляемого эл-та
                parentOwnerDeleteNode->left = ownerDeleteNode->right;
            }
        }
        if (OwnerOwnerDeleteNode->right == ownerDeleteNode) { // если правая ссылка владельца владельца удаляемого элемента указывает на владельца удаляемого эл-та
            OwnerOwnerDeleteNode->right = deleteNode; // то правая ссылка владельца владельца удаляемого эл-та указывает на удаляемый эл-т
        }// если правая ссылка владельца владельца удаляемого элемента указывает на владельца удаляемого эл-та
        else {    // если левая ссылка владельца владельца удаляемого элемента указывает на владельца удаляемого эл-та
            OwnerOwnerDeleteNode->left = deleteNode; // то левая ссылка владельца владельца удаляемого эл-та указывает на удаляемый эл-т
        }
    }
    delete ownerDeleteNode;
    std::cout << "OK" << std::endl;
}

void Patricia::RecursiveSave(const Patricia::Node *node, std::ofstream &stream) {  //Рекурсвиынй обход дерева
    SaveData(node, stream); // сохраняем данные текущего узла и спускаемся вниз по дереву
    if (node->left->index > node->index) {
        RecursiveSave(node->left, stream);
    }
    if (node->right->index > node->index) {
        RecursiveSave(node->right, stream);
    }
}

void Patricia::SaveToFile(const std::string &path) { //Сохранения данных дерева в бинарный файл
    std::ofstream fout;
    fout.open(path, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);  //открываем файл для записи бинарных данных из потока ofstream
    if (!root) { // если нет корня, то сохранять нечего - закрываем файл, возвращаем управление
        fout.close();
        return;
    }

    SaveData(root, fout); // вызываем функцию для сохранения данных текущего узла (в данном случае корня)
    if (root != root->left) { // если у корня есть потомки, то спускаемся по дереву вниз и выполняем обход, считывая данные в файл 
        RecursiveSave(root->left, fout);
    }
    fout.close();
}

void Patricia::SaveData(const Patricia::Node *node, std::ofstream &stream) { //записываем данные текущего узла в файл
    std::string key = node->key;
    size_t keySize = key.size();
    unsigned long long value = node->value;
    stream.write((char *) (&keySize), sizeof(size_t));
    stream.write(key.c_str(), sizeof(char) * keySize);
    stream.write((char *) (&value), sizeof(unsigned long long));
}


void Patricia::LoadFromFile(const std::string &path) { //загрузка словаря из файла
    std::ifstream fin;
    fin.open(path, std::ios_base::in | std::ios_base::binary); // открываем файл на чтение в бинарном виде
    Node* curNode;
    while ((curNode = LoadData(fin)) != nullptr) { //проверяем, если из файла считались данные узла, он успешно создался, то тогда добавляем его в дерево
        Add(curNode->key, curNode->value);
        delete curNode;
    }
}

Patricia::Node *Patricia::LoadData(std::ifstream &stream) { // проверка корректности создания узла
    if (stream.eof()) { // если файл пуст - возвращаем nullptr
        return nullptr;
    }
    size_t keySize;
    std::string key;
    unsigned long long value;
    stream.read((char *) &keySize, sizeof(size_t)); // считываем первый параметр - длину ключа
    if (!stream) {
        return nullptr;
    }
    key.resize(keySize); // делаем размер строки key таким же, как длина ключа
    stream.read(key.data(), sizeof(char) * keySize);
    stream.read((char *) &value, sizeof(sizeof(unsigned long long))); // счиытываем значения из файла и кладем их в переменные
    Node *node = new Node(key, value, 0); // выделяем память для узла, используя полученные из файла данные
    return node;
}

void::Patricia::DeleteNode(Node* node) { // удаление узла из дерева
    if (node->left->index > node->index) { // рекурсивно удаляем узел и его потомков
        DeleteNode(node->left);
    }
    if (node->right->index > node->index) {
        DeleteNode(node->right);
    }

    delete node;
}

void::Patricia::DeletePatricia() { // очистка дерева
    if (!root) { // если корень пустой - возвращаем управление, очищать нечего
        return;
    }
    if (root != root->left) { // если у корня есть потомок - удаялем его и всех его потомков
        DeleteNode(root->left);
    }
    delete root;
    root = nullptr; 
}  


int main () {
    patr *PATRICIA = new patr();
    std::string command;
    while (std::cin >> command) {
        if (command == "+") {
            std::string key;
            std::cin >> key;
            unsigned long long value;
            std::cin >> value;
            try {
                std::string lowerKey = ToLowercase(key);
                PATRICIA->Add(lowerKey, value);
                std::cout << "OK" << std::endl;
            }
            catch (std::runtime_error& e) {
                std::cout << e.what();
            }
        }
        else if (command == "-") {
            std::string key;
            std::cin >> key;
            try {
                std::string lowerKey = ToLowercase(key);
                PATRICIA->Remove(lowerKey);
            }
            catch (std::runtime_error& e) {
                std::cout << e.what();
            }
        }
        else if (command == "!") {
            std::string command2;
            std::cin >> command2;
            if (command2 == "Save") {
                std::string path;
                std::cin >> path;
                PATRICIA->SaveToFile(path);
            }
            else if (command2 == "Load") {
                std::string path;
                std::cin >> path;
                PATRICIA->DeletePatricia();
                PATRICIA->LoadFromFile(path);
            }
            std::cout << "OK" << std::endl; 
        }
        else {
            try {
                std::string lowerCommand = ToLowercase(command);
                unsigned long long val = PATRICIA->Get_Value(lowerCommand);
                std::cout << "OK: " << val << std::endl;
            }
            catch (std::runtime_error& e) {
                std::cout << e.what();
            }
        }
    }
    return 0;
}