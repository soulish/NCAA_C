//
// Created by soulish on 11/20/15.
//

#include <fstream>
#include "Team.h"
#include "ConstantTeamNeutralRatios.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

//declaring the static variables
std::unordered_map<std::string,Team*> Team::teams;
Team *Team::lastTeam;

//Constructor.  All we need is the name of the team.
//The year is determined from the name.
//The team is put into the static hash of teams to be found later.
Team::Team(std::string teamname)
        : name (teamname),
          year(stoi(teamname.substr(0,4))),
          tournament_team(false){
    teams.emplace(teamname,this);
    lastTeam = this;
}

Team *Team::findTeam(std::string name) {
    if (teams.find(name) == teams.end())
        return nullptr;
    else
        return teams[name];

}

//Add TeamGame to the gamesByDate hash, using the date of
//the game in the form YYYY-MM-DD as the key.
void Team::addGame(TeamGame* g) {
    gamesByDate.emplace(boost::gregorian::to_iso_extended_string(*(g->getDate())),g);
}

//Add TeamAverage to the averagesByDate hash.
void Team::addAverage(TeamAverage *a) {
    averagesByDate.emplace(boost::gregorian::to_iso_extended_string(*(a->getDate())),a);
}

//Add TeamWAverage to the averagesByDate hash.
void Team::addWAverage(TeamWAverage *w) {
    waveragesByDate.emplace(boost::gregorian::to_iso_extended_string(*(w->getDate())),w);
}

//Add TeamGames via a file containing their games.
void Team::addGames(std::string fileName) {
    std::string team, opp, loc;
    unsigned short yy, mo, da;
    int game_no, win, pts, opp_pts, fgm, fga,threem, threea, ftm, fta, orb, trb, ast, stl, blk, tov, pf;
    int dfgm, dfga, dthreem, dthreea, dftm, dfta, dorb, dtrb, dast, dstl, dblk, dtov, dpf;
    double spread;

    std::ifstream file(fileName);
    std::string value;
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
        spread = stod(value);

        if (file.good()) {
            this->addGame(
                    new TeamGame(team, opp, game_no, new boost::gregorian::date(yy, mo, da), loc, win, pts, opp_pts,
                                 fgm, fga, threem, threea, ftm, fta, orb, trb, ast, stl, blk, tov, pf,
                                 dfgm, dfga, dthreem, dthreea, dftm, dfta,
                                 dorb, dtrb, dast, dstl, dblk, dtov, dpf, spread));
        }
    }
}

void Team::addAverages(std::string fileName) {
    std::string team;
    unsigned short yy, mo, da;
    int opts, otwoa, othreea, ofta, oora, odra, otoa, dpts, dtwoa, dthreea, dfta, dora, ddra, dtoa;
    double otwop, othreep, oftp, oorp, odrp, otop, dtwop, dthreep, dftp, dorp, ddrp, dtop;
    double spread;
    int num_games;

    std::ifstream file(fileName);
    std::string value;
    while ( file.good() ){
        getline(file, team, ',' );
        getline(file, value, ',' );
        yy = (unsigned short)stoi(value);
        getline(file, value, ',' );
        mo = (unsigned short)stoi(value);
        getline(file, value, ',' );
        da = (unsigned short)stoi(value);
        getline(file, value, ',' );
        opts = stoi(value);
        getline(file, value, ',' );
        otwoa = stoi(value);
        getline(file, value, ',' );
        otwop = stod(value);
        getline(file, value, ',' );
        othreea = stoi(value);
        getline(file, value, ',' );
        othreep = stod(value);
        getline(file, value, ',' );
        ofta = stoi(value);
        getline(file, value, ',' );
        oftp = stod(value);
        getline(file, value, ',' );
        oora = stoi(value);
        getline(file, value, ',' );
        oorp = stod(value);
        getline(file, value, ',' );
        odra = stoi(value);
        getline(file, value, ',' );
        odrp = stod(value);
        getline(file, value, ',' );
        otoa = stoi(value);
        getline(file, value, ',' );
        otop = stod(value);
        getline(file, value, ',' );

        dpts = stoi(value);
        getline(file, value, ',' );
        dtwoa = stoi(value);
        getline(file, value, ',' );
        dtwop = stod(value);
        getline(file, value, ',' );
        dthreea = stoi(value);
        getline(file, value, ',' );
        dthreep = stod(value);
        getline(file, value, ',' );
        dfta = stoi(value);
        getline(file, value, ',' );
        dftp = stod(value);
        getline(file, value, ',' );
        dora = stoi(value);
        getline(file, value, ',' );
        dorp = stod(value);
        getline(file, value, ',' );
        ddra = stoi(value);
        getline(file, value, ',' );
        ddrp = stod(value);
        getline(file, value, ',' );
        dtoa = stoi(value);
        getline(file, value, ',' );
        dtop = stod(value);
        getline(file, value, ',' );
        num_games = stoi(value);

        getline(file, value, '\n' );
        spread = stod(value);

        if (file.good()) {
            this->addAverage(
                    new TeamAverage(team, new boost::gregorian::date(yy, mo, da),
                                    opts, otwoa, otwop, othreea, othreep, ofta, oftp,
                                    oora, oorp, odra, odrp, otoa, otop,
                                    dpts, dtwoa, dtwop, dthreea, dthreep, dfta, dftp,
                                    dora, dorp, ddra, ddrp, dtoa, dtop, num_games, spread));
        }
    }
}

void Team::addWAverages(std::string fileName) {
    std::string team;
    unsigned short yy, mo, da;
    int opts, otwoa, othreea, ofta, oora, odra, otoa, dpts, dtwoa, dthreea, dfta, dora, ddra, dtoa;
    double otwop, othreep, oftp, oorp, odrp, otop, dtwop, dthreep, dftp, dorp, ddrp, dtop;
    double rpi, srs, sos, spread;
    int num_games;

    std::ifstream file(fileName);
    std::string value;
    while ( file.good() ){
        getline(file, team, ',' );
        getline(file, value, ',' );
        yy = (unsigned short)stoi(value);
        getline(file, value, ',' );
        mo = (unsigned short)stoi(value);
        getline(file, value, ',' );
        da = (unsigned short)stoi(value);
        getline(file, value, ',' );
        opts = stoi(value);
        getline(file, value, ',' );
        otwoa = stoi(value);
        getline(file, value, ',' );
        otwop = stod(value);
        getline(file, value, ',' );
        othreea = stoi(value);
        getline(file, value, ',' );
        othreep = stod(value);
        getline(file, value, ',' );
        ofta = stoi(value);
        getline(file, value, ',' );
        oftp = stod(value);
        getline(file, value, ',' );
        oora = stoi(value);
        getline(file, value, ',' );
        oorp = stod(value);
        getline(file, value, ',' );
        odra = stoi(value);
        getline(file, value, ',' );
        odrp = stod(value);
        getline(file, value, ',' );
        otoa = stoi(value);
        getline(file, value, ',' );
        otop = stod(value);
        getline(file, value, ',' );

        dpts = stoi(value);
        getline(file, value, ',' );
        dtwoa = stoi(value);
        getline(file, value, ',' );
        dtwop = stod(value);
        getline(file, value, ',' );
        dthreea = stoi(value);
        getline(file, value, ',' );
        dthreep = stod(value);
        getline(file, value, ',' );
        dfta = stoi(value);
        getline(file, value, ',' );
        dftp = stod(value);
        getline(file, value, ',' );
        dora = stoi(value);
        getline(file, value, ',' );
        dorp = stod(value);
        getline(file, value, ',' );
        ddra = stoi(value);
        getline(file, value, ',' );
        ddrp = stod(value);
        getline(file, value, ',' );
        dtoa = stoi(value);
        getline(file, value, ',' );
        dtop = stod(value);

        getline(file, value, ',' );
        rpi = stod(value);
        getline(file, value, ',' );
        srs = stod(value);
        getline(file, value, ',' );
        sos = stod(value);

        getline(file, value, ',' );
        num_games = stoi(value);
        getline(file, value, '\n' );
        spread = stod(value);

        if (file.good()) {
            this->addWAverage(
                    new TeamWAverage(team, new boost::gregorian::date(yy, mo, da),
                                    opts, otwoa, otwop, othreea, othreep, ofta, oftp,
                                    oora, oorp, odra, odrp, otoa, otop,
                                    dpts, dtwoa, dtwop, dthreea, dthreep, dfta, dftp,
                                    dora, dorp, ddra, ddrp, dtoa, dtop, rpi, srs, sos, num_games, spread));
        }
    }
}

std::vector<double> Team::calcWeightedAverage(boost::gregorian::date date) {
    //these are the stats we are going to be weighting
    std::string stats[] = {"pts.m","two.a","two.p","three.a","three.p","ft.a","ft.p",
                           "or.a","or.p","dr.a","dr.p","to.a","to.p"};
    typedef std::unordered_map<std::string, TeamGame*> gamesByDateType;
    typedef std::unordered_map<std::string, double> hashDoubleType;
    typedef std::unordered_map<std::string, int> hashIntType;
    typedef std::unordered_map<std::string, hashDoubleType * > sumsType;

    hashDoubleType totals, weighted_stats;
    for (std::string &s : stats){
        totals.emplace("o"+s,0);
        totals.emplace("d"+s,0);
        weighted_stats.emplace("o"+s, 0);
        weighted_stats.emplace("d"+s, 0);
    }
    hashIntType opp_wins, opp_losses, opp_opp_wins, opp_opp_losses, oo_wins, oo_losses;
    int owins = 0, olosses = 0, oowins = 0, oolosses = 0;
    double wins = 0, losses = 0;
    int pure_wins = 0, pure_total = 0;
    int pt_diff = 0;
    double opp_avg_pt_diff = 0;
    sumsType opp_sums, opp_opp_sums;
    hashDoubleType opp_pt_diff;
    hashIntType num_opp_games, num_opp_opp_games;

    Team *opp, *opp_opp;

    int num_games = 0;
    for (gamesByDateType::value_type &game : gamesByDate){
        if (*(game.second->getDate()) >= date) continue; //only look at games that happened before this date
        std::string oname = game.second->getOpp();
        opp = findTeam(oname);
        if (!opp) continue; //skip games against non-Division-I opponents
        for (std::string &s : stats){
            totals["o"+s] += game.second->getValue("o"+s);
            totals["d"+s] += game.second->getValue("d"+s);
        }
        num_games++;
        pure_total++;
        if (game.second->getWin()){
            pure_wins++;
            if (game.second->getLoc() == "home") wins += 0.6;
            else if (game.second->getLoc() == "away") wins += 1.4;
            else if (game.second->getLoc() == "neutral") wins += 1.0;
        }
        else{
            if (game.second->getLoc() == "home") losses += 1.4;
            else if (game.second->getLoc() == "away") losses += 0.6;
            else if (game.second->getLoc() == "neutral") losses += 1.0;
        }
        pt_diff += game.second->getOpts() - game.second->getDpts();

        //only go through this the first time for each opponent
        if (opp_sums.find(oname) == opp_sums.end()){ //i.e. if there is no entry for this team
            opp_sums.emplace(oname, new hashDoubleType());

            for (std::string &s : stats){
                opp_sums[oname]->emplace("o"+s,0.0);
                opp_sums[oname]->emplace("d"+s,0.0);
            }
            num_opp_games.emplace(oname,0);
            opp_pt_diff.emplace(oname,0);
            opp_wins.emplace(oname,0);
            opp_losses.emplace(oname,0);
            oo_wins.emplace(oname,0);
            oo_losses.emplace(oname,0);

            gamesByDateType games = opp->getGamesByDate();
            for (gamesByDateType::value_type &opp_game : games){
                if (*(opp_game.second->getDate()) >= date) continue; //only look at games that happened before this date
                if (opp_game.second->getOpp() == this->name) continue; //skip games against self

                std::string ooname = opp_game.second->getOpp();
                opp_opp = findTeam(ooname);
                if (!opp_opp) continue; //skip games against non-Division-I opponents
                num_opp_games[oname]++;
                if (opp_game.second->getWin()) opp_wins[oname]++;
                else opp_losses[oname]++;
                opp_pt_diff[oname] += opp_game.second->getOpts() - opp_game.second->getDpts();

                //only go through this the first time for each opponents' opponent
                if (opp_opp_sums.find(ooname) == opp_opp_sums.end()){ //i.e. if there is no entry for this team
                    opp_opp_sums.emplace(ooname, new hashDoubleType());
                    for (std::string &s : stats){
                        opp_opp_sums[ooname]->emplace("o"+s,0.0);
                        opp_opp_sums[ooname]->emplace("d"+s,0.0);
                    }
                    num_opp_opp_games.emplace(ooname,0);
                    opp_opp_wins.emplace(ooname,0);
                    opp_opp_losses.emplace(ooname,0);

                    gamesByDateType opp_games = opp_opp->getGamesByDate();
                    for (gamesByDateType::value_type &opp_opp_game : opp_games){
                        if (*(opp_opp_game.second->getDate()) >= date) continue; //only look at games that happened before this date
                        if (opp_opp_game.second->getOpp() == this->name) continue; //skip games against self
                        if (!findTeam(opp_opp_game.second->getOpp())) continue; //skip games against non-Division-I opponents

                        num_opp_opp_games[ooname]++;
                        if (opp_opp_game.second->getWin()) opp_opp_wins[ooname]++;
                        else opp_opp_losses[ooname]++;

                        for (std::string &s : stats){
                            //this sum, divided by the total number of games played by opp_opp, is what opp_opp would average offensively on a
                            //neutral floor
                            opp_opp_sums[ooname]->at("o"+s) += opp_opp_game.second->getValue("o"+s) *
                                                               ConstantTeamNeutralRatios::Instance()->get(year, opp_opp_game.second->getLoc(), "o"+s);
                            //this sum, divided by the total number of games played by opp_opp, is what opp_opp would average giving up
                            //defensively on a neutral floor
                            opp_opp_sums[ooname]->at("d"+s) += opp_opp_game.second->getValue("d"+s) *
                                                               ConstantTeamNeutralRatios::Instance()->get(year, opp_opp_game.second->getOppLoc(), "o"+s);//note: all neutral ratios are offensive
                        }
                    }
                }

                oo_wins[oname] += opp_opp_wins[ooname];
                oo_losses[oname] += opp_opp_losses[ooname];

                for (std::string &s : stats){
                    if (num_opp_opp_games[ooname] > 0 && opp_opp_sums[ooname]->at("o"+s) > 0){
                        //this sum, divided by the number of games played by opp, is a weight. It says how much worse
                        //opp performs offensively on a neutral floor compared to the league-wide average,
                        //weighted by how well their opponents have performed.  This form is slightly misleading
                        //because it looks like just a comparison of opp's offense and opp_opp's d (and vice versa),
                        //but in fact it is (opp_sum[offensive stat]/league_average[offensive stat]) /
                        //(opp_opp_sum[defensive stat]/league_average[defensive stat]).
                        //Since the league average is the same for a stat whether for offense or defense
                        //(it doesn't make sense that the league would give up more points than it scores
                        //on average would it?), those two league averages cancel out and we're left with
                        //this.
                        //
                        // Since it will be in the denominator of self's D equation, that means that
                        //1 would mean they play exactly to the level of their opponents, and so self's D
                        //would be unchaged;
                        //greater than 1 means they overperform compared to what their opponents generally allow,
                        //(they're better at offense) and so the weighted average for self will go down (meaning better d);
                        //less than 1 means they underperform compared to what their opponents generally allow,
                        //(they're worse at offense than the league average) and so the weighted average
                        //for self will go up (meaning worse d).
                        opp_sums[oname]->at("o"+s) += (opp_game.second->getValue("o"+s) * ConstantTeamNeutralRatios::Instance()->get(year, opp_game.second->getLoc(), "o"+s)) /
                                                      (opp_opp_sums[ooname]->at("d"+s) / (double)num_opp_opp_games[ooname]);
                        //this is the similar sum for offense.  It is how much they allow compared to how much the
                        //opponents usually get. Greater than 1 means they give up more than usual (they're bad at
                        //defense) and so self's offensive weighted avg will go down (it's not as impressive to score
                        //well against a bad d); less than 1 means they're better than usual so self's offensive
                        //numbers will go up.
                        opp_sums[oname]->at("d"+s) += (opp_game.second->getValue("d"+s) * ConstantTeamNeutralRatios::Instance()->get(year, opp_game.second->getOppLoc(), "o"+s)) /
                                                      (opp_opp_sums[ooname]->at("o"+s) / (double)num_opp_opp_games[ooname]);
                    }
                    else{
                        //early in the season opp's opponents may not have played another game, so we can't tell how
                        //they do on average, so we just treat them as average
                        opp_sums[oname]->at("o"+s) += 1;
                        opp_sums[oname]->at("d"+s) += 1;
                    }
                }
            }
        }

        owins += opp_wins[oname];
        olosses += opp_losses[oname];

        oowins += oo_wins[oname];
        oolosses += oo_losses[oname];

        opp_avg_pt_diff += opp_pt_diff[oname] / (double) num_opp_games[oname];

        for (std::string &s : stats){
            if (num_opp_games[oname] > 0 && opp_sums[oname]->at("o"+s) > 0){
                //this sum, divided by the number of games played by self, gives the weighted average for this team's
                //offensive stats.
                weighted_stats["o"+s] += (game.second->getValue("o"+s) * ConstantTeamNeutralRatios::Instance()->get(year, game.second->getLoc(), "o"+s)) /
                                         (opp_sums[oname]->at("d"+s) / (double)num_opp_games[oname]);
                //similar for the defensive stats
                weighted_stats["d"+s] += (game.second->getValue("d"+s) * ConstantTeamNeutralRatios::Instance()->get(year, game.second->getOppLoc(), "o"+s)) /
                                         (opp_sums[oname]->at("o"+s) / (double)num_opp_games[oname]);
            }
            else{
                //when none of self's opponent's have played any other games we just treat them as unweighted.
                weighted_stats["o"+s] += (game.second->getValue("o"+s) * ConstantTeamNeutralRatios::Instance()->get(year, game.second->getLoc(), "o"+s));
                weighted_stats["d"+s] += (game.second->getValue("d"+s) * ConstantTeamNeutralRatios::Instance()->get(year, game.second->getOppLoc(), "o"+s));
            }
        }
    }

    double rpi = 0.0, srs = 0.0, sos = 0.0;

    if (num_games > 0) {
        if ((owins + olosses > 0) && (oowins + oolosses > 0)) {
            rpi = 0.25 * (wins / (double) (wins + losses)) + 0.5 * (owins / (double) (owins + olosses)) +
                  0.25 * (oowins / (double) (oowins + oolosses));
        }
        else{
            rpi = wins/(double)(wins + losses);
        }
        if (opp_avg_pt_diff > 0) {
            srs = (pt_diff + opp_avg_pt_diff) / (double) num_games;
            sos = opp_avg_pt_diff / (double) num_games;
        }
        else{
            srs = pt_diff/(double) num_games;
            sos = 0.0;
        }

        for (std::string &s : stats){
            if (boost::contains(s,".p") == 0) continue;
            weighted_stats["o"+s] /= (double) num_games;
            weighted_stats["d"+s] /= (double) num_games;
            totals["o"+s] /= (double) num_games;
            totals["d"+s] /= (double) num_games;
        }
    }

    std::vector<double> result;

    for (std::string &s : stats)
        result.push_back(totals["o"+s]);
    for (std::string &s : stats)
        result.push_back(totals["d"+s]);
    for (std::string &s : stats)
        result.push_back(weighted_stats["o"+s]);
    for (std::string &s : stats)
        result.push_back(weighted_stats["d"+s]);
    result.push_back(rpi);
    result.push_back(srs);
    result.push_back(sos);
    result.push_back((double)num_games);

    BOOST_FOREACH(sumsType::value_type &xx, opp_sums){
                    delete xx.second;
                }
    BOOST_FOREACH(sumsType::value_type &xx, opp_opp_sums){
                    delete xx.second;
                }

    return result;
}

TeamGame *Team::GameOnDate(boost::gregorian::date d) const {
    if (gamesByDate.find(boost::gregorian::to_iso_extended_string(d)) == gamesByDate.end())
        return nullptr;
    else
        return gamesByDate.at(boost::gregorian::to_iso_extended_string(d));
}

TeamAverage *Team::AverageOnDate(boost::gregorian::date d) const {
    if (averagesByDate.find(boost::gregorian::to_iso_extended_string(d)) == averagesByDate.end())
        return nullptr;
    else
        return averagesByDate.at(boost::gregorian::to_iso_extended_string(d));
}

TeamWAverage *Team::WAverageOnDate(boost::gregorian::date d) const {
    if (waveragesByDate.find(boost::gregorian::to_iso_extended_string(d)) == waveragesByDate.end())
        return nullptr;
    else
        return waveragesByDate.at(boost::gregorian::to_iso_extended_string(d));
}
