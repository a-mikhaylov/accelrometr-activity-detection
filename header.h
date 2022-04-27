#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <stdint.h>
#include <cstring>

using namespace std;

float Average(float* arr, int n);
float Average(vector<int32_t> arr);

float Stnd_Dev(float *arr, int n, float avg);
float Stnd_Dev(vector<int32_t> arr, float avg);

float Energy(float *arr, int n);
float Energy(vector<int32_t> arr);

float Cov(float* arr1, float *arr2, int n, float math_w1, float math_w2);
float Cov(vector<int32_t> arr1, vector<int32_t> arr2, float math_w1, float math_w2);

float Correlation(float cov, float stnd_dev1, float stnd_dev2);

char axis(int i);
void my_OUT(float *arr, int n);
void my_OUT(vector<float> arr, int n);
void my_OUT(vector<int32_t> arr, int n);

void BIG_OUT(float* arr1, float* arr2, float* arr3, float *avg, float *stnd_dev, float *energy, float *corr, int n);

void Fill_Arr(float* arr, int n);

vector<int32_t> Thin_32(vector<int32_t> arr, int freq);

void Zones(float *buff, int n);
vector<int32_t> Zones(vector<int32_t> buff, int accur, float error, int freq);

void Read_Header(string filename, vector<float> &info, vector<int> &start_end, vector<string> &featurenames, vector<double> &lsbs, vector<string> &units);
vector<vector<int32_t>> Read_Bin(string filename, int num_channels, int read_len);

void CSV_out(string fname, vector<string> names, vector<vector<int32_t>> data, vector<float> clusters);