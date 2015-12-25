require 'libRuby'
gStyle.SetOptStat(0)
gStyle.SetTitleFillColor(10)
gStyle.SetTitleBorderSize(0)
gStyle.SetPalette(1,0)

def printOptions
  puts ""
  puts "showTeamStats.rb Usage options"
  puts ""
  puts "\t-F (string) currently existing file name, or name of output ROOT file (no default)[Optional]"
  puts "\t-t (string) name of team whose stats to plot (no default)[Required if -F not used]"
  puts "\t-H calculate and plot gameScore percentages"
  puts "\t-s (int) (double) srs value to use for weights and histograms (default: \"fixed\")[Optional]"
  puts "\t-g draw game-by-game statistics [Optional]"
  puts "\t-h print this message"
  puts ""
  puts "Ex: ruby showTeamStats.rb -t \"2015 north carolina\" -H -s 0.5"
  puts
  puts "This program is used to show the results of the showTeamStats program, but"
  puts "it can also be used to run that program first and then process the output, so as to make"
  puts "this a one-step process.  If a ROOT file already exists, you can plot its graphs"
  puts "using the -F switch to set the input file name. If a ROOT file does not already exist,"
  puts "then you can create one by using the -t option to set the name of the team you want to plot."
  puts "If you wish to save the ROOT file created, simply use the"
  puts "-F option to set the name, it will be saved by default in the rootFiles/ directory, so"
  puts "only the name is necessary.  If no name is set, then a temporary file is created which"
  puts "is deleted at the end of the program."
  puts ""
end

fileName = nil
drawGames = false
teamName = nil
useHistograms = false
srsVal = "free"
#command line switcher
ARGV.each_with_index do |entry, index|
  case entry
  when /^-F/
    fileName = ARGV[index+1]
  when /^-H/
    useHistograms = true
  when /^-s/
    srsVal = ARGV[index+1]
  when /^-t/
    teamName = ARGV[index+1]
  when /^-D|-d|-g|-G/
    drawGames = true
  end
end

file = nil
if teamName.nil?
  if fileName.nil?
    puts "Must enter a teamName or a fileName"
    exit
  else
    file = TFile.new(fileName)
  end
else
  if fileName.nil?
    if !useHistograms
      `$CLION/showTeamStats -t \"#{teamName}\" -o \"temp.root\"`
    else
      `$CLION/showTeamStats -t \"#{teamName}\" -o \"temp.root\" -H -s #{srsVal}`
    end
    file = TFile.new("rootFiles/temp.root")
  else
    if !useHistograms
      `$CLION/showTeamStats -t \"#{teamName}\" -o \"#{fileName}\"`
    else
      `$CLION/showTeamStats -t \"#{teamName}\" -o \"#{fileName}\" -H -s #{srsVal}`
    end
    file = TFile.new("rootFiles/#{fileName}")
  end
end

avgs_per_time = {}
avgs_per_game = {}
wavgs_per_time = {}
wavgs_per_game = {}
league_avgs = {}
lines = {}
realLines = {}
ss = ["oor.p","oefg.p","oto.p","oftmr.p","srs",
      "dor.p","defg.p","dto.p","dftmr.p","rpi"]

ss.each do |s|
  avgs_per_time[s] = gROOT.FindObject("avg_per_time_#{s.gsub(".","")}")
  wavgs_per_time[s] = gROOT.FindObject("wavg_per_time_#{s.gsub(".","")}")
  avgs_per_game[s] = gROOT.FindObject("avg_per_game_#{s.gsub(".","")}")
  wavgs_per_game[s] = gROOT.FindObject("wavg_per_game_#{s.gsub(".","")}")
  #For some idiot reason I don't understand I can't draw a line read in this way
  #so I have to make a new TLine object using it.
  lines[s] = gROOT.FindObject("#{s.gsub(".","")}Line")
  realLines[s] = TLine.new(lines[s].GetX1,lines[s].GetY1,lines[s].GetX2,lines[s].GetY2)
end

if useHistograms
  avgs_per_time["gameScore"] = gROOT.FindObject("avg_per_time_gameScore")
  wavgs_per_time["gameScore"] = gROOT.FindObject("wavg_per_time_gameScore")
  realLines["gameScore"] = realLines["rpi"]
end

can = TCanvas.new("can","",0,0,1300,700)
can.Divide(5,2)

ss.each_with_index do |s,i|
  can.cd(i+1)
  avgs_per_time[s].SetMinimum(0)
  avgs_per_time[s].SetMaximum(1)
  avgs_per_time[s].SetMinimum(-30) if s == "srs"
  avgs_per_time[s].SetMaximum(30) if s == "srs"
  avgs_per_time[s].SetMarkerStyle(4)
  avgs_per_time[s].SetMarkerColor(1)
  avgs_per_time[s].SetLineColor(1)
  avgs_per_time[s].SetMarkerSize(0.5)
  avgs_per_time[s].Draw("psame")
  wavgs_per_time[s].SetMarkerStyle(8)
  wavgs_per_time[s].SetMarkerColor(4)
  wavgs_per_time[s].SetMarkerSize(0.5)
  wavgs_per_time[s].Draw("psame")

  if drawGames
    avgs_per_game[s].SetMarkerStyle(8)
    avgs_per_game[s].SetMarkerColor(1)
    avgs_per_game[s].SetMarkerSize(0.5)
    avgs_per_game[s].Draw("psame")
    wavgs_per_game[s].SetMarkerStyle(8)
    wavgs_per_game[s].SetMarkerColor(2)
    wavgs_per_game[s].SetMarkerSize(0.5)
    wavgs_per_game[s].Draw("psame")
  end

  realLines[s].SetLineColor(2)
  realLines[s].Draw("same")
end

if useHistograms
  can.cd(10)
  avgs_per_time["gameScore"].SetMinimum(0)
  avgs_per_time["gameScore"].SetMaximum(1)
  avgs_per_time["gameScore"].SetMarkerStyle(4)
  avgs_per_time["gameScore"].SetMarkerColor(1)
  avgs_per_time["gameScore"].SetLineColor(1)
  avgs_per_time["gameScore"].SetMarkerSize(0.5)
  avgs_per_time["gameScore"].Draw("p")
  wavgs_per_time["gameScore"].SetMarkerStyle(8)
  wavgs_per_time["gameScore"].SetMarkerColor(4)
  wavgs_per_time["gameScore"].SetMarkerSize(0.5)
  wavgs_per_time["gameScore"].Draw("psame")

  realLines["gameScore"].SetLineColor(2)
  realLines["gameScore"].Draw("same")
end

if (!teamName.nil? and fileName.nil?)
  `rm rootFiles/temp.root`
end

##This sets it so that the application ends when a canvas is closed. 
##Without it the application can hang in the background if you 
##don't exit properly.
gApplication.Connect("TCanvas","Closed()","TApplication",gApplication,
                     "Terminate()")
##If runROOT selected the window will stay until closed,  
##If not selected the window goes away after the fits run. 
 gApplication.Run# if runROOT             
