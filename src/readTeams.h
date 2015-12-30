//
// Created by soulish on 11/29/15.
//

#ifndef NCAA_C_READTEAMS_H
#define NCAA_C_READTEAMS_H

#include <string>
#include <vector>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/algorithm/string.hpp>

int readTeamsFromDir(std::string dir, std::string gamesOrAverages = "games"){
    std::string filepath;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::vector<std::string> result;
    std::string teamName;
    std::string fileName;
    Team *team;

    dp = opendir(dir.c_str());
    if (dp == NULL) {
        std::cout << "Error"  << " opening " << dir << std::endl;
        return -1;
    }

    while ((dirp = readdir(dp))){
        //make sure it's a file with a proper extension, and the games designation
        fileName = dirp->d_name;
        if (!boost::contains(fileName,".d"))     continue;
        if (!boost::contains(fileName,"." + gamesOrAverages + "."))  continue;

        filepath = dir + fileName;

        //Ignore subdirs and messed up files
        if (stat(filepath.c_str(), &filestat))   continue;
        if (S_ISDIR( filestat.st_mode ))         continue;

        boost::split(result, fileName, boost::is_any_of("."), boost::token_compress_on );
        teamName = result[1];
        boost::replace_all(teamName,"_"," ");
        if (!Team::findTeam(teamName)) new Team(teamName);
        team = Team::findTeam(teamName);

        if (gamesOrAverages == "games")
            team->addGames(filepath);
        else if (gamesOrAverages == "averages")
            team->addAverages(filepath);
        else if (gamesOrAverages == "waverages")
            team->addWAverages(filepath);
        else if (gamesOrAverages == "schedule")
            team->addScheduledGames(filepath);

//        std::cout << "Reading in " << gamesOrAverages << " for " << teamName << "\r";
//        fflush(stdout);
    }
//    std::cout << std::endl;

    return 0;
}

#endif //NCAA_C_READTEAMS_H
