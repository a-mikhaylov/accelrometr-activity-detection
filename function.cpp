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

float Average(float* arr, int n) {
	float res = 0;

	for (int i = 0; i < n; i++) {
		res += arr[i];
	}

	return res / (float)n;
}

float Average(vector<int32_t> arr) {
	int n = arr.size();
	float res = 0;

	for (int i = 0; i < n; i++) {
		res += arr[i];
	}

	return res / (float)n;
}

float Stnd_Dev(float *arr, int n, float avg) {
	float res = 0;

	for (int i = 0; i < n; i++) {
		res += ((arr[i] - avg) * (arr[i] - avg));
	}

	res = sqrt(res / (float)n);
	return res;
}

float Stnd_Dev(vector<int32_t> arr, float avg) {
	float res = 0;
	int n = arr.size();

	for (int i = 0; i < n; i++) {
		res += ((arr[i] - avg) * (arr[i] - avg));
	}

	res = sqrt(res / (float)n);
	return res;
}

float Energy(float *arr, int n) {
	float res = 0;

	for (int i = 0; i < n; i++) {
		res += arr[i] * arr[i];
	}

	res /= (float)n;
	return res;
}

float Energy(vector<int32_t> arr) {
	float res = 0;
	int n = arr.size();

	for (int i = 0; i < n; i++) {
		res += arr[i] * arr[i];
	}

	res /= (float)n;
	return res;
}

float Cov(float* arr1, float *arr2, int n, float math_w1, float math_w2) {
	float res = 0;
	for (int i = 0; i < n; i++) {
		res += ((arr1[i] - math_w1) * (arr2[i] - math_w2));
	}

	res /= (n - 1);
	return res;
}

float Cov(vector<int32_t> arr1, vector<int32_t> arr2, float math_w1, float math_w2) {
	float res = 0;
	int n = arr1.size();

	for (int i = 0; i < n; i++) {
		res += ((arr1[i] - math_w1) * (arr2[i] - math_w2));
	}

	res /= (n - 1);
	return res;
}

float Correlation(float cov, float stnd_dev1, float stnd_dev2) {
	return cov / (stnd_dev1 * stnd_dev2);
}

vector<int32_t> Thin_32(vector<int32_t> arr, int freq) {
	vector<int32_t> res;
	int size = arr.size();
	int step_1 = freq / 32;
	int step_2 = freq / 32 + 1;
	for (int i = 0; i < arr.size() / freq; i++) {
		for (int j = i * freq; j < i * freq + freq; j++) {
			int cur = j - i * freq;
			if (cur <= freq / 2) {
				if (j % step_1 == 0) {
					res.push_back(arr[j]);
				}
			} else {
				if (j % step_2 == 0) {
					res.push_back(arr[j]);
				}
			}
		}
		res.pop_back();
	}

	return res;
}

void Zones(float *buff, int n) {
	vector<float> devs;
	float curr[5] = {0.0};
	int k = 0;

	for (int i = 0; i < n; i += 5) {
		for (int j = i; j - i < 5; j++) {
			curr[j - i] = buff[j];
		}
				
		devs.push_back(Stnd_Dev(curr, 5, Average(curr, 5)));
	}

	float tmp[2] = {0.0};
	int pos_s = 0;
	int pos_e = 0;

	tmp[0] = devs[0];

	for (int i = 1; i < 20; i++) {
		int cnt = i % 2;
		tmp[cnt] = devs[i];

		if (abs(tmp[0] - tmp[1]) <= 0.1) {
			pos_e++;
		} else {
			pos_e++;
			cout << pos_s * 5 << " - " << pos_e * 5 << endl;
			pos_s = pos_e;
		}
	}

	cout << pos_s * 5 << " - " << (pos_e + 1) * 5 << endl;
}

vector<int32_t> Zones(vector<int32_t> buff, int accur, float error, int freq) {
	vector<int32_t> res;

	vector<float> devs;
	int n = buff.size();
	vector<int32_t> curr(accur);
	int k = 0;

	for (int i = 0; i < n; i += accur) {
		for (int j = i; j - i < accur; j++) {
			curr[j - i] = buff[j];
		}
				
		devs.push_back(Stnd_Dev(curr, Average(curr)));
	}

	vector<float> tmp(2);
	int pos_s = 0;
	int pos_e = 0;

	tmp[0] = devs[0];

	for (int i = 1; i < n / accur; i++) {
		int cnt = i % 2;
		tmp[cnt] = devs[i];
		float relation = abs(tmp[0] / tmp[1]);

		if (relation > 1) {
			relation = 1.0 / relation;
		}

		if (relation >= error) {
			pos_e++;
			/*if (pos_e * accur >= 8500 && pos_e * accur <= 10000) {
				cout << pos_e * accur << ": " << relation << endl;
			}*/
		} else {
			pos_e++;
			res.push_back(pos_e * accur);
			//cout << pos_s * accur / freq << "s - " << pos_e * accur / freq << "s" << endl;
			pos_s = pos_e;
			/*if (pos_e * accur >= 8500 && pos_e * accur <= 10000) {
				cout << pos_e * accur << ": " << relation << endl;
			}*/
		}
	}
	res.push_back(pos_e * accur);
	//cout << pos_s * accur / freq << "s - " << (pos_e + 1) * accur / freq << "s" << endl;
	return res;
}
