// File: Praktikum5.cpp

#include "Semaphore.cpp"

#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex;
Semaphore semaphore(1); //nur ein Thread gleichzeitig hat Zugriff, dann wie mutex aus b)

void t1() {
    //mutex.lock();
    semaphore.acquire();
    for (char i = 'a'; i <= 'z'; i++) {
        std::cout << i << ' ';
    }
    semaphore.release();
    //mutex.unlock();
}

void t2() {
    //mutex.lock();
    semaphore.acquire();
    for (int i = 0; i <= 32; i++) {
        std::cout << i << ' ';
    }
    semaphore.release();
    //mutex.unlock();
}

void t3() {
    //mutex.lock();
    semaphore.acquire();
    for (char i = 'A'; i <= 'Z'; i++) {
        std::cout << i << ' ';
    }
    semaphore.release();
    //mutex.unlock();
}

int main() {
    //std::thread ist eine Klasse, um einen neuen Thread zu erstellen
    //Sobald T1,T2,T3 erstellt wurden, werden sie parallel zur main()-Funktion ausgeführt (arbeiten gleichzeitig)
    //die Methode join() wartet darauf, dass die Threads beendet werden, bevor der Code in main() fortgesetzt wird
    //ohne join() würde die main()-Funktion sofort nach dem erstellen der Threads enden
    std::thread T1(t1);
    std::thread T2(t2);
    std::thread T3(t3);

    T1.join();
    T2.join();
    T3.join();

    return 0;
}
