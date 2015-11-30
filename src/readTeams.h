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

int readTeamsFromDir(std::string dir){
    std::string filepath;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::vector<std::string> result;
    std::string teamName;
    char* path = new char();
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
        if (!boost::contains(fileName,".d")) continue;
        if (!boost::contains(fileName,"games"))  continue;

        filepath = dir + "/" + fileName;

        //Ignore subdirs and messed up files
        if (stat(filepath.c_str(), &filestat))   continue;
        if (S_ISDIR( filestat.st_mode ))         continue;

        boost::split(result, fileName, boost::is_any_of("."), boost::token_compress_on );
        teamName = result[1];
        boost::replace_all(teamName,"_"," ");
        new Team(teamName);
        team = Team::findTeam(teamName);
        std::sprintf(path, "%s%s", dir.c_str(), fileName.c_str());
        team->addGames(path);

        std::cout << path << std::endl;
    }

    return 0;
}

#endif //NCAA_C_READTEAMS_H