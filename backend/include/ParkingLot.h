#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include "Stack.h"
#include "Queue.h"
#include "Car.h"
#include <vector>
#include <iostream>
#include <iomanip>

class ParkingLot {
private:
    Stack<Car> parkingStack;
    Queue<Car> waitingQueue;
    int capacity;
    int ratePerUnit;
    int totalRevenue;

public:
    ParkingLot(int cap, int rate = 1) 
        : parkingStack(cap), capacity(cap), ratePerUnit(rate), totalRevenue(0) {}

    bool isFull() const {
        return parkingStack.isFull();
    }

    bool isEmpty() const {
        return parkingStack.isEmpty();
    }

    int getCapacity() const {
        return capacity;
    }

    int getAvailableSpaces() const {
        return capacity - parkingStack.size();
    }

    int getParkedCarsCount() const {
        return parkingStack.size();
    }

    int getWaitingCarsCount() const {
        return waitingQueue.size();
    }

    int getTotalRevenue() const {
        return totalRevenue;
    }

    bool hasCarWithLicensePlate(int licensePlate) const {
        for (int i = 0; i < parkingStack.size(); ++i) {
            const Car& car = parkingStack.at(i);
            if (car.getLicensePlate() == licensePlate) {
                return true;
            }
        }

        std::vector<Car> waitingCars = waitingQueue.toVector();
        for (const auto& car : waitingCars) {
            if (car.getLicensePlate() == licensePlate) {
                return true;
            }
        }

        return false;
    }

    bool carArrive(int licensePlate, int arrivalTime) {
        Car newCar(licensePlate, arrivalTime);

        if (hasCarWithLicensePlate(licensePlate)) {
            std::cout << "[错误] 车牌号 " << licensePlate
                      << " 已在停车场或便道中，不能重复进入" << std::endl;
            return false;
        }

        if (!isFull()) {
            int position = parkingStack.size() + 1;
            newCar.setPosition(position);
            parkingStack.push(newCar);
            std::cout << "[到达] 车牌号 " << licensePlate 
                      << " 在时刻 " << arrivalTime 
                      << " 到达，进入停车场位置 " << position << std::endl;
            return true;
        } else {
            waitingQueue.enqueue(newCar);
            std::cout << "[到达] 车牌号 " << licensePlate 
                      << " 在时刻 " << arrivalTime 
                      << " 到达，停车场已满，在便道位置 " << waitingQueue.size() << " 等待" << std::endl;
            return false;
        }
    }

    bool carDepart(int licensePlate, int departureTime) {
        if (isEmpty()) {
            std::cout << "[错误] 停车场为空，没有车辆可以离开" << std::endl;
            return false;
        }

        Stack<Car> tempStack(capacity);
        Car targetCar;
        bool found = false;

        while (!parkingStack.isEmpty()) {
            Car currentCar = parkingStack.pop();
            if (currentCar.getLicensePlate() == licensePlate) {
                targetCar = currentCar;
                found = true;
                break;
            }
            tempStack.push(currentCar);
        }

        if (!found) {
            while (!tempStack.isEmpty()) {
                parkingStack.push(tempStack.pop());
            }
            std::cout << "[错误] 车牌号 " << licensePlate << " 不在停车场内" << std::endl;
            return false;
        }

        while (!tempStack.isEmpty()) {
            Car car = tempStack.pop();
            parkingStack.push(car);
        }

        int duration = targetCar.getParkingDuration(departureTime);
        int fee = targetCar.calculateParkingFee(departureTime, ratePerUnit);
        totalRevenue += fee;

        std::cout << "[离开] 车牌号 " << licensePlate 
                  << " 在时刻 " << departureTime << " 离开" << std::endl;
        std::cout << "       停留时间: " << duration << " 个时间单位" << std::endl;
        std::cout << "       应交费用: " << fee << " 元" << std::endl;

        if (!waitingQueue.isEmpty()) {
            Car waitingCar = waitingQueue.dequeue();
            int position = parkingStack.size() + 1;
            waitingCar.setPosition(position);
            waitingCar.setArrivalTime(departureTime);  // 更新到达时间为进入停车场的时刻
            parkingStack.push(waitingCar);
            std::cout << "[进场] 车牌号 " << waitingCar.getLicensePlate() 
                      << " 从便道进入停车场位置 " << position << std::endl;
        }

        return true;
    }

    bool carLeaveWaitingQueue(int licensePlate) {
        if (waitingQueue.isEmpty()) {
            std::cout << "[错误] 便道为空，没有车辆可以离开" << std::endl;
            return false;
        }

        Queue<Car> tempQueue;
        bool found = false;
        Car targetCar;

        while (!waitingQueue.isEmpty()) {
            Car currentCar = waitingQueue.dequeue();
            if (currentCar.getLicensePlate() == licensePlate) {
                targetCar = currentCar;
                found = true;
                break;
            }
            tempQueue.enqueue(currentCar);
        }

        if (!found) {
            while (!tempQueue.isEmpty()) {
                waitingQueue.enqueue(tempQueue.dequeue());
            }
            std::cout << "[错误] 车牌号 " << licensePlate << " 不在便道上" << std::endl;
            return false;
        }

        while (!tempQueue.isEmpty()) {
            waitingQueue.enqueue(tempQueue.dequeue());
        }

        std::cout << "[离开便道] 车牌号 " << licensePlate 
                  << " 从便道离开，不收取费用" << std::endl;
        return true;
    }

    void displayStatus() const {
        std::cout << "\n========== 停车场状态 ==========" << std::endl;
        std::cout << "停车场容量: " << capacity << std::endl;
        std::cout << "已停车辆: " << getParkedCarsCount() << std::endl;
        std::cout << "可用车位: " << getAvailableSpaces() << std::endl;
        std::cout << "等待车辆: " << getWaitingCarsCount() << std::endl;
        std::cout << "总收入: " << totalRevenue << " 元" << std::endl;

        if (!isEmpty()) {
            std::cout << "\n停车场内车辆:" << std::endl;
            for (int i = 0; i < parkingStack.size(); i++) {
                const Car& car = parkingStack.at(i);
                std::cout << "  位置 " << car.getPosition() 
                          << ": 车牌号 " << car.getLicensePlate()
                          << ", 到达时间 " << car.getArrivalTime() << std::endl;
            }
        }

        if (!waitingQueue.isEmpty()) {
            std::cout << "\n便道等待车辆:" << std::endl;
            std::vector<Car> waitingCars = waitingQueue.toVector();
            for (size_t i = 0; i < waitingCars.size(); i++) {
                std::cout << "  位置 " << (i + 1) 
                          << ": 车牌号 " << waitingCars[i].getLicensePlate()
                          << ", 到达时间 " << waitingCars[i].getArrivalTime() << std::endl;
            }
        }
        std::cout << "================================\n" << std::endl;
    }
};

#endif
