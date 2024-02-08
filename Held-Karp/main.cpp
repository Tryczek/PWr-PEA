#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <iomanip>

using namespace std;
using namespace chrono;

// Struktura przechowuj¹ca instancjê problemu TSP
struct TSPInstance {
    int numCities;
    vector<vector<int>> distance;

    TSPInstance(int n) : numCities(n), distance(n, vector<int>(n)) {}
};

// Funkcja do wczytywania instancji TSP z pliku
TSPInstance readTSPInstance(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "B³¹d podczas otwierania pliku" << endl;
        exit(1);
    }

    int numCities;
    file >> numCities;
    TSPInstance instance(numCities);

    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            file >> instance.distance[i][j];
        }
    }

    return instance;
}

// Funkcja do obliczania d³ugoœci trasy w problemie TSP
int calculateTourLength(const vector<int>& tour, const TSPInstance& instance) {
    int length = 0;
    for (int i = 0; i < instance.numCities; ++i) {
        int from = tour[i];
        int to = tour[(i + 1) % instance.numCities];
        length += instance.distance[from][to];
    }
    return length;
}

// Algorytm Held-Karp do rozwi¹zania problemu TSP
int heldKarp(const TSPInstance& instance, vector<int>& bestTour) {
    const int n = instance.numCities;
    vector<vector<int>> dp(1 << n, vector<int>(n, -1));
    dp[1][0] = 0;

    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int u = 1; u < n; ++u) {
            if ((mask & (1 << u)) == 0) continue;

            for (int v = 0; v < n; ++v) {
                if ((mask & (1 << v)) && u != v && dp[mask ^ (1 << u)][v] != -1) {
                    int cost = dp[mask ^ (1 << u)][v] + instance.distance[v][u];
                    if (dp[mask][u] == -1 || cost < dp[mask][u]) {
                        dp[mask][u] = cost;
                    }
                }
            }
        }
    }

    int mask = (1 << n) - 1;
    int minTourLength = -1;
    for (int u = 1; u < n; ++u) {
        if (instance.distance[u][0] != -1 && dp[mask][u] != -1) {
            int tourLength = dp[mask][u] + instance.distance[u][0];
            if (minTourLength == -1 || tourLength < minTourLength) {
                minTourLength = tourLength;

                int current = u;
                int maskCopy = mask;
                vector<int> tour;
                tour.push_back(0);
                while (maskCopy != 1) {
                    for (int v = 0; v < n; ++v) {
                        if ((maskCopy & (1 << v)) && v != current && dp[maskCopy][current] == dp[maskCopy ^ (1 << current)][v] + instance.distance[v][current]) {
                            tour.push_back(v);
                            maskCopy ^= (1 << current);
                            current = v;
                            break;
                        }
                    }
                }
                bestTour = tour;
            }
        }
    }

    return minTourLength;
}

int main() {
    const string iniFilename = "plik.ini";
    ofstream resultsFile("wyniki.txt");

    if (!resultsFile) {
        cerr << "B³¹d podczas otwierania pliku wynikowego" << endl;
        exit(1);
    }

    ifstream iniFile(iniFilename);
    if (!iniFile) {
        cerr << "B³¹d podczas otwierania pliku ini" << endl;
        exit(1);
    }

    vector<pair<string, int>> instances;
    string instanceFilename;
    int repetitions;

    while (iniFile >> instanceFilename >> repetitions) {
        instances.push_back({instanceFilename, repetitions});
    }

    for (const auto& instanceData : instances) {
        string filename = instanceData.first;
        int repetitions = instanceData.second;

        TSPInstance instance = readTSPInstance(filename);

        double totalTime = 0;  // Dodaj zmienn¹ do œledzenia czasu

        int minTourLength = 0;  // Przenieœ deklaracjê poza pêtlê

        for (int i = 0; i < repetitions; ++i) {
            auto start = high_resolution_clock::now();
            vector<int> tour;
            minTourLength = heldKarp(instance, tour);  // Zaktualizuj minTourLength
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            totalTime += duration.count() / 1000.0;  // Oblicz czas w ms

            // Wypisz trasê i czas
            cout << "Trasa: ";
            for (int city : tour) {
                cout << city << " -> ";
            }
            cout << tour[0] << endl;

            cout << "Czas: " << fixed << setprecision(2) << duration.count() / 1000.0 << " ms" << endl;
        }

        // Oblicz œredni czas i wypisz go
        double averageTime = totalTime / repetitions;
        cout << "Œredni czas: " << fixed << setprecision(2) << averageTime << " ms" << endl;

        resultsFile << "Plik: " << filename << endl;
        resultsFile << "Minimalna d³ugoœæ trasy: " << minTourLength << endl;
        resultsFile << "Œredni czas: " << fixed << setprecision(2) << averageTime << " ms" << endl;
        resultsFile << endl;
    }

    resultsFile.close();

    return 0;
}

