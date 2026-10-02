#include <unordered_map>
#include <iostream>
#include <charconv>
#include <memory>
#include <cstdio>
#include <thread>
#include <chrono>
#include <mutex>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <cstring>

namespace {
    constexpr std::array<std::string_view, 413> cities = {
        "New York City", "Saskatoon", "St. John's", "La Ceiba", "Hanoi", "Mumbai", 
        "Brussels", "Timbuktu", "Mogadishu", "Wichita", "Bilbao", "Ürümqi", "Manama", 
        "Erzurum", "Kansas City", "Almaty", "Mzuzu", "Vaduz", "Hanga Roa", "Hamburg", 
        "Odesa", "San Juan", "Kingston", "Tbilisi", "Nashville", "Hargeisa", 
        "Palm Springs", "Wau", "Brazzaville", "Villahermosa", "Makassar", "Napoli", 
        "Dallas", "Kuopio", "Saint-Pierre", "Hamilton", "Busan", "Milwaukee", 
        "Columbus", "Irkutsk", "Ahvaz", "İzmir", "Havana", "Baltimore", "Tucson", 
        "Fukuoka", "San José", "Ulaanbaatar", "Bridgetown", "Ankara", "Rabat", 
        "Mexicali", "Muscat", "Karonga", "Bishkek", "Lviv", "Luxembourg City", 
        "New Orleans", "Veracruz", "Jacksonville", "Sokoto", "Libreville", "Baku", 
        "Las Palmas de Gran Canaria", "Murmansk", "Bouaké", "Asmara", "Sana'a", 
        "Bosaso", "Burnie", "Antananarivo", "Heraklion", "Bloemfontein", "Mek'ele", 
        "Ottawa", "Dubai", "Port Moresby", "Gjoa Haven", "Sochi", "Rangpur", "Nuuk", 
        "Vancouver", "Dolisie", "Ouahigouya", "Suwałki", "Sydney", "Pontianak", 
        "Zanzibar City", "Baghdad", "Montreal", "Anchorage", "Gangtok", "Hong Kong", 
        "Johannesburg", "Launceston", "Tunis", "Monterrey", "Praia", "Da Lat", 
        "Brisbane", "Cabo San Lucas", "Tauranga", "Singapore", "Canberra", "Edmonton", 
        "Cape Town", "Budapest", "Pointe-Noire", "Houston", "Honolulu", "Vienna", 
        "Zagreb", "San Jose", "Palermo", "Pittsburgh", "Wrocław", "Porto", "Riga", 
        "Harbin", "Flores,  Petén", "Guangzhou", "Moscow", "Entebbe", "Dakar", 
        "Abha", "Marseille", "Hat Yai", "Nairobi", "Tehran", "Belize City", 
        "Barcelona", "Abidjan", "Calgary", "Monaco", "Gaborone", "Toamasina", 
        "Split", "Yakutsk", "Guatemala City", "Kandi", "City of San Marino", 
        "Saint Petersburg", "Istanbul", "Hiroshima", "Santo Domingo", "Erbil", 
        "Kuwait City", "Louisville", "Boise", "Belgrade", "Tel Aviv", "Guadalajara", 
        "Lisbon", "Chicago", "Lubumbashi", "Perth", "Alexandra", "Nicosia", "Kano", 
        "Denpasar", "Chihuahua", "Adelaide", "Malé", "Halifax", "Batumi", "Durban", 
        "Riyadh", "Frankfurt", "Salt Lake City", "Athens", "Kampala", "Kinshasa", 
        "Berlin", "Auckland", "Warsaw", "Oklahoma City", "Palembang", "Livingstone", 
        "Beijing", "Mombasa", "Ouarzazate", "Tirana", "Nassau", "Toronto", "Malabo", 
        "Kabul", "Dushanbe", "Kuala Lumpur", "Albuquerque", "Podgorica", "Iqaluit", 
        "Lhasa", "Minneapolis", "Prague", "Paris", "Toluca", "Manila", "Bucharest", 
        "Bordeaux", "Dampier", "Reykjavík", "Fresno", "Ashgabat", "Ngaoundéré", 
        "Tripoli", "Oranjestad", "Maputo", "Medan", "San Francisco", "Winnipeg", 
        "Vladivostok", "Arkhangelsk", "Tabriz", "Benghazi", "Novosibirsk", 
        "Lake Havasu City", "Douala", "Dikson", "Chiang Mai", "Tegucigalpa", 
        "Lusaka", "Ho Chi Minh City", "Jayapura", "Bujumbura", "Boston", "Birao", 
        "El Paso", "Washington, D.C.", "Vientiane", "Wellington", "Parakou", "Rome", 
        "Skopje", "Yaoundé", "Ghanzi", "Yellowknife", "Pretoria", "Chișinău", 
        "Ouagadougou", "Niigata", "Naha", "Phnom Penh", "Dublin", "Lodwar", "Dhaka", 
        "Kathmandu", "Harare", "N'Djamena", "Sapporo", "Bulawayo", "Kyiv", "Tallinn", 
        "Willemstad", "Juba", "Yangon", "Yinchuan", "London", "Ségou", 
        "Dar es Salaam", "Bangui", "Sarajevo", "Addis Ababa", "Cairns", "Helsinki", 
        "San Salvador", "Da Nang", "Memphis", "Port Vila", "Roseau", "Toliara", 
        "Changsha", "Sofia", "Tromsø", "Shanghai", "Copenhagen", "Philadelphia", 
        "Astana", "Seattle", "New Delhi", "Moncton", "Dunedin", "Andorra la Vella", 
        "Hobart", "Kankan", "Miami", "Lomé", "Panama City", "Kyoto", "Kolkata", 
        "Lahore", "Melbourne", "Alexandria", "Surabaya", "Charlotte", "Antsiranana", 
        "Nouadhibou", "Damascus", "Las Vegas", "Darwin", "Omaha", "Taipei", 
        "Kumasi", "San Antonio", "Zürich", "Fianarantsoa", "Bratislava", "Xi'an", 
        "Edinburgh", "Madrid", "Conakry", "Cracow", "Colombo", "Djibouti", "Dili", 
        "Kunming", "Bamako", "Lyon", "Cairo", "Pyongyang", "Bata", "Beirut", 
        "Virginia Beach", "St. Louis", "Yerevan", "Bangkok", "Marrakesh", "Cotonou", 
        "Christchurch", "Reggane", "Port Sudan", "Tamanrasset", "Seoul", "Chongqing", 
        "Tijuana", "Lake Tekapo", "Vilnius", "Chittagong", "Stockholm", "Maun", 
        "Sacramento", "Abéché", "Baguio", "Khartoum", "Gagnoa", "Mahajanga", 
        "Indianapolis", "Oulu", "Ljubljana", "Port-Gentil", "Napier", "Bergen", 
        "Nouakchott", "Thessaloniki", "Upington", "Nakhon Ratchasima", "Garoua", 
        "Anadyr", "Tampa", "Ifrane", "Mandalay", "Denver", "Mango", "Blantyre", 
        "Milan", "Tashkent", "Los Angeles", "Karachi", "Austin", "Garissa", 
        "Whitehorse", "Jerusalem", "Lagos", "Luanda", "Tabora", "Thiès", "Tamale", 
        "Atlanta", "Suva", "Banjul", "La Paz", "Minsk", "Palmerston North", 
        "Detroit", "Assab", "Algiers", "Aden", "Alice Springs", "Valencia", "Bissau", 
        "Dodoma", "Managua", "Accra", "Jakarta", "Oslo", "Valletta", "Ndola", 
        "Portland (OR)", "Petropavlovsk-Kamchatsky", "Seville", "George Town", 
        "Gabès", "Fairbanks", "Mexico City", "San Diego", "Niamey", "Odienné", 
        "Rostov-on-Don", "Amsterdam", "Makurdi", "Tokyo", "Phoenix", "Jos", "Honiara"
    };
}

class MemoryMappedFile {
    using unique_file_t = std::unique_ptr<std::FILE, int(*)(FILE*)>;
public:
    explicit MemoryMappedFile(std::string file)
        : fd_(0), ptr_(nullptr), sb_(), file_(std::fopen(file.c_str(), "r+"), &std::fclose)
    {
        if (!file_) {
            throw std::runtime_error("open");
        }

        fd_ = fileno(file_.get());

        if (fstat(fd_, &sb_) == -1) {
            throw std::runtime_error("fstat");
        }

        ptr_ = mmap(NULL, sb_.st_size, PROT_READ, MAP_PRIVATE, fd_, 0);
        if (ptr_ == MAP_FAILED) {
            throw std::runtime_error("mmap");
        }
        madvise(ptr_, sb_.st_size, MADV_SEQUENTIAL);
    }

    ~MemoryMappedFile() {
        if (ptr_ && munmap(ptr_, sb_.st_size) == -1) {
            std::fprintf(stderr, "unmap");
        }
    }

    std::size_t file_size() const {
        return static_cast<std::size_t>(sb_.st_size);
    }

    void* base_ptr() const {
        return ptr_;
    }

private:
    int fd_;
    void* ptr_;
    struct stat sb_;
    unique_file_t file_;
};

struct Measurements {
    Measurements()
        : min(std::numeric_limits<int16_t>::max()), max(std::numeric_limits<int16_t>::min()), count(0), total(0) {}

    Measurements& operator+=(const Measurements& rhs) {
        this->min = std::min(this->min, rhs.min);
        this->max = std::max(this->max, rhs.max);

        this->total += rhs.total;
        this->count += rhs.count;
        return *this;
    }

    constexpr void add(int16_t value) noexcept {
        min = std::min(min, value);
        max = std::max(max, value);

        count += 1;
        total += value;
    }

    bool is_empty() noexcept {
        return count == 0;
    }

    int16_t min, max;
    int32_t count;
    int64_t total;
};

class FxHash {
public:
    constexpr std::size_t operator()(std::string_view s) noexcept {
        const auto l = s.length();
        return (static_cast<uint64_t>(l) << 48)
            | (static_cast<uint64_t>(static_cast<uint8_t>(s[0])) << 40)
            | (static_cast<uint64_t>(static_cast<uint8_t>(s[1])) << 32)
            | (static_cast<uint64_t>(static_cast<uint8_t>(s[2])) << 24)
            | (static_cast<uint64_t>(static_cast<uint8_t>(s[l-3])) << 16)
            | (static_cast<uint64_t>(static_cast<uint8_t>(s[l-2])) << 8)
            |  static_cast<uint64_t>(static_cast<uint8_t>(s[l-1]));
    }
};

class FxTable {
public:
    static constexpr std::size_t TABLE_SIZE = 1 << 12;
    static constexpr uint64_t MAGIC_MULTIPLIER = 0xf0c4bb7a050d13a5ULL;

    template <class T>
    using fx_table_t = std::array<T, TABLE_SIZE>;

    using fixed_city_t = std::array<char, 28>;

    constexpr FxTable() noexcept = default;
    ~FxTable() noexcept = default;

    FxTable& operator+=(const FxTable& rhs) {
        for (std::size_t i = 0; i < TABLE_SIZE; i++) {
            table_[i] += rhs.table_[i];
        }
        return *this;
    }

    void insert(std::string_view s, int m) noexcept {
        std::size_t index = (FxHash()(s) * MAGIC_MULTIPLIER) >> 52;
        table_[index].add(m);
    }

    void print() {
        for (std::size_t i = 0; i < 413; i++) {
            std::size_t index = (FxHash()(cities[i]) * MAGIC_MULTIPLIER) >> 52;
            if (table_[index].is_empty()) continue;
            std::cout << cities[i] << ": " << static_cast<float>(table_[index].min)/10.f
                    << "," << static_cast<float>(table_[index].total) / table_[index].count / 10.f
                    << "," << static_cast<float>(table_[index].max)/10.f << "\n";
        }
    }

private:
    fx_table_t<Measurements> table_{};
};

class Parser {
public:
    constexpr explicit Parser(void* base_ptr, std::size_t index, std::size_t chunk_size, FxTable* fx_table) noexcept
        : data_(reinterpret_cast<char*>(base_ptr)), fx_table_(fx_table), index_(index), chunk_size_(chunk_size) {}

    constexpr void operator()() const {
        using namespace std::chrono_literals;

        std::size_t current_offset = index_ * chunk_size_;
        char* start = &data_[current_offset];
        const char* current = start;

        // if not first: find next line break, advance read counter
        if (index_ > 0) {
            while (*current != '\n') {
                current += 1;
            }
            current += 1;
        }

        // read lines until last line of chunk
        while (current - start < static_cast<long int>(chunk_size_)) {
            const char* line_start = current;
            const char* semi = nullptr;
            if (current[0] == ';')  semi = current + 0;
            else if (current[1] == ';') semi = current + 1;
            else if (current[2] == ';') semi = current + 2;
            else if (current[3] == ';') semi = current + 3;
            else if (current[4] == ';') semi = current + 4;
            else if (current[5] == ';') semi = current + 5;
            else if (current[6] == ';') semi = current + 6;
            else if (current[7] == ';') semi = current + 7;
            else if (current[8] == ';') semi = current + 8;
            else if (current[9] == ';') semi = current + 9;
            else if (current[10] == ';') semi = current + 10;
            else if (current[11] == ';') semi = current + 11;
            else if (current[12] == ';') semi = current + 12;
            else if (current[13] == ';') semi = current + 13;
            else if (current[14] == ';') semi = current + 14;
            else if (current[15] == ';') semi = current + 15;
            else if (current[16] == ';') semi = current + 16;
            else if (current[17] == ';') semi = current + 17;
            else if (current[18] == ';') semi = current + 18;
            else if (current[19] == ';') semi = current + 19;
            else if (current[20] == ';') semi = current + 20;
            else if (current[21] == ';') semi = current + 21;
            else if (current[22] == ';') semi = current + 22;
            else if (current[23] == ';') semi = current + 23;
            else if (current[24] == ';') semi = current + 24;
            else if (current[25] == ';') semi = current + 25;
            else if (current[26] == ';') semi = current + 26;
            else if (current[27] == ';') semi = current + 27;
            else if (current[28] == ';') semi = current + 28;

            current = semi + 1;
            int16_t sign = 1;
            if (*current == '-') {
                sign = -1;
                current++;
            }

            int16_t n = 0;
            if (current[1] == '.') { // d.d
                n = (current[0] - '0') * 10 + (current[2] - '0');
                current += 4;
            } else { // dd.d
                n = (current[0] - '0') * 100 + (current[1] - '0') * 10 + (current[3] - '0');
                current += 5;
            }
            n *= sign;

            std::string_view city(line_start, semi - line_start);
            //std::cout << "city: " << city << "\n";
            
            //auto start = std::chrono::high_resolution_clock::now();
            fx_table_->insert(city, n);
            //auto finish = std::chrono::high_resolution_clock::now();
            //std::cout << "inserting measurement took " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";

            //current += 1;
        }

        //std::printf("finished thread %lu\n", index_);
    }
private:
    char* data_;
    FxTable* fx_table_;
    std::size_t index_, chunk_size_;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr, "call like this: \"%s file\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    try {
        MemoryMappedFile mmap_file(argv[1]);
        std::printf("%s size: %ld\n", argv[1], mmap_file.file_size());

        constexpr std::size_t workers = 64;
        std::size_t chunk_size = mmap_file.file_size() / workers;

        struct AlignedFxTable {
            alignas(64) FxTable map_;
        };
        std::array<AlignedFxTable, workers> tables{};
        std::vector<std::jthread> worker_threads{};

        for (std::size_t i = 0; i < workers; i++) {
            Parser p(mmap_file.base_ptr(), i, chunk_size, &tables[i].map_);
            worker_threads.emplace_back(p);
        }

        for (auto& m : worker_threads) {
            m.join();
        }

        FxTable global_map = tables[0].map_;
        for (std::size_t i = 1; i < tables.size(); i++) {
            global_map += tables[i].map_;
        }
        global_map.print();

    } catch (const std::exception& e) {
        std::fprintf(stderr, "failed to create mmap file: %s", e.what());
    }

    return 0;
}
