#include <chrono>
#include <fstream>
#include <map>
#include <print>
#include <regex>
#include <unordered_set>
#include <vector>

int main()
{
    namespace ch = std::chrono;

    auto input_start = ch::high_resolution_clock::now();
    std::map<char, std::vector<std::pair<int, int>>> map;
    std::pair dimentions{0, 0};
    auto &[dx, dy] = dimentions;

    {
        std::ifstream input{"../2024/input/day8.txt"};
        std::regex tr{R"(((?:\d)|(?:\w)))"};

        for (std::string line; std::getline(input, line); ++dy)
        {
            dx = static_cast<int>(line.size());
            for (std::sregex_iterator it{line.begin(), line.end(), tr}, end_it; it != end_it; ++it)
            {
                auto matches = &*it;
                char freq = matches[0].str()[0];

                if (!map.contains(freq))
                    map[freq] = {};

                map[freq].emplace_back(matches->position(), dy);
            }
        }
    }

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println();

    struct pair_hash
    {
        std::size_t operator()(const std::pair<int, int> &p) const
        {
            return static_cast<size_t>(p.first) ^ static_cast<size_t>(p.second) << 1;
        }
    };

    std::unordered_set<std::pair<int, int>, pair_hash> antinodes;

    for (auto &[freq, towers] : map)
    {
        for (std::size_t i = 0; i < towers.size(); ++i)
        {
            for (std::size_t j = i + 1; j < towers.size(); ++j)
            {
                auto &[ax, ay] = towers[i];
                auto &[bx, by] = towers[j];

                std::pair offset{ax - bx, ay - by};
                auto &[ox, oy] = offset;

                if (auto rx = ax + ox, ry = ay + oy; rx >= 0 && rx < dx && ry >= 0 && ry < dy)
                    antinodes.emplace(rx, ry);

                if (auto rx = bx - ox, ry = by - oy; rx >= 0 && rx < dx && ry >= 0 && ry < dy)
                    antinodes.emplace(rx, ry);
            }
        }
    }

    auto first_end = ch::high_resolution_clock::now();
    std::println("n of antinodes: {}", antinodes.size());
    std::println("time taken: {}", ch::duration_cast<ch::microseconds>(first_end - start));
    std::println();

    antinodes.clear();
    for (auto &[freq, towers] : map)
    {
        for (std::size_t i = 0; i < towers.size(); ++i)
        {
            for (std::size_t j = i + 1; j < towers.size(); ++j)
            {
                auto &[ax, ay] = towers[i];
                auto &[bx, by] = towers[j];

                std::pair offset{ax - bx, ay - by};
                auto &[ox, oy] = offset;

                for (auto rx = ax, ry = ay; rx >= 0 && rx < dx && ry >= 0 && ry < dy; rx += ox, ry += oy)
                    antinodes.emplace(rx, ry);

                for (auto rx = bx, ry = by; rx >= 0 && rx < dx && ry >= 0 && ry < dy; rx -= ox, ry -= oy)
                    antinodes.emplace(rx, ry);
            }
        }
    }

    std::println("n of antinodes (any in line): {}", antinodes.size());
    std::println("time taken: {}", ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - first_end));

    return 0;
}