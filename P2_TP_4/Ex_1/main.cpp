#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main() 
{
    try 
    {
        LoadBinary("secret_message.txt");
    }
    catch (const exception& e) 
    {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}

string base64_decode(const string& in) 
{
    string out;
    vector<int> T(256, -1);

    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;

    for (unsigned char c : in) 
    {
        if (T[c] == -1) break;

        val = (val << 6) + T[c];
        valb += 6;

        if (valb >= 0) 
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return out;
}

void LoadBinary(const string& filePath)
{
    ifstream file(filePath);

    if (!file) 
    {
        throw runtime_error("Unable to open file " + filePath);
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string base64Content = buffer.str();
    file.close();

    string binaryContent = base64_decode(base64Content);

    istringstream binaryStream(binaryContent, ios::binary);

    int size;

    binaryStream.read(reinterpret_cast<char*>(&size), sizeof(size));

    if (!binaryStream) 
    {
        throw runtime_error("Error reading the size from " + filePath);
    }

    for (int i = 0; i < size; ++i)
    {
        int value;
        binaryStream.read(reinterpret_cast<char*>(&value), sizeof(value));

        if (!binaryStream)
        {
            throw runtime_error("Error reading value from " + filePath);
        }

        cout << "Value " << i << ": " << value << endl;
    }
}


