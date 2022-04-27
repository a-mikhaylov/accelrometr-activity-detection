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

char axis(int i) {
	if (i == 0) {
		return 'X';
	} else if (i == 1) {
		return 'Y';
	} else if (i == 2) {
		return 'Z';
	}

	return 'E';
}

void my_OUT(float *arr, int n) {
	for (int i = 0; i < n; i++) {
		cout << arr[i] << endl;
	}
	cout << endl;
}

void my_OUT(vector<float> arr, int n) {
	for (int i = 0; i < n; i++) {
		cout << arr[i] << endl;
	}
	cout << endl;
}

void my_OUT(vector<int32_t> arr, int n) {
	for (int i = 0; i < n; i++) {
		cout << arr[i] << endl;
	}
	cout << endl;
}

void BIG_OUT(float* arr1, float* arr2, float* arr3, float *avg, float *stnd_dev, float *energy, float *corr, int n) {
	cout << "DATA: " << endl << endl;
	cout << "X: " << endl;
	my_OUT(arr1, n);
	cout << "Y: " << endl;
	my_OUT(arr2, n);
	cout << "Z: " << endl;
	my_OUT(arr3, n);

	for (int i = 0; i < 3; i++) {
		cout << "average of\t\t" << axis(i) << " axis: " << avg[i] << endl;
		cout << "standart deviation of\t" << axis(i) << " axis: " << stnd_dev[i] << endl;
		cout << "energy of\t\t" << axis(i) << " axis: " << energy[i] << endl << endl;
		cout << "correlation of\t\t" << axis(i) << " and " << axis((i + 1) % 3) << " axis: "<< corr[i] << endl << endl;
	}
}

void Fill_Arr(float* arr, int n) {
	int count = 7;
	int step[10] = {10, 20, 10, 12, 8, 37, 3};
	int k = 0;
	bool check = true;

	for (int i = 0; i < count; i++) {
		for (int j = 0; j < step[i]; j++) {
			if (i % 2 == 0) {
				if (check) {
					arr[k] = float(rand() / (float)(RAND_MAX)) * 0.2;
					check = false;
				} else {
					arr[k] = -(float(rand() / (float)(RAND_MAX)) * 0.2);
					check = true;
				}
			} else {
				if (check) {
					arr[k] = float(rand() / (float)(RAND_MAX)) * 0.1 + 1.5;
					check = false;
				} else {
					arr[k] = -(float(rand() / (float)(RAND_MAX)) * 0.1 + 1.5);
					check = true;
				}
			}

			k++;
		}	
	}
}

void Read_Header(string filename, vector<float> &info, vector<int> &start_end, vector<string> &featurenames, vector<double> &lsbs, vector<string> &units) {
	ifstream stream(filename, ios::in);
	
	string type = "int32";    // тип данных
	double freq = 0;            // частота
	double lsb = 0;             // вес младшего разряда
	time_t startPoint = 0;      // первая точка
	time_t endPoint = 0;        // ? последняя точка сигнала
	string realTime;            // реальное время
	//vector<string> featurenames;      // имена каналов(полей структуры)
	//vector<double> lsbs;        // веса младшего разряда для каждого канала
	//vector<string> units;       // единицы измерения для каждого канала

	int fieldnum;
	string line;
	getline(stream, line);
	auto tmpstream = istringstream(line);
	tmpstream >> fieldnum >> freq >> lsb; // >> type;

	info.push_back(fieldnum);
	info.push_back(freq);
	info.push_back(lsb);

	if (!tmpstream.eof()) tmpstream >> type;

	featurenames.resize(fieldnum);
	lsbs.resize(fieldnum);
	units.resize(fieldnum);

	stream >> startPoint >> endPoint >> realTime;
	start_end.push_back(startPoint);
	start_end.push_back(endPoint);

	for (int i = 0; i < fieldnum; i++) stream >> featurenames[i];
	for (int i = 0; i < fieldnum; i++) stream >> lsbs[i];
	for (int i = 0; i < fieldnum; i++) stream >> units[i];
	
	stream.close();
}

vector<vector<int32_t>> Read_Bin(string filename, int num_channels, int read_len) {
	ifstream stream(filename, ios::in | ios::binary); 
	
	size_t len = num_channels * read_len;
	int32_t* buf = new int32_t[len];

	vector<vector<int32_t>> data(num_channels);
	for (size_t ch = 0; ch < num_channels; ch++)
		data[ch] = vector<int32_t>(read_len, 0);
		
	if (stream.eof()) return data;
	stream.read((char*)buf, len * sizeof(int32_t));
	stream.close();
	
	for (size_t ch = 0; ch < num_channels; ch++)
	{
	  //auto out = data[ch];
	  for (size_t i = 0; i < read_len; i++)
	  {
		data[ch][i] = buf[i * num_channels + ch];
	  }
	  //data[ch] = out;
	}
	
	delete buf;

	return data;
}

void CSV_out(string fname, vector<string> names, vector<vector<int32_t>> data, vector<float> clusters) {
	ofstream output(fname);

	for(int i = 0; i < names.size(); i++) {
		output << names[i];
		if (i < names.size() - 1) {
			output << ";";
		}
	}
	output << endl;

	for (int i = 0; i < data[0].size(); i++) {
		output << data[0][i] << ";";
		output << data[1][i] << ";";
		output << data[2][i];
		if (i == 0) {
			output << ";";
			for (int j = 0; j < clusters.size(); j++) {
				output << clusters[j] << ";";
			}
		}
		output << endl;
	}
}