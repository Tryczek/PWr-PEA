#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main() {
    int numIterations;
    cout << "Podaj ilosc iteracji: ";
    cin >> numIterations;

    string inputFileName;  // Dodaj zmienn¹ do przechowywania nazwy pliku wejœciowego
    cout << "Podaj nazwe pliku wejsciowego: ";
    cin >> inputFileName;

    ofstream outputFile("wyjscie.txt"); // Otwieramy plik do zapisu wyników
    if (!outputFile) {
        cerr << "Nie uda³o siê otworzyæ pliku wyjœcia." << endl;
        return 1;
    }

    for (int iteration = 0; iteration < numIterations; iteration++) {
        ifstream inputFile(inputFileName);  // U¿yj wybranej nazwy pliku wejœciowego
        if (!inputFile) {
            cerr << "Nie uda³o siê otworzyæ pliku z danymi." << endl;
            return 1;
        }

        int n; // Liczba miast
        inputFile >> n;

        vector<vector<int>> costMatrix(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                inputFile >> costMatrix[i][j];
            }
        }

        vector<int> cities(n);
        for (int i = 0; i < n; i++) {
            cities[i] = i;
        }

        int minCost = INT_MAX;
        vector<int> optimalTour;

        steady_clock::time_point startTime = steady_clock::now();

        do {
            int currentCost = 0;
            for (int i = 0; i < n - 1; i++) {
                currentCost += costMatrix[cities[i]][cities[i + 1]];
            }
            currentCost += costMatrix[cities[n - 1]][cities[0]]; // Wracamy do pierwszego miasta

            if (currentCost < minCost) {
                minCost = currentCost;
                optimalTour = cities;
            }
        } while (next_permutation(cities.begin() + 1, cities.end()));

        steady_clock::time_point endTime = steady_clock::now();
        auto duration = duration_cast<microseconds>(endTime - startTime).count();

        outputFile << "Najlepsza trasa (" << iteration + 1 << "): ";
        for (int city : optimalTour) {
            outputFile << city << " ";
        }
        outputFile << optimalTour[0] << endl; // Wracamy do pierwszego miasta
        outputFile << "Koszt najkrotszej trasy (" << iteration + 1 << "): " << minCost << endl;
        outputFile << "Czas przeszukiwania (" << iteration + 1 << "): " << duration << " mikrosekund" << endl;

        inputFile.close();
    }

    outputFile.close();
    return 0;
}

