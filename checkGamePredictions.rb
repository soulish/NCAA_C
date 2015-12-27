require 'libRuby'
gStyle.SetOptStat(0)
gStyle.SetTitleFillColor(10)
gStyle.SetTitleBorderSize(0)
gStyle.SetPalette(1,0)

def printOptions
  puts ""
  puts "checkGamePredictions.rb Usage options"
  puts ""
  puts "\t-F (string) currently existing file name, or name of output ROOT file (no default)[Optional]"
  puts "\t-C Create new ROOT file? If not chosen then -F must be set. If chosen"
  puts "\t\t\t and -F is set, a new ROOT file will be created using the checkGamePredictions"
  puts "\t\t\t program with this name.  If chosen and -F is not set, a temporary file will be created."
  puts "\t\t\t (false)[Optional]"
  puts "\t-y (int,int,...) comma-separated list of input years. (no default)[Required if -C used]"
  puts "\t-s (double) SRS value to use (default: \"free\")[Optional with -C]"
  puts "\t-S (double) number of sigmas to use in calculations (default: 3)[Optional with -C]"
  puts "\t-n (int) number of bins to use in the histograms (default: 33)[Optional with -C]"
  puts "\t-R run ROOT? Canvas drawn if selected [Optional]"
  puts "\t-h print this message"
  puts ""
  puts "Ex: ruby checkGamePredictions.rb -y 2015 -H -C -R -s 0.5"
  puts
  puts "This program is used to show the results of the checkGamePredictions.cpp program, but"
  puts "it can also be used to run that program first and then process the output, so as to make"
  puts "this a one-step process.  If a ROOT file already exists, you can plot its graphs"
  puts "using the -F switch to set the input file name.  If a ROOT file does not already exist,"
  puts "then you can use the -C switch to create one, and then you must specify the input years,"
  puts "and optionally the SRS value to use and the number of sigmas to use.  This program gets"
  puts "the number of wins per predicted winning percentage bin and divides that by the number"
  puts "of total games in that bin to get a graph of the actual winning percentage vs the predicted"
  puts "ones.  The resulting plot is then fit with a first-order polynomial.  If everything has gone"
  puts "well, then the resulting fit should be a line with intercept of 0 and slope of 1.  The"
  puts "parameters of the fit are output on the screen, to see the actual plots, use the -R switch."
  puts ""
end

fileName = nil
srsVal = "free"
years = nil
sigmas = nil
create = false
runROOT = false
nbins = nil

#command line switcher
ARGV.each_with_index do |entry, index|
  case entry
  when /^-C|-c/
    create = true
  when /^-F/
    fileName = ARGV[index+1]
  when /^-y/
    years = ARGV[index+1]
  when /^-s/
    srsVal = ARGV[index+1]
  when /^-S/
    sigmas = ARGV[index+1]
  when /^-n/
    nbins = ARGV[index+1]
  when /^-R/
    runROOT = true
  when /^-h/
    printOptions
    exit
  end
end

file = nil
if create
  if years.nil?
    puts "When creating a file, you must specify the input years with the -y switch."
    printOptions
    exit
  end

  results = nil
  if fileName.nil?
    results = `$CLION/checkGamePredictions -y #{years} -o predictions.root -H #{srsVal.nil? ? "" : "-s #{srsVal}"} #{sigmas.nil? ? "" : "-S #{sigmas}"} #{nbins.nil? ? "" : "-n #{nbins}"}`
    file = TFile.new("rootFiles/predictions.root")
  else
    results = `$CLION/checkGamePredictions -y #{years} -o #{fileName} -H #{srsVal.nil? ? "" : "-s #{srsVal}"} #{sigmas.nil? ? "" : "-S #{sigmas}"} #{nbins.nil? ? "" : "-n #{nbins}"}`
    file = TFile.new("rootFiles/#{fileName}")
  end
  puts results.split("\n")[-5]
  puts results.split("\n")[-4]
  puts results.split("\n")[-3]
  puts results.split("\n")[-2]
  puts results.split("\n")[-1]
else
  if fileName.nil?
    puts "Must enter a file name with -F switch, or use the -C switch to create the file"
    printOptions
    exit
  else
    file = TFile.new(fileName)
  end
end

h_wins = gROOT.FindObject("h_wins")
h_total = gROOT.FindObject("h_total")

h_wins.Divide(h_total)

can = TCanvas.new("can","",0,0,1300,700)
can.Divide(2,1)

can.cd(1)
h_total.SetTitle(";Predicted Winning Percentage;Number of Games")
h_total.GetYaxis.SetTitleOffset(1.6)
h_total.SetMarkerStyle(20)
h_total.SetMarkerColor(1)
h_total.Draw("p")

can.cd(2)
h_wins.SetTitle(";Predicted Winning Percentage;Actual Winning Percentage")
h_wins.GetYaxis.SetTitleOffset(1.2)
h_wins.SetMarkerStyle(20)
h_wins.SetMarkerColor(1)
h_wins.Draw("p")
fn = TF1.new("fn","pol1")
h_wins.Fit(fn,"q")
puts
printf("Linear fit function: %.3f + %.3fx\n", fn.GetParameter(0),fn.GetParameter(1))

if (create and fileName.nil?)
  `rm rootFiles/predictions.root`
end

##This sets it so that the application ends when a canvas is closed. 
##Without it the application can hang in the background if you 
##don't exit properly.
gApplication.Connect("TCanvas","Closed()","TApplication",gApplication,
                     "Terminate()")
##If runROOT selected the window will stay until closed,  
##If not selected the window goes away after the fits run. 
gApplication.Run if runROOT             
