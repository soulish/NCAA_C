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
          pcts_all(),
          challengePoints(0) { }

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
                    printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s\t% .3f\t%.3f\n",
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
                    printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
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

            //try to find the game played by this team on this date (either two or three days after
            //the start of the tournament). If you find it that means that the team played in a game
            //and if they won we get credit, no matter who they played.
            TeamGame *x;
            x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(2));
            if (!x)
                x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                      boost::gregorian::date_duration(3));
            if (x && x->getWin() == 1){
                //there is a bonus in this round for correctly selecting an upset
//                if (teams[teamsBySeed[matchups[i]->at(0)]] >= 10) challengePoints += 2;
//                else challengePoints += 1;
                challengePoints += 10;
            }
        }
        else if (teamsBySeed[matchups[i]->at(1)] == chosenTeam ||
                 (teamsBySeed[matchups[i]->at(0)] != chosenTeam && gameScore < 0)){
            firstRoundWinners.push_back(teamsBySeed[matchups[i]->at(1)]);

            TeamGame *x;
            x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(2));
            if (!x)
                x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                      boost::gregorian::date_duration(3));
            if (x && x->getWin() == 1){
                //there is a bonus in this round for correctly selecting an upset
//                if (teams[teamsBySeed[matchups[i]->at(0)]] >= 10) challengePoints += 2;
//                else challengePoints += 1;
                challengePoints += 10;
            }
        }
        pcts[teamsBySeed[matchups[i]->at(0)]] *= gamePct;
        pcts[teamsBySeed[matchups[i]->at(1)]] *= (1 - gamePct);

        if (verbose){
            printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
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

            TeamGame *x;
            x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(4));
            if (!x)
                x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                      boost::gregorian::date_duration(5));
            if (x && x->getWin() == 1)
                challengePoints += 20;
        }
        else if (firstRoundWinners[matchups[i]->at(1)] == chosenTeam ||
                 (firstRoundWinners[matchups[i]->at(0)] != chosenTeam && gameScore < 0)){
            secondRoundWinners.push_back(firstRoundWinners[matchups[i]->at(1)]);

            TeamGame *x;
            x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(4));
            if (!x)
                x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                      boost::gregorian::date_duration(5));
            if (x && x->getWin() == 1)
                challengePoints += 20;
        }
        pcts[firstRoundWinners[matchups[i]->at(0)]] *= gamePct;
        pcts[firstRoundWinners[matchups[i]->at(1)]] *= (1 - gamePct);

        if (verbose){
            printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
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

            TeamGame *x;
            x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(9));
            if (!x)
                x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                      boost::gregorian::date_duration(10));
            if (x && x->getWin() == 1)
                challengePoints += 40;
        }
        else if (secondRoundWinners[matchups[i]->at(1)] == chosenTeam ||
                 (secondRoundWinners[matchups[i]->at(0)] != chosenTeam && gameScore < 0)){
            thirdRoundWinners.push_back(secondRoundWinners[matchups[i]->at(1)]);

            TeamGame *x;
            x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(9));
            if (!x)
                x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                      boost::gregorian::date_duration(10));
            if (x && x->getWin() == 1)
                challengePoints += 40;
        }
        pcts[secondRoundWinners[matchups[i]->at(0)]] *= gamePct;
        pcts[secondRoundWinners[matchups[i]->at(1)]] *= (1 - gamePct);

        if (verbose){
            printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
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

        TeamGame *x;
        x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(11));
        if (!x)
            x = teamA->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(12));
        if (x && x->getWin() == 1)
            challengePoints += 80;
    }
    else if (thirdRoundWinners[1] == chosenTeam ||
             (thirdRoundWinners[0] != chosenTeam && gameScore < 0)){
        regionChamp = thirdRoundWinners[1];

        TeamGame *x;
        x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                              boost::gregorian::date_duration(11));
        if (!x)
            x = teamB->GameOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start") +
                                  boost::gregorian::date_duration(12));
        if (x && x->getWin() == 1)
            challengePoints += 80;
    }
    pcts[thirdRoundWinners[0]] *= gamePct;
    pcts[thirdRoundWinners[1]] *= (1 - gamePct);

    if (verbose){
        printf("%-15s (%2d) %-30s vs (%2d) %-30s  =>  (%2d) %-30s % .3f\t%.3f\n",
               "Elite Eight:",teams[thirdRoundWinners[0]], thirdRoundWinners[0].c_str(),
               teams[thirdRoundWinners[1]], thirdRoundWinners[1].c_str(),
               teams[regionChamp], regionChamp.c_str(),
               gameScore, gameScore >= 0 ? gamePct : 1 - gamePct);
        std::cout << "Challenge Points: " << challengePoints << std::endl;
    }
}

void TournamentRegion::reset() {
    pcts_all.clear();
    challengePoints = 0;
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

//  #This methods are used to find the likelihood for each team to reach each round of the tournament
//  #It is called by playTotalPercentages in Tournament.  It figures out that percentage by taking
//  #the weighted average of the possibility they could beat the team they are playing, weighted by the probability
//  #that they would reach that game.  For the first round, this is mostly academic (except for games involving play-in
//  #game winners), but for other rounds, it involves playing each possible opponent, and weighing the probabilities.
//  #
//  #Note: I could have set this up so that each game is only played once (i.e. only play A vs B, skipping B vs A),
//  #but this already runs extrememly quickly, so it's not really necessary.
//  #
//  #Note: In each round, after the percentages have been calculated, they are normalized.  This is done
//  #because pct_A != 1 - pct_B exactly for each game.  They are very close, but generally not perfect.
//  #so this fixes that.  generally the correction is very small.
//  #
//  #
//  #In each method, opps lists all the possible seeds that each seed could play in that round.  So the second seed
//  #must face the number 15 seed in round 1, then the 7 or 10 seed in round 2, then the 6, 11, 3 or 14 seeds in the
//  #third round, etc.  Seeds that involve play-in games (11, 12, 16) include both +/- versions of their seed
//  #because I call the loser of the play-in game -seed for convenience.
void TournamentRegion::firstRoundTotalPercentages(TH1F *hist) {
    std::unordered_map<int, std::string> teamsBySeed;
    for (auto &t : teams)
        teamsBySeed.emplace(t.second,t.first);

    //this array maps the seed onto its possible opponents, i.e. 1 plays 16, 2 plays 15, etc.
    //the few entries with negative values are due to possible play-in games
    std::array<std::vector<int>*,17> opps;
    for (int i = 0; i < 17; i++)
        opps[i] = new std::vector<int>();
    opps[0]->push_back(0);
    opps[1]->push_back(16);    opps[1]->push_back(-16);
    opps[2]->push_back(15);
    opps[3]->push_back(14);
    opps[4]->push_back(13);
    opps[5]->push_back(12);    opps[5]->push_back(-12);
    opps[6]->push_back(11);    opps[6]->push_back(-11);
    opps[7]->push_back(10);
    opps[8]->push_back(9);
    opps[9]->push_back(8);
    opps[10]->push_back(7);
    opps[11]->push_back(6);
    opps[12]->push_back(5);
    opps[13]->push_back(4);
    opps[14]->push_back(3);
    opps[15]->push_back(2);
    opps[16]->push_back(1);

    double s = 0;

    for (int &seed : seeds){
        std::vector<int> *oppSeeds = opps[abs(seed)];
        for (int &oppSeed : *oppSeeds){
            if (std::find(std::begin(seeds), std::end(seeds), oppSeed) == seeds.end()) continue;

            Team *teamA = Team::findTeam(teamsBySeed[seed]);
            Team *teamB = Team::findTeam(teamsBySeed[oppSeed]);

            TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
            TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

            double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
            double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

            pcts_all[teamsBySeed[seed]]->at(1) += pcts_all[teamsBySeed[seed]]->at(0) *
                                                  pcts_all[teamsBySeed[oppSeed]]->at(0) * gamePct;
            s += pcts_all[teamsBySeed[seed]]->at(0) * pcts_all[teamsBySeed[oppSeed]]->at(0) * gamePct;
        }
    }

    //We renormalize here because it sometimes happens that, due to rounding issues, the sum is not
    //exactly 1 in each game.  Here we multiply each pct by 8 / s, because the pcts should add up to
    //1 for each game, and there are 8 games total.
    double sum = 0; //this variable is really just a check.  Output if you want to ensure normalization
    for (int &seed : seeds) {
        pcts_all[teamsBySeed[seed]]->at(1) = pcts_all[teamsBySeed[seed]]->at(1) * 8 / s;
        sum += pcts_all[teamsBySeed[seed]]->at(1);
    }
}

void TournamentRegion::secondRoundTotalPercentages(TH1F *hist) {
    std::unordered_map<int, std::string> teamsBySeed;
    for (auto &t : teams)
        teamsBySeed.emplace(t.second,t.first);

    //this array maps the seed onto its possible opponents in the second round,
    //i.e. 1 can play 8 or 9, 2 can play 7 or 10.
    //the few entries with negative values are due to possible play-in games
    std::array<std::vector<int>*,17> opps;
    for (int i = 0; i < 17; i++)
        opps[i] = new std::vector<int>();
    opps[0]->push_back(0);
    opps[1]->push_back(8);     opps[1]->push_back(9);
    opps[2]->push_back(7);     opps[2]->push_back(10);
    opps[3]->push_back(6);     opps[3]->push_back(11);     opps[3]->push_back(-11);
    opps[4]->push_back(5);     opps[4]->push_back(12);     opps[4]->push_back(-12);
    opps[5]->push_back(4);     opps[5]->push_back(13);
    opps[6]->push_back(3);     opps[6]->push_back(14);
    opps[7]->push_back(2);     opps[7]->push_back(15);
    opps[8]->push_back(1);     opps[8]->push_back(16);     opps[8]->push_back(-16);
    opps[9] = opps[8];//->push_back(1);     opps[9]->push_back(16);     opps[9]->push_back(-16);
    opps[10] = opps[7];//->push_back(2);    opps[10]->push_back(15);
    opps[11] = opps[6];//->push_back(3);    opps[11]->push_back(14);
    opps[12] = opps[5];//->push_back(4);    opps[12]->push_back(13);
    opps[13] = opps[4];//->push_back(5);    opps[13]->push_back(12);    opps[13]->push_back(-12);
    opps[14] = opps[3];//->push_back(6);    opps[14]->push_back(11);    opps[14]->push_back(-11);
    opps[15] = opps[2];//->push_back(7);    opps[15]->push_back(10);
    opps[16] = opps[1];//->push_back(8);    opps[16]->push_back(9);

    double s = 0;

    for (int &seed : seeds){
        std::vector<int> *oppSeeds = opps[abs(seed)];
        for (int &oppSeed : *oppSeeds){
            if (std::find(std::begin(seeds), std::end(seeds), oppSeed) == seeds.end()) continue;

            Team *teamA = Team::findTeam(teamsBySeed[seed]);
            Team *teamB = Team::findTeam(teamsBySeed[oppSeed]);

            TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
            TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

            double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
            double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

            pcts_all[teamsBySeed[seed]]->at(2) += pcts_all[teamsBySeed[seed]]->at(1) *
                                                  pcts_all[teamsBySeed[oppSeed]]->at(1) * gamePct;
            s += pcts_all[teamsBySeed[seed]]->at(1) * pcts_all[teamsBySeed[oppSeed]]->at(1) * gamePct;
        }
    }

    //We renormalize here because it sometimes happens that, due to rounding issues, the sum is not
    //exactly 1 in each game.  Here we multiply each pct by 4 / s, because the pcts should add up to
    //1 for each game, and there are 4 games total in this round.
    double sum = 0; //this variable is really just a check.  Output if you want to ensure normalization
    for (int &seed : seeds) {
        pcts_all[teamsBySeed[seed]]->at(2) = pcts_all[teamsBySeed[seed]]->at(2) * 4/s;
        sum += pcts_all[teamsBySeed[seed]]->at(2);
    }
}

void TournamentRegion::thirdRoundTotalPercentages(TH1F *hist) {
    std::unordered_map<int, std::string> teamsBySeed;
    for (auto &t : teams)
        teamsBySeed.emplace(t.second,t.first);

    //this array maps the seed onto its possible opponents in the third round,
    //i.e. 1 can play 5, 12 (either of the play-in teams at 12), 4, or 13.
    //the few entries with negative values are due to possible play-in games
    std::array<std::vector<int>*,17> opps;
    for (int i = 0; i < 17; i++)
        opps[i] = new std::vector<int>();
    opps[0]->push_back(0);
    opps[1]->push_back(5);     opps[1]->push_back(12);      opps[1]->push_back(-12);
    opps[1]->push_back(4);     opps[1]->push_back(13);
    opps[2]->push_back(6);     opps[2]->push_back(11);     opps[2]->push_back(-11);
    opps[2]->push_back(3);     opps[2]->push_back(14);
    opps[3]->push_back(7);     opps[3]->push_back(10);     opps[3]->push_back(2);     opps[3]->push_back(15);
    opps[4]->push_back(1);     opps[4]->push_back(16);     opps[4]->push_back(-16);
    opps[4]->push_back(8);     opps[4]->push_back(9);
    opps[5] = opps[4];
    opps[6] = opps[3];
    opps[7] = opps[2];
    opps[8] = opps[1];
    opps[9] = opps[1];
    opps[10] = opps[2];
    opps[11] = opps[3];
    opps[12] = opps[4];
    opps[13] = opps[4];
    opps[14] = opps[3];
    opps[15] = opps[2];
    opps[16] = opps[1];

    double s = 0;

    for (int &seed : seeds){
        std::vector<int> *oppSeeds = opps[abs(seed)];
        for (int &oppSeed : *oppSeeds){
            if (std::find(std::begin(seeds), std::end(seeds), oppSeed) == seeds.end()) continue;

            Team *teamA = Team::findTeam(teamsBySeed[seed]);
            Team *teamB = Team::findTeam(teamsBySeed[oppSeed]);

            TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
            TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

            double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
            double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

            pcts_all[teamsBySeed[seed]]->at(3) += pcts_all[teamsBySeed[seed]]->at(2) *
                                                  pcts_all[teamsBySeed[oppSeed]]->at(2) * gamePct;
            s += pcts_all[teamsBySeed[seed]]->at(2) * pcts_all[teamsBySeed[oppSeed]]->at(2) * gamePct;
        }
    }

    //We renormalize here because it sometimes happens that, due to rounding issues, the sum is not
    //exactly 1 in each game.  Here we multiply each pct by 2 / s, because the pcts should add up to
    //1 for each game, and there are 2 games total in this round.
    double sum = 0; //this variable is really just a check.  Output if you want to ensure normalization
    for (int &seed : seeds) {
        pcts_all[teamsBySeed[seed]]->at(3) = pcts_all[teamsBySeed[seed]]->at(3) * 2/s;
        sum += pcts_all[teamsBySeed[seed]]->at(3);
    }
}

void TournamentRegion::fourthRoundTotalPercentages(TH1F *hist) {
    std::unordered_map<int, std::string> teamsBySeed;
    for (auto &t : teams)
        teamsBySeed.emplace(t.second,t.first);

    //this array maps the seed onto its possible opponents in the fourth round,
    //e.g. 1 can play 2, 3, 6, 7, 10, 11, (or the other 11), 14, or 15.
    //the few entries with negative values are due to possible play-in games
    std::array<std::vector<int>*,17> opps;
    for (int i = 0; i < 17; i++)
        opps[i] = new std::vector<int>();
    opps[0]->push_back(0);
    opps[1]->push_back(2);     opps[1]->push_back(3);      opps[1]->push_back(6);
    opps[1]->push_back(7);     opps[1]->push_back(10);     opps[1]->push_back(11);
    opps[1]->push_back(-11);   opps[1]->push_back(14);     opps[1]->push_back(15);

    opps[2]->push_back(1);     opps[2]->push_back(4);      opps[2]->push_back(5);
    opps[2]->push_back(8);     opps[2]->push_back(9);;     opps[2]->push_back(12);
    opps[2]->push_back(-12);   opps[2]->push_back(13);
    opps[2]->push_back(16);    opps[2]->push_back(-16);

    opps[3] = opps[2];
    opps[4] = opps[1];
    opps[5] = opps[4];
    opps[6] = opps[2];
    opps[7] = opps[2];
    opps[8] = opps[1];
    opps[9] = opps[1];
    opps[10] = opps[2];
    opps[11] = opps[2];
    opps[12] = opps[1];
    opps[13] = opps[1];
    opps[14] = opps[2];
    opps[15] = opps[2];
    opps[16] = opps[1];

    double s = 0;

    for (int &seed : seeds){
        std::vector<int> *oppSeeds = opps[abs(seed)];
        for (int &oppSeed : *oppSeeds){
            if (std::find(std::begin(seeds), std::end(seeds), oppSeed) == seeds.end()) continue;

            Team *teamA = Team::findTeam(teamsBySeed[seed]);
            Team *teamB = Team::findTeam(teamsBySeed[oppSeed]);

            TeamWAverage *waA = teamA->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));
            TeamWAverage *waB = teamB->WAverageOnDate(ConstantSeasonInfo::Instance()->get(year, "tournament start"));

            double gameScore = ConstantGameFunction::Instance()->predictGame(waA,waB,year,"neutral","neutral");
            double gamePct = hist->GetBinContent((gameScore+3)*1600/6.0);

            pcts_all[teamsBySeed[seed]]->at(4) += pcts_all[teamsBySeed[seed]]->at(3) *
                                                  pcts_all[teamsBySeed[oppSeed]]->at(3) * gamePct;
            s += pcts_all[teamsBySeed[seed]]->at(3) * pcts_all[teamsBySeed[oppSeed]]->at(3) * gamePct;
        }
    }

    //We renormalize here because it sometimes happens that, due to rounding issues, the sum is not
    //exactly 1 in each game.  Here we multiply each pct by 1 / s, because the pcts should add up to
    //1 for each game, and there is only 1 game in this round
    double sum = 0; //this variable is really just a check.  Output if you want to ensure normalization
    for (int &seed : seeds) {
        pcts_all[teamsBySeed[seed]]->at(4) = pcts_all[teamsBySeed[seed]]->at(4) * 1/s;
        sum += pcts_all[teamsBySeed[seed]]->at(4);
    }
}

std::unordered_map<int, std::string> TournamentRegion::getTeamsBySeed() {
    std::unordered_map<int, std::string> teamsBySeed;
    for (auto &t : teams)
        teamsBySeed.emplace(t.second,t.first);

    return teamsBySeed;
}
