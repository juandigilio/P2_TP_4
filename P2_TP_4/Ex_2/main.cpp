#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>
#include <future>
#include <mutex>
#include <chrono>

using namespace std;

struct Statistics
{
    int characters = 0;
    int words = 0;
    int vocals = 0;
    int consonants = 0;
    int punctuationMarks = 0;
};

struct Data
{
    Statistics statistics;
    string text;
};

void ReadFile(const char dir[]);
bool KeepRunning();
void GetStatistics(string text, Statistics& statistics);
bool IsVowel(char character);
bool IsConsonant(char character);
void CountWords(Statistics& statistics, char character, bool wasLetter);
Statistics TotalProm();
void ShowStatistics(Statistics statistics);
void ShowProm(Statistics statistics);


const int bufferSize = 512;

vector<Data> inputData;
mutex dataMutex;
vector<future<void>> futures;

int main()
{
    futures.push_back(async(launch::async, ReadFile, "save/text_1.txt"));
    futures.push_back(async(launch::async, ReadFile, "save/text_2.txt"));
    futures.push_back(async(launch::async, ReadFile, "save/text_3.txt"));

    while (KeepRunning())
    {
        cout << "Reading files..." << endl;
        this_thread::sleep_for(chrono::milliseconds(800));
    }

    Statistics prom = TotalProm();
    ShowProm(prom);

    cin.get();
    cin.get();

    return 0;
}

void ReadFile(const char dir[])
{
    Data data;

    int delay = rand() % 500 + 1000;

    try
    {
        ifstream inputStream(dir);

        if (!inputStream.is_open())
        {
            throw runtime_error("Error opening file");
        }

        while (!inputStream.eof())
        {
            char tempText[bufferSize];
            inputStream.getline(tempText, bufferSize);
            data.text += tempText;
            data.text += "\n";

            GetStatistics(tempText, data.statistics);
        }

        inputStream.close();
        
        lock_guard<mutex> guard(dataMutex);

        inputData.push_back(data);    

        this_thread::sleep_for(chrono::milliseconds(delay));

        cout << endl << endl << data.text << endl << endl;

        ShowStatistics(data.statistics);
    }
    catch (const exception& e)
    {
        cerr << "File could not be open >> " << dir << " : " << e.what() << endl;
    }
}

bool KeepRunning()
{
    for (auto& activeFuture : futures)
    {
        if (activeFuture.valid() && activeFuture.wait_for(chrono::seconds(0)) != future_status::ready)
        {
            return true;
        }
    }

    return false;
}

void GetStatistics(string text, Statistics& statistics)
{
    bool wasLetter = false;

    for (int i = 0; i < text.length(); i++)
    {
        statistics.characters++;

        CountWords(statistics, text[i], wasLetter);

        if (IsVowel(text[i]))
        {
            statistics.vocals++;
            wasLetter = true;
        }
        else if (ispunct(text[i]))
        {
            statistics.punctuationMarks++;
            wasLetter = true;
        }
        else if (IsConsonant(text[i]))
        {
            statistics.consonants++;
            wasLetter = false;
        }
        else
        {
            wasLetter = false;
        }
    }
}

bool IsVowel(char character)
{
    return tolower(character) == 'a' || tolower(character) == 'e' || tolower(character) == 'i' || tolower(character) == 'o' || tolower(character) == 'u';
}

bool IsConsonant(char character)
{
    character = tolower(character);

    return (isalpha(character) && !IsVowel(character));
}

void CountWords(Statistics& statistics, char character, bool wasLetter)
{
    if (IsVowel(character) || IsConsonant(character))
    {
        if (!wasLetter)
        {
            statistics.words++;
        }
    }
}

Statistics TotalProm()
{
    Statistics prom;

    for (auto& data : inputData)
    {
        prom.characters += data.statistics.characters;
        prom.words += data.statistics.words;
        prom.vocals += data.statistics.vocals;
        prom.consonants += data.statistics.consonants;
        prom.punctuationMarks += data.statistics.punctuationMarks;
    }

    prom.characters /= 3;
    prom.words /= 3;
    prom.vocals /= 3;
    prom.consonants /= 3;
    prom.punctuationMarks /= 3;
    
    return prom;
}

void ShowStatistics(Statistics statistics)
{
    cout << "Statistics:" << endl;
    cout << "Total words: " << statistics.words << endl;
    cout << "Total vowels: " << statistics.vocals << endl;
    cout << "Total consonants: " << statistics.consonants << endl;
    cout << "Total punctuation marks: " << statistics.punctuationMarks << endl;
    cout << "Total characters: " << statistics.characters << endl << endl << endl;
}

void ShowProm(Statistics statistics)
{
    cout << "Average statistics:" << endl;
    cout << "Average words: " << statistics.words << endl;
    cout << "Average vowels: " << statistics.vocals << endl;
    cout << "Average consonants: " << statistics.consonants << endl;
    cout << "Average punctuation marks: " << statistics.punctuationMarks << endl;
    cout << "Average characters: " << statistics.characters << endl << endl << endl;
}