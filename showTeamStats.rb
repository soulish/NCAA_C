require 'libRuby'
gStyle.SetOptStat(0)
gStyle.SetTitleFillColor(10)
gStyle.SetTitleBorderSize(0)
gStyle.SetPalette(1,0)

fileName = nil
drawGames = false
teamName = ""
#command line switcher
ARGV.each_with_index do |entry, index|
  case entry
  when /^-F/
    fileName = ARGV[index+1]
  when /^-t/
    teamName = ARGV[index+1]
  when /^-D|-d|-g|-G/
    drawGames = true
  end
end

file = nil
if (teamName.nil? and fileName)
  file = TFile.new(fileName)
elsif (!teamName.nil?)
  `$CLION/showTeamStats -t \"#{teamName}\" -o \"temp.root\"`
  sleep 3
  file = TFile.new("rootFiles/temp.root")
else
  puts "Either input a team name using the -t switch, or a file name using the -F switch"
  exit
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

can = TCanvas.new("can","",0,0,1300,700)
can.Divide(5,2)

ss.each_with_index do |s,i|
  can.cd(i+1)
  avgs_per_time[s].SetMinimum(0)
  avgs_per_time[s].SetMaximum(1)
  avgs_per_time[s].SetMinimum(-30) if s == "srs"
  avgs_per_time[s].SetMaximum(30) if s == "srs"
  avgs_per_time[s].SetMinimum(0) if s == "rpi"
  avgs_per_time[s].SetMaximum(1) if s == "rpi"
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

  # lines[s] = TLine.new(0,league_avgs[s],180,league_avgs[s])
  realLines[s].SetLineColor(2)
  realLines[s].Draw("same")
end

##This sets it so that the application ends when a canvas is closed. 
##Without it the application can hang in the background if you 
##don't exit properly.
gApplication.Connect("TCanvas","Closed()","TApplication",gApplication,
                     "Terminate()")
##If runROOT selected the window will stay until closed,  
##If not selected the window goes away after the fits run. 
 gApplication.Run# if runROOT             
