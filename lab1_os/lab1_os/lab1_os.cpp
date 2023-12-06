// lab1_os.cpp : 
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <functional>

std::mutex resultMutex;                                     // М'ютекс для синхронізації доступу до глобальних результатів
std::condition_variable calculationFinished;  // Умовна змінна для сигналізації про завершення обчислення
double resultF = 0.0;                                           // Результат CalculationF
double resultG = 0.0;                                          // Результат CalculationG
std::atomic<bool> canceled(false);                   // Прапорець для скасування обчислення


void CalculationF(double x, double addToX) {
    {
        std::unique_lock<std::mutex> lock(resultMutex);  // Захоплення м'ютексу
        std::cout << "CalculationF: Obtained number from f(x): " << x << std::endl;  
        std::this_thread::sleep_for(std::chrono::seconds(2));  // Імітація довгого обчислення
        if (!canceled.load()) {
            resultF = x + addToX;  // Обчислення результату з додаванням addToX
        }
    }
    calculationFinished.notify_one();  // Сигнал про завершення обчислення
}


void CalculationG(double x, double addToX) {
    {
        std::unique_lock<std::mutex> lock(resultMutex); 
        std::cout << "CalculationG: Obtained number from g(x): " << x << std::endl; 
        std::this_thread::sleep_for(std::chrono::seconds(3));  // Імітація довгого обчислення
        if (!canceled.load()) {
            resultG = x + addToX;  
        }
    }
    calculationFinished.notify_one();  
}

int main() {
    double x, addToXF, addToXG;

    // Зчитування вхідних значень з консолі
    std::cout << "Enter a value for x: ";
    std::cin >> x;

    std::cout << "Enter the number to add for CalculationF: ";
    std::cin >> addToXF;

    std::cout << "Enter the number to add for CalculationG: ";
    std::cin >> addToXG;

    // Створення потоків для обчислення результатів функцій з використанням додавання
    std::thread threadF(CalculationF, x, addToXF);
    std::thread threadG(CalculationG, x, addToXG);

    std::unique_lock<std::mutex> lock(resultMutex);
    auto timeout = std::chrono::system_clock::now() + std::chrono::seconds(10);  // Тайм-аут для очікування результатів

    // Очікування завершення обчислень або тайм-ауту
    while (resultF == 0.0 || resultG == 0.0) {
        if (std::cv_status::timeout == calculationFinished.wait_until(lock, timeout) && !canceled.load()) {
            std::cout << "Calculation timeout. Canceling..." << std::endl;
            canceled.store(true);
            calculationFinished.notify_all();  // Скасування всіх чекаючих потоків
            break;
        }
    }

    // Обробка результатів
    if (!canceled.load()) {
        if (resultF == 0.0 || resultG == 0.0) {
            std::cout << "Calculation failed. Check the functions f and g." << std::endl;
        }
        else {
            std::cout << "Results: CalculationF = " << resultF << ", CalculationG = " << resultG << std::endl;
            std::cout << "Final Result: " << resultF + resultG << std::endl;
        }
    }

    // Очікування завершення обчислень для кожного потоку
    threadF.join();
    threadG.join();

    return 0;
}
