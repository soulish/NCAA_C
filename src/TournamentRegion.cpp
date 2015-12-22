//
// Created by soulish on 12/21/15.
//

#include "TournamentRegion.h"

TournamentRegion::TournamentRegion(int _year)
        : year(_year),
          teams(),
          pcts(),
          firstRoundWinners(),
          secondRoundWinners(),
          thirdRoundWinners(),
          regionChamp(""),
          playInSeeds(),
          seeds(),
          pcts_all() { }

void TournamentRegion::addTeam(std::string team, int seed) {
    teams.emplace(team,seed);
    pcts.emplace(team,1);
    pcts_all.emplace(team, new std::array<double,5>());
    for (int i = 0; i < 5; i++)
        pcts_all[team]->at(i) = i == 0 ? 1 : 0;
    if (std::find(std::begin(seeds), std::end(seeds), seed) == seeds.end())
        seeds.push_back(seed);
}

void TournamentRegion::playInGames(TH1F *hist, bool verbose, std::string chosenTeam) {
    std::unordered_map<int, std::vector<std::string> *> teamsBySeed;
    for (int i = 1; i <= 16; i++)
        teamsBySeed.emplace(i, new std::vector<std::string>());

    for (auto &t : teams)
        teamsBySeed[t.second]->push_back(t.first);

    for (auto & teamSeed : teamsBySeed){
        if (teamSeed.second->size() == 2){
            playInSeeds.push_back(teamSeed.first);
            seeds.push_back(-1*teamSeed.first);

            Team *teamA = Team::findTeam(teamSeed.second->at(0));
            Team *teamB = Team::findTeam(teamSeed.second->at(1));

            TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
            TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

            double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
            double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

            pcts[teamSeed.second->at(0)] = gamePct;
            pcts[teamSeed.second->at(1)] = 1 - gamePct;
            pcts_all[teamSeed.second->at(0)]->at(0) = gamePct;
            pcts_all[teamSeed.second->at(1)]->at(0) = 1 - gamePct;

            if (teamSeed.second->at(0) == chosenTeam ||
                (teamSeed.second->at(1) != chosenTeam && gameScore >= 0)){

                if (verbose) {
                    printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
                           "Play in Game:",teams[teamSeed.second->at(0)], teamSeed.second->at(0).c_str(),
                           teams[teamSeed.second->at(1)], teamSeed.second->at(1).c_str(),
                           teams[teamSeed.second->at(0)], teamSeed.second->at(0).c_str(),
                           gameScore, gameScore >= 0 ? gamePct : 1 - gamePct);
                }

                teams[teamSeed.second->at(0)] = teamSeed.first;
                teams[teamSeed.second->at(1)] = -1*teamSeed.first;
            }
            else if (teamSeed.second->at(1) == chosenTeam ||
                     (teamSeed.second->at(0) != chosenTeam && gameScore < 0)){

                if (verbose) {
                    printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
                           "Play in Game:",teams[teamSeed.second->at(0)], teamSeed.second->at(0).c_str(),
                           teams[teamSeed.second->at(1)], teamSeed.second->at(1).c_str(),
                           teams[teamSeed.second->at(1)], teamSeed.second->at(1).c_str(),
                           gameScore, gameScore >= 0 ? gamePct : 1 - gamePct);
                }

                teams[teamSeed.second->at(0)] = -1*teamSeed.first;
                teams[teamSeed.second->at(1)] = teamSeed.first;
            }
        }
    }

    //clean up pointers
    for (int i = 1; i <= 16; i++)
        delete teamsBySeed[i];
}

void TournamentRegion::firstRound(TH1F *hist, bool verbose, std::string chosenTeam) {
    std::unordered_map<int, std::string> teamsBySeed;
    for (auto &t : teams)
        teamsBySeed.emplace(t.second,t.first);

    std::vector<std::array<int,2>*> matchups;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 1; matchups.back()->at(1) = 16;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 8; matchups.back()->at(1) = 9;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 5; matchups.back()->at(1) = 12;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 4; matchups.back()->at(1) = 13;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 6; matchups.back()->at(1) = 11;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 3; matchups.back()->at(1) = 14;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 7; matchups.back()->at(1) = 10;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 2; matchups.back()->at(1) = 15;

    std::array<double,8> gameScores;

    for (int i = 0; i < matchups.size(); i++){
        Team *teamA = Team::findTeam(teamsBySeed[matchups[i]->at(0)]);
        Team *teamB = Team::findTeam(teamsBySeed[matchups[i]->at(1)]);

        TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
        TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

        double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
        double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

        if (teamsBySeed[matchups[i]->at(0)] == chosenTeam ||
            (teamsBySeed[matchups[i]->at(1)] != chosenTeam && gameScore >= 0)){
            firstRoundWinners.push_back(teamsBySeed[matchups[i]->at(0)]);
        }
        else if (teamsBySeed[matchups[i]->at(1)] == chosenTeam ||
                 (teamsBySeed[matchups[i]->at(0)] != chosenTeam && gameScore < 0)){
            firstRoundWinners.push_back(teamsBySeed[matchups[i]->at(1)]);
        }
        pcts[teamsBySeed[matchups[i]->at(0)]] *= gamePct;
        pcts[teamsBySeed[matchups[i]->at(1)]] *= (1 - gamePct);

        if (verbose){
            printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
                   "Round of 64:",teams[teamsBySeed[matchups[i]->at(0)]], teamsBySeed[matchups[i]->at(0)].c_str(),
                   teams[teamsBySeed[matchups[i]->at(1)]], teamsBySeed[matchups[i]->at(1)].c_str(),
                   teams[firstRoundWinners.back()], firstRoundWinners.back().c_str(),
                   gameScore, gameScore >= 0 ? gamePct : 1 - gamePct);
        }
    }
}

void TournamentRegion::secondRound(TH1F *hist, bool verbose, std::string chosenTeam) {
    std::vector<std::array<int,2>*> matchups;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 0; matchups.back()->at(1) = 1;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 2; matchups.back()->at(1) = 3;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 4; matchups.back()->at(1) = 5;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 6; matchups.back()->at(1) = 7;

    std::array<double,4> gameScores;

    for (int i = 0; i < matchups.size(); i++){
        Team *teamA = Team::findTeam(firstRoundWinners[matchups[i]->at(0)]);
        Team *teamB = Team::findTeam(firstRoundWinners[matchups[i]->at(1)]);

        TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
        TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

        double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
        double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

        if (firstRoundWinners[matchups[i]->at(0)] == chosenTeam ||
            (firstRoundWinners[matchups[i]->at(1)] != chosenTeam && gameScore >= 0)){
            secondRoundWinners.push_back(firstRoundWinners[matchups[i]->at(0)]);
        }
        else if (firstRoundWinners[matchups[i]->at(1)] == chosenTeam ||
                 (firstRoundWinners[matchups[i]->at(0)] != chosenTeam && gameScore < 0)){
            secondRoundWinners.push_back(firstRoundWinners[matchups[i]->at(1)]);
        }
        pcts[firstRoundWinners[matchups[i]->at(0)]] *= gamePct;
        pcts[firstRoundWinners[matchups[i]->at(1)]] *= (1 - gamePct);

        if (verbose){
            printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
                   "Round of 32:",teams[firstRoundWinners[matchups[i]->at(0)]], firstRoundWinners[matchups[i]->at(0)].c_str(),
                   teams[firstRoundWinners[matchups[i]->at(1)]], firstRoundWinners[matchups[i]->at(1)].c_str(),
                   teams[secondRoundWinners.back()], secondRoundWinners.back().c_str(),
                   gameScore, gameScore >= 0 ? gamePct : 1 - gamePct);
        }
    }
}

void TournamentRegion::thirdRound(TH1F *hist, bool verbose, std::string chosenTeam) {
    std::vector<std::array<int,2>*> matchups;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 0; matchups.back()->at(1) = 1;
    matchups.push_back(new std::array<int,2>());
    matchups.back()->at(0) = 2; matchups.back()->at(1) = 3;

    std::array<double,2> gameScores;

    for (int i = 0; i < matchups.size(); i++){
        Team *teamA = Team::findTeam(secondRoundWinners[matchups[i]->at(0)]);
        Team *teamB = Team::findTeam(secondRoundWinners[matchups[i]->at(1)]);

        TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
        TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

        double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
        double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

        if (secondRoundWinners[matchups[i]->at(0)] == chosenTeam ||
            (secondRoundWinners[matchups[i]->at(1)] != chosenTeam && gameScore >= 0)){
            thirdRoundWinners.push_back(secondRoundWinners[matchups[i]->at(0)]);
        }
        else if (secondRoundWinners[matchups[i]->at(1)] == chosenTeam ||
                 (secondRoundWinners[matchups[i]->at(0)] != chosenTeam && gameScore < 0)){
            thirdRoundWinners.push_back(secondRoundWinners[matchups[i]->at(1)]);
        }
        pcts[secondRoundWinners[matchups[i]->at(0)]] *= gamePct;
        pcts[secondRoundWinners[matchups[i]->at(1)]] *= (1 - gamePct);

        if (verbose){
            printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
                   "Sweet Sixteen:",teams[secondRoundWinners[matchups[i]->at(0)]], secondRoundWinners[matchups[i]->at(0)].c_str(),
                   teams[secondRoundWinners[matchups[i]->at(1)]], secondRoundWinners[matchups[i]->at(1)].c_str(),
                   teams[thirdRoundWinners.back()], thirdRoundWinners.back().c_str(),
                   gameScore, gameScore >= 0 ? gamePct : 1 - gamePct);
        }
    }
}

void TournamentRegion::fourthRound(TH1F *hist, bool verbose, std::string chosenTeam) {
    Team *teamA = Team::findTeam(thirdRoundWinners[0]);
    Team *teamB = Team::findTeam(thirdRoundWinners[1]);

    TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
    TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

    double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
    double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

    if (thirdRoundWinners[0] == chosenTeam ||
        (thirdRoundWinners[1] != chosenTeam && gameScore >= 0)){
        regionChamp = thirdRoundWinners[0];
    }
    else if (thirdRoundWinners[1] == chosenTeam ||
             (thirdRoundWinners[0] != chosenTeam && gameScore < 0)){
        regionChamp = thirdRoundWinners[1];
    }
    pcts[thirdRoundWinners[0]] *= gamePct;
    pcts[thirdRoundWinners[1]] *= (1 - gamePct);

    if (verbose){
        printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
               "Elite Eight:",teams[thirdRoundWinners[0]], thirdRoundWinners[0].c_str(),
               teams[thirdRoundWinners[1]], thirdRoundWinners[1].c_str(),
               teams[regionChamp], regionChamp.c_str(),
               gameScore, gameScore >= 0 ? gamePct : 1 - gamePct);
    }
}

void TournamentRegion::reset() {
    pcts_all.clear();
    for (auto &t : teams){
        pcts[t.first] = 1;
        pcts_all.emplace(t.first, new std::array<double,5>());
        for (int i = 0; i < 5; i++)
            pcts_all[t.first]->at(i) = i == 0 ? 1 : 0;
        t.second = abs(t.second);
    }
    firstRoundWinners.clear();
    secondRoundWinners.clear();
    thirdRoundWinners.clear();
    regionChamp = "";
}
