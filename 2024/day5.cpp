#include "algorithm"
#include "chrono"
#include "fstream"
#include "iostream"
#include "map"
#include "ranges"
#include "regex"
#include "set"

int main()
{
    namespace ch = std::chrono;

    auto input_start = ch::high_resolution_clock::now();

    std::map<int, std::set<int>> ordering_rules;
    std::vector<std::pair<std::vector<int>, bool>> updates;

    // read and parse input
    {
        std::ifstream input{"../2024/input/day5.txt"};

        if (!input.is_open())
        {
            std::perror("Error opening file:");
            return 1;
        }

        std::string rules, line;

        while (std::getline(input, line) && !line.empty())
        {
            rules.append(line + ',');
        }

        std::regex pattern{R"((?:(\d+)\|(\d+)))"};
        std::set<int> pages;
        for (std::smatch matches; std::regex_search(rules, matches, pattern); rules = matches.suffix())
        {
            auto key = std::stoi(matches[1]), value = std::stoi(matches[2]);

            if (ordering_rules.contains(key))
            {
                ordering_rules[key].insert(value);
            }
            else
            {
                ordering_rules[key] = {value};
            }
        }

        pattern = std::regex{R"((\d+))"};
        std::smatch matches;
        std::vector<int> update;

        while (std::getline(input, line))
        {
            update.clear();
            for (; std::regex_search(line, matches, pattern); line = matches.suffix())
            {
                update.push_back(std::stoi(matches[0]));
            }

            updates.emplace_back(update, false);
        }
    }

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println();

    std::set<int> found_pages;
    unsigned int result = 0;

    for (auto &[update, is_valid] : updates)
    {
        found_pages.clear();
        is_valid = true;

        for (auto page : update)
        {
            if (!is_valid)
                break;
            if (!ordering_rules.contains(page))
            {
                found_pages.insert(page);
                continue;
            }

            // set_intersection non-exhaustive
            auto [first1, last1] = std::pair{found_pages.begin(), found_pages.end()};
            auto [first2, last2] = std::pair{ordering_rules[page].begin(), ordering_rules[page].end()};

            while (first1 != last1 && first2 != last2)
            {
                if (*first1 < *first2)
                    ++first1;
                else if (*first1 > *first2)
                {
                    ++first2;
                }
                else
                {
                    is_valid = false;
                    break;
                }
            }

            found_pages.insert(page);
        }
    }

    for (auto &[update, is_valid] : updates)
    {
        if (is_valid)
            result += update[update.size() / 2];
    }

    auto first_end = ch::high_resolution_clock::now();

    std::cout << std::format("time taken (verify): {}\n", ch::duration_cast<ch::microseconds>(first_end - start));
    std::cout << "middle page numbers added: " << result << std::endl;

    auto compare = [&ordering_rules](int a, int b) -> bool {
        if (ordering_rules.contains(a))
            return ordering_rules[a].contains(b);
        else
            return false;
    };

    auto filtered_updates =
        updates | std::views::filter([](std::pair<std::vector<int>, bool> &update) { return !update.second; });
    for (auto &[update, is_valid] : filtered_updates)
    {
        std::sort(update.begin(), update.end(), compare);
    }

    result = 0;
    for (auto &[update, is_valid] : updates)
    {
        if (!is_valid)
            result += update[update.size() / 2];
    }

    std::cout << std::endl;
    std::cout << std::format("time taken (sort): {}\n",
                             ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - first_end));
    std::cout << std::format("middle page numbers added (previously unsorted): {}\n", result);

    return 0;
}
