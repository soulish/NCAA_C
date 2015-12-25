require 'libRuby'
gStyle.SetOptStat(0)
gStyle.SetTitleFillColor(10)
gStyle.SetTitleBorderSize(0)
gStyle.SetPalette(1,0)

def printOptions
  puts ""
  puts "showTournamentMatchup.rb Usage options"
  puts ""
  puts "\t-F (string) currently existing file name, or name of output ROOT file (no default)[Optional]"
  puts "\t-t (string) teamnameA (no default)[Required if -F not used]"
  puts "\t-T (string) teamnameB (no default)[Required if -F not used]"
  puts "\t-H calculate and plot gameScore percentages"
  puts "\t-s (int) (double) srs value to use for weights and histograms (default: \"fixed\")[Optional]"
  puts "\t-h print this message"
  puts ""
  puts "Ex: ruby showTournamentMatchup.rb -t \"2015 north carolina\" -T \"2015 harvard\" -H -s 0.5"
  puts
  puts "This program is used to show the results of the showTournamentMatchup program, but"
  puts "it can also be used to run that program first and then process the output, so as to make"
  puts "this a one-step process.  If a ROOT file already exists, you can plot its graphs"
  puts "using the -F switch to set the input file name. If a ROOT file does not already exist,"
  puts "then you can create one by using the -t and -T options to set the names of the teams you want to plot."
  puts "If you wish to save the ROOT file created, simply use the"
  puts "-F option to set the name, it will be saved by default in the rootFiles/ directory, so"
  puts "only the name is necessary.  If no name is set, then a temporary file is created which"
  puts "is deleted at the end of the program."
  puts ""
end

fileName = nil
teamNameA = nil
teamNameB = nil
srsVal = "free"
useHistograms = false

#command line switcher
ARGV.each_with_index do |entry, index|
  case entry
  when /^-F/
    fileName = ARGV[index+1]
  when /^-s/
    srsVal = ARGV[index+1]
  when /^-H/
    useHistograms = true
  when /^-t/
    teamNameA = ARGV[index+1]
  when /^-T/
    teamNameB = ARGV[index+1]
  end
end

file = nil
if (teamNameA.nil? && teamNameB.nil?)
  if fileName.nil?
    puts "Must enter teamNameA and teamNameB or a fileName"
    exit
  else
    file = TFile.new(fileName)
  end
else
  if fileName.nil?
    if !useHistograms
      `$CLION/showTournamentMatchup -t \"#{teamNameA}\" -T \"#{teamNameB}\" -o \"temp.root\"`
    else
      `$CLION/showTournamentMatchup -t \"#{teamNameA}\" -T \"#{teamNameB}\" -o \"temp.root\" -H -s #{srsVal}`
    end
    #sleep 3
    file = TFile.new("rootFiles/temp.root")
  else
    if !useHistograms
      `$CLION/showTournamentMatchup -t \"#{teamNameA}\" -T \"#{teamNameB}\" -o \"#{fileName}\"`
    else
      `$CLION/showTournamentMatchup -t \"#{teamNameA}\" -T \"#{teamNameB}\" -o \"#{fileName}\" -H -s #{srsVal}`
    end
    #sleep 3
    file = TFile.new("rootFiles/#{fileName}")
  end
end

avgsA_per_time = {}
wavgsA_per_time = {}
avgsB_per_time = {}
wavgsB_per_time = {}
league_avgs = {}
lines = {}
realLines = {}
ss = ["oor.p","oefg.p","oftmr.p","oto.p",
      "dor.p","defg.p","dftmr.p","dto.p","srs","rpi"]

ss.each do |s|
  avgsA_per_time[s] = gROOT.FindObject("avgA_per_time_#{s.gsub(".","")}")
  wavgsA_per_time[s] = gROOT.FindObject("wavgA_per_time_#{s.gsub(".","")}")

  avgsB_per_time[s] = gROOT.FindObject("avgB_per_time_#{s.gsub(".","")}")
  wavgsB_per_time[s] = gROOT.FindObject("wavgB_per_time_#{s.gsub(".","")}")

  #For some idiot reason I don't understand I can't draw a line read in this way
  #so I have to make a new TLine object using it.
  lines[s] = gROOT.FindObject("#{s.gsub(".","")}Line")
  realLines[s] = TLine.new(lines[s].GetX1,lines[s].GetY1,lines[s].GetX2,lines[s].GetY2)
end

if useHistograms
  avgsA_per_time["gameScore"] = gROOT.FindObject("avgA_per_time_gameScore")
  wavgsA_per_time["gameScore"] = gROOT.FindObject("wavgA_per_time_gameScore")

  avgsB_per_time["gameScore"] = gROOT.FindObject("avgB_per_time_gameScore")
  wavgsB_per_time["gameScore"] = gROOT.FindObject("wavgB_per_time_gameScore")

  realLines["gameScore"] = realLines["rpi"]
end

can = TCanvas.new("can","",0,0,1300,700)
can.Divide(4,3)

ss.each_with_index do |s,i|
  can.cd(i+1)
  wavgsA_per_time[s].SetMinimum(0)
  wavgsA_per_time[s].SetMaximum(1)
  wavgsA_per_time[s].SetMinimum(-30) if s == "srs"
  wavgsA_per_time[s].SetMaximum(30) if s == "srs"
  wavgsA_per_time[s].SetMarkerStyle(4)
  wavgsA_per_time[s].SetMarkerColor(4)
  wavgsA_per_time[s].SetLineColor(4)
  wavgsA_per_time[s].SetMarkerSize(0.5)
  wavgsA_per_time[s].Draw("psame")
  # avgsA_per_time[s].SetMarkerStyle(8)
  # avgsA_per_time[s].SetMarkerColor(4)
  # avgsA_per_time[s].SetMarkerSize(0.5)
  # avgsA_per_time[s].Draw("psame")

  wavgsB_per_time[s].SetMarkerStyle(4)
  wavgsB_per_time[s].SetMarkerColor(2)
  wavgsB_per_time[s].SetLineColor(2)
  wavgsB_per_time[s].SetMarkerSize(0.5)
  wavgsB_per_time[s].Draw("psame")

  realLines[s].SetLineColor(2)
  realLines[s].Draw("same")
end

if useHistograms
  can.cd(11)
  wavgsA_per_time["gameScore"].SetMinimum(0)
  wavgsA_per_time["gameScore"].SetMaximum(1)
  wavgsA_per_time["gameScore"].SetMarkerStyle(4)
  wavgsA_per_time["gameScore"].SetMarkerColor(4)
  wavgsA_per_time["gameScore"].SetLineColor(4)
  wavgsA_per_time["gameScore"].SetMarkerSize(0.5)
  wavgsA_per_time["gameScore"].Draw("psame")
  # avgsA_per_time["gameScore"].SetMarkerStyle(8)
  # avgsA_per_time["gameScore"].SetMarkerColor(4)
  # avgsA_per_time["gameScore"].SetMarkerSize(0.5)
  # avgsA_per_time["gameScore"].Draw("psame")

  wavgsB_per_time["gameScore"].SetMarkerStyle(4)
  wavgsB_per_time["gameScore"].SetMarkerColor(2)
  wavgsB_per_time["gameScore"].SetLineColor(2)
  wavgsB_per_time["gameScore"].SetMarkerSize(0.5)
  wavgsB_per_time["gameScore"].Draw("psame")

  realLines["gameScore"].SetLineColor(2)
  realLines["gameScore"].Draw("same")
end

if (!teamNameA.nil? and fileName.nil?)
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
