#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

struct Process {
	int processId;
	int arrivalTime;
	int cpuBurst;
	int remainingTime;
	int startTime;
	int finishTime;
	int waitingTime;
	int turnaroundTime;
};


void outputInfo(vector<Process>& processes)
{
	cout << "\nFinish time, Waiting time, and Turnaround time for each process:" << endl;
	for (int i = 0; i < processes.size(); ++i) {
		cout << "Process " << processes[i].processId << ": Finish time = " << processes[i].finishTime
			<< ", Waiting time = " << processes[i].waitingTime
			<< ", Turnaround time = " << processes[i].turnaroundTime << endl;
	}
}

void cpuUtilization(vector<Process>& processes) {


	double totaleExecutionTime = 0;
	int maxTime = 0;
	for (int i = 0; i < processes.size(); ++i) {
		totaleExecutionTime += processes[i].cpuBurst;
		if (processes[i].finishTime > maxTime)
			maxTime = processes[i].finishTime;
	}
	double total_time = maxTime;
	double cpu_utilization = (totaleExecutionTime / total_time) * 100;
	cout << "CPU Utilization : " << fixed << setprecision(2) << cpu_utilization << "%" << endl;
}



void FCFS(vector<Process>& processes, int contextSwitch) {

	int currentTime = 0;
	cout << "FCFS Algorithm:" << endl;
	cout << "start\t| Process |\tend" << endl;
	cout << "------------------------------" << endl;
	for (int i = 0; i < processes.size(); ++i) {

		if (currentTime < processes[i].arrivalTime)
			currentTime = processes[i].arrivalTime;

		processes[i].startTime = currentTime;
		cout << currentTime << "\t| P" << processes[i].processId << " |\t";
		currentTime += processes[i].cpuBurst + contextSwitch;
		processes[i].finishTime = currentTime - contextSwitch;
		processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
		processes[i].waitingTime = processes[i].startTime - processes[i].arrivalTime;
		cout << "\t" << processes[i].finishTime << endl;

	}
	cout << "------------------------------" << endl;

	cpuUtilization(processes);
	outputInfo(processes);
}






void SRT(vector<Process>& processes, int contextSwitch) {
	int currentTime = 0;
	int lastProcessId = -1;
	int processesNum = processes.size();

	cout << "SRT Schedule:" << endl;
	cout << "start\t| Process |\tend" << endl;
	cout << "---------------------------------" << endl;
	int processed = 0;

	while (processed < processesNum) {
		int shortest_remaining_time = INT_MAX;
		int selectedProcessId = -1;

		

		for (int i = 0; i < processesNum; ++i) {
			if (processes[i].remainingTime > 0 && processes[i].arrivalTime <= currentTime && processes[i].remainingTime < shortest_remaining_time) {
				shortest_remaining_time = processes[i].remainingTime;
				selectedProcessId = i;
			}
		}

		if (selectedProcessId != -1) {


			if (currentTime < processes[selectedProcessId].arrivalTime)
				currentTime = processes[selectedProcessId].arrivalTime;

			if (lastProcessId != selectedProcessId && lastProcessId != -1 && processes[lastProcessId].remainingTime > 0)
				currentTime += contextSwitch;

			processes[selectedProcessId].startTime = currentTime;
			processes[selectedProcessId].remainingTime--;
			currentTime += 1;
			cout << processes[selectedProcessId].startTime << "\t| P" << processes[selectedProcessId].processId << " |\t\t" << currentTime << endl;

			if (processes[selectedProcessId].remainingTime == 0) {
				processes[selectedProcessId].finishTime = currentTime;
				processes[selectedProcessId].turnaroundTime = processes[selectedProcessId].finishTime - processes[selectedProcessId].arrivalTime;
				processes[selectedProcessId].waitingTime = processes[selectedProcessId].turnaroundTime - processes[selectedProcessId].cpuBurst;
				processed++;
				currentTime += contextSwitch;
			}
		}
		lastProcessId = selectedProcessId;
	}
	cout << "---------------------------------" << endl;

	cpuUtilization(processes);
	outputInfo(processes);
}



void RR(vector<Process>& processes, int quantum, int context_switch) {
	int currentTime = 0;
	int lastProcessId = -1;
	queue<Process> process_queue;
	cout << "RR Algorithm:" << endl;
	cout << "start\t| Process |\tend" << endl;
	cout << "----------------------------" << endl;

	int num_processes = processes.size();
	int processed = 0;

	while (processed < num_processes) {
		for (int i = 0; i < num_processes; ++i) {
			if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
				if (lastProcessId == processes[i].processId)
				{
					currentTime -= context_switch;
				}
				cout << currentTime << "\t| P" << processes[i].processId << " | \t";
				if (processes[i].remainingTime <= quantum) {
					currentTime += processes[i].remainingTime;
					processes[i].finishTime = currentTime;
					processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
					processes[i].waitingTime = processes[i].turnaroundTime - processes[i].cpuBurst;
					processes[i].remainingTime = 0;
					processed++;
					cout << "\t" << currentTime << endl;
					currentTime += context_switch;
				}
				else {

					currentTime += quantum;
					processes[i].remainingTime -= quantum;
					cout << "\t" << currentTime << endl;
					currentTime += context_switch;
				}

				lastProcessId = processes[i].processId;
			}
		}



	}

	cout << "----------------------------" << endl;

	cpuUtilization(processes);
	outputInfo(processes);
}





int main() {
	vector<Process> processes;
	int contextSwitch, quantum;

	ifstream infile("input.txt");
	if (!infile) {
		cerr << " can not to open input file." << endl;
		return 1;
	}

	int processesNum;
	infile >> processesNum;
	processes.resize(processesNum);

	for (int i = 0; i < processesNum; ++i) {
		infile >> processes[i].arrivalTime >> processes[i].cpuBurst;
		processes[i].processId = i + 1;
		processes[i].remainingTime = processes[i].cpuBurst;
	}


	infile >> contextSwitch >> quantum;

	infile.close();


	vector<Process> RRprocesses = processes;
	RR(RRprocesses, quantum, contextSwitch);

	cout << endl;
	vector<Process> FCFSprocesses = processes;
	FCFS(FCFSprocesses, contextSwitch);

	cout << endl;

	vector<Process> SRTprocesses = processes;
	SRT(SRTprocesses, contextSwitch);






	return 0;
}


