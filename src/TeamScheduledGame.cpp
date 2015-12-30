//
// Created by soulish on 12/29/15.
//

#include "TeamScheduledGame.h"

TeamScheduledGame::TeamScheduledGame(std::string _teamname, std::string _oppname, int _game_no,
                                     boost::gregorian::date _date, std::string _location)
        : teamname(_teamname),
          oppname(_oppname),
          game_no(_game_no),
          date(_date),
          location(_location)    {   }
