#include "algorithm"
#include "chrono"
#include "fstream"
#include "iostream"
#include "ranges"
#include "sstream"
#include "unordered_set"
#include "vector"

bool verify(const std::vector<int> &numbers, int candidate = -1)
{
    int i = 0, last, current;

    if (candidate == 0)
    {
        i++;
    }

    last = numbers[i++];

    if (candidate == 1)
    {
        i++;
    }

    current = numbers[i];

    int diff = std::abs(last - current);
    bool is_safe = !(diff > 3 || diff < 1);
    const bool increasing = last < current;

    last = current;

    while (is_safe && ++i < numbers.size())
    {
        if (i == candidate)
        {
            continue;
        }

        current = numbers[i];
        diff = std::abs(last - current);

        is_safe = increasing == (last < current);
        is_safe = is_safe && !(diff > 3 || diff < 1);

        last = current;
    }

    return is_safe;
}

int main()
{
    namespace ch = std::chrono;

    auto input_start = ch::high_resolution_clock::now();

    std::vector<std::vector<int>> reports;

    // read and parse input
    {
        std::ifstream input{"../2024/input/day2.txt"};

        if (!input.is_open())
        {
            std::perror("Error opening file:");
            return 1;
        }

        std::vector<int> report;

        std::string line;
        std::stringstream ss;

        while (std::getline(input, line))
        {
            ss.clear();
            ss.str(line);

            int token;
            while (ss >> token)
                report.push_back(token);

            reports.push_back(report);
            report.clear();
        }
    }

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println();

    unsigned long safe_reports = 0;

    for (const auto &report : reports)
    {
        auto num = report.begin();

        int last = *(num++);
        int current = *(num++);

        int diff = std::abs(last - current);
        bool is_safe = !(diff > 3 || diff < 1);
        const bool increasing = last < current;

        last = current;

        while (is_safe && num != report.end())
        {
            current = *(num++);

            diff = std::abs(last - current);

            is_safe = increasing == (last < current);
            is_safe = is_safe && !(diff > 3 || diff < 1);

            last = current;
        }

        if (is_safe)
            safe_reports++;
    }

    auto first_end = ch::high_resolution_clock::now();
    std::cout << "safe reports: " << safe_reports << std::endl;
    std::cout << std::format("time taken: {}\n", ch::duration_cast<ch::microseconds>(first_end - start));

    safe_reports = 0;

    for (auto const &report : reports)
    {
        auto iota = std::views::iota(0, (int)report.size());

        if (std::any_of(iota.begin(), iota.end(), [&report](int candidate) { return verify(report, candidate); }))
        {
            safe_reports++;
        }
    }

    std::cout << std::endl;
    std::cout << "safe reports (with dampener): " << safe_reports << std::endl;
    std::cout << std::format("time taken: {}\n",
                             ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - first_end));

    return 0;
}