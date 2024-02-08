#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;

void print_path(const vector<int>& path) {
    for (int city : path) {
        cout << city << " ";
    }
    cout << endl;
}

int get_current_path_value(const vector<vector<int>>& matrix, const vector<int>& path) {
    int value = 0;
    int previous_node = 0;

    for (int i = 0; i < path.size(); ++i) {
        value += matrix[previous_node][path[i]];
        previous_node = path[i];
    }

    value += matrix[previous_node][0]; // Return to the starting city
    return value;
}

vector<int> swap_elements(const vector<int>& path, vector<vector<int>>& tabu_list, const vector<vector<int>>& matrix, int tabu_size) {
    vector<int> best_path;
    int best_path_value = 99999999;
    int best_city_x = 0;
    int best_city_y = 0;

    for (int i = 0; i < path.size(); ++i) {
        for (int j = i + 1; j < path.size(); ++j) {
            if (tabu_list[i][j] == 0) {
                vector<int> current_path = path;
                swap(current_path[i], current_path[j]);
                int current_path_value = get_current_path_value(matrix, current_path);

                if (current_path_value < best_path_value) {
                    best_path = current_path;
                    best_path_value = current_path_value;
                    best_city_x = i;
                    best_city_y = j;
                }
            }
        }
    }

    tabu_list[best_city_x][best_city_y] = tabu_size;

    return best_path;
}

vector<vector<int>> generate_empty_tabu_list(int size) {
    return vector<vector<int>>(size, vector<int>(size, 0));
}

void solve(vector<vector<int>>& matrix, int iterations, int max_time_in_seconds, int tabu_size, const string& file_name) {
    cout << "Przygotowywanie zmiennych dla pliku: " << file_name << endl;

    // Pocz¹tek zliczania czasu
    auto timer_start = chrono::high_resolution_clock::now();
    // Iloœæ istotnych zdarzeñ
    int critical_events = 0;
    // Aktualna œcie¿ka
    vector<int> current_path(matrix.size());
    iota(current_path.begin(), current_path.end(), 0);
    random_shuffle(current_path.begin(), current_path.end());
    // Koszt aktualnej œcie¿ki
    int current_path_value = get_current_path_value(matrix, current_path);
    // Najlepsza œcie¿ka
    vector<int> best_path = current_path;
    // Koszt najlepszej œcie¿ki
    int best_path_value = current_path_value;
    // Lista tabu
    vector<vector<int>> tabu_list = generate_empty_tabu_list(matrix.size());

    // Parametry algorytmu
    int max_critical_events = 100;

    // Pêtla wykonuj¹ca zadan¹ iloœæ iteracji
    for (int i = 0; i < iterations; ++i) {
        // Obliczenie aktualnego czasu
        auto elapsed_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - timer_start).count();

// Warunek koñcz¹cy czasowo


        // Zmiana elementów w œcie¿ce
        current_path = swap_elements(current_path, tabu_list, matrix, tabu_size);
        // Aktualizacja wagi aktualnej œcie¿ki
        current_path_value = get_current_path_value(matrix, current_path);

        // Przypisanie wartoœci aktualnej œcie¿ki jako najlepszej
        if (current_path_value < best_path_value) {
            best_path = current_path;
            best_path_value = current_path_value;
        }

        // Minimalizacja wartoœci w liœcie tabu
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix.size(); ++j) {
                if (tabu_list[i][j] > 0) {
                    tabu_list[i][j] = max(0, tabu_list[i][j] - 1);
                }
            }
        }

        // Je¿eli wynik nie jest poprawny, zwiêkszenie licznika krytycznych b³êdów
        if (current_path_value < best_path_value) {
            critical_events = 0;
        } else {
            critical_events++;
        }

        // Diversification: Reset œcie¿ki po pewnej liczbie krytycznych b³êdów
        if (critical_events >= max_critical_events) {
            random_shuffle(current_path.begin(), current_path.end());
            current_path_value = get_current_path_value(matrix, current_path);
            critical_events = 0;
        }
    }

    // Obliczenie czasu wykonania
    auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - timer_start).count();

    // Wypisz dane z pliku, wynik i czas wykonania
    cout << "Plik: " << file_name << " Wynik: " << best_path_value << " Czas wykonania: " << elapsed_time << " ms" << endl;

    // Zapisz wynik do pliku
    ofstream result_file("wyniki.txt", ios::app);
    if (result_file.is_open()) {
        result_file << "Plik: " << file_name << " Wynik: " << best_path_value << " Czas wykonania: " << elapsed_time << " ms" << endl;
        result_file.close();
    } else {
        cerr << "Nie uda³o siê otworzyæ pliku wyniki.txt do zapisu." << endl;
    }

    // Wyczyszczenie œcie¿ki i tabu przed kolejnym plikiem
    current_path.clear();
    best_path.clear();
    tabu_list.clear();
}

int main() {
    // Odczytaj dane z pliku test.ini
    ifstream config_file("atest.ini");
    if (!config_file.is_open()) {
        cerr << "Nie uda³o siê otworzyæ pliku konfiguracyjnego." << endl;
        return 1;
    }

    // Clear the content of the results file
    ofstream clear_results("wyniki.txt", ios::trunc);
    clear_results.close();

    string line;
    while (getline(config_file, line)) {
        istringstream iss(line);
        string file_name;
        int iterations, tabu_size;
        if (iss >> file_name >> iterations >> tabu_size) {
            // Wczytaj dane z pliku
            ifstream file(file_name);
            if (!file.is_open()) {
                cerr << "Nie uda³o siê otworzyæ pliku." << endl;
                return 1;
            }

            int num_cities;
            file >> num_cities;

            vector<vector<int>> matrix(num_cities, vector<int>(num_cities));

            for (int i = 0; i < num_cities; ++i) {
                for (int j = 0; j < num_cities; ++j) {
                    file >> matrix[i][j];
                }
            }

            file.close();

            // Parametry algorytmu
            int max_time_in_seconds = 180;

            // Rozwi¹zanie problemu komiwoja¿era za pomoc¹ metody Tabu Search
            solve(matrix, iterations, max_time_in_seconds, tabu_size, file_name);
        }
    }

    config_file.close();

    return 0;
}
