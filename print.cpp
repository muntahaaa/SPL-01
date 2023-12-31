#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
#include "FCB.cpp"
#include "JaccardSimilarity.cpp"

#define ANSI_RESET "\033[0m"
#define ANSI_WHITE "\033[97m"
#define ANSI_ON_WHITE "\033[107m"
#define ANSI_ON_YELLOW "\033[43m"

using namespace std;
vector<string> directoryNames;
string files, nameOfFile;

vector<char> charVector1;
vector<char> charVector2;

void printProperties(vector<string> directoryItems, vector<string> &names)
{
    int count = 0;
    cout << ANSI_ON_YELLOW << ANSI_BOLD << "\nFile properties are: "
         << ANSI_RESET << endl;
    for (const string &itemPath : directoryItems)
    {

        files = itemPath;
        nameOfFile = names[count++];

        if (loadFile(files, fcb, nameOfFile))
        {
            displayFileInformation(fcb);

            cout << endl;
        }
    }
}
void printMat(double mat[100][100], vector<string> directoryItems)
{

    cout << ANSI_ON_YELLOW << ANSI_BOLD << "\nJaccard Similarity Matrix:"
         << ANSI_RESET << endl;
    for (size_t i = 0; i < directoryItems.size(); i++)
    {
        for (size_t j = 0; j < directoryItems.size(); j++)
        {

            printf("%.2lf        ", mat[i][j]);
        }
        cout << endl;
    }
}

string getFileExtension(const string &filename)
{

    size_t dotPos = filename.find_last_of('.');

    if (dotPos != std::string::npos && dotPos < filename.length() - 1)
    {

        return filename.substr(dotPos + 1);
    }

    return "";
}

vector<char> convertToChar(vector<string> contents)
{
    vector<char> charVector;

    for (const string &str : contents)
    {
        charVector.insert(charVector.end(), str.begin(), str.end());
    }
    return charVector;
}
void similarityChecking(vector<string> directoryItems, FileControlBlock &fcb, vector<string> &names)
{
    cout << ANSI_ON_YELLOW << ANSI_BOLD << "Text Similarity Between Files Are:"
         << ANSI_RESET << endl;
    double jaccardMat[100][100];
    for (size_t i = 0; i < directoryItems.size(); i++)
    {
        for (size_t j = 0; j < directoryItems.size(); j++)
        {

            string filename1 = directoryNames[i];
            string path1 = directoryItems[i];
            loadFile(path1, fcb, filename1);

            vector<char> charVector1(content.begin(), content.end());

            fcb.content.clear();

            string filename2 = directoryNames[j];
            string path2 = directoryItems[j];
            loadFile(path2, fcb, filename2);

            vector<char> charVector2(content.begin(), content.end());

            cout << "path1 " << path1 << endl;
            cout << "file1 " << filename1 << endl;

            displayVector(charVector1, "File 1");
            displayVector(charVector2, "File 2");

            vector<char> unionVector;
            unionVectors(charVector1, charVector2, unionVector);

            vector<char> intersectionVector;
            intersectionVectors(charVector1, charVector2, intersectionVector);

            printNotMatchedCharacters(charVector1, intersectionVector, filename1, filename2);

            double jaccardSimilarity = static_cast<double>(intersectionVector.size()) / unionVector.size();
            cout << ANSI_BOLD "\nJaccard similarity: " << ANSI_RESET
                 << ANSI_ON_WHITE << ANSI_BOLD << jaccardSimilarity * 100 << "%" << ANSI_RESET << endl
                 << endl;

            double similar = jaccardSimilarity * 100;

            jaccardMat[i][j] = similar;
        }
    }

    printMat(jaccardMat, directoryItems);
}

void checkEqualities(vector<string> directoryItems, FileControlBlock &fcb, vector<string> &names, const char *directoryPath)
{
    struct FileControlBlocks
    {
        string filename;
        string name;
        string content;
        string hash;
        size_t size;
        FileType type;
        FileStatus status;
        string permissions;
        string extension;

        bool operator==(const FileControlBlocks &other) const
        {
            return (

                content == other.content &&
                hash == other.hash &&
                size == other.size &&
                type == other.type &&
                status == other.status &&
                permissions == other.permissions &&
                extension == other.extension);
        }

        FileControlBlocks &operator=(const FileControlBlocks &other)
        {
            if (this != &other)
            {
                filename = other.filename;
                name = other.name;
                content = other.content;
                hash = other.hash;
                size = other.size;
                type = other.type;
                status = other.status;
                permissions = other.permissions;
                extension = other.extension;
            }
            return *this;
        }
    };

    vector<FileControlBlocks> fcbs;

    for (size_t i = 0; i < directoryItems.size(); i++)
    {
        FileControlBlocks currentFile;
        loadFile(directoryItems[i], fcb, directoryNames[i]);

        currentFile.filename = fcb.filename;
        currentFile.name = fcb.name;
        currentFile.content = content;
        currentFile.hash = fcb.hash;
        currentFile.size = fcb.size;
        currentFile.type = fcb.type;
        currentFile.status = fcb.status;
        currentFile.permissions = returnPermission(fcb);
        currentFile.extension = getFileExtension(directoryItems[i]);

        fcbs.push_back(currentFile);
    }

    bool similarityFound = false;
    string fileNametoDelete;

    for (size_t i = 0; i < directoryItems.size(); i++)
    {
        cout << fcbs[i].name << " " << fcbs[i].content << "=" << fcbs[i].permissions << endl
             << endl;
        for (size_t j = i + 1; j < directoryItems.size(); j++)
        {

            if (fcbs[i] == fcbs[j] && i != j)
            {
                cout << ANSI_ON_WHITE << "similarity found in ";
                cout << ANSI_BOLD << fcbs[i].name << ANSI_RESET
                     << ANSI_ON_WHITE << " and " << ANSI_BOLD << fcbs[j].name
                     << ANSI_RESET << endl;
                fileNametoDelete = fcbs[j].name;

                const char *fileName = fileNametoDelete.c_str();
                char fullPath[256];
                snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryPath, fileName);

                int result = remove(fullPath);

                if (result == 0)
                {
                    cout << ANSI_ON_WHITE << "duplicate file " << ANSI_BOLD
                         << fileNametoDelete << ANSI_RESET
                         << ANSI_ON_WHITE << " has been deleted" << ANSI_RESET << endl;
                }
                else
                {
                    cout << "Error in deleting" << endl;
                }
                similarityFound = true;
            }
        }
    }

    if (!similarityFound)
    {
        cout << ANSI_ON_WHITE << ANSI_BOLD << "No similarity found"
             << ANSI_RESET << endl;
    }
}
