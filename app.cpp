#include <string>
#include <iostream>
#include "Table.h"
#include "Room.h"

using namespace Rooms;
using namespace Program;
const std::string Menu[] = { "1. Добавить номер", "2. Найти номер",
                            "3. Удалить номер", "4. Показать все",
                            "5. Показать доступные", "6. Зарегистрировать гостя", "0. Выход" },
    Choice = "Ваш выбор",
    Msg = "Неверный вариант, попорбуйте еще раз";

int Answer(const std::string alt[], int n, const std::string question = "Что вы хотите сделать?");

int Add(Table&), Find(Table&), Remove(Table&), ShowAll(Table&), ReserveRoom(Table&, int index), Checkout(Table& a, int index), ShowFree(Table&), Register(Table&);

int (*Funcs[])(Table&) = { nullptr, Add, Find, Remove, ShowAll, ShowFree, Register };

const int Num = sizeof(Menu) / sizeof(Menu[0]);

int main() {
    setlocale(LC_ALL, "Russian");
    Table ar;
    int ind;
    try {
        while (ind = Answer(Menu, Num)) Funcs[ind](ar);
        std::cout << "Завершение работы. Еще увидимся!" << std::endl;
    }
    catch (const std::exception& er) {
        std::cout << er.what() << std::endl;
    }
    return 0;
}

int ReserveRoom(Table& a, int index) {
    if (a.find(index)->second->reserve()) {
        std::cout << "Ok" << std::endl;
    }
    return 0;
}

int Checkout(Table& a, int index) {
    if (a.find(index)->second->checkout()) {
        std::cout << "Ok" << std::endl;
    }
    return 0;
}

int Answer(const std::string alt[], int n, const std::string question) {
    int answer;
    std::string prompt = Choice;
    std::cout << "\n" << question << std::endl;
    for (int i = 0; i < n; i++)
        std::cout << alt[i].c_str() << std::endl;
    do {
        std::cout << prompt.c_str() << ": -> ";
        prompt = Msg;
        std::cin >> answer;
        if (!std::cin.good()) {
            std::cout << "Error when number of choice was entered; \nRepeat, please." << std::endl;
            std::cin.ignore(80, '\n');
            std::cin.clear();
        }
    } while (answer < 0 || answer >= n);
    std::cin.ignore(80, '\n');
    return answer;
}

const std::string Sh[] = { "1. Одноместный", "2. Многоместный", "3. Люкс", "0. Назад" };
const int NumSh = sizeof(Sh) / sizeof(Sh[0]);

int Add(Table& a) {
    Room* ptr = nullptr;
    SingleRoom c;
    SuiteRoom s;
    MultiRoom m;
    int ans;
    while (ans = Answer(Sh, NumSh, "Выберите тип номера")) {
        double index = a.lastIndex() + 1;
        if (!std::cin.good())
            throw std::exception();
        switch (ans) {
        case 1:
            std::cout << "Введите цену за день: -> ";
            ptr = &c;
            break;
        case 2:
            std::cout << "Введите цену за день и количество мест в номере: -> ";
            ptr = &m;
            break;
        case 3:
            std::cout << "Введите цену за день и количество комнат: -> ";
            ptr = &s;
            break;
        }
        std::cin >> (*ptr);
        if (!std::cin.good())
            throw std::exception();
        std::cin.ignore(80, '\n');
        if (a.insert(index, ptr))
            std::cout << "Ok" << std::endl;
    }
    return 0;
}

const std::string FindFreeActions[] = { "1. Занять", "0. Назад" };
const std::string FindReservedActions[] = { "1. Освободить", "0. Назад" };
const std::string AllActions[] = { "1. Занять", "2. Освободить", "0. Назад" };
const int findActionsNum = 2;
const int allActionsNum = 3;

int Find(Table& a) {
    bool reserved;
    bool allActionsAvailable;
    std::string index;
    Table::Const_Iterator it;
    Room* ptr;
    std::cout << "Введите указатель номера: -> ";
    std::cin >> index;
    if (!std::cin.good())
        throw std::exception();
    it = a.find(std::stoi(index));
    if (it == a.end()) {
        std::cout << "Номер с указателем \"" << index << "\" не найден" << std::endl;
        return -1;
    }
    ptr = (*it).second;
    reserved = ptr->getReserved();
    allActionsAvailable = ptr->isAllActionsAvailable();
    std::cout << "\nНомер с указателем \"" << (*it).first << "\":" << std::endl;
    std::cout << (*ptr) << std::endl;
    if (allActionsAvailable) {
        switch (Answer(AllActions, allActionsNum)) {
        case 1:
            ReserveRoom(a, (*it).first);
            break;
        case 2:
            Checkout(a, (*it).first);
            break;
        }
    }
    else {
        if (!reserved) {
            switch (Answer(FindFreeActions, findActionsNum)) {
            case 1:
                ReserveRoom(a, (*it).first);
                break;
            }
        }
        else {
            switch (Answer(FindReservedActions, findActionsNum)) {
            case 1:
                Checkout(a, (*it).first);
                break;
            }
        }
    }
    return 0;
}

int Remove(Table& a) {
    std::string index;
    const Room* ptr = nullptr;
    std::cout << "Введите указатель номера: -> ";
    std::cin >> index;
    if (!std::cin.good())
        throw std::exception();
    if (a.remove(std::stoi(index)))
        std::cout << "Ok"
        << std::endl;
    else
        std::cout << "Номер с указателем \"" << index << "\" не найден" << std::endl;
    return 0;
}

int ShowAll(Table& a) {
    Table::Const_Iterator it;
    std::cout << "\nСписок номеров:";
    for (it = a.begin(); it != a.end(); ++it)
        std::cout << "\n" << (*it) << std::endl;
    return 0;
}

int ShowFree(Table& a) {
    Table::Const_Iterator it;
    std::cout << "\nСписок доступных номеров:";
    int counter = 0;
    for (it = a.begin(); it != a.end(); ++it) {
        if (!(it->second->getReserved())) {
            counter++;
            std::cout << "\n" << (*it) << std::endl;
        }
    }
    if (counter == 0) {
        std::cout << "\nК сожалению, в данный момент доступных номеров нет";
    }
    return 0;
}

int Register(Table& a) {
    int index;
    Table::Const_Iterator it;
    Room* ptr;
    ShowFree(a);
    std::cout << "\nВведите указатель номера для заселения: -> ";
    std::cin >> index;
    it = a.find(index);
    if (it == a.end()) {
        std::cout << "Номер с указателем \"" << index << "\" не найден" << std::endl;
        return -1;
    }
    if (it->second->getReserved()) {
        std::cout << "К сожалению, номер с указателем \"" << index << "\" уже занят" << std::endl;
        return -1;
    }
    std::cout << "\nВыбран номер с указателем \"" << (*it).first << "\":" << std::endl;
    ptr = (*it).second;
    std::cout << (*ptr) << std::endl;
    ptr->reserve();
    return 0;
}
