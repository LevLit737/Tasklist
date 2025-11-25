#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <locale>

using namespace std;

struct Task {
    int id;
    string description;
    bool completed;
};

class TaskManager {
private:
    vector<Task> tasks;
    string filename;
    int nextId;

    void loadTasks() {
        ifstream file(filename);
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            Task task;
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                task.id = stoi(line.substr(0, pos1));
                task.description = line.substr(pos1 + 1, pos2 - pos1 - 1);
                task.completed = (line.substr(pos2 + 1) == "1");
                tasks.push_back(task);
            }
        }

        if (!tasks.empty()) {
            nextId = tasks.back().id + 1;
        }
        file.close();
    }

    void saveTasks() {
        ofstream file(filename);
        if (!file) {
            cout << "Ошибка сохранения файла!" << endl;
            return;
        }

        for (const auto& task : tasks) {
            file << task.id << "|" << task.description << "|" << (task.completed ? 1 : 0) << endl;
        }
        file.close();
    }

public:
    TaskManager(const string& file = "tasks.txt") : filename(file), nextId(1) {
        loadTasks();
    }

    void addTask(const string& description) {
        Task task{ nextId++, description, false };
        tasks.push_back(task);
        saveTasks();
        cout << "Задача добавлена! ID: " << task.id << endl;
    }

    void viewTasks() {
        if (tasks.empty()) {
            cout << "Список задач пуст!" << endl;
            return;
        }

        cout << "\n    СПИСОК ЗАДАЧ" << endl;
        for (const auto& task : tasks) {
            cout << "ID: " << task.id
                << " | " << (task.completed ? "[X]" : "[ ]")
                << " " << task.description << endl;
        }
        cout << "" << endl;
    }

    void deleteTask(int id) {
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
            if (it->id == id) {
                tasks.erase(it);
                saveTasks();
                cout << "Задача с ID " << id << " удалена!" << endl;
                return;
            }
        }
        cout << "Задача с ID " << id << " не найдена!" << endl;
    }

    void markCompleted(int id) {
        for (auto& task : tasks) {
            if (task.id == id) {
                task.completed = true;
                saveTasks();
                cout << "Задача с ID " << id << " отмечена как выполненная!" << endl;
                return;
            }
        }
        cout << "Задача с ID " << id << " не найдена!" << endl;
    }
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    setlocale(LC_ALL, "Russian");

    TaskManager manager;
    int choice;

    cout << "МЕНЕДЖЕР ЗАДАЧ" << endl;

    while (true) {
        cout << "\n1. Просмотреть все задачи" << endl;
        cout << "2. Добавить задачу" << endl;
        cout << "3. Удалить задачу" << endl;
        cout << "5. Выход" << endl;
        cout << "Выберите действие: ";

        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Ошибка! Введите число от 1 до 5." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            manager.viewTasks();
            break;

        case 2: {
            string description;
            cout << "Введите описание задачи: ";
            clearInput();
            getline(cin, description);

            if (!description.empty()) {
                manager.addTask(description);
            }
            else {
                cout << "Описание не может быть пустым!" << endl;
            }
            break;
        }

        case 3: {
            int id;
            cout << "Введите ID задачи для удаления: ";
            cin >> id;

            if (cin.fail()) {
                clearInput();
                cout << "Ошибка! Введите корректный ID." << endl;
            }
            else {
                manager.deleteTask(id);
            }
            break;
        }

        case 4: {
            int id;
            cout << "Введите ID задачи для отметки: ";
            cin >> id;

            if (cin.fail()) {
                clearInput();
                cout << "Ошибка! Введите корректный ID." << endl;
            }
            else {
                manager.markCompleted(id);
            }
            break;
        }

        case 5:
            cout << "Выход из программы. До свидания!" << endl;
            return 0;

        default:
            cout << "Неверный выбор! Введите число от 1 до 5." << endl;
            break;
        }
    }

    return 0;
}