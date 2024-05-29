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
    // Методы для редактирования данных студента
    void setFIO(string& newFIO) {
        FIO = newFIO;
    }
    void setNumber(int newNumber) {
        number = newNumber;
    }
    void setGrades(int newGrades[5]) {
        for (int i = 0; i < 5; i++) {
            grades[i] = newGrades[i];
        }
    }
    void setScholarship(int newScholarship) {
        scholarship = newScholarship;
    }
    void setEmail(string& newEmail) {
        email = newEmail;
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

    // Удаление студента из группы по email
    void removeStudent(std::string& email) {
        students.remove(email);
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

    Student* findStudent(const string& email) {
        return students.findByEmail(email);
    }
    // Обновление информации о студенте
    bool updateStudentFIO(string& email, string& newFIO) {
        Student* student = findStudent(email);
        if (student) {
            student->setFIO(newFIO);
            return true;
        }
        return false;
    }

    bool updateStudentNumber(string& email, int newNumber) {
        Student* student = findStudent(email);
        if (student) {
            student->setNumber(newNumber);
            return true;
        }
        return false;
    }

    bool updateStudentGrades(string& email, int newGrades[5]) {
        Student* student = findStudent(email);
        if (student) {
            student->setGrades(newGrades);
            return true;
        }
        return false;
    }

    bool updateStudentScholarship(string& email, int newScholarship) {
        Student* student = findStudent(email);
        if (student) {
            student->setScholarship(newScholarship);
            return true;
        }
        return false;
    }

    bool updateStudentEmail(string& email, string& newEmail) {
        Student* student = findStudent(email);
        if (student) {
            student->setEmail(newEmail);
            return true;
        }
        return false;
    }

    // Фильтрация студентов по заданному значению
    void filterStudents(int field, const string& value, int intValue = 0) {
        Student* studentArray = students.ToArray();
        for (int i = 0; i < students.count(); i++) {
            bool match = false;
            switch (field) {
            case 1:
                if (studentArray[i].FIO == value) match = true;
                break;
            case 2:
                if (studentArray[i].email == value) match = true;
                break;
            case 3:
                for (int j = 0; j < 5; j++) {
                    if (studentArray[i].grades[j] == intValue) match = true;
                }
                break;
            case 4:
                if (studentArray[i].scholarship == intValue) match = true;
                break;
            case 5:
                if (studentArray[i].number == intValue) match = true;
                break;
            }
            if (match) {
                cout << "ФИО: " << studentArray[i].FIO << ", Номер группы: " << studentArray[i].number
                    << ", Стипендия: " << studentArray[i].scholarship << ", Email: " << studentArray[i].email << endl;
                cout << "Оценки: ";
                for (int j = 0; j < 5; j++) {
                    cout << studentArray[i].grades[j] << " ";
                }
                cout << endl;
            }
        }
        delete[] studentArray;
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
    // Удаляет студента из группы по email
    void removeStudentFromGroup(int groupNumber, string& email) {
        Group* group = findGroup(groupNumber);
        if (group) {
            group->removeStudent(email);
        }
    }


    // Редактирование данных студента в группе
    bool editStudentFIOInGroup(int groupNumber, string& email, string& newFIO) {
        Group* group = findGroup(groupNumber);
        if (group) {
            return group->updateStudentFIO(email, newFIO);
        }
        return false;
    }

    bool editStudentNumberInGroup(int currentGroupNumber, string& email, int newGroupNumber) {
        Group* currentGroup = findGroup(currentGroupNumber);
        Group* newGroup = findGroup(newGroupNumber);
        if (currentGroup && newGroup) {
            Student* student = currentGroup->findStudent(email);
            if (student) {
                newGroup->addStudent(*student);
                currentGroup->removeStudent(email);
                delete student;
                return true;
            }
        }
        return false;
    }

    bool editStudentGradesInGroup(int groupNumber, string& email, int newGrades[5]) {
        Group* group = findGroup(groupNumber);
        if (group) {
            return group->updateStudentGrades(email, newGrades);
        }
        return false;
    }

    bool editStudentScholarshipInGroup(int groupNumber, string& email, int newScholarship) {
        Group* group = findGroup(groupNumber);
        if (group) {
            return group->updateStudentScholarship(email, newScholarship);
        }
        return false;
    }

    bool editStudentEmailInGroup(int groupNumber, string& email, string& newEmail) {
        Group* group = findGroup(groupNumber);
        if (group) {
            return group->updateStudentEmail(email, newEmail);
        }
        return false;
    }
    // Фильтрация студентов по заданному значению
    void filterStudents(int field, const string& value, int intValue = 0) {
        int* groupsArray = groupTree.ToArray(Tree<Group*>::Infix);
        for (int i = 0; i < groupTree.count(); i++) {
            Group* group = findGroup(groupsArray[i]);
            group->filterStudents(field, value, intValue);
        }
        delete[] groupsArray;
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
    void saveToFile(string& filename) {
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
    void loadFromFile(string& filename) {
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
    cout << "9. Редактировать данные студента\n";
    cout << "10. Фильтровать студентов\n";
    cout << "11. Выход\n";
}

int main()
{
    Flow flow;
    int choice;
    setlocale(LC_ALL, "RU");
    while (true) {
        printMenu();
        cin >> choice;

        if (choice == 11) {
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
            string email;
            cout << "Введите номер группы: ";
            cin >> groupNumber;
            cout << "Введите email студента: ";
            cin.ignore();
            getline(cin, email);
            flow.removeStudentFromGroup(groupNumber, email);
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
        case 9: {
            int groupNumber;
            string email;
            cout << "Введите номер группы: ";
            cin >> groupNumber;
            cout << "Введите email студента: ";
            cin.ignore();
            getline(cin, email);
            int editChoice;
            cout << "Что вы хотите изменить?\n";
            cout << "1. ФИО\n";
            cout << "2. Номер группы\n";
            cout << "3. Оценки\n";
            cout << "4. Стипендию\n";
            cout << "5. Email\n";
            cin >> editChoice;

            switch (editChoice) {
            case 1: {
                string newFIO;
                cout << "Введите новые ФИО: ";
                cin.ignore();
                getline(cin, newFIO);
                if (flow.editStudentFIOInGroup(groupNumber, email, newFIO)) {
                    cout << "ФИО успешно обновлено.\n";
                }
                else {
                    cout << "Студент не найден.\n";
                }
                break;
            }
            case 2: {
                int newNumber;
                cout << "Введите новый номер группы: ";
                cin >> newNumber;
                if (flow.editStudentNumberInGroup(groupNumber, email, newNumber)) {
                    cout << "Номер группы успешно обновлен и студент перемещен.\n";
                }
                else {
                    cout << "Студент не найден.\n";
                }
                break;
            }
            case 3: {
                int newGrades[5];
                cout << "Введите новые оценки (5 штук): ";
                for (int i = 0; i < 5; i++) {
                    cin >> newGrades[i];
                }
                if (flow.editStudentGradesInGroup(groupNumber, email, newGrades)) {
                    cout << "Оценки успешно обновлены.\n";
                }
                else {
                    cout << "Студент не найден.\n";
                }
                break;
            }
            case 4: {
                int newScholarship;
                cout << "Введите новую стипендию: ";
                cin >> newScholarship;
                if (flow.editStudentScholarshipInGroup(groupNumber, email, newScholarship)) {
                    cout << "Стипендия успешно обновлена.\n";
                }
                else {
                    cout << "Студент не найден.\n";
                }
                break;
            }
            case 5: {
                string newEmail;
                cout << "Введите новый email: ";
                cin.ignore();
                getline(cin, newEmail);
                if (flow.editStudentEmailInGroup(groupNumber, email, newEmail)) {
                    cout << "Email успешно обновлен.\n";
                }
                else {
                    cout << "Студент не найден.\n";
                }
                break;
            }
            default:
                cout << "Неверный выбор.\n";
            }
            break;
        }
        case 10: {
            int field;
            cout << "Выберите поле для фильтрации:\n";
            cout << "1. ФИО\n";
            cout << "2. Email\n";
            cout << "3. Оценка\n";
            cout << "4. Стипендия\n";
            cout << "5. Номер группы\n";
            cin >> field;

            if (field == 1 || field == 2) {
                string value;
                cout << "Введите значение: ";
                cin.ignore();
                getline(cin, value);
                flow.filterStudents(field, value);
            }
            else if (field == 3 || field == 4 || field == 5) {
                int intValue;
                cout << "Введите значение: ";
                cin >> intValue;
                flow.filterStudents(field,"", intValue);
            }
            else {
                cout << "Неверный выбор.\n";
            }
            break;
        }
        default:
            cout << "Неверный выбор. Пожалуйста, выберите снова.\n";
        }
    }
    
}