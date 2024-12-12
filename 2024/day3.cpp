#include "chrono"
#include "fstream"
#include "iostream"
#include "regex"
#include "sstream"

int main()
{
    namespace ch = std::chrono;

    auto input_start = ch::high_resolution_clock::now();

    std::string content;

    // read and parse input
    {
        std::ifstream input{"../2024/input/day3.txt"};

        if (!input.is_open())
        {
            std::perror("Unable to open file:");
            return 1;
        }

        std::stringstream buffer;
        buffer << input.rdbuf();
        content = buffer.str();
    }

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println();

    std::regex pattern{R"((?:mul\((\d+),(\d+)\)))"};
    unsigned long result = 0;

    std::string search{content};
    for (std::smatch matches; std::regex_search(search, matches, pattern); search = matches.suffix())
    {
        result += std::stoi(matches[1]) * std::stoi(matches[2]);
    }

    auto first_end = std::chrono::high_resolution_clock::now();

    std::cout << std::format("multiplications: {}\n", result);
    std::cout << std::format("time taken: {}\n", ch::duration_cast<ch::milliseconds>(first_end - start));

    result = 0;
    pattern = R"((?:mul\((\d+),(\d+)\))|(?:(do(?:n't)?)\(\)))";
    bool active = true;

    search = std::string{content};
    for (std::smatch matches; std::regex_search(search, matches, pattern); search = matches.suffix())
    {
        if (matches[3].matched)
        {
            active = (matches[3] == "do");
        }
        else if (active)
        {
            result += std::stoi(matches[1]) * std::stoi(matches[2]);
        }
    }

    std::cout << std::endl;
    std::cout << std::format("enabled multiplications only: {}\n", result);
    std::cout << std::format("time taken: {}",
                             ch::duration_cast<ch::milliseconds>(ch::high_resolution_clock::now() - first_end));

    return 0;
}