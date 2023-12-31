#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>

#define ANSI_RESET "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_ITALIC "\033[3m"

using namespace std;

void readCharactersFromFile(const string &filepath, vector<char> &charVector)
{
    ifstream file(filepath);

    if (file.is_open())
    {
        char c;
        while (file.get(c))
        {
            charVector.push_back(c);
        }
        file.close();
    }
    else
    {
        cerr << "Error opening file: " << filepath << endl;
    }
}

void displayVector(const vector<char> charVector, const string vectorName)
{
    cout << vectorName << ": ";
    for (const char c : charVector)
    {
        cout << c;
    }
    cout << endl;
}

void unionVectors(const vector<char> &vec1, const vector<char> &vec2, vector<char> &result)
{
    unordered_set<char> resultSet(vec1.begin(), vec1.end());

    for (const char &c : vec2)
    {
        resultSet.insert(c);
    }

    result.assign(resultSet.begin(), resultSet.end());
}

void intersectionVectors(const vector<char> &vec1, const vector<char> &vec2, vector<char> &result)
{
    unordered_set<char> set1(vec1.begin(), vec1.end());
    unordered_set<char> set2(vec2.begin(), vec2.end());

    for (const char &c : set1)
    {
        if (set2.count(c) > 0)
        {
            result.push_back(c);
        }
    }
}

void printNotMatchedCharacters(const vector<char> &vec1, const vector<char> &vec2, string file1, string file2)
{
    unordered_set<char> set1(vec1.begin(), vec1.end());
    unordered_set<char> set2(vec2.begin(), vec2.end());

    if (file1 != file2)
    {
        cout << ANSI_ITALIC << "Unmatched characters between "
             << ANSI_BOLD << file1 << ANSI_RESET << ANSI_ITALIC << " and "
             << ANSI_BOLD << file2 << ": " << ANSI_RESET;
        for (const char &c : set1)
        {
            if (set2.count(c) <= 0)
            {
                cout << c;
            }
        }
    }
}
