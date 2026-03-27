#include "ParkingLot.h"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cstdlib>

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

std::string readInputLine(const std::string& prompt) {
#ifdef USE_READLINE
    char* line = readline(prompt.c_str());
    if (line == nullptr) {
        return "";
    }
    std::string result(line);
    if (!result.empty()) {
        add_history(line);
    }
    free(line);
    return result;
#else
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
#endif
}

std::string normalizeInput(const std::string& input) {
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find("，", pos)) != std::string::npos) {
        result.replace(pos, 3, ",");
        pos += 1;
    }
    pos = 0;
    while ((pos = result.find("（", pos)) != std::string::npos) {
        result.replace(pos, 3, "(");
        pos += 1;
    }
    pos = 0;
    while ((pos = result.find("）", pos)) != std::string::npos) {
        result.replace(pos, 3, ")");
        pos += 1;
    }
    return result;
}

bool hasChineseParentheses(const std::string& input) {
    return input.find("（") != std::string::npos || input.find("）") != std::string::npos;
}

bool startsWithParentheses(const std::string& input) {
    if (input.empty()) return false;
    if (input[0] == '(') return true;
    return input.substr(0, 3) == "（";
}

void printUsage() {
    std::cout << "\n========== 停车场管理系统 ==========" << std::endl;
    std::cout << "使用说明:" << std::endl;
    std::cout << "  (A, 车牌号, 时间)   - 车辆到达" << std::endl;
    std::cout << "  (D, 车牌号, 时间)   - 车辆离开停车场" << std::endl;
    std::cout << "  (L, 车牌号)         - 车辆离开便道" << std::endl;
    std::cout << "  S                   - 显示停车场状态" << std::endl;
    std::cout << "  (E, 0, 0)           - 退出程序" << std::endl;
    std::cout << "  H                   - 显示帮助信息" << std::endl;
    std::cout << "====================================" << std::endl;
}

void runInteractiveMode(ParkingLot& lot) {
    std::string input;
    
    std::cout << "\n进入交互模式，输入 'H' 查看帮助信息" << std::endl;
    
    while (true) {
        input = readInputLine("\n> ");
        
        if (input.empty()) {
            continue;
        }
        
        input = normalizeInput(input);
        
        std::istringstream iss(input);
        
        if (input == "S") {
            lot.displayStatus();
            continue;
        } else if (input == "H") {
            printUsage();
            continue;
        }
        
        char command;
        char openParen, comma1, comma2, closeParen;
        int licensePlate, time;
        
        if (startsWithParentheses(input)) {
            iss >> openParen >> command >> comma1 >> licensePlate >> comma2 >> time >> closeParen;
            command = toupper(command);
            
            if (command == 'E') {
                if (licensePlate == 0 && time == 0) {
                    std::cout << "程序结束" << std::endl;
                    lot.displayStatus();
                    break;
                }
            } else if (command == 'A') {
                if (licensePlate <= 0) {
                    std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                } else if (time < 0) {
                    std::cout << "[错误] 时间不能为负数" << std::endl;
                } else {
                    lot.carArrive(licensePlate, time);
                }
            } else if (command == 'D') {
                if (licensePlate <= 0) {
                    std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                } else if (time < 0) {
                    std::cout << "[错误] 时间不能为负数" << std::endl;
                } else {
                    lot.carDepart(licensePlate, time);
                }
            } else if (command == 'L') {
                if (licensePlate <= 0) {
                    std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                } else {
                    lot.carLeaveWaitingQueue(licensePlate);
                }
            } else {
                std::cout << "[错误] 未知命令，输入 'H' 查看帮助信息" << std::endl;
            }
        } else {
            size_t commaPos = input.find(',');
            bool hasComma = (commaPos != std::string::npos);
            
            if (hasComma) {
                char commaChar;
                iss >> command >> commaChar;
                command = toupper(command);
                
                if (command == 'E') {
                    if (iss >> licensePlate >> commaChar >> time) {
                        if (licensePlate == 0 && time == 0) {
                            std::cout << "程序结束" << std::endl;
                            lot.displayStatus();
                            break;
                        }
                    }
                } else if (command == 'A') {
                    if (iss >> licensePlate >> commaChar >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carArrive(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: A,车牌号,时间 或 A 车牌号 时间" << std::endl;
                    }
                } else if (command == 'D') {
                    if (iss >> licensePlate >> commaChar >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carDepart(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: D,车牌号,时间 或 D 车牌号 时间" << std::endl;
                    }
                } else if (command == 'L') {
                    if (iss >> licensePlate) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else {
                            lot.carLeaveWaitingQueue(licensePlate);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: L,车牌号 或 L 车牌号" << std::endl;
                    }
                } else {
                    std::cout << "[错误] 未知命令，输入 'H' 查看帮助信息" << std::endl;
                }
            } else {
                iss >> command;
                command = toupper(command);
                
                if (command == 'E') {
                    std::cout << "程序结束" << std::endl;
                    lot.displayStatus();
                    break;
                } else if (command == 'A') {
                    if (iss >> licensePlate >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carArrive(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: A <车牌号> <时间> 或 (A, 车牌号, 时间)" << std::endl;
                    }
                } else if (command == 'D') {
                    if (iss >> licensePlate >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carDepart(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: D <车牌号> <时间> 或 (D, 车牌号, 时间)" << std::endl;
                    }
                } else if (command == 'L') {
                    if (iss >> licensePlate) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else {
                            lot.carLeaveWaitingQueue(licensePlate);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: L <车牌号> 或 (L, 车牌号)" << std::endl;
                    }
                } else {
                    std::cout << "[错误] 未知命令，输入 'H' 查看帮助信息" << std::endl;
                }
            }
        }
    }
}

void runBatchMode(ParkingLot& lot) {
    std::string input;
    
    std::cout << "进入批处理模式，输入 '(E, 0, 0)' 结束" << std::endl;
    
    while (true) {
        input = readInputLine("");
        
        if (input.empty()) {
            continue;
        }
        
        input = normalizeInput(input);
        
        std::istringstream iss(input);
        
        char command;
        char openParen, comma1, comma2, closeParen;
        int licensePlate, time;
        
        if (startsWithParentheses(input)) {
            iss >> openParen >> command >> comma1 >> licensePlate >> comma2 >> time >> closeParen;
            command = toupper(command);
            
            if (command == 'E') {
                if (licensePlate == 0 && time == 0) {
                    std::cout << "批处理结束" << std::endl;
                    lot.displayStatus();
                    break;
                }
            } else if (command == 'A') {
                if (licensePlate <= 0) {
                    std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                } else if (time < 0) {
                    std::cout << "[错误] 时间不能为负数" << std::endl;
                } else {
                    lot.carArrive(licensePlate, time);
                }
            } else if (command == 'D') {
                if (licensePlate <= 0) {
                    std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                } else if (time < 0) {
                    std::cout << "[错误] 时间不能为负数" << std::endl;
                } else {
                    lot.carDepart(licensePlate, time);
                }
            } else {
                std::cout << "[错误] 批处理模式只支持 A 和 D 命令" << std::endl;
            }
        } else {
            size_t commaPos = input.find(',');
            bool hasComma = (commaPos != std::string::npos);
            
            if (hasComma) {
                char commaChar;
                iss >> command >> commaChar;
                command = toupper(command);
                
                if (command == 'E') {
                    if (iss >> licensePlate >> commaChar >> time) {
                        if (licensePlate == 0 && time == 0) {
                            std::cout << "批处理结束" << std::endl;
                            lot.displayStatus();
                            break;
                        }
                    }
                } else if (command == 'A') {
                    if (iss >> licensePlate >> commaChar >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carArrive(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: A,车牌号,时间 或 A 车牌号 时间" << std::endl;
                    }
                } else if (command == 'D') {
                    if (iss >> licensePlate >> commaChar >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carDepart(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: D,车牌号,时间 或 D 车牌号 时间" << std::endl;
                    }
                } else {
                    std::cout << "[错误] 批处理模式只支持 A 和 D 命令" << std::endl;
                }
            } else {
                iss >> command;
                command = toupper(command);
                
                if (command == 'E') {
                    iss >> licensePlate >> time;
                    if (licensePlate == 0 && time == 0) {
                        std::cout << "批处理结束" << std::endl;
                        lot.displayStatus();
                        break;
                    }
                } else if (command == 'A') {
                    if (iss >> licensePlate >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carArrive(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: A <车牌号> <时间> 或 (A, 车牌号, 时间)" << std::endl;
                    }
                } else if (command == 'D') {
                    if (iss >> licensePlate >> time) {
                        if (licensePlate <= 0) {
                            std::cout << "[错误] 车牌号必须为正整数" << std::endl;
                        } else if (time < 0) {
                            std::cout << "[错误] 时间不能为负数" << std::endl;
                        } else {
                            lot.carDepart(licensePlate, time);
                        }
                    } else {
                        std::cout << "[错误] 格式错误，请使用: D <车牌号> <时间> 或 (D, 车牌号, 时间)" << std::endl;
                    }
                } else {
                    std::cout << "[错误] 批处理模式只支持 A 和 D 命令" << std::endl;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int capacity = 5;
    int rate = 1;
    bool interactiveMode = true;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-c" || arg == "--capacity") {
            if (i + 1 < argc) {
                capacity = std::atoi(argv[i + 1]);
                i++;
            }
        } else if (arg == "-r" || arg == "--rate") {
            if (i + 1 < argc) {
                rate = std::atoi(argv[i + 1]);
                i++;
            }
        } else if (arg == "-b" || arg == "--batch") {
            interactiveMode = false;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "用法: parking_lot [选项]" << std::endl;
            std::cout << "选项:" << std::endl;
            std::cout << "  -c, --capacity <数量>  设置停车场容量 (默认: 5)" << std::endl;
            std::cout << "  -r, --rate <费率>      设置每小时费率 (默认: 1)" << std::endl;
            std::cout << "  -b, --batch            批处理模式" << std::endl;
            std::cout << "  -h, --help             显示帮助信息" << std::endl;
            return 0;
        }
    }

    if (capacity <= 0) {
        std::cerr << "[错误] 停车场容量必须为正整数" << std::endl;
        return 1;
    }

    if (rate <= 0) {
        std::cerr << "[错误] 费率必须为正整数" << std::endl;
        return 1;
    }

    ParkingLot lot(capacity, rate);

    std::cout << "========== 停车场管理系统 ==========" << std::endl;
    std::cout << "停车场容量: " << capacity << std::endl;
    std::cout << "费率: " << rate << " 元/时间单位" << std::endl;
    std::cout << "模式: " << (interactiveMode ? "交互模式" : "批处理模式") << std::endl;
    std::cout << "====================================" << std::endl;

    if (interactiveMode) {
        runInteractiveMode(lot);
    } else {
        runBatchMode(lot);
    }

    return 0;
}
