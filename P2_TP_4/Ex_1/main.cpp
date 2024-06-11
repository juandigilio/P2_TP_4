#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void LoadBinary(string filePath, vector<int>& nums);
void SaveBinary(const string filePath, vector<int>& nums);
void ShowData(vector<int>& nums);
void SortAndDuplicates(vector<int>& nums);

void main()
{
	try
	{
		vector<int> nums = vector<int>();
		int totalNums;

		LoadBinary("save/numbers_1.dat", nums);
		LoadBinary("save/numbers_2.dat", nums);

		cout << "Values found: " << endl;

		ShowData(nums);

		cout << endl << endl;

		SortAndDuplicates(nums);

		cout << "Values without duplicates: " << endl;

		ShowData(nums);

		SaveBinary("save/numbers_result.dat", nums);
	}
	catch (const std::exception& e)
	{
		cerr << "Error: " << e.what() << endl;
	}

	cin.get();
	cin.get();
}

void LoadBinary(string filePath, vector<int>& nums)
{
	ifstream file(filePath, ios::binary);

	if (!file) 
	{
		throw runtime_error("Unable to open file " + filePath);
	}

	int size;

	//en esta linea no hago el push asi salteo el prier valor que es el encabezado (en este caso tenemos encabezado)
	file.read(reinterpret_cast<char*>(&size), sizeof(size));

	if (!file) 
	{
		throw runtime_error("Error reading the size from " + filePath);
	}

	nums.reserve(nums.size() + size);

	for (int i = 0; i < size; ++i) 
	{
		int value;

		file.read(reinterpret_cast<char*>(&value), sizeof(value));

		if (!file)
		{
			throw runtime_error("Error reading value from " + filePath);
		}

		nums.push_back(value);
	}
}

void SaveBinary(const string filePath, vector<int>& nums)
{
	ofstream file(filePath, ios::binary);

	if (!file) 
	{
		throw runtime_error("Unable to open file " + filePath);
	}

	int size = nums.size();

	file.write(reinterpret_cast<const char*>(&size), sizeof(size));

	if (!file) 
	{
		throw runtime_error("Error writing the size to " + filePath);
	}

	for (int value : nums)
	{
		file.write(reinterpret_cast<const char*>(&value), sizeof(value));

		if (!file) 
		{
			throw runtime_error("Error writing value to " + filePath);
		}
	}
}

void ShowData(vector<int>& nums)
{
	for (auto it = nums.begin(); it != nums.end(); ++it)
	{
		cout << *it << ", ";
	}
}

void SortAndDuplicates(vector<int>& nums)
{
	sort(nums.begin(), nums.end());

	vector<int>::iterator last = unique(nums.begin(), nums.end());

	nums.erase(last, nums.end());
}