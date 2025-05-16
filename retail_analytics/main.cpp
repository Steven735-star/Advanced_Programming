#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include "CLI/CLI.hpp"
#include "barkeep/progress.hpp"

struct Transaction {
    std::string invoiceNo;
    std::string stockCode;
    std::string description;
    int quantity;
    std::string invoiceDate;
    double unitPrice;
    std::string customerID;
    std::string country;
};

std::vector<Transaction> readCSV(const std::string& filename, barkeep::Progress& progressBar) {
    std::vector<Transaction> transactions;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return transactions;
    }
    
    // Count lines for progress bar
    std::ifstream countFile(filename);
    size_t lineCount = 0;
    std::string line;
    while (std::getline(countFile, line)) {
        lineCount++;
    }
    progressBar.set_total(lineCount);
    
    // Skip header
    std::getline(file, line);
    progressBar.update(1);
    
    // Read data
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Transaction transaction;
        
        // Parse CSV fields
        std::getline(ss, transaction.invoiceNo, ',');
        std::getline(ss, transaction.stockCode, ',');
        
        // Handle description field which might contain commas
        if (ss.peek() == '"') {
            ss.ignore(); // Skip opening quote
            std::getline(ss, transaction.description, '"');
            ss.ignore(); // Skip comma after closing quote
        } else {
            std::getline(ss, transaction.description, ',');
        }
        
        // Parse remaining fields
        std::string quantityStr, unitPriceStr;
        std::getline(ss, quantityStr, ',');
        std::getline(ss, transaction.invoiceDate, ',');
        std::getline(ss, unitPriceStr, ',');
        std::getline(ss, transaction.customerID, ',');
        std::getline(ss, transaction.country);
        
        // Convert string to numeric types
        try {
            transaction.quantity = std::stoi(quantityStr);
            transaction.unitPrice = std::stod(unitPriceStr);
        } catch (const std::exception& e) {
            // Skip invalid records
            progressBar.update(1);
            continue;
        }
        
        transactions.push_back(transaction);
        progressBar.update(1);
    }
    
    return transactions;
}

void countTransactionsByCountry(const std::vector<Transaction>& transactions, const std::string& country = "") {
    std::map<std::string, int> countryCount;
    barkeep::Progress progressBar("Counting transactions by country", transactions.size());
    
    for (const auto& transaction : transactions) {
        countryCount[transaction.country]++;
        progressBar.update(1);
    }
    
    if (country.empty()) {
        // Display all countries
        std::cout << "\nTransaction count by country:\n";
        for (const auto& [countryName, count] : countryCount) {
            std::cout << countryName << ": " << count << std::endl;
        }
    } else {
        // Display specific country
        auto it = countryCount.find(country);
        if (it != countryCount.end()) {
            std::cout << "\nTransaction count for " << country << ": " << it->second << std::endl;
        } else {
            std::cout << "\nNo transactions found for country: " << country << std::endl;
        }
    }
}

void calculateTotalAmount(const std::vector<Transaction>& transactions, bool onlyUK) {
    double totalAmount = 0.0;
    int processedTransactions = 0;
    barkeep::Progress progressBar("Calculating total amount", transactions.size());
    
    for (const auto& transaction : transactions) {
        if (!onlyUK || transaction.country == "United Kingdom") {
            totalAmount += transaction.quantity * transaction.unitPrice;
            processedTransactions++;
        }
        progressBar.update(1);
    }
    
    std::cout << "\nTotal amount";
    if (onlyUK) {
        std::cout << " for United Kingdom";
    }
    std::cout << ": £" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
    std::cout << "Based on " << processedTransactions << " transactions." << std::endl;
}

int main(int argc, char** argv) {
    CLI::App app{"Online Retail Data Analytics"};
    
    std::string country;
    bool onlyUK = false;
    std::string filename = "online_retail.csv";
    
    app.add_option("-c,--country", country, "Specify country for transaction count");
    app.add_flag("--only-uk", onlyUK, "Calculate total amount only for UK");
    app.add_option("-f,--file", filename, "Input CSV file");
    
    CLI11_PARSE(app, argc, argv);
    
    std::cout << "Reading data from " << filename << "..." << std::endl;
    barkeep::Progress loadingBar("Loading data");
    auto transactions = readCSV(filename, loadingBar);
    
    if (transactions.empty()) {
        std::cerr << "No transactions were loaded. Please check the file path." << std::endl;
        return 1;
    }
    
    std::cout << "\nLoaded " << transactions.size() << " transactions." << std::endl;
    
    // Analysis 1: Count transactions by country
    countTransactionsByCountry(transactions, country);
    
    // Analysis 2: Calculate total amount
    calculateTotalAmount(transactions, onlyUK);
    
    return 0;
}