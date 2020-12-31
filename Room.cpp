#include <iostream>
#include "Room.h"

namespace Rooms {

    double Reserve::getFullPrice(double rate) {
        return rate * getReservedDays();
    }

    std::ostream& operator<<(std::ostream& os, const std::vector<Reserve>& r) {
        std::vector<Reserve>::const_iterator it;
        for (std::size_t i = 0; i < r.size(); ++i) {
            os << i + 1 << ") " << r[i] << "\n";
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Reserve& r) {
        return os << "Дата заезда - " << r.registration_date << ", количество дней - " << r.reserved_days;
    }

    std::istream& operator>>(std::istream& is, Reserve& r) {
        return is >> r.registration_date >> r.reserved_days;
    }

    std::ostream& operator<<(std::ostream& os, const Room& r) {
        return r.show(os);
    }

    std::istream& operator>>(std::istream& is, Room& r) {
        return r.get(is);
    }

    std::ostream& SingleRoom::show(std::ostream& os) const {

        if (reserved) {
            return os << "Одноместный, " << daily_rate << " в сутки, занят\n"
                << "Информация о бронировании:\n" << reserve_info;
        }
        else {
            return os << "Одноместный, " << daily_rate << " в сутки, свободен";
        }
    }

    // Одноместный номер
    std::istream& SingleRoom::get(std::istream& is) {
        return is >> daily_rate;
    }

    bool SingleRoom::reserve() {
        Reserve r;
        std::cout << "Введите дату заезда и количество дней проживания через пробел:" << std::endl;
        std::cin >> r;
        setReserved(true);
        setReserveInfo(r);
        return true;
    }

    bool SingleRoom::checkout() {
        setReserved(false);
        std::cout << "Цена за проживание: " << reserve_info.getFullPrice(daily_rate) << std::endl;
        setReserveInfo(Reserve());
        return true;
    }

    // Многоместный номер
    std::ostream& MultiRoom::show(std::ostream& os) const {

        if (reserved) {
            return os << "Многоместный, " << "мест - " << max_guests << ", " << daily_rate
                << " в сутки, полностью занят\n"
                << "Информация о бронировании:\n" << reserve_info;
        }
        else {
            if (reserve_info.size() == 0) {
                return os << "Многоместный, " << "мест - " << max_guests << ", " << daily_rate << " в сутки, свободен";
            }
            else {
                return os << "Многоместный, " << "мест - " << max_guests << ", " << daily_rate
                    << " в сутки, частично занят (" << reserve_info.size() << "/" << max_guests << ")\n"
                    << "Информация о бронировании:\n" << reserve_info;
            }
        }
    }

    std::istream& MultiRoom::get(std::istream& is) {
        return is >> daily_rate >> max_guests;
    }

    bool MultiRoom::reserve() {
        Reserve g;
        std::cout << "Введите дату заезда и количество дней проживания через пробел:" << std::endl;
        std::cin >> g;
        setReserveInfo(g);
        if (reserve_info.size() == max_guests) {
            setReserved(true);
        }
        return true;
    }

    bool MultiRoom::checkout() {
        if (reserve_info.size() < 2) {
            std::cout << "Цена за проживание: " << reserve_info[0].getFullPrice(daily_rate) << std::endl;
            reserve_info.clear();
            return true;
        }
        int i = 0;
        std::cout << "Введите номер брони: ";
        while (i < 1 || i > reserve_info.size()) {
            std::cin >> i;
            if (i < 1 || i > reserve_info.size()) { std::cout << "Неверное значение, попробуйте еще раз: "; }
        }
        auto iter = reserve_info.cbegin();
        std::cout << "Цена за проживание: " << reserve_info[i - 1].getFullPrice(daily_rate) << std::endl; // todo проверить
        reserve_info.erase(iter + i - 1);
        setReserved(false);
        return true;
    }

    bool MultiRoom::isAllActionsAvailable() {
        if (!reserved && reserve_info.size() > 0) {
            return true;
        }
        else {
            return false;
        }
    }

    // Номер люкс
    std::ostream& SuiteRoom::show(std::ostream& os) const {

        if (reserved) {
            return os << "Люкс, " << "комнат - " << rooms << ", " << daily_rate << " в сутки, занят\n"
                << "Информация о бронировании:\n" << reserve_info << ". Количество гостей - " << guests_count;
        }
        else {
            return os << "Люкс, " << "комнат - " << rooms << ", " << daily_rate << " в сутки, свободен";
        }
    }

    std::istream& SuiteRoom::get(std::istream& is) {
        return is >> daily_rate >> rooms;
    }

    bool SuiteRoom::reserve() {
        int guests = 0;
        std::cout << "Введите количество гостей (1 или 2): ";
        while (guests < 1 || guests > 2) {
            std::cin >> guests;
            if (guests < 1 || guests > 2) { std::cout << "Неверное значение, попробуйте еще раз: "; }
        }
        Reserve r;
        std::cout << "Введите дату заезда и количество дней проживания через пробел:" << std::endl;
        std::cin >> r;
        setGuestsCount(guests);
        setReserved(true);
        setReserveInfo(r);
        return true;
    }

    bool SuiteRoom::checkout() {
        setGuestsCount(0);
        setReserved(false);
        std::cout << "Цена за проживание: " << reserve_info.getFullPrice(daily_rate) << std::endl;
        setReserveInfo(Reserve());
        return true;
    }
};
