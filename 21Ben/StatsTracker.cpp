#include "StatsTracker.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Singleton accessor
StatsTracker& StatsTracker::getInstance() {
    static StatsTracker instance;
    return instance;
}

// Constructor initializes with default values and tries to load existing stats
StatsTracker::StatsTracker() {
    loadStats();
}

void StatsTracker::recordGamePlayed(bool isWin, bool isDraw, int chipsWon, int chipsLost, float betSize, const std::string& strategy) {
    totalGamesPlayed++;

    // Update outcome stats
    if (isWin) {
        totalWins++;
        recordWin();
    }
    else if (isDraw) {
        totalDraws++;
        // Draws don't affect win/loss streaks
    }
    else {
        totalLosses++;
        recordLoss();
    }

    // Update chip and bet stats
    totalChipsWon += chipsWon;
    totalChipsLost += chipsLost;
    totalBetAmount += betSize;
    totalBets++;

    // Update strategy-specific stats
    auto& stats = strategyStats[strategy];
    stats.gamesPlayed++;
    if (isWin) {
        stats.wins++;
    }
    stats.chipsWon += chipsWon;
    stats.chipsLost += chipsLost;

    // Save stats after each game
    saveStats();
}

void StatsTracker::recordWin() {
    currentWinStreak++;
    currentLossStreak = 0;

    if (currentWinStreak > maxWinStreak) {
        maxWinStreak = currentWinStreak;
    }
}

void StatsTracker::recordLoss() {
    currentLossStreak++;
    currentWinStreak = 0;

    if (currentLossStreak > maxLossStreak) {
        maxLossStreak = currentLossStreak;
    }
}

bool StatsTracker::saveStats() {
    std::ofstream outFile(statsFilePath, std::ios::binary);

    if (!outFile.is_open()) {
        std::cerr << "Error opening statistics file for writing: " << statsFilePath << std::endl;
        return false;
    }

    // Write general game stats
    outFile.write(reinterpret_cast<char*>(&totalGamesPlayed), sizeof(totalGamesPlayed));
    outFile.write(reinterpret_cast<char*>(&totalWins), sizeof(totalWins));
    outFile.write(reinterpret_cast<char*>(&totalLosses), sizeof(totalLosses));
    outFile.write(reinterpret_cast<char*>(&totalDraws), sizeof(totalDraws));

    // Write betting stats
    outFile.write(reinterpret_cast<char*>(&totalChipsWon), sizeof(totalChipsWon));
    outFile.write(reinterpret_cast<char*>(&totalChipsLost), sizeof(totalChipsLost));
    outFile.write(reinterpret_cast<char*>(&totalBets), sizeof(totalBets));
    outFile.write(reinterpret_cast<char*>(&totalBetAmount), sizeof(totalBetAmount));

    // Write streak stats
    outFile.write(reinterpret_cast<char*>(&currentWinStreak), sizeof(currentWinStreak));
    outFile.write(reinterpret_cast<char*>(&currentLossStreak), sizeof(currentLossStreak));
    outFile.write(reinterpret_cast<char*>(&maxWinStreak), sizeof(maxWinStreak));
    outFile.write(reinterpret_cast<char*>(&maxLossStreak), sizeof(maxLossStreak));

    // Write strategy stats
    size_t numStrategies = strategyStats.size();
    outFile.write(reinterpret_cast<char*>(&numStrategies), sizeof(numStrategies));

    for (const auto& pair : strategyStats) {
        // Write strategy name
        size_t nameLength = pair.first.length();
        outFile.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        outFile.write(pair.first.c_str(), nameLength);

        // Make copies of the member variables to avoid const issues
        int gamesPlayed = pair.second.gamesPlayed;
        int wins = pair.second.wins;
        int chipsWon = pair.second.chipsWon;
        int chipsLost = pair.second.chipsLost;

        // Write strategy stats (using non-const copies)
        outFile.write(reinterpret_cast<char*>(&gamesPlayed), sizeof(gamesPlayed));
        outFile.write(reinterpret_cast<char*>(&wins), sizeof(wins));
        outFile.write(reinterpret_cast<char*>(&chipsWon), sizeof(chipsWon));
        outFile.write(reinterpret_cast<char*>(&chipsLost), sizeof(chipsLost));
    }

    outFile.close();
    return true;
}

bool StatsTracker::loadStats() {
    std::ifstream inFile(statsFilePath, std::ios::binary);

    if (!inFile.is_open()) {
        std::cerr << "Statistics file not found. Starting with fresh statistics." << std::endl;
        return false;
    }

    // Read general game stats
    inFile.read(reinterpret_cast<char*>(&totalGamesPlayed), sizeof(totalGamesPlayed));
    inFile.read(reinterpret_cast<char*>(&totalWins), sizeof(totalWins));
    inFile.read(reinterpret_cast<char*>(&totalLosses), sizeof(totalLosses));
    inFile.read(reinterpret_cast<char*>(&totalDraws), sizeof(totalDraws));

    // Read betting stats
    inFile.read(reinterpret_cast<char*>(&totalChipsWon), sizeof(totalChipsWon));
    inFile.read(reinterpret_cast<char*>(&totalChipsLost), sizeof(totalChipsLost));
    inFile.read(reinterpret_cast<char*>(&totalBets), sizeof(totalBets));
    inFile.read(reinterpret_cast<char*>(&totalBetAmount), sizeof(totalBetAmount));

    // Read streak stats
    inFile.read(reinterpret_cast<char*>(&currentWinStreak), sizeof(currentWinStreak));
    inFile.read(reinterpret_cast<char*>(&currentLossStreak), sizeof(currentLossStreak));
    inFile.read(reinterpret_cast<char*>(&maxWinStreak), sizeof(maxWinStreak));
    inFile.read(reinterpret_cast<char*>(&maxLossStreak), sizeof(maxLossStreak));

    // Read strategy stats
    size_t numStrategies;
    inFile.read(reinterpret_cast<char*>(&numStrategies), sizeof(numStrategies));

    for (size_t i = 0; i < numStrategies; i++) {
        // Read strategy name
        size_t nameLength;
        inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

        std::vector<char> nameBuffer(nameLength + 1);
        inFile.read(nameBuffer.data(), nameLength);
        nameBuffer[nameLength] = '\0';
        std::string strategyName(nameBuffer.data());

        // Read strategy stats
        StrategyStats stats;
        inFile.read(reinterpret_cast<char*>(&stats.gamesPlayed), sizeof(stats.gamesPlayed));
        inFile.read(reinterpret_cast<char*>(&stats.wins), sizeof(stats.wins));
        inFile.read(reinterpret_cast<char*>(&stats.chipsWon), sizeof(stats.chipsWon));
        inFile.read(reinterpret_cast<char*>(&stats.chipsLost), sizeof(stats.chipsLost));

        strategyStats[strategyName] = stats;
    }

    inFile.close();
    return true;
}

void StatsTracker::resetStats() {
    totalGamesPlayed = 0;
    totalWins = 0;
    totalLosses = 0;
    totalDraws = 0;

    totalChipsWon = 0;
    totalChipsLost = 0;
    totalBets = 0;
    totalBetAmount = 0.0f;

    currentWinStreak = 0;
    currentLossStreak = 0;
    maxWinStreak = 0;
    maxLossStreak = 0;

    strategyStats.clear();

    saveStats();
}

float StatsTracker::getWinPercentage() const {
    if (totalGamesPlayed == 0) {
        return 0.0f;
    }
    return (static_cast<float>(totalWins) / totalGamesPlayed) * 100.0f;
}

float StatsTracker::getAvgBetSize() const {
    if (totalBets == 0) {
        return 0.0f;
    }
    return totalBetAmount / totalBets;
}

float StatsTracker::getStrategyWinRate(const std::string& strategy) const {
    auto it = strategyStats.find(strategy);
    if (it == strategyStats.end() || it->second.gamesPlayed == 0) {
        return 0.0f;
    }
    return (static_cast<float>(it->second.wins) / it->second.gamesPlayed) * 100.0f;
}

float StatsTracker::getStrategyAvgChips(const std::string& strategy) const {
    auto it = strategyStats.find(strategy);
    if (it == strategyStats.end() || it->second.gamesPlayed == 0) {
        return 0.0f;
    }
    float netChips = static_cast<float>(it->second.chipsWon - it->second.chipsLost);
    return netChips / it->second.gamesPlayed;
}

std::vector<std::string> StatsTracker::getUsedStrategies() const {
    std::vector<std::string> strategies;
    strategies.reserve(strategyStats.size());

    for (const auto& pair : strategyStats) {
        strategies.push_back(pair.first);
    }

    return strategies;
}