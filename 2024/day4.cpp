#include "iostream"
#include "fstream"
#include "vector"
#include "chrono"
#include "thread"

int main() {
    namespace ch = std::chrono;

    std::vector<std::string> word_map;

    // read and parse input
    {
        std::ifstream input{"../2024/input/day4.txt"};

        if (!input.is_open()) {
            std::perror("Error opening file:");
            return 1;
        }
        std::string line;

        while (std::getline(input, line)) {
            word_map.emplace_back(line);
        }
    }

    auto start = ch::high_resolution_clock::now();

    constexpr std::string word{"XMAS"};
    constexpr std::string word_reverse{word.rbegin(), word.rend()};
    std::vector<std::pair<int, int>> candidates;

    for (size_t y = 0; y < word_map.size(); y++) {
        for (size_t x = 0; x < word_map[y].size(); x++) {
            if (word_map[y][x] == word[0]) {
                candidates.emplace_back(y, x);
            }
        }
    }

    unsigned long count = 0;

    for (auto [y, x]: candidates) {
        // straight
        if (x + word.size() <= word_map[y].size() && word_map[y].substr(x, word.size()) == word) {
            count++;
        }
        if (x - (int)word.size() + 1 >= 0 && word_map[y].substr(x - word.size() + 1, word.size()) == word_reverse) {
            count++;
        }
        if (y - (int)word.size() + 1 >= 0) {
            bool found = true;
            for (int i = 0; i < word.size() && found; i++) {
                found = word_map[y - i][x] == word[i];
            }

            count += found;
        }
        if (y + word.size() <= word_map.size()) {
            bool found = true;
            for (int i = 0; i < word.size() && found; i++) {
                found = word_map[y + i][x] == word[i];
            }

            count += found;
        }
        // diagonal
        if (y - (int)word.size() + 1 >= 0 && x - (int)word.size() + 1 >= 0) {
            bool found = true;
            for (int i = 0; i < word.size() && found; i++) {
                found = word_map[y - i][x - i] == word[i];
            }

            count += found;
        }
        if (y - (int)word.size() + 1 >= 0 && x + word.size() <= word_map[y].size()) {
            bool found = true;
            for (int i = 0; i < word.size() && found; i++) {
                found = word_map[y - i][x + i] == word[i];
            }

            count += found;
        }
        if (y + word.size() <= word_map.size() && x - (int)word.size() + 1 >= 0) {
            bool found = true;
            for (int i = 0; i < word.size() && found; i++) {
                found = found && word_map[y + i][x - i] == word[i];
            }

            count += found;
        }
        if (y + word.size() <= word_map.size() && x + word.size() <= word_map[y].size()) {
            bool found = true;
            for (int i = 0; i < word.size() && found; i++) {
                found = word_map[y + i][x + i] == word[i];
            }

            count += found;
        }
    }

    auto first_end = ch::high_resolution_clock::now();

    std::cout << std::format("XMAS: {}\n", count);
    std::cout << std::format("time taken: {}\n", ch::duration_cast<ch::microseconds>(first_end - start));

    candidates.clear();
    count = 0;

    for (size_t y = 1; y < word_map.size() - 1; y++) {
        for (size_t x = 1; x < word_map[y].size() - 1; x++) {
            if (word_map[y][x] == 'A') {
                candidates.emplace_back(y, x);
            }
        }
    }

    for (auto [y, x] : candidates) {
        if (!((word_map[y - 1][x - 1] == 'M' && word_map[y + 1][x + 1] == 'S')
            || ((word_map[y - 1][x - 1] == 'S' && word_map[y + 1][x + 1] == 'M')))) {
            continue;
        }

        if (!((word_map[y - 1][x + 1] == 'M' && word_map[y + 1][x - 1] == 'S')
              || ((word_map[y - 1][x + 1] == 'S' && word_map[y + 1][x - 1] == 'M')))) {
            continue;
        }

        count++;
    }

    std::cout << std::endl;
    std::cout << std::format("X-MAS: {}\n", count);
    std::cout << std::format("time taken: {}\n", ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - first_end));

    return 0;
}