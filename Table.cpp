#include <iostream>
#include "Table.h"

namespace Program {
    std::ostream& operator<<(std::ostream& os, const std::pair<const int, Rooms::Room*>& p) {
        return os << p.first << ". " << (*p.second);
    }

    Table::Table(const Table& a) {
        std::map<const int, Rooms::Room*>::const_iterator p;
        for (p = a.arr.begin(); p != a.arr.end(); ++p)
            arr.insert(std::make_pair(p->first, p->second->clone()));
    }

    Table::~Table() {
        std::map<const int, Rooms::Room*>::iterator p;
        for (p = arr.begin(); p != arr.end(); ++p) {
            delete p->second;
            p->second = nullptr;
        }
    }

    Table& Table::operator=(const Table& a) {
        std::map<const int, Rooms::Room*>::iterator p;
        if (this != &a) {
            for (p = arr.begin(); p != arr.end(); ++p) delete p->second;
            arr.clear();
            std::map<const int, Rooms::Room*>::const_iterator pp;
            for (pp = a.arr.begin(); pp != a.arr.end(); ++pp)
                arr.insert(std::make_pair(pp->first, pp->second->clone()));
        }
        return *this;
    }

    bool Table::insert(const int& s, const Rooms::Room* f) {
        bool res = false;
        std::map<const int, Rooms::Room*>::iterator p = arr.find(s);
        if (p == arr.end()) { // 􏰯􏰦􏰮􏰧􏰵 􏰭􏰬􏰸􏰧􏰯􏰬􏰹􏰬􏰭
            std::pair<std::map<const int, Rooms::Room*>::iterator, bool> pp = arr.insert(
                std::make_pair(s, f->clone()));
            if (!pp.second)
                throw std::exception();
            res = true;
        }
        return res;
    }

    bool Table::remove(const int& s) {
        bool res = false;
        std::map<const int, Rooms::Room*>::iterator p = arr.find(s); //arr.erase(s);
        if (p != arr.end()) {
            delete p->second;
            p->second = nullptr;
            arr.erase(p);
            res = true;
        }
        return res;
    }

    int Table::lastIndex() {
        if (arr.size() == 0) return 0;
        return arr.rbegin()->first;
    }

    Table::Const_Iterator Table::find(const int& s) const {
        std::map<const int, Rooms::Room*>::const_iterator p = arr.find(s);
        return ConstTableIt(p);
    }

    Table::Const_Iterator Table::begin() const {
        return ConstTableIt(arr.begin());
    }

    Table::Const_Iterator Table::end() const {
        return ConstTableIt(arr.end());
    }

    int Table::Const_Iterator::operator!=(const ConstTableIt& it) const {
        return cur != it.cur;
    }

    int ConstTableIt::operator==(const ConstTableIt& it) const {
        return cur == it.cur;
    }

    const std::pair<const int, Rooms::Room*>& ConstTableIt::operator*() {
        return *cur;
    }

    const std::pair<const int, Rooms::Room*>* ConstTableIt::operator->() {
        return &*cur;
    }

    ConstTableIt& ConstTableIt::operator++() {
        ++cur;
        return *this;
    }

    ConstTableIt ConstTableIt::operator++(int) {
        ConstTableIt res(*this);
        ++cur;
        return res;
    }
};
