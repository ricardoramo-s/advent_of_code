#include <chrono>
#include <fstream>
#include <print>
#include <regex>
#include <vector>

bool branch(size_t index, const std::vector<long> &numbers, long running_sum, long result, bool with_concat = false)
{
    if (index == numbers.size())
    {
        return running_sum == result;
    }

    long sum = running_sum + numbers[index];

    if (branch(index + 1, numbers, sum, result, with_concat))
    {
        return true;
    }

    long mul = running_sum * numbers[index];

    if (branch(index + 1, numbers, mul, result, with_concat))
    {
        return true;
    }

    if (with_concat)
    {
        long pow = 10;
        while (numbers[index] >= pow)
        {
            pow *= 10;
        }

        long concat = running_sum * pow + numbers[index];

        if (branch(index + 1, numbers, concat, result, with_concat))
        {
            return true;
        }
    }

    return false;
}

int main()
{
    namespace ch = std::chrono;

    auto input_start = ch::high_resolution_clock::now();

    std::vector<std::pair<long, std::vector<long>>> equations;

    {
        std::ifstream input{"../2024/input/day7.txt"};

        if (!input.is_open())
        {
            std::perror("Error opening input file:");
            return 1;
        }

        std::string line;
        std::regex res{R"((?:(\d+):))"}, num{R"((\d+))"};
        long result = 0;
        std::vector<long> numbers;
        while (std::getline(input, line))
        {
            std::smatch matches;
            numbers.clear();

            if (!std::regex_search(line, matches, res))
            {
                continue;
            }

            result = std::stol(matches[1]);
            line = matches.suffix();

            for (; std::regex_search(line, matches, num); line = matches.suffix())
            {
                numbers.push_back(std::stol(matches[1]));
            }

            equations.emplace_back(result, numbers);
        }
    }

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println();

    long total_calibration = 0;
    for (auto [result, numbers] : equations)
    {
        if (branch(1, numbers, numbers[0], result))
            total_calibration += result;
    }

    auto first_end = ch::high_resolution_clock::now();
    std::println("total calibration result: {}", total_calibration);
    std::println("time taken: {}\n", ch::duration_cast<ch::microseconds>(first_end - start));

    total_calibration = 0;
    for (auto [result, numbers] : equations)
    {
        if (branch(1, numbers, numbers[0], result, true))
            total_calibration += result;
    }

    std::println("total calibration result (with concat): {}", total_calibration);
    std::println("time taken: {}", ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - first_end));

    return 0;
}