#ifndef CAR_H
#define CAR_H

#include <string>
#include <iostream>

class Car {
private:
    int licensePlate;
    int arrivalTime;
    int position;

public:
    Car() : licensePlate(0), arrivalTime(0), position(-1) {}

    Car(int plate, int time, int pos = -1) 
        : licensePlate(plate), arrivalTime(time), position(pos) {}

    int getLicensePlate() const {
        return licensePlate;
    }

    void setLicensePlate(int plate) {
        licensePlate = plate;
    }

    int getArrivalTime() const {
        return arrivalTime;
    }

    void setArrivalTime(int time) {
        arrivalTime = time;
    }

    int getPosition() const {
        return position;
    }

    void setPosition(int pos) {
        position = pos;
    }

    int calculateParkingFee(int departureTime, int ratePerUnit = 1) const {
        if (departureTime < arrivalTime) {
            return 0;
        }
        int duration = departureTime - arrivalTime;
        return duration * ratePerUnit;
    }

    int getParkingDuration(int departureTime) const {
        if (departureTime < arrivalTime) {
            return 0;
        }
        return departureTime - arrivalTime;
    }

    void display() const {
        std::cout << "车牌号: " << licensePlate 
                  << ", 到达时间: " << arrivalTime
                  << ", 位置: " << position;
    }
};

#endif
