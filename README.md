This is my NCAA basketball analysis code.  It was originally written in
Ruby, however I have converted it into C++ for this project.  There are 
still a few Ruby files around to easily display some of the graphs and
histograms made by some of the programs, but all of the actual work is done
in C++.

I have written a document explaining all of the theory and formulas used
throughout the project, which can be found 
[on my website](http://www.brianvernarsky.com/files/brian_vernarsky_college_basketball_project.pdf). 
This document will simply try to explain what all of the files are and their
purposes are, and also show the order in which to run the program files which
exist in this main directory.

#Introduction

The basic idea behind this project is that I want to have a fairly simple
equation to quickly tell me, for a game involving Team A and Team B, at
a set location (home for A, home for B, or at a neutral site), on a given
day, how likely team A is to win, and how likely team B is to win.  And I
want this equation to rely solely on values derived from very basic team
statistics.  There is no information about the players involved, what 
conference they are in, who the coach is, etc.  Only information that can
be gleaned from the box scores of games played prior to the date in 
question will be used.

In order to make my predictions in a given year, I rely on averages, ratios,
and other constants that have been calculated using the 5 seasons prior.  Thus
to make a prediction for a game in the 2016 season (note: I list seasons
by the year in which they end, so the 2016 season is shorthand for the
2015-2016 season), I will need the statistics for the 2011, 2012, 2013, 2014
and 2015 seasons.  

I have gathered the box scores for every single Division-I
college basketball game played since the 2001 season, and they reside in the
teams/YEAR/ directory, where YEAR is the year in question, i.e. any number
between 2001 and 2016.  In each year directory, there is a single file for
every team labelled teams.YEAR_team_name.games.d (e.g. 
teams.2015_north_carolina.games.d), which contains the box score information
for every game they played that season, including the opponent's name, the
date, the location, whether they won or lost, how many points they scored,
how many field goals they made and attempted, etc, as well as those same
stats for their opponents.  The data is stored in comma-separated lists,
with each game on a different line.  Because of the sheer number of files
required, and their unimportance for understanding the rest of the code,
I have only uploaded a single example file to github, but in reality there
are nearly 350 files per year.

#Directory Stucture

There is a constants directory, which contains many files that will be 
created throughout the course of running the program files, but initially
it would be empty.

There is also a tournamentInfo directory, which contains information about
the NCAA tournament for every year since 2001.  Those files will not be 
needed until the very end of the process.

Lastly, there is a helpers directory, which contains very small bits of
helpful code that will be useful throughout the project.  One is called
doubleFormatter.h, which contains a single function, doubleFormatter, which
takes a double and returns a string with that double formatted to have 
a set number of digits after the decimal point.  The other is called
vectorMinMax.h, which has a function to return the index of the minimum
value of a vector.


#The Main Classes

Now, for the src/ directory.  This directory contains all of the class
definitions. There are 9 main classes, and 9 singleton classes to contain
the constants generated by and needed for the programs.  

The Team class is the main class that the user has access to.  It has
hashes to contain the TeamGames (explained below), TeamAverages (explained
below), and TeamWAverages (explained below), each indexed by the date.  It
contains methods used to add an individual TeamGame, TeamAverage or 
TeamWAverage, as well as to add all of the games in team game file at once.  It
also has methods to get those hashes, as well as the TeamGame, TeamAverage,
or TeamWAverage on a given date.  There is a static hash used to store
the pointer to the team by the team name.  It also contains a method
to calculate the weighted average (explained below) on a given date.

There is a file called readTeams.h that provides a method to read all of 
the files in a given directory (one of the `teams/YEAR/` directories) and add
in all of the games, averages, or waverage files in that directory.  This is
used to add all of the teams in a given year in at once.  This is the main
way to read in the game, average, or waverage information in the programs,
except for a few programs where only one or two teams' information is read
in at once.

The Pct class is used to store variables that have two main bits of
information, the number of attempts, and the number of makes, and from them
calculate a percentage.  A Pct can be constructed either with two integers,
two doubles (necessary when considering averages),  or by specifying the
percentage as well as the number of makes and attempts.  The class provides
methods for getting the number of makes, or attempts, the percentage, and
also to calculate the variance, standard deviation, and a few other 
statistical values.  Almost all of the statistics used in the TeamGame,
TeamAverage, and TeamWAverage classes are stored in Pct variables.

The Pcts class is a fancy container to hold Pct pointers and to make 
calcuations with them.  The average percentage over all of the Pct pointers
can be calculated with p_bar(), as can the weighted average, standard 
deviation, and weighted standard deviation.

The Game class is a superclass containing some basic information about
games.  In this project there probably is not a need for this class to be
separate from TeamGame, but as this same code is also used for the NBA 
project, which contains a PlayerGame class, I have decided to leave it this
way.  The properties of a game are the name, opponent, location, date, 
number of points scored, number of personal fouls, and then Pct properties
for offensive field goals, offensive two-point field goals, offensive
three-point field goals, offensive free-throws, offensive effective field
goal percentage, offensive true-shooting percentage, and offensive
free-throw-made rate.  There are no methods other than standard getters 
in this class.

The TeamGame class inherits from the Game class, and adds on variables for
game number, whether the game was a win or loss, the number of offensive
possessions, the spread of the game, and then Pct variables for
offensive offensive rebounding percentage (note, the double offensive was
intentional, offensive rebounding percentage is a stat, and the initial
offensive is there to indicate this is what the team in question got,
defensive ORB would be what the opposing team got), offensive defensive
rebounding percentage, offensive total rebounding rate, offensive
assist, steal, block, and turnover percentages.  Then all of the stats
stored for the offense are also stored for the defense (both the Game
and TeamGame stats).  There are standard getter functions for each of the
variables, but there are also two functions, getPct and getValue, which
return values based on an input string.

The TeamAverage class is used both for averages and for weighted averages,
as the TeamWAverage class is a subclass, adding only a few extra variables.
It contains fields for team name, date, and the number of games played by
the team up to the date of the instance.  There are also fields for the
number of offensive points and defensive points points scored to this point
of the season.  The rest of the variables are Pct variables.  The main ones
of which are offensive offensive rebounds, offensive effective field goals,
offensive free-throw-made rate, and offensive turnover rate, as well as 
their defensive countepoints.There are standard getter functions for each of 
the variables, but there are also two functions, getPct and getValue, which
return values based on an input string.

The TeamWAverage class is a subclass of TeamAverage.  It adds variables
for RPI, SRS, original SRS (useful because SRS is calculated through
and iterative process), and strength of schedule, SOS.

The two remaining classes are for used for simulating the NCAA tournament.
TournamentRegion contains holds information about the
starting state of the tournament (read in from the files in the 
tournamentInfo directory), and then contains methods for simulating each
of the first four rounds plus the play-in games (if necessary).  There are
also methods for calculating the likelihood of each team in the region
reaching each of the rounds in the tournament, as well as winning the
entire region.  

The Tournament class has a container to hold 4 TournamentRegion pointers,
which it then fills up using the method readTournamentInfo.  It then
has methods to play out the entire tournament, or calculate the likelihood
of each team in the tournament reaching each of the rounds in the tournament,
as well as winning the entire tournament.


##The Constant Classes

These classes are used to store constants needed in the programs.  Each of
the classes are singleton classes with few methods (generally only a way
to read in the information, and a way to access that info).  The exception
to that are the ConstantGameFunction and ConstantWAverageFunctions classes,
which contain methods to evaulate the functions stored within them, both
given a pair of teams and the location of the game, or given only one team
where we simulate a game against a completely average opponent.

The classes are:

ConstantSeasonInfo - containing the important dates in the season

ConstantTeam5YearAverages - containing the team averages for each stat
			    over the previous 5 seasons, for each
			    season from 2003-2016.

ConstantTeamNeutralRatios - containing the neutral ratios for each stat
			    at each location (home, away, neutral) over
			    the previous 5 seasons, for each season
			    from 2003-2016.

ConstantTeamPointDifferentials - containing the average point differentials
			    over the previous 5 seasons, for each
			    season from 2003-2016.

ConstantTeamWinRatios - containing the win ratios
			over the previous 5 seasons, for each
			season from 2003-2016.

ConstantSRSadditions - containing the SRS additions determined from the
		       previous 5 seasons, for each season from 2005-2016.

ConstantStandardDeviations - containing the standard deviations for the
			     TeamWAverages for each of the eight useful
			     stats over the previous 5 seasons, for each 
			     season from 2005-2016.

ConstantWAverageFunctions - containing the functions used to determine the
			    predicted value of an offensive stat given
			    an offensive weighted average and defensive
			    weighted average.  These were calculated using
			    the previous 5 years stats for every season
			    from 2005-2016.

ConstantGameFunction - containing the functions used to predict a winner
		       in a game.  These were calculated using the previous
		       5 years stats for every season from 2005-2016.



#The Programs

These are the main programs that you need to run.  In general, assume
that you have to run them in order, meaning you generally can't run
a program unless you've run all of the programs before it.  There are
some other programs listed below that are used to look at the results
of these programs.  I will note how many of them you have to run before
you can use them.

###generateAverages:
This file is for generating the averages for each of the stats over all teams
in a year or a specified set of years.  It will yield the average of the 
number of successes (e.g. made field goals), attempts (e.g. attempted field 
goals), and percentage for each of the main stats.

Then, it will also provide the ratio of these averages in home, away, and 
neutral games compared to all games (i.e. the sum of all home, neutral, and 
away games). This ratio is used when weighting the stats to come up with 
opponent-adjusted stats.


###generateWeightedAverages.cpp:
This program will generate the weighted averages for every team in a given 
year. It requires you to specify the year.  Then it searches the `teams/YEAR/` 
directory to find all of the team's game information.  It also reads in the 
averages and neutral ratios files found in the constants directory.  It 
outputs two files for each team, an (unweighted) averages file and a weighted 
averages file. If you have already generated some of the averages files in 
the year's directory, you may choose whether or not to overwrite them with 
the -o switch.


###generateSRS.cpp:
The program iterates over the SRS values to come up with the final values.
It processes one year at a time.  It defaults to iterate 100 times.  You
may choose to write the output to file with the -o switch, or print the 
results for three different teams at three different dates throughout the
season at each iteration.  This latter option is so that you can see whether
or not the results are converging.  Since you may wish start this process
over, due to changes in a few weighted average files, or a change to the
calculation, without having to start from the current iteration, it is
possible to start over using the original SRS values using the -O switch.


###generateSRSadditions.cpp:
The program calculates the number of points to add to a team's SRS when playing
at home.  It does so for a given outYear (specified with the -Y switch), by
analyzing the years sepcified as the input years using the -y switch and a
comma-separated list of years.  The output is always written to the screen
but can be written to a file, specified with the -o switch, which is created
in the constants directory.


###generateWAverageFunctions.cpp:
This program generates the histograms which will be used to generate the
functions which take as input Team A's offensive WAverages and Team B's
defensive WAverage, and returns a prediction for what Team A's offensive
WAverage would be in that game.  There is a function for each of the four
weighted average stats.

It takes as input 5 years worth of WAverages, and output a ROOT file with
the histograms and graphs needed to calculate the functions using the 
showWAverageFns.rb program.  The number of bins will depend on the number
of input years (generally 5 => 301, less => 165).


###showWAverageFns.rb:
This program is used to show the results of the generateWAverageFunctions 
program, but it can also be used to run that program first and then process 
the output, so as to make this a one-step process.  If a ROOT file already 
exists, you can plot its graphs using the -F and -n switches to set the input 
file name and the number of bins used. If a ROOT file does not already exist, 
then you can create one by using the -C option, and the -y and -n options.  
If you wish to save the ROOT file created, simply use the -F option to set 
the name, it will be saved by default in the rootFiles/ directory, so
only the name is necessary.  If no name is set, then a temporary file is 
created which is deleted at the end of the program.

The results of the fits are written to the screen by default, if you wish 
to write the output to a text file, simply use the -o option and provide a 
name.

If the -R option is chosen, then the canvases will be drawn, otherwise 
they will not.


###generateStandardDeviations.cpp:
This program generates the standard deviations necessary for the game 
functions. It takes as input 5 years worth of weighted averages and then 
outputs the standard deviations for a given year (set with -Y option) either 
to the terminal or to a file (set by the -o option).


###generateWeights.cpp:
This program determines the best weights to use to maximize the number
of games correctly predicted by the game function.  It takes as input
5 seasons worth of weighted averages, and the associated standard deviations
and outputs the set of weights that has the highest percentage of correct
games over the number of iterations chosen.

By default, a fresh set of 5 random starting values are generated for each
iteration and then a fit is run starting from those values.  But if you
wish to, you can enter a (properly formatted) file with set starting values
using the -S option.


###generateWeightsFixedSRS.cpp:
This is basically the same program as generateWeights except that you
fix the value of SRS and don't fit it, so there are only four
parameters to fit each iteration.


###generateGameFunctionHistograms.cpp:
This program generates the histograms which are used to determine the
likely winning percentage of a team based on their game score.  It requires
all of the years for which the game score function weights have been defined
and the name of the output ROOT file, which will be created in the 
rootFiles/ directory.


###checkGamePredictions.cpp:
This program checks every game in the list of input years using the game
function to determine how well it did.  If an SRS value is provided, using the
-s option, a different game function weights file is used.  The histograms
file is used to convert from the game score to likelihood of winning.

###checkGamePredictions.rb
This program is used to show the results of the checkGamePredictions.cpp 
program, but it can also be used to run that program first and then process 
the output, so as to make this a one-step process.  If a ROOT file already 
exists, you can plot its graphs using the -F switch to set the input file 
name.  If a ROOT file does not already exist, then you can use the -C switch 
to create one, and then you must specify the input years, and optionally 
the SRS value to use and the number of sigmas to use.  This program gets
the number of wins per predicted winning percentage bin and divides that by 
the number of total games in that bin to get a graph of the actual winning 
percentage vs the predicted ones.  The resulting plot is then fit with a 
first-order polynomial.  If everything has gone well, then the resulting fit 
should be a line with intercept of 0 and slope of 1.  The parameters of the 
fit are output on the screen, to see the actual plots, use the -R switch.

###tournamentSimulation.cpp:
This program will simulate the NCAA tournament for a given year (must be
between 2007 and 2015 (currently)), using a histograms file to calculate
the winning percentages.  The results will be output on the screen.
The -t option allows the user to calculate the odds of each team reaching
each round of the tournament as well as winning the whole tournament.




#Helper Programs

These programs are generally used to look at the results of previous 
programs in some way, shape, or form.  I note how far down the list of
programs you need to go to be able to use them.

###showTeamStats.cpp:
This program creates histograms for each of the four factors, offense and
defense, over the course of the season.  It creates histograms for both
the regular averages as well as the weighted averages.  It also plots
SRS and gameScore or RPI.  You view the resultant ROOT histograms
using the showTeamStats.rb Ruby program.  All you have to specify is the
team name, and the name of the output file.  You can also decide to 
plot the game score by selecting the -H option, and specifying an
SRS value for the weights if necessary.
Necessary programs: all up to and including generateSRS, must go to
generateGameFunctionHistograms to use the -H option.

###showTeamStats.rb:
This program is used to show the results of the showTeamStats program, but
it can also be used to run that program first and then process the output, 
so as to make this a one-step process.  If a ROOT file already exists, you 
can plot its graphs using the -F switch to set the input file name. If a 
ROOT file does not already exist, then you can create one by using the 
-t option to set the name of the team you want to plot. If you wish to 
save the ROOT file created, simply use the -F option to set the name, 
it will be saved by default in the rootFiles/ directory, so only the 
name is necessary.  If no name is set, then a temporary file is created 
which is deleted at the end of the program.
Necessary programs: all up to and including generateSRS, must go to
generateGameFunctionHistograms to use the -H option.

###predictGameOnDate.cpp:
This program creates histograms for each of the four factors, offense and
defense, over the course of the season for two teams.  It creates histograms 
for both the regular averages as well as the weighted averages.  It also plots
SRS and gameScore or RPI.  Lastly it calculates the predictions for both teams'
four factors, and likelihood of winning.  You view the resultant ROOT 
histograms using the predictGameOnDate.rb Ruby program.  All you have to
specify is the teams' names, and the name of the output file.  You can also 
decide to plot the game score by selecting the -H option, and specifying an
SRS value for the weights if necessary.
Necessary programs: all up to and including generateSRS, must go to
generateGameFunctionHistograms to use the -H option.

###predictGameOnDate.rb:
This program is used to show the results of the predictGameOnDate program,
but it can also be used to run that program first and then process the output,
so as to make this a one-step process.  If a ROOT file already exists, you 
can plot its graphs using the -F switch to set the input file name. If a ROOT 
file does not already exist, then you can create one by using the -t and -T 
options to set the names of the teams you want to plot. If you wish to save 
the ROOT file created, simply use the -F option to set the name, it will 
be saved by default in the rootFiles/ directory, so only the name is 
necessary.  If no name is set, then a temporary file is created which
is deleted at the end of the program.
Necessary programs: all up to and including generateSRS, must go to
generateGameFunctionHistograms to use the -H option.

###predictAllGamesOnDate.rb:
This program will calculate the odds of each team winning for every game to be
played on the date specified by the -d switch.  The games to be played on the 
date are determined by the schedules files.  Every team is cycled through, and
if they have a game (or games) on the date, then the program will produce a 
prediction for them using the weights and histograms file (if desired) 
associated with the SRS indicated with the -s switch.  The TeamWAverage 
to use for the predictions defaults to the day of the game, but can be set 
with the -D option; this is so that you can make predictions for a day even 
if you don't have completely up-to-date statistics, and haven't calculated 
the waverage for the game date yet, which will be common since you probably 
won't be able to instantly download all of the game information immediately 
for every day, so the stats lag behind by a few days.
Necessary programs: all up to and including generateSRS, must go to
generateGameFunctionHistograms to use the -H option.

###rankTeamsByGameScore.cpp:
This program ranks all of the teams in the list of years provided by their 
gameScore as it is calculated on the day the NCAA tournament began in that year
using either the constants appropriate for the year of the team, or a 
specific year which can be provided using the -Y switch.  In the example, 
all the teams from 2014 and 2015 will be ranked using 2015's constants.  
If a histogram file is provided then the winning percentage will also 
be output.
Necessary programs: all up to and including generateSRS, must go to
generateGameFunctionHistograms to use the -H option.

