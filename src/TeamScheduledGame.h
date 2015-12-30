//
// Created by soulish on 12/29/15.
//

#ifndef NCAA_C_TEAMSCHEDULEDGAME_H
#define NCAA_C_TEAMSCHEDULEDGAME_H

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

class TeamScheduledGame {
public:
    TeamScheduledGame(std::string _teamname, std::string _oppname, int _game_no,
                      boost::gregorian::date _date, std::string _location);

    const std::string &getTeamname() const { return teamname; }
    const std::string &getOppname() const { return oppname; }
    const int getGame_no() const { return game_no; }
    const boost::gregorian::date &getDate() const { return date; }
    const std::string &getLocation() const { return location; }

private:
    const std::string teamname;
    const std::string oppname;
    const int game_no;
    const boost::gregorian::date date;
    const std::string location;

};


#endif //NCAA_C_TEAMSCHEDULEDGAME_H
