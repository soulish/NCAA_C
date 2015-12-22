//
// Created by soulish on 12/21/15.
//

#include "Tournament.h"
#include <fstream>

Tournament::Tournament(int _year)
        : year(_year),
          teams(),
          pcts(),
          regions(),
          regionChamps(),
          championshipGameTeams(),
          champion(""),
          championPct(1.0) {
    for (int i = 0; i < 4; i++)
        regions[i] = new TournamentRegion(_year);
}

void Tournament::addTeam(int region, std::string teamName, int seed) {
    regions[region]->addTeam(teamName,seed);
    teams.emplace(teamName, seed);
    pcts.emplace(teamName, 1.0);
}

void Tournament::play(TH1F *hist, bool verbose, std::string chosenTeam) {
    regionChamps.fill("");
    championshipGameTeams.fill("");
    champion = "";
    championPct = 1;
    if (verbose) {
        printf("\n%-15s %-33s vs %-33s  =>  %-31s %10s %-9s\n\n",
               "Round:", "Team A","Team B", "Winner",
               "gameScore","gamePct");
    }
    for (int i = 0; i < 4; i++){
        regions[i]->reset();
        regions[i]->playInGames(hist,verbose,chosenTeam);
    }

    if (verbose) std::cout << "\n\n" << std::endl;
    for (int i = 0; i < 4; i++){
        regions[i]->firstRound(hist,verbose,chosenTeam);
        if (verbose) std::cout << std::endl;
    }

    if (verbose) std::cout << "\n\n" << std::endl;
    for (int i = 0; i < 4; i++){
        regions[i]->secondRound(hist,verbose,chosenTeam);
        if (verbose) std::cout << std::endl;
    }

    if (verbose) std::cout << "\n\n" << std::endl;
    for (int i = 0; i < 4; i++){
        regions[i]->thirdRound(hist,verbose,chosenTeam);
    }

    if (verbose) std::cout << "\n\n" << std::endl;
    for (int i = 0; i < 4; i++){
        regions[i]->fourthRound(hist,verbose,chosenTeam);
        regionChamps[i] = regions[i]->getRegionChamp();
        pcts.emplace(regionChamps[i], regions[i]->getPct(regionChamps[i]));
    }

    if (verbose) std::cout << "\n\n" << std::endl;

    //Done with the early rounds, now do the final four
    Team *teamA = Team::findTeam(regionChamps[0]);
    Team *teamB = Team::findTeam(regionChamps[1]);
    Team *teamC = Team::findTeam(regionChamps[2]);
    Team *teamD = Team::findTeam(regionChamps[3]);

    TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
    TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
    TeamWAverage *waC = teamC->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
    TeamWAverage *waD = teamD->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

    double gameScoreAB = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
    double gamePctAB = hist->GetBinContent((gameScoreAB+3)*1600/6.0);

    double gameScoreCD = ConstantGameFunction::Instance()->predictGame(waC,waD,year,"neutral","neutral");
    double gamePctCD = hist->GetBinContent((gameScoreCD+3)*1600/6.0);

    if (regionChamps[0] == chosenTeam || (regionChamps[1] != chosenTeam && gameScoreAB >= 0))
        championshipGameTeams[0] = regionChamps[0];
    else if (regionChamps[1] == chosenTeam || (regionChamps[0] != chosenTeam && gameScoreAB < 0))
        championshipGameTeams[0] = regionChamps[1];
    pcts[regionChamps[0]] *= gamePctAB;
    pcts[regionChamps[1]] *= (1 - gamePctAB);
    if (verbose){
        printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
               "Final Four:",teams[regionChamps[0]], regionChamps[0].c_str(),
               teams[regionChamps[1]], regionChamps[1].c_str(),
               teams[championshipGameTeams[0]], championshipGameTeams[0].c_str(),
               fabs(gameScoreAB), gameScoreAB >= 0 ? gamePctAB : 1 - gamePctAB);
    }

    if (regionChamps[2] == chosenTeam || (regionChamps[3] != chosenTeam && gameScoreCD >= 0))
        championshipGameTeams[1] = regionChamps[2];
    else if (regionChamps[3] == chosenTeam || (regionChamps[2] != chosenTeam && gameScoreCD < 0))
        championshipGameTeams[1] = regionChamps[3];
    pcts[regionChamps[2]] *= gamePctCD;
    pcts[regionChamps[3]] *= (1 - gamePctCD);
    if (verbose){
        printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
               "Final Four:",teams[regionChamps[2]], regionChamps[2].c_str(),
               teams[regionChamps[3]], regionChamps[3].c_str(),
               teams[championshipGameTeams[1]], championshipGameTeams[1].c_str(),
               fabs(gameScoreCD), gameScoreCD >= 0 ? gamePctCD : 1 - gamePctCD);
    }


    if (verbose) std::cout << "\n\n" << std::endl;

    //Championship game
    Team *teamE = Team::findTeam(championshipGameTeams[0]);
    Team *teamF = Team::findTeam(championshipGameTeams[1]);

    TeamWAverage *waE = teamE->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
    TeamWAverage *waF = teamF->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

    double gameScoreEF = ConstantGameFunction::Instance()->predictGame(waE,waF,year,"neutral","neutral");
    double gamePctEF = hist->GetBinContent((gameScoreEF+3)*1600/6.0);

    if (championshipGameTeams[0] == chosenTeam || (championshipGameTeams[1] != chosenTeam && gameScoreEF >= 0))
        champion = championshipGameTeams[0];
    else if (championshipGameTeams[1] == chosenTeam || (championshipGameTeams[0] != chosenTeam && gameScoreEF < 0))
        champion = championshipGameTeams[1];
    pcts[championshipGameTeams[0]] *= gamePctEF;
    pcts[championshipGameTeams[1]] *= (1 - gamePctEF);
    if (verbose){
        printf("%-15s %2d %-30s vs %2d %-30s  =>  %2d %-30s\t% .3f\t%.3f\n",
               "Championship:",teams[championshipGameTeams[0]], championshipGameTeams[0].c_str(),
               teams[championshipGameTeams[1]], championshipGameTeams[1].c_str(),
               teams[champion], champion.c_str(),
               fabs(gameScoreEF), gameScoreEF >= 0 ? gamePctEF : 1 - gamePctEF);
    }

    championPct = pcts[champion];
}

void Tournament::readTournamentInfo(std::string fileName) {
    std::ifstream file(fileName);
    std::string value;
    int regionNumber, seed;
    std::string teamName;
    while (file.good()) {
        getline(file, value, ',');
        if (value == "") break; //stops when reading last line
        regionNumber = stoi(value);
        getline(file, teamName, ',');
        getline(file, value, '\n');
        seed = stoi(value);
        this->addTeam(regionNumber,teamName,seed);
    }
}
