#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <functional>
#include <queue>

struct Wine {
    std::string title;
    std::string country;
    std::string variety;
    std::string winery;
    int points;
    double price;
    std::string province;
    std::string region_1;
};

std::vector<Wine> ReadWinesFromFile(const std::string& file_name) {
    std::vector<Wine> wines;
    std::ifstream file(file_name);
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        Wine wine;
        std::getline(ss, wine.title, ',');
        std::getline(ss, wine.country, ',');
        std::getline(ss, wine.variety, ',');
        std::getline(ss, wine.winery, ',');
        ss >> wine.points;
        ss.ignore();
        ss >> wine.price;
        ss.ignore();
        std::getline(ss, wine.province, ',');
        std::getline(ss, wine.region_1);

        wines.push_back(wine);
    }



    return wines;
}

std::unordered_map<std::string, std::vector<Wine>> hashtable_func(const std::vector<Wine>& wines) {
    std::unordered_map<std::string, std::vector<Wine>> hashTable;

    for (const auto& wine : wines) {
        hashTable[wine.country].push_back(wine);
    }

    return hashTable;
}


std::unordered_map<std::string, std::vector<Wine>> hashtable_var(const std::vector<Wine>& wines) {
    std::unordered_map<std::string, std::vector<Wine>> hashTable;

    for (const auto& wine : wines) {
        hashTable[wine.variety].push_back(wine);
    }

    return hashTable;
}

std::vector<Wine> GetWinesByCountry(const std::unordered_map<std::string, std::vector<Wine>>& countryHashTable, const std::string& country) {
    auto it = countryHashTable.find(country);
    if (it != countryHashTable.end()) {
        return it->second;
    } else {
        return {};
    }

}

std::vector<Wine> GetWinesByVariety(const std::unordered_map<std::string, std::vector<Wine>>& countryHashTable, const std::string& variety) {
    auto it = countryHashTable.find(variety);
    if (it != countryHashTable.end()) {
        return it->second;
    } else {
        return {};
    }

}

struct CompareWineByPrice {
    bool operator()(const Wine& a, const Wine& b) {
        return a.price > b.price;
    }
};


std::priority_queue<Wine, std::vector<Wine>, CompareWineByPrice> CreatePriceHeap(const std::vector<Wine>& wines) {
    std::priority_queue<Wine, std::vector<Wine>, CompareWineByPrice> priceHeap;

    for (const auto& wine : wines) {
        priceHeap.push(wine);
    }

    return priceHeap;
}

std::map<std::string, Wine> GetWinesLessThanValue(const std::vector<Wine>& wines, double userValue) {
    std::map<std::string, Wine> filteredWines;

    for (const Wine& wine : wines) {
        if (wine.price < userValue) {
            filteredWines[wine.title] = wine;
        }
    }

    return filteredWines;
}

void DisplayPriceHeap(std::priority_queue<Wine, std::vector<Wine>, CompareWineByPrice> priceHeap) {
    while (!priceHeap.empty()) {
        Wine topWine = priceHeap.top();
        priceHeap.pop();

        std::cout << "Wine: " << topWine.title
                  << ", Price: " << topWine.price
                  << ", Country: " << topWine.country
                  << std::endl;
    }
}


