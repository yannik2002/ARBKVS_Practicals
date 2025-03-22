// File: Semaphore.cpp

#include <mutex>

class Semaphore {
private:
	std::mutex m;
	std::condition_variable cond_var;
	int count; // Ressource kann count mal belegt werden (gem. Initialisierung)

public:

	Semaphore(uint32_t m) : count(m) {}

	//Eintritt in kritischen Abschnitt
	//Gehe in Warteschlange der cond_var
	void acquire() {
		// unique_lock kümmert sich auch automatisch um das Entsperren des Mutex
		// sperrt den mutex beim erstellen automatisch, Standardverhalten des Konstruktors
		std::unique_lock<std::mutex> lock(m); // atomarer Zugriff, keine Race Conditions
		count--; // Zugriff auf Ressource vermerken

		// Keine Zugriff auf die Ressource mehr verfügbar
		if (count < 0) {
			// der Thread blockiert und wartet auf Benachrichtigung zum aufwachen
			// es sobald der Thread wieder aufwacht durch einen notify_one() Aufruf
			// gibt während des Wartens den lock frei, damit sich ein anderer Thread auch in der Queue einreihen kann
			// Non-busy waiting bzw. inaktives warten in der Warteschlange von cond_var, es werden keine CPU-Ressourcen verbraucht
			// Bei Benachrichtigung wird der wartende Thread deblockiert und der mutex übernommen
			cond_var.wait(lock);
		}
	}

	//Verlasse kritischen Abschnitt
	//Thread aus cond_var Warteschlange übernimmt den Mutex
	void release() {
		std::unique_lock<std::mutex> lock(m); // atomarer Zugriff, keine Race Conditions
		count++; // Freigabe der Ressource
		cond_var.notify_one(); // Weckt einen schlafenden Thread aus der Warteschlange wieder auf
	}
};