#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TH1F.h"
#include "TLine.h"
#include <boost/algorithm/string.hpp>
#include "src/Team.h"
#include "src/ConstantTeam5YearAverages.h"
#include "src/ConstantSeasonInfo.h"

int main(int argc,char *argv[]){
    int c;
    int year = 0;
    std::string teamName = "", outFileName = "";

    /*____________________________Parse Command Line___________________________*/
    while((c = getopt(argc,argv,"t:o:")) != -1){
        switch(c){
            case 't':
                teamName = optarg;
                break;
            case 'o':
                outFileName = optarg;
                break;
            default:
                // not an option
                break;
        }
    }

    if (teamName == "" || outFileName == ""){
        std::cout << "You must specify a team with the -t switch and an outFileName with the -o switch" << std::endl;
        return 0;
    }

    year = stoi(teamName.substr(0,4));

    char *homePath, path[256], histName[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/team_5yr_averages.d", homePath);
    ConstantTeam5YearAverages::Instance()->initialize(path);
    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo::Instance()->initialize(path);

    //read in the averages and waverages for this one team
    Team *team = new Team(teamName);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.averages.d",
            homePath,team->getYear(),boost::replace_all_copy(teamName," ","_").c_str());
    team->addAverages(path);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.waverages.d",
            homePath,team->getYear(),boost::replace_all_copy(teamName," ","_").c_str());
    team->addWAverages(path);

    //create the output ROOT file
    sprintf(path, "%s/cpp/NCAA_C/rootFiles/%s",homePath,outFileName.c_str());
    TFile outFile(path,"recreate");

    //create and fill the containers for the averages and waverages
    std::unordered_map<std::string, TH1F*> avgs_per_time, wavgs_per_time, avgs_per_game, wavgs_per_game;
    std::unordered_map<std::string, TLine*> lines;
    std::string stats[] = {"oor.p", "oefg.p", "oto.p", "oftmr.p", "srs",
                           "dor.p", "defg.p", "dto.p", "dftmr.p", "rpi"};
    for (std::string &s : stats){
        sprintf(histName, "avg_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        avgs_per_time.emplace(s, new TH1F(histName,s.c_str(),140,0,140));
        sprintf(histName, "wavg_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        wavgs_per_time.emplace(s, new TH1F(histName,s.c_str(),140,0,140));

        sprintf(histName, "avg_per_game_%s",(boost::replace_all_copy(s,".","").c_str()));
        avgs_per_game.emplace(s, new TH1F(histName,s.c_str(),140,0,140));
        sprintf(histName, "wavg_per_game_%s",(boost::replace_all_copy(s,".","").c_str()));
        wavgs_per_game.emplace(s, new TH1F(histName,s.c_str(),140,0,140));
    }

    //create the lines that hold the league averages for each stat
    lines.emplace("oor.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oor.p"),140,
                                     ConstantTeam5YearAverages::Instance()->get(year,"oor.p")));
    lines["oor.p"]->Write("oorpLine");
    lines.emplace("dor.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oor.p"),140,
                                     ConstantTeam5YearAverages::Instance()->get(year,"oor.p")));
    lines["dor.p"]->Write("dorpLine");
    lines.emplace("oefg.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oefg.p"),140,
                                      ConstantTeam5YearAverages::Instance()->get(year,"oefg.p")));
    lines["oefg.p"]->Write("oefgpLine");
    lines.emplace("defg.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oefg.p"),140,
                                      ConstantTeam5YearAverages::Instance()->get(year,"oefg.p")));
    lines["defg.p"]->Write("defgpLine");
    lines.emplace("oftmr.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oftmr.p"),140,
                                       ConstantTeam5YearAverages::Instance()->get(year,"oftmr.p")));
    lines["oftmr.p"]->Write("oftmrpLine");
    lines.emplace("dftmr.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oftmr.p"),140,
                                       ConstantTeam5YearAverages::Instance()->get(year,"oftmr.p")));
    lines["dftmr.p"]->Write("dftmrpLine");
    lines.emplace("oto.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oto.p"),140,
                                     ConstantTeam5YearAverages::Instance()->get(year,"oto.p")));
    lines["oto.p"]->Write("otopLine");
    lines.emplace("dto.p", new TLine(0,ConstantTeam5YearAverages::Instance()->get(year,"oto.p"),140,
                                     ConstantTeam5YearAverages::Instance()->get(year,"oto.p")));
    lines["dto.p"]->Write("dtopLine");
    lines.emplace("srs", new TLine(0,0,140,0));
    lines["srs"]->Write("srsLine");
    lines.emplace("rpi", new TLine(0,0.5,140,0.5));
    lines["rpi"]->Write("rpiLine");

    //bring the averages here locally
    std::unordered_map<std::string, TeamAverage *> averageHash = team->getAveragesByDate();

    //some placeholders
    TeamAverage *last_average;
    TeamWAverage *waverage, *last_waverage;

    //some calendar tools
    boost::gregorian::date_duration dateDuration(1);
    boost::gregorian::days indDuration;

    //cycle through the averages hash
    for (auto &average : averageHash){
        last_average = team->AverageOnDate(average.second->getDate() - dateDuration);
        if (!last_average) continue; //skip the first date
        waverage = team->WAverageOnDate(average.second->getDate());
        last_waverage = team->WAverageOnDate(average.second->getDate() - dateDuration);

        int last_num_games = last_average->getNum_games();
        indDuration = average.second->getDate() -
                      ConstantSeasonInfo::Instance()->get(team->getYear(),"season start");
        int ind = (int)indDuration.days();

        //cycle through the strings
        for (std::string &s : stats) {
            if (boost::contains(s, ".p") == 1){
                //only do this on days where there was a game.
                if (average.second->getNum_games() != last_num_games) {
                    //to get the percentage for a game, we take the difference between
                    //the makes and attempts today and the day before, and then taking the
                    //retio.
                    double m = average.second->getValue(boost::replace_first_copy(s, ".p", ".m")) -
                               last_average->getValue(boost::replace_first_copy(s, ".p", ".m"));
                    double a = average.second->getValue(boost::replace_first_copy(s, ".p", ".a")) -
                               last_average->getValue(boost::replace_first_copy(s, ".p", ".a"));
                    double p = m / a;
                    avgs_per_game[s]->SetBinContent(ind,p);

                    m = waverage->getValue(boost::replace_first_copy(s, ".p", ".m")) -
                        last_waverage->getValue(boost::replace_first_copy(s, ".p", ".m"));
                    a = waverage->getValue(boost::replace_first_copy(s, ".p", ".a")) -
                        last_waverage->getValue(boost::replace_first_copy(s, ".p", ".a"));
                    p = m / a;
                    wavgs_per_game[s]->SetBinContent(ind,p);
                }
                avgs_per_time[s]->SetBinContent(ind,average.second->getValue(s));
                wavgs_per_time[s]->SetBinContent(ind,waverage->getValue(s));
            }
            else if (boost::contains(s, ".m") == 1){
                //makes have to take into account the fact that I store the total number of makes
                //(or total number of weighted attempts) for each day, not the average.

                if (average.second->getNum_games() != last_num_games) {
                    avgs_per_game[s]->SetBinContent(ind,average.second->getValue(s) -
                            last_average->getValue(s));
                    wavgs_per_game[s]->SetBinContent(ind,waverage->getValue(s) -
                            last_waverage->getValue(s));
                }
                avgs_per_time[s]->SetBinContent(ind,average.second->getValue(s) / (double)average.second->getNum_games());
                wavgs_per_time[s]->SetBinContent(ind,waverage->getValue(s) / (double)waverage->getNum_games());
            }
            else if (s == "srs") {
                avgs_per_time[s]->SetBinContent(ind,waverage->getOrigSRS());
                wavgs_per_time[s]->SetBinContent(ind,waverage->getSrs());
            }
            else if (s == "rpi") {
                wavgs_per_time[s]->SetBinContent(ind,waverage->getRpi());
            }
        }
    }

    outFile.Write();
    outFile.Close();

    return 0;
}