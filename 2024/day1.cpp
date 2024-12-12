#include "algorithm"
#include "chrono"
#include "fstream"
#include "iostream"
#include "ranges"
#include "unordered_map"
#include "vector"

int main()
{
    namespace ch = std::chrono;

    auto input_start = ch::high_resolution_clock::now();
    std::vector<int> col1 = {}, col2 = {};

    // read and parse input
    {
        std::ifstream input{"../2024/input/day1.txt"};

        if (!input.is_open())
        {
            std::perror("Error opening the file:");
            return 1;
        }

        std::string line;

        while (std::getline(input, line))
        {
            size_t delim = line.find(' ');

            col1.push_back(std::stoi(line.substr(0, delim)));
            col2.push_back(std::stoi(line.substr(delim + 1)));
        }
    }

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println();

    unsigned long distance = 0;

    std::sort(col1.begin(), col1.end());
    std::sort(col2.begin(), col2.end());

    for (auto [num1, num2] : std::views::zip(col1, col2))
    {
        distance += abs(num1 - num2);
    }

    auto first_end = ch::high_resolution_clock::now();

    std::cout << "distance: " << distance << std::endl;
    std::cout << std::format("time taken: {}\n", ch::duration_cast<ch::microseconds>(first_end - start));

    std::unordered_map<int, int> mem;

    unsigned long similarity = 0;

    for (const auto &num1 : col1)
    {
        if (mem.contains(num1))
        {
            similarity += mem[num1] * num1;
            continue;
        }

        auto lower = std::lower_bound(col2.begin(), col2.end(), num1);
        int count = 0;

        while (lower != col2.end() && *(lower++) == num1)
        {
            count++;
        }

        similarity += num1 * count;
        mem[num1] = count;
    }

    std::cout << std::endl;
    std::cout << "similarity: " << similarity << std::endl;
    std::cout << std::format("time taken: {}\n",
                             ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - first_end));

    return 0;
}