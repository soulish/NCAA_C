#include <iostream>
#include <fstream>
#include "TFile.h"
#include  "TH1F.h"
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

    char *homePath, path[256], histName[256];
    homePath = getenv("HOME");

    sprintf(path, "%s/cpp/NCAA_C/constants/team_5yr_averages.d", homePath);
    ConstantTeam5YearAverages::Instance()->initialize(path);
    sprintf(path, "%s/cpp/NCAA_C/constants/season_info.d", homePath);
    ConstantSeasonInfo::Instance()->initialize(path);

    Team *team = new Team(teamName);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.averages.d",
            homePath,team->getYear(),boost::replace_all_copy(teamName," ","_").c_str());
    team->addAverages(path);
    sprintf(path, "%s/cpp/NCAA_C/teams/%i/teams.%s.waverages.d",
            homePath,team->getYear(),boost::replace_all_copy(teamName," ","_").c_str());
    team->addWAverages(path);

    sprintf(path, "%s/cpp/NCAA_C/rootFiles/%s",
            homePath,outFileName.c_str());
    TFile outFile(path,"recreate");

    typedef std::unordered_map<std::string, TH1F*> hashType;
    hashType avgs_per_time, wavgs_per_time;
    std::string stats[] = {"oor.p", "oefg.p", "oto.p", "oftmr.p", "srs",
                           "dor.p", "defg.p", "dto.p", "dftmr.p", "rpi"};

    for (std::string &s : stats){
        sprintf(histName, "avg_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        avgs_per_time.emplace(s, new TH1F(histName,s.c_str(),180,0,180));
        sprintf(histName, "wavg_per_time_%s",(boost::replace_all_copy(s,".","").c_str()));
        wavgs_per_time.emplace(s, new TH1F(histName,s.c_str(),180,0,180));
    }

    typedef std::unordered_map<std::string, TeamAverage *> averageHashType;
    typedef std::unordered_map<std::string, TeamWAverage *> waverageHashType;

    averageHashType averageHash = team->getAveragesByDate();
    waverageHashType waverageHash = team->getWAveragesByDate();

    TeamAverage *last_average;
    TeamWAverage *waverage, *last_waverage;

    boost::gregorian::date_duration dateDuration(1);
    boost::gregorian::days indDuration;

    for (auto &average : averageHash){
//        last_average = team->AverageOnDate(*(average.second->getDate()) - dateDuration);
//        if (!last_average) continue; //skip the first date
        waverage = team->WAverageOnDate(*(average.second->getDate()));
//        last_waverage = team->WAverageOnDate(*(average.second->getDate()) - dateDuration);

//        int last_num_games = last_average->getNum_games();
        indDuration = *(waverage->getDate()) - ConstantSeasonInfo::Instance()->get(team->getYear(),"season start");
        int ind = (int)indDuration.days();

        for (std::string &s : stats) {
            if (boost::contains(s, ".p") == 1){
//                double m = average.second->getValue(boost::replace_first_copy(s,".p",".m")) -
//                       last_average->getValue(boost::replace_first_copy(s,".p",".m"));
//                double a = average.second->getValue(boost::replace_first_copy(s,".p",".a")) -
//                       last_average.second->getValue(boost::replace_first_copy(s,".p",".a"));
//                double p = m/a;
                avgs_per_time[s]->SetBinContent(ind,average.second->getValue(s));
                wavgs_per_time[s]->SetBinContent(ind,waverage->getValue(s));
            }
            else if (boost::contains(s, ".m") == 1){
                avgs_per_time[s]->SetBinContent(ind,average.second->getValue(s) / (double)average.second->getNum_games());
                wavgs_per_time[s]->SetBinContent(ind,waverage->getValue(s) / (double)waverage->getNum_games());
            }
            else if (s == "srs") {
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