#include <iostream>
#include <fstream>
#include <sstream>
#include "Tree.h"
#include "HashSet.h"
#include <string>
using namespace std;

// Структура для хранения информации о студенте
struct Student {
    string FIO;         // ФИО студента
    int number;         // Номер группы
    int grades[5];      // Оценки за последнюю сессию
    int scholarship;    // Размер стипендии
    string email;       // Электронная почта
    // Конструктор по умолчанию
    Student() : FIO(""), number(0), scholarship(0), email("") {
        for (int i = 0; i < 5; i++) {
            grades[i] = 0;
        }
    }
    Student(string FIO, int number, int gradesArray[5], int scholarship, string email)
        : FIO(FIO.substr(0, 50)), number(number), scholarship(scholarship), email(email) {
        for (int i = 0; i < 5; i++) {
            this->grades[i] = gradesArray[i];
        }
    }
    int getTotalScholarship() {
        return scholarship;
    }

    // Возвращает общую сумму баллов студента
    int getTotalGrades() {
        int total = 0;
        for (int grade : grades) {
            total += grade;
        }
        return total;
    }

};

// Класс для хранения группы студентов
class Group {
private:
    int groupNumber;
    HashSet<Student> students;
public:
    Group(int groupNumber) : groupNumber(groupNumber) {}

    // Добавление студента в группу
    void addStudent(Student& student) {
        students.add(student);
    }

    // Удаление студента из группы по ФИО
    void removeStudent(std::string& FIO) {
        students.remove(FIO);
    }

    // Получение общей суммы стипендии всех студентов группы
    int getTotalScholarship(){
        int total = 0;
        Student* studentArray = students.ToArray();
        for (int i = 0; i < students.count(); i++) {
            total += studentArray[i].getTotalScholarship();
        }
        delete[] studentArray;
        return total;
    }
    // Получение общей суммы баллов всех студентов группы
    int getTotalGrades() {
        int total = 0;
        Student* studentArray = students.ToArray();
        for (int i = 0; i < students.count(); i++) {
            total += studentArray[i].getTotalGrades();
        }
        delete[] studentArray;
        return total;
    }
    // Получение номера группы
    int getGroupNumber() {
        return groupNumber;
    }

    // Вывод в консоль информации о всех студентах группы
    void printStudents() {
        Student* studentArray = students.ToArray();
        for (int i = 0; i < students.count(); i++) {
            cout << "ФИО: " << studentArray[i].FIO << ", Номер группы: " << studentArray[i].number
                << ", Стипендия: " << studentArray[i].scholarship << ", Email: " << studentArray[i].email << endl;
            cout << "Оценки: ";
            for (int j = 0; j < 5; j++) {
                cout << studentArray[i].grades[j] << " ";
            }
            cout << endl;
        }
        delete[] studentArray;
    }

    // Сохранение информации о группе в файл
    void saveToFile(ofstream& file) {
        Student* studentArray = students.ToArray();
        for (int i = 0; i < students.count(); i++) {
            file << studentArray[i].FIO << "," << studentArray[i].number << "," << studentArray[i].scholarship << "," << studentArray[i].email;
            for (int j = 0; j < 5; j++) {
                file << "," << studentArray[i].grades[j];
            }
            file << endl;
        }
        delete[] studentArray;
    }

    // Получение информации о группе из файла
    void loadFromFile(ifstream& file) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string FIO, email;
            int number, scholarship, grades[5];
            getline(ss, FIO, ',');
            ss >> number;
            ss.ignore();
            ss >> scholarship;
            ss.ignore();
            getline(ss, email, ',');
            for (int i = 0; i < 5; i++) {
                ss >> grades[i];
                if (i < 4) ss.ignore();
            }
            Student student(FIO, number, grades, scholarship, email);
            addStudent(student);
        }
    }
};

// Класс для управления потоками студентов
class Flow {
private:
    Tree<Group*> groupTree;

public:
    // Поиск группы по номеру
    Group* findGroup(int groupNumber) {
        Node<Group*>* root = groupTree.getRoot();
        while (root != nullptr) {
            if (groupNumber == root->key) {
                return root->data;
            }
            else if (groupNumber < root->key) {
                root = root->left;
            }
            else {
                root = root->right;
            }
        }
        return nullptr;
    }
    // Добавление группы в поток
    void addGroup(int groupNumber) {
        groupTree.add(groupNumber, new Group(groupNumber));
    }
    // Удаление группы из потока по номеру
    void removeGroup(int groupNumber) {
        Group* group = findGroup(groupNumber);
        if (group) {
            delete group;
            groupTree.remove(groupNumber);
        }
    }

    // Добавление студента в группу
    void addStudentToGroup(int groupNumber, Student& student) {
        Group* group = findGroup(groupNumber);
        if (group) {
            group->addStudent(student);
        }
        else {
            Group* newGroup = new Group(groupNumber);
            newGroup->addStudent(student);
            groupTree.add(groupNumber, newGroup);
        }
    }
    // Удаляет студента из группы по ФИО
    void removeStudentFromGroup(int groupNumber, string& FIO) {
        Group* group = findGroup(groupNumber);
        if (group) {
            group->removeStudent(FIO);
        }
    }
    // Получение списка номеров групп с соотношением стипендия/баллы выше заданного значения
    List<int> getGroupsWithScholarshipRatioAbove(double ratio) {
        List<int> result;
        int* groupsArray = groupTree.ToArray(Tree<Group*>::Infix);
        for (int i = 0; i < groupTree.count(); i++) {
            Group* group = findGroup(groupsArray[i]);
            if (group && group->getTotalGrades() > 0) {
                double currentRatio = static_cast<double>(group->getTotalScholarship()) / group->getTotalGrades();
                if (currentRatio > ratio) {
                    result.add(group->getGroupNumber());
                }
            }
        }
        delete[] groupsArray;
        return result;
    }
    // Вывод в консоль информации о всех группах и студентах
    void printAllGroups() {
        int* groupsArray = groupTree.ToArray(Tree<Group*>::Infix);
        for (int i = 0; i < groupTree.count(); i++) {
            Group* group = findGroup(groupsArray[i]);
            cout << "Группа номер: " << group->getGroupNumber() << endl;
            group->printStudents();
        }
        delete[] groupsArray;
    }
    // Сохранение информации о всех группах и студентах в файл
    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            int* groupsArray = groupTree.ToArray(Tree<Group*>::Infix);
            for (int i = 0; i < groupTree.count(); i++) {
                Group* group = findGroup(groupsArray[i]);
                file << "Группа номер: " << group->getGroupNumber() << endl;
                group->saveToFile(file);
            }
            delete[] groupsArray;
            file.close();
        }
    }
    // Получение информации о всех группах и студентах из файла
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (line.find("Группа номер:") != string::npos) {
                    int groupNumber = stoi(line.substr(line.find(":") + 1));
                    Group* group = new Group(groupNumber);
                    group->loadFromFile(file);
                    groupTree.add(groupNumber, group);
                }
            }
            file.close();
        }
    }
};

void printMenu() {
    cout << "Меню:\n";
    cout << "1. Добавить группу\n";
    cout << "2. Удалить группу\n";
    cout << "3. Добавить студента в группу\n";
    cout << "4. Удалить студента из группы\n";
    cout << "5. Показать всех студентов\n";
    cout << "6. Показать группы с соотношением стипендия/баллы выше заданного\n";
    cout << "7. Сохранить данные в файл\n";
    cout << "8. Загрузить данные из файла\n";
    cout << "9. Выход\n";
}

int main()
{
    Flow flow;
    int choice;
    setlocale(LC_ALL, "RU");
    while (true) {
        printMenu();
        cin >> choice;

        if (choice == 9) {
            break;
        }

        switch (choice) {
        case 1: {
            int groupNumber;
            cout << "Введите номер группы: ";
            cin >> groupNumber;
            flow.addGroup(groupNumber);
            break;
        }
        case 2: {
            int groupNumber;
            cout << "Введите номер группы: ";
            cin >> groupNumber;
            flow.removeGroup(groupNumber);
            break;
        }
        case 3: {
            int groupNumber;
            string FIO, email;
            int scholarship, grades[5];
            cout << "Введите номер группы: ";
            cin >> groupNumber;
            cout << "Введите ФИО студента: ";
            cin.ignore();
            getline(cin, FIO);
            cout << "Введите стипендию студента: ";
            cin >> scholarship;
            cout << "Введите email студента: ";
            cin.ignore();
            getline(cin, email);
            cout << "Введите 5 оценок студента: ";
            for (int i = 0; i < 5; i++) {
                cin >> grades[i];
            }
            Student student(FIO, groupNumber, grades, scholarship, email);
            flow.addStudentToGroup(groupNumber, student);
            break;
        }
        case 4: {
            int groupNumber;
            string FIO;
            cout << "Введите номер группы: ";
            cin >> groupNumber;
            cout << "Введите ФИО студента: ";
            cin.ignore();
            getline(cin, FIO);
            flow.removeStudentFromGroup(groupNumber, FIO);
            break;
        }
        case 5: {
            flow.printAllGroups();
            break;
        }
        case 6: {
            double ratio;
            cout << "Введите соотношение стипендия/баллы: ";
            cin >> ratio;
            List<int> result = flow.getGroupsWithScholarshipRatioAbove(ratio);
            for (int i = 0; i < result.count(); i++) {
                cout << "Группа номер: " << result.elementAt(i) << endl;
            }
            break;
        }
        case 7: {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;
            flow.saveToFile(filename);
            break;
        }
        case 8: {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;
            flow.loadFromFile(filename);
            break;
        }
        default:
            cout << "Неверный выбор. Пожалуйста, выберите снова.\n";
        }
    }
    
}