#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <stdint.h>
#include <cstring>
#include "header.h"

using namespace std;

#define N 100

int main() {
	float buff[N] = {0};

	float avg[6] = {0}; // = math waiting
	float energy[6] = {0};
	float stnd_dev[6] = {0};
	float cov[3] = {0};		//cov[0] - (x,y); cov[1] - (y,z); cov[2] - (x,z) 
	float corr[3] = {0};

	vector<float> info;
	vector<int> start_end;
	vector<string> featurenames;
	vector<double> lsbs;
	vector<string> units;
	vector<vector<int32_t>> markup;

	string hf_name;
	string bf_name;

	cout << "Input header file name: ";
	cin >> hf_name;
	cout << "Input binary file name: ";
	cin >> bf_name;

	Read_Header("PX102140321173241.hdr", info, start_end, featurenames, lsbs, units); //PX102140321173241.hdr

	vector<vector<int32_t>> data = Read_Bin("PX102140321173241.bin", (int)info[0], (int)start_end[1] + 1);

	for (int i = 0; i < 6; i++) {
		avg[i] = Average(data[i]);
		stnd_dev[i] = Stnd_Dev(data[i], avg[i]);
		energy[i] = Energy(data[i]);
	}

	for (int i = 0; i < 3; i++) {
		cov[i] = Cov(data[i], data[(i + 1) % 3], stnd_dev[i], stnd_dev[(i + 1) % 3]);
		corr[i] = Correlation(cov[i], stnd_dev[i], stnd_dev[(i + 1) % 3]);
	}
	
	while (true) {
		int var;
		cout << "What you want to do?\n";
		cout << "0 - exit\n";
		cout << "1 - Make your data thin\n";
		cout << "2 - Output data in file\n";
		cout << "3 - Output parameters on screen\n";
		cout << "4 - Calculate zones of activities\n";
		cout << "5 - Output zones markup in file" << endl << endl;
		cin >> var;
		cout << endl;
		
		if (var == 0) {
			return 0;
		} else if (var == 1) {

			for (int i = 0; i < (int)info[0]; i++) {
				data[i] = Thin_32(data[i], info[1]);
			}

		} else if (var == 2) {

			for(int i = 0; i < 3; i++) featurenames.pop_back();

			featurenames.push_back("Average_X");
			featurenames.push_back("Average_Y");
			featurenames.push_back("Average_Z");

			featurenames.push_back("StndDev_X");
			featurenames.push_back("StndDev_Y");
			featurenames.push_back("StndDev_Z");

			featurenames.push_back("Energy_X");
			featurenames.push_back("Energy_Y");
			featurenames.push_back("Energy_Z");

			featurenames.push_back("Correlation_X-Y");
			featurenames.push_back("Correlation_Y-Z");
			featurenames.push_back("Correlation_Z-X");

			vector<float> clus;

			for (int i = 0; i < 3; i++) {
				clus.push_back(avg[i]);
			}

			for (int i = 0; i < 3; i++) {
				clus.push_back(stnd_dev[i]);
			}

			for (int i = 0; i < 3; i++) {
				clus.push_back(energy[i]);
			}

			for (int i = 0; i < 3; i++) {
				clus.push_back(corr[i]);
			}

			string dof_name;
			cout << "Input output file name: ";
			cin >> dof_name;
			CSV_out(dof_name, featurenames, data, clus);

		} else if (var == 3) {
			for (int i = 0; i < 6; i++) {
				cout << "average of\t\t" << axis(i) << " axis: " << avg[i] << endl;
				cout << "standart deviation of\t" << axis(i) << " axis: " << stnd_dev[i] << endl;
				cout << "energy of\t\t" << axis(i) << " axis: " << energy[i] << endl << endl;
				if (i < 3) {
					cout << "correlation of\t\t" << axis(i) << " and " << axis((i + 1) % 3) << " axis: "<< corr[i] << endl << endl;
				}
			}		
		} else if (var == 4) {
			int step, freq;
			float error;
			cout << "Input accurancy (step) of calculating: ";
			cin >> step;
			cout << "Input frequency of data: ";
			cin >> freq;
			cout << "Input error of calculating: ";
			cin >> error;	
			markup.push_back(Zones(data[0], step, error, freq));
			markup.push_back(Zones(data[1], step, error, freq));
			markup.push_back(Zones(data[2], step, error, freq));
		} else if (var == 5) {
			if (markup.size() == 0) {
				cout << "You haven't calculate markup" << endl;
			} else {
				vector<string> head;
				head.push_back("X_markup");
				head.push_back("Y_markup");
				head.push_back("Z_markup");
				vector<float> empty;
				string mof_name;
				cout << "Input output file name: ";
				cin >> mof_name;
				CSV_out(mof_name, head, markup, empty);
			}
		}
		cout << endl << endl << endl;
	}

	//cout << i << "%";		
	//cout << "\r";
}
