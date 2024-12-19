#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <print>
#include <ranges>
#include <set>
#include <thread>
#include <vector>

int main()
{
    namespace ch = std::chrono;

    struct Block
    {
        size_t id = 0;
        size_t size = 0;
        bool free = false;
    };

    auto input_start = ch::high_resolution_clock::now();
    std::vector<int> disk;
    std::list<Block> block_disk;

    std::list<std::list<Block>::iterator> free_blocks, file_blocks;
    std::array<std::list<std::list<Block>::iterator>::iterator, 9> helper{};
    helper.fill(free_blocks.end());

    {
        std::ifstream input{"../2024/input/day9.txt"};

        bool free = false;
        size_t id = 0;

        char c;
        while (input.get(c))
        {
            auto size = static_cast<size_t>(c - '0');

            for (auto _ : std::views::iota(0, static_cast<int>(size)))
            {
                disk.emplace_back(!free ? id : -1);
            }

            auto it = block_disk.insert(block_disk.end(), {id, size, free});

            if (free)
            {
                auto fr = free_blocks.insert(free_blocks.end(), it);

                for (int i = static_cast<int>(size) - 1; i >= 0; --i)
                {
                    if (auto h = helper[i]; h == free_blocks.end() && (*fr)->size - 1 >= i)
                        helper[i] = fr;
                }
            }
            else
            {
                file_blocks.push_back(it);
            }

            if (!free)
                id++;
            free = !free;
        }

        std::ranges::reverse(file_blocks);
    }

    std::println("time taken to parse input: {}",
                 ch::duration_cast<ch::microseconds>(ch::high_resolution_clock::now() - input_start));
    std::println("");

    auto start = ch::high_resolution_clock::now();

    {
        auto block_it = disk.rbegin();
        auto free_it = std::ranges::find_if(disk, [](auto &id) { return id == -1; });

        while (free_it < block_it.base())
        {
            std::swap(*block_it, *free_it);

            block_it = std::ranges::find_if(++block_it, disk.rend(), [](auto &id) { return id != -1; });
            free_it = std::ranges::find_if(++free_it, disk.end(), [](auto &id) { return id == -1; });
        }
    }

    unsigned long checksum = 0;

    for (auto [i, id] : std::views::zip(std::views::iota(0), disk))
    {
        if (id == -1)
            continue;

        checksum += i * id;
    }

    auto first_end = ch::high_resolution_clock::now();
    std::println("checksum: {}", checksum);
    std::println("time taken: {}", ch::duration_cast<ch::microseconds>(first_end - start));
    std::println("");

    for (auto &file_it : file_blocks)
    {
        auto file = *file_it;

        if (file.size == 0)
        {
            block_disk.erase(file_it);
            continue;
        }

        for (auto h_it : helper | std::views::drop(file.size - 1))
        {
            if (h_it == free_blocks.end())
            {
                continue;
            }

            if (auto free_it = *h_it; free_it->size == file.size)
            {
                std::swap(*free_it, *file_it);
            }
            else
            {
                block_disk.insert(free_it, file);
                free_it->size -= file.size;
                file_it->free = true;
            }

            break;
        }

        for (int i = 0; i < 9; ++i)
        {
            auto fl = Block{file.id, static_cast<size_t>(i + 1), false};
            helper[i] = std::ranges::find_if(helper[i], free_blocks.end(), [&fl](const auto &block_it) {
                auto &[id, size, free] = *block_it;
                return id < fl.id && free && fl.size <= size;
            });
        }
    }

    checksum = 0;
    int index = 0;
    for (auto [id, size, free] : block_disk)
    {
        if (free)
        {
            index += static_cast<int>(size);
            continue;
        }

        for (int s = 0; s < size; s++)
            checksum += index++ * id;
    }

    auto end = ch::high_resolution_clock::now();
    std::println("checksum (by block): {}", checksum);
    std::println("time taken: {}", ch::duration_cast<ch::microseconds>(end - first_end));

    return 0;
}