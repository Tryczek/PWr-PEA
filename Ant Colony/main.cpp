#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>

using namespace std;

struct City {
    double x, y;
};

double distance(const City& city1, const City& city2) {
    return sqrt((city1.x - city2.x) * (city1.x - city2.x) + (city1.y - city2.y) * (city1.y - city2.y));
}

double calculateTotalDistance(const vector<City>& cities, const vector<int>& path) {
    double totalDistance = 0.0;
    int n = path.size();

    for (int i = 0; i < n - 1; ++i) {
        totalDistance += distance(cities[path[i]], cities[path[i + 1]]);
    }

    totalDistance += distance(cities[path[n - 1]], cities[path[0]]);

    return totalDistance;
}

void initializePheromone(vector<vector<double>>& pheromone, const vector<City>& cities) {
    int n = pheromone.size();
    double totalDistance = 0.0;
    int numAnts = min(10, n);

    for (int ant = 0; ant < numAnts; ++ant) {
        vector<int> randomPath(n);
        iota(randomPath.begin(), randomPath.end(), 0);
        shuffle(randomPath.begin(), randomPath.end(), default_random_engine(random_device()()));

        double antDistance = calculateTotalDistance(cities, randomPath);

        for (int i = 0; i < n - 1; ++i) {
            pheromone[randomPath[i]][randomPath[i + 1]] = 1.0 / antDistance;
        }
        pheromone[randomPath[n - 1]][randomPath[0]] = 1.0 / antDistance;

        totalDistance += antDistance;
    }

    double initial_pheromone = 1.0 / totalDistance;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pheromone[i][j] = initial_pheromone;
        }
    }
}

void calculateVisibility(const vector<City>& cities, vector<vector<double>>& visibility) {
    int n = cities.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                visibility[i][j] = 1.0 / distance(cities[i], cities[j]);
            }
        }
    }
}

int selectNextCity(int currentCity, const vector<bool>& visited, const vector<vector<double>>& pheromone, const vector<vector<double>>& visibility, double alpha, double beta) {
    int n = pheromone.size();
    double totalProbability = 0.0;
    vector<double> probabilities(n, 0.0);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            double pheromoneValue = pow(pheromone[currentCity][i], alpha);
            double visibilityValue = pow(visibility[currentCity][i], beta);
            probabilities[i] = pheromoneValue * visibilityValue;
            totalProbability += probabilities[i];
        }
    }

    uniform_real_distribution<double> distribution(0.0, 1.0);
    std::random_device rd;
    std::mt19937 gen(rd());
    double randomValue = distribution(gen);

    double cumulativeProbability = 0.0;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            cumulativeProbability += probabilities[i] / totalProbability;
            if (cumulativeProbability >= randomValue) {
                return i;
            }
        }
    }

    return -1;
}

void antColonyOptimization(const vector<City>& cities, int max_iterations, double alpha, double beta, double rho, int m, double initial_pheromone, double& bestDistance, vector<int>& bestPath, double& elapsedTime) {
    int n = cities.size();
    vector<vector<double>> pheromone(n, vector<double>(n, initial_pheromone));
    vector<vector<double>> visibility(n, vector<double>(n, 0.0));

    initializePheromone(pheromone, cities);
    calculateVisibility(cities, visibility);

    auto start_time = chrono::high_resolution_clock::now();

    bestPath.clear();
    bestDistance = numeric_limits<double>::infinity();

    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        vector<vector<double>> deltaPheromone(n, vector<double>(n, 0.0));

        for (int ant = 0; ant < m; ++ant) {
            vector<bool> visited(n, false);
            vector<int> path;
            int currentCity = rand() % n;
            visited[currentCity] = true;
            path.push_back(currentCity);

            for (int step = 1; step < n; ++step) {
                int nextCity = selectNextCity(currentCity, visited, pheromone, visibility, alpha, beta);

                if (nextCity != -1) {
                    visited[nextCity] = true;
                    deltaPheromone[currentCity][nextCity] += 1.0 / distance(cities[currentCity], cities[nextCity]);
                    currentCity = nextCity;
                    path.push_back(currentCity);
                }
            }

            double totalDistance = calculateTotalDistance(cities, path);

            for (int i = 0; i < n - 1; ++i) {
                deltaPheromone[path[i]][path[i + 1]] += 1.0 / totalDistance;
            }
            deltaPheromone[path[n - 1]][path[0]] += 1.0 / totalDistance;

            if (totalDistance < bestDistance) {
                bestDistance = totalDistance;
                bestPath = path;
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                pheromone[i][j] = pheromone[i][j] * (1.0 - rho) + deltaPheromone[i][j];
            }
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    elapsedTime = chrono::duration<double>(end_time - start_time).count();
}

void readConfigurationFile(const string& configFile, vector<pair<string, int>>& fileIterations) {
    ifstream config(configFile);

    if (!config.is_open()) {
        cerr << "Nie udalo sie otworzyc pliku konfiguracyjnego: " << configFile << endl;
        exit(EXIT_FAILURE);
    }

    while (!config.eof()) {
        string fileName;
        int iterations;
        config >> fileName >> iterations;
        if (!fileName.empty()) {  // Sprawdzenie, czy linia nie jest pusta
            fileIterations.emplace_back(fileName, iterations);
        }
    }

    config.close();
}

void saveResultsToFile(const string& fileName, double cost, double elapsedTime, const vector<int>& bestPath) {
    cout << fileName << " " << cost << " " << elapsedTime << " sekundy" << endl;
    
    if (cost != numeric_limits<double>::infinity()) {
        cout << "Best Path: ";
        for (int city : bestPath) {
            cout << city << " ";
        }
        cout << endl;
    } else {
        cout << "No valid path found." << endl;
    }
}


int main() {
    vector<pair<string, int>> fileIterations;
    readConfigurationFile("conf.ini", fileIterations);

    for (const auto& entry : fileIterations) {
        vector<City> cities;
        double bestDistance;
        vector<int> bestPath;
        double elapsedTime;

        ifstream file(entry.first);

        if (!file.is_open()) {
            cerr << "Nie udalo sie otworzyc pliku: " << entry.first << endl;
            continue;
        }

        int n;
        file >> n;

        cities.resize(n);
        for (int i = 0; i < n; ++i) {
            file >> cities[i].x >> cities[i].y;
        }

        file.close();

        antColonyOptimization(cities, entry.second, 1.0, 2.0, 0.5, 10, 0.01, bestDistance, bestPath, elapsedTime);
        saveResultsToFile(entry.first, bestDistance, elapsedTime, bestPath);
    }

    return 0;
}

