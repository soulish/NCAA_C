#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TH1F.h"
#include "TLine.h"
#include <boost/algorithm/string.hpp>
#include <TROOT.h>
#include "src/Team.h"
#include "src/ConstantTeam5YearAverages.h"
#include "src/ConstantSeasonInfo.h"
#include "src/ConstantGameFunction.h"
#include "src/ConstantSRSadditions.h"
#include "src/ConstantTeamNeutralRatios.h"
#include "src/ConstantWAverageFunctions.h"
#include "src/ConstantStandardDeviations.h"

void printOptions();

int main(int argc,char *argv[]){
    int c;
    int year = 0;
    std::string teamNameA = "", teamNameB = "", outFileName = "";
    bool useHistogramsFile = false;
    std::string srsValue = "free";

    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"t:T:o:hHs:")) != -1){
        switch(c){
            case 't':
                teamNameA = optarg;
                break;
            case 'T':
                teamNameB = optarg;
                break;
            case 'o':
                outFileName = optarg;
                break;
            case 'H':
                useHistogramsFile = true;
                break;
            case 's':
                srsValue.assign(optarg);
                break;
            case 'h':
                printOptions();
                return 0;
            default:
                // not an option
                break;
        }
    }

    if (teamNameA == "" || teamNameB == "" || outFileName == ""){
        std::cout << "You must specify one team with the -t switch " << std::endl;
        std::cout << "and another team with the -T switch" << std::endl;
        std::cout << "and an outFileName with the -o switch" << std::endl;
        return 0;
    }

    year = stoi(teamNameA.substr(0,4));

    char *homePath, path[256], histName[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/team_5yr_averages.d", homePath);
    ConstantTeam5YearAverages *averages = ConstantTeam5YearAverages::Instance();
    averages->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo *seasonInfo = ConstantSeasonInfo::Instance();
    seasonInfo->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/waverage_functions.d", homePath);
    ConstantWAverageFunctions *functions = ConstantWAverageFunctions::Instance();
    functions->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/team_neutral_ratios.d", homePath);
    ConstantTeamNeutralRatios *ratios = ConstantTeamNeutralRatios::Instance();
    ratios->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/srs_additions.d", homePath);
    ConstantSRSadditions *additions = ConstantSRSadditions::Instance();
    additions->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/game_function_weights.%s.d", homePath,srsValue.c_str());
    ConstantGameFunction *gameFunction = ConstantGameFunction::Instance();
    gameFunction->initialize(path);

    sprintf(path, "%s/cpp/NCAA_C/constants/waverage_standard_deviations.d", homePath);
    ConstantStandardDeviations *stdDevs = ConstantStandardDeviations::Instance();
    stdDevs->initialize(path);

    //read in the averages and waverages for teamA
    Team *teamA = new Team(teamNameA);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.averages.d",
            homePath,teamA->getYear(),boost::replace_all_copy(teamNameA," ","_").c_str());
    teamA->addAverages(path);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.waverages.d",
            homePath,teamA->getYear(),boost::replace_all_copy(teamNameA," ","_").c_str());
    teamA->addWAverages(path);

    //read in the averages and waverages for teamA
    Team *teamB = new Team(teamNameB);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.averages.d",
            homePath,teamB->getYear(),boost::replace_all_copy(teamNameB," ","_").c_str());
    teamB->addAverages(path);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.waverages.d",
            homePath,teamB->getYear(),boost::replace_all_copy(teamNameB," ","_").c_str());
    teamB->addWAverages(path);

    TFile *histsFile;
    std::unordered_map<int, TH1F*> probs_by_year, probs_err_by_year;
    if (useHistogramsFile){
        sprintf(path, "%s/cpp/NCAA_C/rootFiles/gameFunctionHistograms.%s.root", homePath, srsValue.c_str());
        histsFile = new TFile(path);

        for (int y = 2007; y <= 2016; y++){
            sprintf(path, "probs_by_year_%d", y);
            probs_by_year.emplace(y, (TH1F*)gROOT->FindObject(path));
            sprintf(path, "probs_err_by_year_%d", y);
            probs_err_by_year.emplace(y, (TH1F*)gROOT->FindObject(path));
        }
    }

    //create the output ROOT file
    sprintf(path, "%s/cpp/NCAA_C/rootFiles/%s",homePath,outFileName.c_str());
    TFile outFile(path,"recreate");

    //create and fill the containers for the averages and waverages
    std::unordered_map<std::string, TH1F*> avgsA_per_time, wavgsA_per_time;
    std::unordered_map<std::string, TH1F*> avgsB_per_time, wavgsB_per_time;
    std::unordered_map<std::string, TLine*> lines;
    std::string stats[] = {"oor.p", "oefg.p", "oto.p", "oftmr.p", "srs",
                           "dor.p", "defg.p", "dto.p", "dftmr.p", "rpi"};
    for (std::string &s : stats){
        sprintf(histName, "avgA_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        avgsA_per_time.emplace(s, new TH1F(histName,s.c_str(),150,0,150));
        sprintf(histName, "wavgA_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        wavgsA_per_time.emplace(s, new TH1F(histName,s.c_str(),150,0,150));

        sprintf(histName, "avgB_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        avgsB_per_time.emplace(s, new TH1F(histName,s.c_str(),150,0,150));
        sprintf(histName, "wavgB_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        wavgsB_per_time.emplace(s, new TH1F(histName,s.c_str(),150,0,150));
    }
    if (useHistogramsFile) {
        avgsA_per_time.emplace("gameScore", new TH1F("avgA_per_time_gameScore", "gameScore", 150,0,150));
        wavgsA_per_time.emplace("gameScore", new TH1F("wavgA_per_time_gameScore", "gameScore", 150,0,150));

        avgsB_per_time.emplace("gameScore", new TH1F("avgB_per_time_gameScore", "gameScore", 150,0,150));
        wavgsB_per_time.emplace("gameScore", new TH1F("wavgB_per_time_gameScore", "gameScore", 150,0,150));
    }

    //create the lines that hold the league averages for each stat
    lines.emplace("oor.p", new TLine(0,averages->get(year,"oor.p"),150,
                                     averages->get(year,"oor.p")));
    lines["oor.p"]->Write("oorpLine");
    lines.emplace("dor.p", new TLine(0,averages->get(year,"oor.p"),150,
                                     averages->get(year,"oor.p")));
    lines["dor.p"]->Write("dorpLine");
    lines.emplace("oefg.p", new TLine(0,averages->get(year,"oefg.p"),150,
                                      averages->get(year,"oefg.p")));
    lines["oefg.p"]->Write("oefgpLine");
    lines.emplace("defg.p", new TLine(0,averages->get(year,"oefg.p"),150,
                                      averages->get(year,"oefg.p")));
    lines["defg.p"]->Write("defgpLine");
    lines.emplace("oftmr.p", new TLine(0,averages->get(year,"oftmr.p"),150,
                                       averages->get(year,"oftmr.p")));
    lines["oftmr.p"]->Write("oftmrpLine");
    lines.emplace("dftmr.p", new TLine(0,averages->get(year,"oftmr.p"),150,
                                       averages->get(year,"oftmr.p")));
    lines["dftmr.p"]->Write("dftmrpLine");
    lines.emplace("oto.p", new TLine(0,averages->get(year,"oto.p"),150,
                                     averages->get(year,"oto.p")));
    lines["oto.p"]->Write("otopLine");
    lines.emplace("dto.p", new TLine(0,averages->get(year,"oto.p"),150,
                                     averages->get(year,"oto.p")));
    lines["dto.p"]->Write("dtopLine");
    lines.emplace("srs", new TLine(0,0,150,0));
    lines["srs"]->Write("srsLine");
    lines.emplace("rpi", new TLine(0,0.5,150,0.5));
    lines["rpi"]->Write("rpiLine");
    lines.emplace("gameScoreLine", new TLine(0,0.5,150,0.5));
    lines["gameScoreLine"]->Write("gameScoreLine");

    //bring the averages here locally
    std::unordered_map<std::string, TeamAverage *> averageHashA = teamA->getAveragesByDate();
    std::unordered_map<std::string, TeamAverage *> averageHashB = teamB->getAveragesByDate();

    //some placeholders
    TeamAverage *last_average;
    TeamWAverage *waverage, *last_waverage;

    //some calendar tools
    boost::gregorian::date_duration dateDuration(1);
    boost::gregorian::days indDuration;

    //cycle through the averages hash for teamA
    for (auto &average : averageHashA){
        last_average = teamA->AverageOnDate(average.second->getDate() - dateDuration);
        if (!last_average) continue; //skip the first date
        waverage = teamA->WAverageOnDate(average.second->getDate());
        last_waverage = teamA->WAverageOnDate(average.second->getDate() - dateDuration);

        int last_num_games = last_average->getNum_games();
        indDuration = average.second->getDate() - seasonInfo->get(teamA->getYear(),"season start");
        int ind = (int)indDuration.days();

        //cycle through the strings
        for (std::string &s : stats) {
            if (boost::contains(s, ".p") == 1){
                avgsA_per_time[s]->SetBinContent(ind,average.second->getValue(s));
                wavgsA_per_time[s]->SetBinContent(ind,waverage->getValue(s));
            }
            else if (boost::contains(s, ".m") == 1){
                //makes have to take into account the fact that I store the total number of makes
                //(or total number of weighted attempts) for each day, not the average.
                avgsA_per_time[s]->SetBinContent(ind,average.second->getValue(s) / (double)average.second->getNum_games());
                wavgsA_per_time[s]->SetBinContent(ind,waverage->getValue(s) / (double)waverage->getNum_games());
            }
            else if (s == "srs") {
                avgsA_per_time[s]->SetBinContent(ind,waverage->getOrigSRS());
                wavgsA_per_time[s]->SetBinContent(ind,waverage->getSrs());
            }
            else if (s == "rpi") {
                wavgsA_per_time[s]->SetBinContent(ind,waverage->getRpi());
            }
        }
        if (useHistogramsFile) {
            double gameScore = gameFunction->predictGame(waverage, teamA->getYear(), "neutral", "neutral");
            double gameScorePct, gameScorePct_err;
            if (gameScore >= 3) {
                gameScorePct = 1;
                gameScorePct_err = 1;
            }
            else if (gameScore <= -3) {
                gameScorePct = 0;
                gameScorePct_err = 1;
            }
            else {
                gameScorePct = probs_by_year[teamA->getYear()]->GetBinContent((gameScore + 3) * 1600 / 6.0);
                gameScorePct_err = probs_err_by_year[teamA->getYear()]->
                        GetBinContent((gameScore + 3) * 1600 / 6.0);
            }
            avgsA_per_time["gameScore"]->SetBinContent(ind, gameScorePct);
            avgsA_per_time["gameScore"]->SetBinError(ind, gameScorePct_err);
            wavgsA_per_time["gameScore"]->SetBinContent(ind, gameScorePct);
            wavgsA_per_time["gameScore"]->SetBinError(ind, gameScorePct_err);
        }
    }

    //cycle through the averages hash for teamB
    for (auto &average : averageHashB){
        last_average = teamB->AverageOnDate(average.second->getDate() - dateDuration);
        if (!last_average) continue; //skip the first date
        waverage = teamB->WAverageOnDate(average.second->getDate());
        last_waverage = teamB->WAverageOnDate(average.second->getDate() - dateDuration);

        int last_num_games = last_average->getNum_games();
        indDuration = average.second->getDate() - seasonInfo->get(teamB->getYear(),"season start");
        int ind = (int)indDuration.days();

        //cycle through the strings
        for (std::string &s : stats) {
            if (boost::contains(s, ".p") == 1){
                avgsB_per_time[s]->SetBinContent(ind,average.second->getValue(s));
                wavgsB_per_time[s]->SetBinContent(ind,waverage->getValue(s));
            }
            else if (boost::contains(s, ".m") == 1){
                //makes have to take into account the fact that I store the total number of makes
                //(or total number of weighted attempts) for each day, not the average.
                avgsB_per_time[s]->SetBinContent(ind,average.second->getValue(s) / (double)average.second->getNum_games());
                wavgsB_per_time[s]->SetBinContent(ind,waverage->getValue(s) / (double)waverage->getNum_games());
            }
            else if (s == "srs") {
                avgsB_per_time[s]->SetBinContent(ind,waverage->getOrigSRS());
                wavgsB_per_time[s]->SetBinContent(ind,waverage->getSrs());
            }
            else if (s == "rpi") {
                wavgsB_per_time[s]->SetBinContent(ind,waverage->getRpi());
            }
        }
        if (useHistogramsFile) {
            double gameScore = gameFunction->predictGame(waverage, teamB->getYear(), "neutral", "neutral");
            double gameScorePct, gameScorePct_err;
            if (gameScore >= 3) {
                gameScorePct = 1;
                gameScorePct_err = 1;
            }
            else if (gameScore <= -3) {
                gameScorePct = 0;
                gameScorePct_err = 1;
            }
            else {
                gameScorePct = probs_by_year[teamB->getYear()]->GetBinContent((gameScore + 3) * 1600 / 6.0);
                gameScorePct_err = probs_err_by_year[teamB->getYear()]->
                        GetBinContent((gameScore + 3) * 1600 / 6.0);
            }
            avgsB_per_time["gameScore"]->SetBinContent(ind, gameScorePct);
            avgsB_per_time["gameScore"]->SetBinError(ind, gameScorePct_err);
            wavgsB_per_time["gameScore"]->SetBinContent(ind, gameScorePct);
            wavgsB_per_time["gameScore"]->SetBinError(ind, gameScorePct_err);
        }
    }

    TeamWAverage *waA= teamA->WAverageOnDate(seasonInfo->get(teamA->getYear(),"tournament start"));
    TeamWAverage *waB= teamB->WAverageOnDate(seasonInfo->get(teamB->getYear(),"tournament start"));

    std::unordered_map<std::string, double> predictionsA = functions->predictStats(waA, waB, teamA->getYear());
    std::unordered_map<std::string, double> predictionsB = functions->predictStats(waB, waA, teamB->getYear());

    wavgsA_per_time["oor.p"]->SetBinContent(140,predictionsA["oor.p"]*waA->getValue("oor.p"));
    wavgsB_per_time["oor.p"]->SetBinContent(140,predictionsB["oor.p"]*waB->getValue("oor.p"));
    wavgsA_per_time["oefg.p"]->SetBinContent(140,predictionsA["oefg.p"]*waA->getValue("oefg.p"));
    wavgsB_per_time["oefg.p"]->SetBinContent(140,predictionsB["oefg.p"]*waB->getValue("oefg.p"));
    wavgsA_per_time["oftmr.p"]->SetBinContent(140,predictionsA["oftmr.p"]*waA->getValue("oftmr.p"));
    wavgsB_per_time["oftmr.p"]->SetBinContent(140,predictionsB["oftmr.p"]*waB->getValue("oftmr.p"));
    wavgsA_per_time["oto.p"]->SetBinContent(140,predictionsA["oto.p"]*waA->getValue("oto.p"));
    wavgsB_per_time["oto.p"]->SetBinContent(140,predictionsB["oto.p"]*waB->getValue("oto.p"));

    double gameScoreA = gameFunction->predictGame(waA,waB,teamA->getYear(),"neutral","neutral");
    double gameScoreB = gameFunction->predictGame(waB,waA,teamB->getYear(),"neutral","neutral");

    double gamePctA = probs_by_year[teamA->getYear()]->GetBinContent((gameScoreA + 3) * 1600 / 6.0);
    double gamePctB = probs_by_year[teamB->getYear()]->GetBinContent((gameScoreB + 3) * 1600 / 6.0);
    double gamePctA_err = probs_err_by_year[teamA->getYear()]->GetBinContent((gameScoreA + 3) * 1600 / 6.0);
    double gamePctB_err = probs_err_by_year[teamB->getYear()]->GetBinContent((gameScoreB + 3) * 1600 / 6.0);

    wavgsA_per_time["gameScore"]->SetBinContent(140,gamePctA);
    wavgsB_per_time["gameScore"]->SetBinContent(140,gamePctB);
    wavgsA_per_time["gameScore"]->SetBinError(140,gamePctA_err);
    wavgsB_per_time["gameScore"]->SetBinError(140,gamePctB_err);

    outFile.Write();
    outFile.Close();

    return 0;
}

void printOptions(){
    std::cout << std::endl;
    std::cout << "showTournamentMatchup Usage options" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "\t-t (string) teamnameA (no default)[Required]" << std::endl;
    std::cout << "\t-T (string) teamnameB (no default)[Required]" << std::endl;
    std::cout << "\t-o (string) outFileName (no default)[Required]" << std::endl;
    std::cout << "\t-s (double) srs value to use for weights and histograms (default: \tfree\")[Optional]" << std::endl;
    std::cout << "\t-H calculate game score using histograms file (no default)[Optional]" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Ex: showTournamentMatchup -t \"2015 north carolina\" -T \"2015 harvard\" -o \"temp.root\" -H -s 0.5" << std::endl;
    std::cout << std::endl;
    std::cout << "This program creates histograms for each of the four factors, offense and" << std::endl;
    std::cout << "defense, over the course of the season for two teams.  It creates histograms for both" << std::endl;
    std::cout << "the regular averages as well as the weighted averages.  It also plots" << std::endl;
    std::cout << "SRS and gameScore or RPI.  Lastly it calculates the predictions for both teams'" << std::endl;
    std::cout << "four factors, and likelihood of winning.  You view the resultant ROOT histograms" << std::endl;
    std::cout << "using the showTournamentMatch.rb Ruby program.  All you have to specify is the" << std::endl;
    std::cout << "teams' names, and the name of the output file.  You can also decide to " << std::endl;
    std::cout << "plot the game score by selecting the -H option, and specifying an" << std::endl;
    std::cout << "SRS value for the weights if necessary." << std::endl;
    std::cout << std::endl;
}