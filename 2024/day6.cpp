#include "array"
#include "chrono"
#include "fstream"
#include "print"
#include "ranges"
#include "vector"

int main()
{
    namespace ch = std::chrono;

    auto input_start = ch::high_resolution_clock::now();

    std::vector<std::string> horizontal;
    {
        std::ifstream input{"../2024/input/day6.txt"};

        if (!input.is_open())
        {
            std::perror("Error opening file:");
            return 1;
        }

        std::string line;
        while (std::getline(input, line))
        {
            horizontal.push_back(line);
        }
    }

    struct vec2
    {
        long x, y;
    };

    enum class Direction
    {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT
    };

    constexpr auto toDirection = [](const char c) {
        if (c == '^')
            return Direction::UP;
        if (c == 'v')
            return Direction::DOWN;
        if (c == '<')
            return Direction::LEFT;
        if (c == '>')
            return Direction::RIGHT;

        throw std::runtime_error("Invalid direction!");
    };

    auto start = ch::high_resolution_clock::now();
    std::println("time taken to parse input: {}", ch::duration_cast<ch::microseconds>(start - input_start));
    std::println();

    vec2 guards_position = {0, 0},
         dimensions = {static_cast<int>(horizontal[0].size()), static_cast<int>(horizontal.size())};
    Direction guard_direction;
    constexpr std::array guard_chars{'^', 'v', '<', '>'};
    bool found = false;

    for (auto [row, y] : std::views::zip(horizontal, std::views::iota(0)))
    {
        for (const auto guard_char : guard_chars)
        {
            if (auto x = row.find(guard_char); x != std::string::npos)
            {
                guards_position = {static_cast<long>(x), y};
                guard_direction = toDirection(guard_char);
                found = true;
                break;
            }
        }

        if (found)
            break;
    }

    std::println("guard's position: ({}, {})", guards_position.x, guards_position.y);
    std::println("guard direction: {}\n", static_cast<int>(guard_direction));

    vec2 original_guard_position = guards_position;
    Direction original_guard_direction = guard_direction;

    unsigned long positions = 0;
    bool out_of_bounds = false;
    while (!out_of_bounds)
    {
        char &c = horizontal[guards_position.y][guards_position.x];
        if (c == '#')
            throw std::runtime_error("Guard should never be inside a wall");
        if (c != 'X')
        {
            positions++;
            c = 'X';
        }

        switch (guard_direction)
        {
        case Direction::UP:
            if (guards_position.y - 1 < 0)
            {
                out_of_bounds = true;
                break;
            }

            if (horizontal[guards_position.y - 1][guards_position.x] == '#')
            {
                guard_direction = Direction::RIGHT;
                break;
            }

            guards_position.y--;
            break;
        case Direction::DOWN:
            if (guards_position.y + 1 >= dimensions.y)
            {
                out_of_bounds = true;
                break;
            }
            if (horizontal[guards_position.y + 1][guards_position.x] == '#')
            {
                guard_direction = Direction::LEFT;
                break;
            }

            guards_position.y++;
            break;
        case Direction::LEFT:
            if (guards_position.x - 1 < 0)
            {
                out_of_bounds = true;
                break;
            }
            if (horizontal[guards_position.y][guards_position.x - 1] == '#')
            {
                guard_direction = Direction::UP;
                break;
            }

            guards_position.x--;

            break;
        case Direction::RIGHT:
            if (guards_position.x + 1 >= dimensions.x)
            {
                out_of_bounds = true;
                break;
            }
            if (horizontal[guards_position.y][guards_position.x + 1] == '#')
            {
                guard_direction = Direction::DOWN;
                break;
            }

            guards_position.y++;

            break;
        default:
            std::println("Invalid guard direction!");
            break;
        }
    }

    auto first_end = ch::high_resolution_clock::now();

    std::println("unique positions: {}", positions);
    std::println("time taken: {}\n", ch::duration_cast<ch::microseconds>(first_end - start));

    std::vector<vec2> candidates;
    unsigned long possible_loops = 0;
    horizontal[original_guard_position.y][original_guard_position.x] = '.';

    for (auto y : std::views::iota(0, dimensions.y))
    {
        for (auto x : std::views::iota(0, dimensions.x))
        {
            if (horizontal[y][x] == 'X')
                candidates.emplace_back(x, y);
        }
    }

    for (auto [candidate_x, candidate_y] : candidates)
    {
        horizontal[candidate_y][candidate_x] = '#';

        std::optional<bool> loop = std::nullopt;
        vec2 phantom_position = original_guard_position;
        Direction phantom_direction = original_guard_direction;
        std::vector directions(dimensions.y, std::vector<std::bitset<4>>(dimensions.x));

        // check for loop
        while (!loop.has_value())
        {
            if (directions[phantom_position.y][phantom_position.x][static_cast<int>(phantom_direction)])
            {
                loop = true;
                break;
            }

            directions[phantom_position.y][phantom_position.x][static_cast<int>(phantom_direction)] = true;

            switch (phantom_direction)
            {
            case Direction::UP: {
                if (phantom_position.y - 1 < 0)
                {
                    loop = false;
                    break;
                }

                if (horizontal[phantom_position.y - 1][phantom_position.x] == '#')
                {
                    phantom_direction = Direction::RIGHT;
                    break;
                }

                phantom_position.y--;
            }
            break;
            case Direction::DOWN: {
                if (phantom_position.y + 1 >= dimensions.y)
                {
                    loop = false;
                    break;
                }
                if (horizontal[phantom_position.y + 1][phantom_position.x] == '#')
                {
                    phantom_direction = Direction::LEFT;
                    break;
                }

                phantom_position.y++;
            }
            break;
            case Direction::LEFT: {
                if (phantom_position.x - 1 < 0)
                {
                    loop = false;
                    break;
                }
                if (horizontal[phantom_position.y][phantom_position.x - 1] == '#')
                {
                    phantom_direction = Direction::UP;
                    break;
                }

                phantom_position.x--;
            }
            break;
            case Direction::RIGHT: {
                if (phantom_position.x + 1 >= dimensions.x)
                {
                    loop = false;
                    break;
                }
                if (horizontal[phantom_position.y][phantom_position.x + 1] == '#')
                {
                    phantom_direction = Direction::DOWN;
                    break;
                }

                phantom_position.x++;
            }
            break;
            }
        }

        if (loop.value())
            possible_loops++;

        horizontal[candidate_y][candidate_x] = '.';
    }

    std::println("possible loops: {}", possible_loops);
    std::println("time taken: {}", ch::duration_cast<ch::milliseconds>(ch::high_resolution_clock::now() - first_end));

    return 0;
}
