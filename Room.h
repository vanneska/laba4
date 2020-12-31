#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <iostream>
#include <vector>

namespace Rooms {
    class Reserve {
    private:
        std::string registration_date;
        int reserved_days;
    public:
        Reserve(
            std::string date = "",
            int days = 0
        ) : registration_date(date), reserved_days(days) {}

        friend std::ostream& operator<<(std::ostream&, const Reserve&);

        friend std::istream& operator>>(std::istream&, Reserve&);

        int getReservedDays() { return reserved_days; }

        virtual double getFullPrice(double rate);

    };

    class Room {
    protected:
        bool reserved = false;
        double daily_rate;

        virtual std::ostream& show(std::ostream&) const = 0;

        virtual std::istream& get(std::istream&) = 0;

    public:
        Room(double rate) : daily_rate(rate) {}

        virtual Room* clone() const = 0;

        friend std::ostream& operator<<(std::ostream&, const Room&);

        friend std::istream& operator>>(std::istream&, Room&);

        virtual bool setReserved(bool value) {
            reserved = value;
            return true;
        }

        virtual bool getReserved() {
            return reserved;
        }

        virtual bool reserve() {
            return true;
        };
        
        virtual bool checkout() {
            return true;
        };
        
        virtual bool isAllActionsAvailable() { return false; };

        virtual ~Room() {}
    };

    class SingleRoom : public Room {
    protected:
        Reserve reserve_info;

        virtual std::ostream& show(std::ostream&) const;

        virtual std::istream& get(std::istream&);

    public:
        SingleRoom(double rate = 0) : Room(rate), reserve_info() {}

        virtual SingleRoom* clone() const {
            return new SingleRoom(*this);
        }

        virtual void setReserveInfo(Reserve g) {
            reserve_info = g;
        }

        virtual bool reserve();

        virtual bool checkout();
    };

    class MultiRoom : public Room {
    protected:
        std::vector<Reserve> reserve_info;
        int max_guests;

        virtual std::ostream& show(std::ostream&) const;

        virtual std::istream& get(std::istream&);

    public:
        MultiRoom(double rate = 0, int guests = 4) : Room(rate), max_guests(guests) {}

        virtual MultiRoom* clone() const {
            return new MultiRoom(*this);
        }

        virtual bool setReserveInfo(Reserve r) {
            reserve_info.push_back(r);
            return true;
        }

        virtual bool reserve();

        virtual bool checkout();

        virtual bool isAllActionsAvailable();
    };

    class SuiteRoom : public SingleRoom {
    protected:
        int rooms;
        int guests_count;

        virtual std::ostream& show(std::ostream&) const;

        virtual std::istream& get(std::istream&);

    public:
        SuiteRoom(double rate = 0, int rooms_count = 2) : SingleRoom(rate), rooms(rooms_count) {}

        virtual SuiteRoom* clone() const {
            return new SuiteRoom(*this);
        }

        virtual void setGuestsCount(int g) {
            guests_count = g;
        }

        virtual bool reserve();

        virtual bool checkout();
    };
};
#endif
