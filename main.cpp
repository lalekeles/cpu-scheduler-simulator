#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    bool isCompleted = false; // SJF icin isin bitip bitmedigini takip eder
};

void displayResults(const vector<Process>& processes, string algoName) {
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
}

// 1. FCFS ALGORITMASI
void runFCFS(vector<Process> processes) {
    int currentTime = 0;

    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }

        p.completionTime = currentTime + p.burstTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;

        currentTime = p.completionTime;
    }

    displayResults(processes, "FCFS");
}

// 2. SJF (Non-Preemptive) ALGORITMASI
void runSJF(vector<Process> processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9; // Cok buyuk bir sayi ile basliyoruz

        // O an gelmis olan ve bitmemis en kisa islem sureli process'i bul
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !processes[i].isCompleted) {
                if (processes[i].burstTime < minBurst) {
                    minBurst = processes[i].burstTime;
                    idx = i;
                }
            }
        }

        // Eger o an hicbir process gelmediyse zamani 1 birim ilerlet
        if (idx == -1) {
            currentTime++;
        } else {
            // En kisa isi calistir
            processes[idx].completionTime = currentTime + processes[idx].burstTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            processes[idx].isCompleted = true;

            currentTime = processes[idx].completionTime;
            completed++;
        }
    }

    displayResults(processes, "SJF (Non-Preemptive)");
}

int main() {
    vector<Process> processes = {
        {1, 0, 8, 8, 0, 0, 0},
        {2, 1, 4, 4, 0, 0, 0},
        {3, 2, 9, 9, 0, 0, 0},
        {4, 3, 5, 5, 0, 0, 0}
    };

    // Iki algoritmayi da ayni verilerle calistirip kiyaslayalim
    runFCFS(processes);
    runSJF(processes);

    return 0;
}