#ifndef STATS_TRACKER_H
#define STATS_TRACKER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

// Single global instance to track statistics across the game
class StatsTracker {
public:
    // Singleton accessor
    static StatsTracker& getInstance();

    // Game outcome tracking
    void recordGamePlayed(bool isWin, bool isDraw, int chipsWon, int chipsLost, float betSize, const std::string& strategy);

    // Streak tracking
    void recordWin();
    void recordLoss();

    // Save and load statistics
    bool saveStats();
    bool loadStats();

    // Reset statistics (for testing)
    void resetStats();

    // Getters for statistics
    int getTotalGamesPlayed() const { return totalGamesPlayed; }
    int getTotalWins() const { return totalWins; }
    int getTotalLosses() const { return totalLosses; }
    int getTotalDraws() const { return totalDraws; }
    float getWinPercentage() const;

    int getTotalChipsWon() const { return totalChipsWon; }
    int getTotalChipsLost() const { return totalChipsLost; }
    float getAvgBetSize() const;
    int getMaxWinStreak() const { return maxWinStreak; }
    int getMaxLossStreak() const { return maxLossStreak; }

    // Basic statistics for all strategies
    float getStrategyWinRate(const std::string& strategy) const;
    float getStrategyAvgChips(const std::string& strategy) const;

    // Strategy-specific win rates (convenience methods)
    float getBasicStrategyWinRate() const { return getStrategyWinRate("Standard"); }
    float getHiLoCountingWinRate() const { return getStrategyWinRate("HiLo"); }
    float getKOCountingWinRate() const { return getStrategyWinRate("KO"); }
    float getHiOpt1CountingWinRate() const { return getStrategyWinRate("HiOpt1"); }
    float getHiOpt2CountingWinRate() const { return getStrategyWinRate("HiOpt2"); }
    float getMentorCountingWinRate() const { return getStrategyWinRate("Mentor"); }
    float getOmega2CountingWinRate() const { return getStrategyWinRate("Omega2"); }
    float getRPCCountingWinRate() const { return getStrategyWinRate("RPC"); }
    float getRAPCCountingWinRate() const { return getStrategyWinRate("RAPC"); }
    float getR14CCountingWinRate() const { return getStrategyWinRate("R14C"); }
    float getWongHalvesCountingWinRate() const { return getStrategyWinRate("WongHalves"); }
    float getZenCountingWinRate() const { return getStrategyWinRate("Zen"); }

    // Strategy-specific chip averages (convenience methods)
    float getAvgChipsBasicStrategy() const { return getStrategyAvgChips("Standard"); }
    float getAvgChipsHiLoCounting() const { return getStrategyAvgChips("HiLo"); }
    float getAvgChipsKOCounting() const { return getStrategyAvgChips("KO"); }
    float getAvgChipsHiOpt1Counting() const { return getStrategyAvgChips("HiOpt1"); }
    float getAvgChipsHiOpt2Counting() const { return getStrategyAvgChips("HiOpt2"); }
    float getAvgChipsMentorCounting() const { return getStrategyAvgChips("Mentor"); }
    float getAvgChipsOmega2Counting() const { return getStrategyAvgChips("Omega2"); }
    float getAvgChipsRPCCounting() const { return getStrategyAvgChips("RPC"); }
    float getAvgChipsRAPCCounting() const { return getStrategyAvgChips("RAPC"); }
    float getAvgChipsR14CCounting() const { return getStrategyAvgChips("R14C"); }
    float getAvgChipsWongHalvesCounting() const { return getStrategyAvgChips("WongHalves"); }
    float getAvgChipsZenCounting() const { return getStrategyAvgChips("Zen"); }

    // Get a list of all strategies that have been used
    std::vector<std::string> getUsedStrategies() const;

private:
    // Private constructor for singleton
    StatsTracker();

    // Prevent copying
    StatsTracker(const StatsTracker&) = delete;
    StatsTracker& operator=(const StatsTracker&) = delete;

    // Statistics file path
    const std::string statsFilePath = "assets/statistics.dat";

    // Game statistics
    int totalGamesPlayed = 0;
    int totalWins = 0;
    int totalLosses = 0;
    int totalDraws = 0;

    // Betting statistics
    int totalChipsWon = 0;
    int totalChipsLost = 0;
    int totalBets = 0;
    float totalBetAmount = 0.0f;

    // Streak tracking
    int currentWinStreak = 0;
    int currentLossStreak = 0;
    int maxWinStreak = 0;
    int maxLossStreak = 0;

    // Strategy statistics
    struct StrategyStats {
        int gamesPlayed = 0;
        int wins = 0;
        int chipsWon = 0;
        int chipsLost = 0;
    };

    std::unordered_map<std::string, StrategyStats> strategyStats;
};

#endif // STATS_TRACKER_H 