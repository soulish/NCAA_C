//
// Created by soulish on 11/20/15.
//

#include <fstream>
#include "Team.h"

//declaring the static variables
std::unordered_map<string,Team*> Team::teams;
Team *Team::lastTeam;

//Constructor.  All we need is the name of the team.
//The year is determined from the name.
//The team is put into the static hash of teams to be found later.
Team::Team(string teamname)
        : name (teamname),
          year(stoi(teamname.substr(0,4))),
          tournament_team(false){
    std::pair<std::string,Team*> teampair (teamname,this);
    teams.insert(teampair);
    lastTeam = this;
}

//Add TeamGame to the gamesByDate hash, using the date of
//the game in the form YYYY-MM-DD as the key.
void Team::addGame(TeamGame* g) {
    string d = boost::gregorian::to_iso_extended_string(*(g->getDate()));
    std::pair<string,TeamGame*> gamePair (d,g);
    gamesByDate.insert(gamePair);
}

//Add TeamAverage to the averagesByDate hash.
void Team::addAverage(TeamAverage *a) {
    string d = boost::gregorian::to_iso_extended_string(*(a->getDate()));
    std::pair<string,TeamAverage*> averagePair (d,a);
    averagesByDate.insert(averagePair);
}

//Add TeamWAverage to the averagesByDate hash.
void Team::addWAverage(TeamWAverage *w) {
    string d = boost::gregorian::to_iso_extended_string(*(w->getDate()));
    std::pair<string,TeamWAverage*> waveragePair (d,w);
    waveragesByDate.insert(waveragePair);
}

//Add TeamGames via a file containing their games.
void Team::addGames(string fileName) {
    string team, opp, loc;
    unsigned short yy, mo, da;
    int game_no, win, pts, opp_pts, fgm, fga,threem, threea, ftm, fta, orb, trb, ast, stl, blk, tov, pf;
    int dfgm, dfga, dthreem, dthreea, dftm, dfta, dorb, dtrb, dast, dstl, dblk, dtov, dpf;
    float spread;

    ifstream file(fileName);
    string value;
    while ( file.good() ){
        getline(file, team, ',' );
        getline(file, opp, ',' );
        getline(file, value, ',' );
        game_no = stoi(value);
        getline(file, value, ',' );
        yy = (unsigned short)stoi(value);
        getline(file, value, ',' );
        mo = (unsigned short)stoi(value);
        getline(file, value, ',' );
        da = (unsigned short)stoi(value);
        getline(file, loc, ',' );
        getline(file, value, ',' );
        win = stoi(value);
        getline(file, value, ',' );
        pts = stoi(value);
        getline(file, value, ',' );
        opp_pts = stoi(value);
        getline(file, value, ',' );
        fgm = stoi(value);
        getline(file, value, ',' );
        fga = stoi(value);
        getline(file, value, ',' );
        threem = stoi(value);
        getline(file, value, ',' );
        threea = stoi(value);
        getline(file, value, ',' );
        ftm = stoi(value);
        getline(file, value, ',' );
        fta = stoi(value);
        getline(file, value, ',' );
        orb = stoi(value);
        getline(file, value, ',' );
        trb = stoi(value);
        getline(file, value, ',' );
        ast = stoi(value);
        getline(file, value, ',' );
        stl = stoi(value);
        getline(file, value, ',' );
        blk = stoi(value);
        getline(file, value, ',' );
        tov = stoi(value);
        getline(file, value, ',' );
        pf = stoi(value);
        getline(file, value, ',' );
        dfgm = stoi(value);
        getline(file, value, ',' );
        dfga = stoi(value);
        getline(file, value, ',' );
        dthreem = stoi(value);
        getline(file, value, ',' );
        dthreea = stoi(value);
        getline(file, value, ',' );
        dftm = stoi(value);
        getline(file, value, ',' );
        dfta = stoi(value);
        getline(file, value, ',' );
        dorb = stoi(value);
        getline(file, value, ',' );
        dtrb = stoi(value);
        getline(file, value, ',' );
        dast = stoi(value);
        getline(file, value, ',' );
        dstl = stoi(value);
        getline(file, value, ',' );
        dblk = stoi(value);
        getline(file, value, ',' );
        dtov = stoi(value);
        getline(file, value, ',' );
        dpf = stoi(value);
        getline(file, value, '\n' );
        spread = stof(value);

        if (file.good()) {
            this->addGame(
                    new TeamGame(team, opp, game_no, new boost::gregorian::date(yy, mo, da), loc, win, pts, opp_pts,
                                 fgm, fga, threem, threea, ftm, fta, orb, trb, ast, stl, blk, tov, pf,
                                 dfgm, dfga, dthreem, dthreea, dftm, dfta,
                                 dorb, dtrb, dast, dstl, dblk, dtov, dpf, spread));
        }
    }
}
