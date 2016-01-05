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
          championPct(1.0),
          pcts_all(),
          challengePoints(0){
    for (int i = 0; i < 4; i++)
        regions[i] = new TournamentRegion(_year);
}

void Tournament::addTeam(int region, std::string teamName, int seed) {
    regions[region]->addTeam(teamName,seed);
    teams.emplace(teamName, seed);
    pcts.emplace(teamName, 1.0);
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

void Tournament::play(TH1F *hist, bool verbose, std::string chosenTeam) {
    regionChamps.fill("");
    championshipGameTeams.fill("");
    champion = "";
    championPct = 1;
    if (verbose) {
        printf("\n%-15s %-35s vs %-35s  =>  %-33s %10s %9s\n\n",
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
        challengePoints += regions[i]->getChallengePoints();
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

    if (regionChamps[0] == chosenTeam || (regionChamps[1] != chosenTeam && gameScoreAB >= 0)) {
        championshipGameTeams[0] = regionChamps[0];

        TeamGame *x;
        x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(18));
        if (x && x->getWin() == 1)
            challengePoints += 160;
    }
    else if (regionChamps[1] == chosenTeam || (regionChamps[0] != chosenTeam && gameScoreAB < 0)) {
        championshipGameTeams[0] = regionChamps[1];

        TeamGame *x;
        x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(18));
        if (x && x->getWin() == 1)
            challengePoints += 160;
    }
    pcts[regionChamps[0]] *= gamePctAB;
    pcts[regionChamps[1]] *= (1 - gamePctAB);
    if (verbose){
        printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
               "Final Four:",teams[regionChamps[0]], regionChamps[0].c_str(),
               teams[regionChamps[1]], regionChamps[1].c_str(),
               teams[championshipGameTeams[0]], championshipGameTeams[0].c_str(),
               fabs(gameScoreAB), gameScoreAB >= 0 ? gamePctAB : 1 - gamePctAB);
    }

    if (regionChamps[2] == chosenTeam || (regionChamps[3] != chosenTeam && gameScoreCD >= 0)) {
        championshipGameTeams[1] = regionChamps[2];

        TeamGame *x;
        x = teamC->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(18));
        if (x && x->getWin() == 1)
            challengePoints += 160;
    }
    else if (regionChamps[3] == chosenTeam || (regionChamps[2] != chosenTeam && gameScoreCD < 0)) {
        championshipGameTeams[1] = regionChamps[3];

        TeamGame *x;
        x = teamD->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(18));
        if (x && x->getWin() == 1)
            challengePoints += 160;
    }
    pcts[regionChamps[2]] *= gamePctCD;
    pcts[regionChamps[3]] *= (1 - gamePctCD);
    if (verbose){
        printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
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

    if (championshipGameTeams[0] == chosenTeam || (championshipGameTeams[1] != chosenTeam && gameScoreEF >= 0)) {
        champion = championshipGameTeams[0];

        TeamGame *x;
        x = teamE->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(18));
        if (x && x->getWin() == 1)
            challengePoints += 320;
    }
    else if (championshipGameTeams[1] == chosenTeam || (championshipGameTeams[0] != chosenTeam && gameScoreEF < 0)) {
        champion = championshipGameTeams[1];

        TeamGame *x;
        x = teamF->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(18));
        if (x && x->getWin() == 1)
            challengePoints += 320;
    }
    pcts[championshipGameTeams[0]] *= gamePctEF;
    pcts[championshipGameTeams[1]] *= (1 - gamePctEF);
    if (verbose){
        printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
               "Championship:",teams[championshipGameTeams[0]], championshipGameTeams[0].c_str(),
               teams[championshipGameTeams[1]], championshipGameTeams[1].c_str(),
               teams[champion], champion.c_str(),
               fabs(gameScoreEF), gameScoreEF >= 0 ? gamePctEF : 1 - gamePctEF);

        std::cout << "Challenge Points: " << challengePoints << std::endl;
    }

    championPct = pcts[champion];
}

void Tournament::calculateTotalPercentages(TH1F *hist) {
    regionChamps.fill("");
    championshipGameTeams.fill("");
    champion = "";
    championPct = 1;
    for (int i = 0; i < 4; i++){
        regions[i]->reset();
        regions[i]->playInGames(hist, false);
    }

    for (int i = 0; i < 4; i++){
        regions[i]->firstRoundTotalPercentages(hist);
    }

    for (int i = 0; i < 4; i++){
        regions[i]->secondRoundTotalPercentages(hist);
    }

    for (int i = 0; i < 4; i++){
        regions[i]->thirdRoundTotalPercentages(hist);
    }

    //copying in the pcts_all arrays from the regions
    for (int i = 0; i < 4; i++){
        regions[i]->fourthRoundTotalPercentages(hist);
        std::unordered_map<std::string, std::array<double,5> *> regionPctsAll = regions[i]->getPctsAll();
        for (auto &p : regionPctsAll){
            pcts_all.emplace(p.first, new std::array<double,7>());
            for (int ii = 0; ii < 5; ii++)
                pcts_all[p.first]->at(ii) = p.second->at(ii);
        }
    }

    //handle the Final Four
    double s = 0;
    for (int i = 0; i < 4; i++){
        TournamentRegion *region = regions[i];
        TournamentRegion *oppRegion;
        if (i == 0) oppRegion = regions[1];
        else if (i == 1) oppRegion = regions[0];
        else if (i == 2) oppRegion = regions[3];
        else if (i == 3) oppRegion = regions[2];

        std::unordered_map<int, std::string> teamsBySeed = region->getTeamsBySeed();
        std::unordered_map<int, std::string> oppTeamsBySeed = oppRegion->getTeamsBySeed();

        for (auto &teamSeed : teamsBySeed){
            pcts_all[teamSeed.second]->at(5) = 0;
            Team *teamA = Team::findTeam(teamSeed.second);
            TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
            for (auto &oppTeamSeed : oppTeamsBySeed){
                Team *teamB = Team::findTeam(teamSeed.second);
                TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

                double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
                double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

                pcts_all[teamSeed.second]->at(5) += pcts_all[teamSeed.second]->at(4) *
                                                    pcts_all[oppTeamSeed.second]->at(4) *
                                                    gamePct;
                s += pcts_all[teamSeed.second]->at(4) * pcts_all[oppTeamSeed.second]->at(4) * gamePct;
            }
        }
    }

    //We renormalize here because it sometimes happens that, due to rounding issues, the sum is not
    //exactly 1 in each game.  Here we multiply each pct by 2 / s, because the pcts should add up to
    //1 for each game, and there are only 2 games in this round
    for (int i = 0; i < 4; i++) {
        TournamentRegion *region = regions[i];
        std::unordered_map<int, std::string> teamsBySeed = region->getTeamsBySeed();

        for (auto &teamSeed : teamsBySeed) {
            pcts_all[teamSeed.second]->at(5) = pcts_all[teamSeed.second]->at(5) * 2/s;
        }
    }



    //handle the Championship
    s = 0;
    for (int i = 0; i < 4; i++){
        TournamentRegion *region = regions[i];
        std::array<TournamentRegion*,2> oppRegions;
        if (i == 0 || i == 1) {
            oppRegions[0] = regions[2];
            oppRegions[1] = regions[3];
        }
        if (i == 2 || i == 3) {
            oppRegions[0] = regions[2];
            oppRegions[1] = regions[3];
        }

        std::unordered_map<int, std::string> teamsBySeed = region->getTeamsBySeed();

        for (auto &teamSeed : teamsBySeed){
            pcts_all[teamSeed.second]->at(6) = 0;
            Team *teamA = Team::findTeam(teamSeed.second);
            TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
            for (int j = 0; j < 2; j++) {
                TournamentRegion *oppRegion = oppRegions[j];
                std::unordered_map<int, std::string> oppTeamsBySeed = oppRegion->getTeamsBySeed();

                for (auto &oppTeamSeed : oppTeamsBySeed) {
                    Team *teamB = Team::findTeam(teamSeed.second);
                    TeamWAverage *waB = teamB->WAverageOnDate(
                            ConstantSeasonInfo::Instance()->get(year, "tournament start"));

                    double gameScore = ConstantGameFunction::Instance()->predictGame(waA, waB, year, "neutral",
                                                                                     "neutral");
                    double gamePct = hist->GetBinContent((gameScore + 3) * 1600 / 6.0);

                    pcts_all[teamSeed.second]->at(6) += pcts_all[teamSeed.second]->at(5) *
                                                        pcts_all[oppTeamSeed.second]->at(5) *
                                                        gamePct;
                    s += pcts_all[teamSeed.second]->at(5) * pcts_all[oppTeamSeed.second]->at(5) * gamePct;
                }
            }
        }
    }

    //We renormalize here because it sometimes happens that, due to rounding issues, the sum is not
    //exactly 1 in each game.  Here we multiply each pct by 1 / s, because the pcts should add up to
    //1 for each game, and there is only 1 game in this round
    for (int i = 0; i < 4; i++) {
        TournamentRegion *region = regions[i];
        std::unordered_map<int, std::string> teamsBySeed = region->getTeamsBySeed();

        for (auto &teamSeed : teamsBySeed) {
            pcts_all[teamSeed.second]->at(6) = pcts_all[teamSeed.second]->at(6) * 1/s;
        }
    }


    //Finally, print out the results.
    std::cout << std::endl;
    std::cout << "The numbers below are the percentages of getting INTO each round" << std::endl;
    for (int i = 0; i < 4; i++){
        std::cout << std::endl;
        printf("%s %-30s\t%5s\t\t%6s\t\t%5s\t\t%6s\t\t%5s\t\t%5s\t\t%5s\n",
               "Seed", "Team name","First","Second","Third","Fourth","Fifth","Sixth","Champ");
        TournamentRegion *region = regions[i];
        std::unordered_map<int, std::string> teamsBySeed = region->getTeamsBySeed();
        for (int j = 1; j < 17; j++){
            std::string teamName = teamsBySeed[j];
            printf("(%2d) %-30s\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\n",
                   j, teamName.c_str(),
                   pcts_all[teamName]->at(0)*100,pcts_all[teamName]->at(1)*100,
                   pcts_all[teamName]->at(2)*100,pcts_all[teamName]->at(3)*100,
                   pcts_all[teamName]->at(4)*100,pcts_all[teamName]->at(5)*100,
                   pcts_all[teamName]->at(6)*100);

            //possible play-in teams
            if (j == 11 || j == 12 || j == 16){
                if (teamsBySeed.find(-j) == teamsBySeed.end()) continue;
                teamName = teamsBySeed[-j];
                printf("(%2d) %-30s\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\t\t%5.1f\n",
                       j, teamName.c_str(),
                       pcts_all[teamName]->at(0)*100,pcts_all[teamName]->at(1)*100,
                       pcts_all[teamName]->at(2)*100,pcts_all[teamName]->at(3)*100,
                       pcts_all[teamName]->at(4)*100,pcts_all[teamName]->at(5)*100,
                       pcts_all[teamName]->at(6)*100);
            }
        }
    }




}
