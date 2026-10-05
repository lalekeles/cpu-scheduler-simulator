#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    bool isCompleted = false;
};

// Gantt Şeması için zaman aralığı tutan yapı
struct GanttBlock {
    int processId;
    int startTime;
    int endTime;
};

// Gantt Şemasını Ekrana Çizen Fonksiyon
void printGanttChart(const vector<GanttBlock>& gantt) {
    cout << "\n--- GANTT CHART (ZAMAN CIZELGESI) ---\n";
    
    // Üst Çizgi
    for (const auto& block : gantt) {
        cout << "---------";
    }
    cout << "-\n|";

    // Process ID'leri
    for (const auto& block : gantt) {
        if (block.processId == -1) {
            cout << " IDLE  |"; // CPU boşta
        } else {
            cout << "  P" << setw(2) << left << block.processId << "  |";
        }
    }
    cout << "\n";

    // Alt Çizgi
    for (const auto& block : gantt) {
        cout << "---------";
    }
    cout << "-\n";

    // Zaman Çizelgesi Sayıları
    cout << gantt[0].startTime;
    for (const auto& block : gantt) {
        cout << setw(9) << right << block.endTime;
    }
    cout << "\n";
}

void displayResults(const vector<Process>& processes, const vector<GanttBlock>& gantt, string algoName) {
    cout << "\n=================== " << algoName << " ALGORITMASI SONUCLARI ===================\n";
    cout << "-------------------------------------------------------------------\n";
    cout << left << setw(10) << "Process" 
         << setw(15) << "Arrival Time" 
         << setw(15) << "Burst Time" 
         << setw(18) << "Completion Time" 
         << setw(18) << "Turnaround Time" 
         << setw(15) << "Waiting Time" << endl;
    cout << "-------------------------------------------------------------------\n";

    float totalWT = 0, totalTAT = 0;

    for (const auto& p : processes) {
        cout << left << setw(10) << ("P" + to_string(p.id))
             << setw(15) << p.arrivalTime
             << setw(15) << p.burstTime
             << setw(18) << p.completionTime
             << setw(18) << p.turnaroundTime
             << setw(15) << p.waitingTime << endl;

        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;
    }

    cout << "-------------------------------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Ortalama Bekleme Suresi (Avg Waiting Time)   : " << totalWT / processes.size() << endl;
    cout << "Ortalama Donus Suresi (Avg Turnaround Time) : " << totalTAT / processes.size() << endl;
    cout << "-------------------------------------------------------------------\n";

    printGanttChart(gantt);
}

// 1. FCFS ALGORITMASI
void runFCFS(vector<Process> processes) {
    int currentTime = 0;
    vector<GanttBlock> gantt;

    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            gantt.push_back({-1, currentTime, p.arrivalTime});
            currentTime = p.arrivalTime;
        }

        int startTime = currentTime;
        p.completionTime = currentTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;

        currentTime = p.completionTime;
        gantt.push_back({p.id, startTime, currentTime});
    }

    displayResults(processes, gantt, "FCFS");
}

// 2. SJF ALGORITMASI
void runSJF(vector<Process> processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    vector<GanttBlock> gantt;

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !processes[i].isCompleted) {
                if (processes[i].burstTime < minBurst) {
                    minBurst = processes[i].burstTime;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (!processes[i].isCompleted) {
                    nextArrival = min(nextArrival, processes[i].arrivalTime);
                }
            }
            gantt.push_back({-1, currentTime, nextArrival});
            currentTime = nextArrival;
        } else {
            int startTime = currentTime;
            processes[idx].completionTime = currentTime + processes[idx].burstTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            processes[idx].isCompleted = true;

            currentTime = processes[idx].completionTime;
            completed++;
            gantt.push_back({processes[idx].id, startTime, currentTime});
        }
    }

    displayResults(processes, gantt, "SJF (Non-Preemptive)");
}

// 3. ROUND ROBIN ALGORITMASI
void runRoundRobin(vector<Process> processes, int quantum) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    
    queue<int> readyQueue;
    vector<bool> inQueue(n, false);
    vector<GanttBlock> gantt;

    // Zaman 0'da gelenleri bul
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {
        if (readyQueue.empty()) {
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (!processes[i].isCompleted) {
                    nextArrival = min(nextArrival, processes[i].arrivalTime);
                }
            }
            gantt.push_back({-1, currentTime, nextArrival});
            currentTime = nextArrival;

            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime == currentTime && !processes[i].isCompleted && !inQueue[i]) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        int startTime = currentTime;
        int executeTime = min(quantum, processes[idx].remainingTime);
        
        currentTime += executeTime;
        processes[idx].remainingTime -= executeTime;
        gantt.push_back({processes[idx].id, startTime, currentTime});

        // Yeni gelen süreçleri ekle
        for (int i = 0; i < n; i++) {
            if (i != idx && processes[i].arrivalTime <= currentTime && !processes[i].isCompleted && !inQueue[i]) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (processes[idx].remainingTime == 0) {
            processes[idx].completionTime = currentTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            processes[idx].isCompleted = true;
            completed++;
        } else {
            readyQueue.push(idx);
        }
    }

    displayResults(processes, gantt, "ROUND ROBIN (Quantum = " + to_string(quantum) + ")");
}

// Kullanıcıdan İşlem Verilerini Alan Fonksiyon
vector<Process> getUserInput() {
    int n;
    cout << "\nKac adet Process girmek istiyorsunuz? : ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "\n--- Process P" << processes[i].id << " ---\n";
        cout << "Arrival Time (Gelis Zamani) : ";
        cin >> processes[i].arrivalTime;
        cout << "Burst Time (Islem Suresi)   : ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
    }
    return processes;
}

int main() {
    vector<Process> processes;
    
    cout << "========================================================\n";
    cout << "         CPU SCHEDULING SIMULATOR (OS PROJECT)          \n";
    cout << "========================================================\n";
    cout << "1. Varsayilan Test Verileriyle Calistir\n";
    cout << "2. Kendi Verilerimi Girecegim\n";
    cout << "Seciminiz (1/2): ";
    
    int choice;
    cin >> choice;

    if (choice == 2) {
        processes = getUserInput();
    } else {
        processes = {
            {1, 0, 8, 8, 0, 0, 0},
            {2, 1, 4, 4, 0, 0, 0},
            {3, 2, 9, 9, 0, 0, 0},
            {4, 3, 5, 5, 0, 0, 0}
        };
    }

    int quantum = 2;
    cout << "\nRound Robin icin Time Quantum degeri girin (Varsayilan 2): ";
    cin >> quantum;

    runFCFS(processes);
    runSJF(processes);
    runRoundRobin(processes, quantum);

    return 0;
}