#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_set>

using namespace std;

vector<double> weights;
const int numOfInputs = 6;
//int dataSetNum = 1 << numOfInputs;
int dataSetNum;
double corr_d;
//auto activationFunc = [&](double x) {return 1 / (1 + exp(-x)); };
auto activationFunc = [&](double x) {return x / 6; };

bool function(vector<int> &inputs)
{
	double sum = 0;
	for (int i = 0; i < numOfInputs; i++) {
		sum += inputs[i] * weights[i];
	}
	return activationFunc(sum) >= 1;
}

int main()
{
	// (a ∧ b ∧ c ∧ d) ∨ e ∨ f
	setlocale(LC_ALL, "Russian");
	ifstream fin("../input.txt");
	ifstream dfin("../data.txt");
	vector<int> inputs(numOfInputs);
	weights = vector<double>(numOfInputs);

	string toDo;
	fin >> toDo;
	if (toDo == "learn") {
		fin >> dataSetNum >> corr_d;

		vector<int> nums(1 << numOfInputs);
		for (int i = 0; i < 1 << numOfInputs; i++) {
			nums[i] = i;
		}
		srand(time(NULL));
		random_shuffle(nums.begin(), nums.end());
		//sort(nums.begin(), nums.begin() + dataSetNum);

		int loops = 0, j = 0, right_answ_per_cicle = 0, failture_loops = 100;

		while (right_answ_per_cicle != dataSetNum && loops < failture_loops) { // with exit condition
		//while (loops < failture_loops) { // without exit condition
			right_answ_per_cicle = 0;
			loops++;

			for (int j = 0; j < dataSetNum; j++) {
				dfin.seekg(nums[j] * 17, ios_base::beg);
				for (auto &input_unit : inputs) {
					dfin >> input_unit;
				}
				bool check_answer; dfin >> check_answer;
				if (function(inputs) == check_answer) {
					right_answ_per_cicle++;
				} else {
					for (int i = 0; i < numOfInputs; i++) {
						if (inputs[i]) {
							if (check_answer) {
								weights[i] += corr_d;
							} else {
								weights[i] -= corr_d;
							}
						}
					}
				}
			}

			cout << "Проход #" << loops << " Количество правильных ответов: " << right_answ_per_cicle << endl;
		}

		cout << "\nВсего было использовано " << loops * dataSetNum << " наборов данных\n";

		ofstream fout("../output.txt");
		for (auto &wi : weights) {
			fout << wi << endl;
		}
		fout.close();
	}

	// Проход по всем значениям готовой функцией
	int right_answ_per_cicle = 0;
	ifstream weightsStream("../output.txt");
	for (auto &weight : weights) {
		weightsStream >> weight;
	}

	dfin.seekg(0, ios_base::beg);
	for (int j = 0; j < 1 << numOfInputs; j++) {
		for (auto &input_unit : inputs) {
			dfin >> input_unit;
		}
		bool check_answer; dfin >> check_answer;
		if (function(inputs) == check_answer) {
			right_answ_per_cicle++;
		}
	}
	cout << "\nПосле обучения функция выдаёт " << right_answ_per_cicle << " правильных значений из 64\n";
}