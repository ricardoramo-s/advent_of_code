#include <chrono>
#include <fstream>
#include <mdspan>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <vector>

int main()
{
    namespace ch = std::chrono;

    struct TrailNode
    {
        int y, x;

        bool operator<(const TrailNode &other) const
        {
            return std::tie(y, x) < std::tie(other.y, other.x);
        }
    };

    std::vector<char> map;
    std::pair<size_t, size_t> dimensions;

    auto input_start = ch::high_resolution_clock::now();

    {
        std::ifstream input{"../2024/input/day10.txt"};

        if (!input.is_open())
        {
            std::perror("Error opening input file:");
            return 1;
        }

        std::string line;
        while (std::getline(input, line))
        {
            dimensions.first = line.size();
            dimensions.second++;

            for (auto c : line)
                map.push_back(static_cast<char>(c - '0'));
        }
    }

    auto mdmap = std::mdspan(map.data(), dimensions.second, dimensions.first);
    std::set<TrailNode> trailheads;

    for (auto y : std::views::iota(0, static_cast<int>(dimensions.second)))
    {
        for (auto x : std::views::iota(0, static_cast<int>(dimensions.first)))
        {
            if (mdmap[y, x] == 0)
                trailheads.emplace(y, x);
        }
    }

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println("");

    unsigned long unique, repeated = 0;
    std::queue<TrailNode> queue;
    std::set<TrailNode> peaks;

    for (auto [ty, tx] : trailheads)
    {
        peaks.clear();
        queue.emplace(ty, tx);
        auto rating = 0;

        while (!queue.empty())
        {
            auto [y, x] = queue.front();
            auto value = mdmap[y, x];
            queue.pop();

            if (value == 9)
            {
                rating++;
                peaks.emplace(y, x);
                continue;
            }

            if (y > 0 && mdmap[y - 1, x] - value == 1)
            {
                queue.emplace(y - 1, x);
            }

            if (y < dimensions.second - 1 && mdmap[y + 1, x] - value == 1)
            {
                queue.emplace(y + 1, x);
            }

            if (x > 0 && mdmap[y, x - 1] - value == 1)
            {
                queue.emplace(y, x - 1);
            }

            if (x < dimensions.first - 1 && mdmap[y, x + 1] - value == 1)
            {
                queue.emplace(y, x + 1);
            }
        }

        unique += peaks.size();
        repeated += rating;
    }

    std::println("score of all trailheads: {}", unique);
    std::println("score of all trailheads raitings: {}", repeated);
    std::println("time taken: {}", ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - start));

    return 0;
}