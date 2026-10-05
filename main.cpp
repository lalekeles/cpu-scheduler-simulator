#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

void displayResults(const vector<Process>& processes) {
    cout << "\n-------------------------------------------------------------------\n";
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
    cout << "Ortalama Bekleme Süresi (Avg Waiting Time)   : " << totalWT / processes.size() << endl;
    cout << "Ortalama Dönüş Süresi (Avg Turnaround Time) : " << totalTAT / processes.size() << endl;
    cout << "-------------------------------------------------------------------\n";
}

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

    cout << "\n=================== FCFS ALGORİTMASI SONUÇLARI ===================";
    displayResults(processes);
}

int main() {
    vector<Process> processes = {
        {1, 0, 8, 8, 0, 0, 0},
        {2, 1, 4, 4, 0, 0, 0},
        {3, 2, 9, 9, 0, 0, 0},
        {4, 3, 5, 5, 0, 0, 0}
    };

    runFCFS(processes);

    return 0;
}