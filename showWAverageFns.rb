require 'libRuby'
gStyle.SetOptStat(0)
gStyle.SetTitleFillColor(10)
gStyle.SetTitleBorderSize(0)
gStyle.SetPalette(1,0)

def printOptions
  puts ""
  puts "showWAverageFns.rb Usage options"
  puts ""
  puts "\t-F (string) currently existing file name, or name of output ROOT file (no default)[Optional]"
  puts "\t-C Create new ROOT file? If not chosen then -F must be set. If chosen"
  puts "\t\t\t and -F is set, a new ROOT file will be created using the generateWAverageFunctions"
  puts "\t\t\t program with this name.  If chosen and -F is not set, a temporary file will be created."
  puts "\t\t\t (false)[Optional]"
  puts "\t-o|-O (string) name of output text file (no default)[Optional]"
  puts "\t-y (int,int,...) comma-separated list of input years. (no default)[Required if -C used]"
  puts "\t-Y (int) output year (no default)[Required if -o|-O used]"
  puts "\t-n (int) number of bins used for histograms (no default)[Required]"
  puts "\t-v|-V verbose mode [Optional]"
  puts "\t-R run ROOT? Canvas drawn if selected [Optional]"
  puts ""
  puts "Ex: ruby showWAverageFns.rb -C -y 2010,2011,2012,2013,2014 -Y 2015 -F functions2015_301.root"
  puts "\t\t\t-n 301 -o constants/waverage_functions.d"
  puts
  puts "This program is used to show the results of the generateWAverageFunctions program, but"
  puts "it can also be used to run that program first and then process the output, so as to make"
  puts "this a one-step process.  If a ROOT file already exists, you can plot its graphs"
  puts "using the -F and -n switches to set the input file name and the number of bins used."
  puts "If a ROOT file does not already exist, then you can create one by using the -C option,"
  puts "and the -y and -n options.  If you wish to save the ROOT file created, simply use the"
  puts "-F option to set the name, it will be saved by default in the rootFiles/ directory, so"
  puts "only the name is necessary.  If no name is set, then a temporary file is created which"
  puts "is deleted at the end of the program."
  puts ""
  puts "The results of the fits are written to the screen by default, if you wish to write the"
  puts "output to a text file, simply use the -o option and provide a name."
  puts ""
  puts "If the -R option is chosen, then the canvases will be drawn, otherwise they will not."
  puts ""
end

class Float
  def to_n(n)
    sprintf("%.#{n}f",self)
  end
end

fileName = nil
years = nil
outYear = nil
nbins = nil
create = false
verbose = false
outFileName = nil
runROOT = false

#command line switcher
ARGV.each_with_index do |entry, index|
  case entry
  when /^-C|-c/
    create = true
  when /^-V|-v/
    verbose = true
  when /^-F/
    fileName = ARGV[index+1]
  when /^-o|-O/
    outFileName = ARGV[index+1]
  when /^-y|--year/
    years = *ARGV[index + 1].split(",")
  when /^-Y/
    outYear = ARGV[index + 1].to_i
  when /^-n/
    nbins = ARGV[index + 1].to_i
  when /^-R/
    runROOT = true
  when /^-h/
    printOptions
    exit
  end
end

file = nil
if create
  if (years.nil? or nbins.nil?)
    puts "When creating a file, you must specify the input years with"
    puts "the -y switch, and the number of bins with the -n switch."
    exit
  end

  if fileName.nil?
    `$CLION/generateWAverageFunctions -y #{years.join(",")} -o fns.root -n #{nbins}`
    file = TFile.new("rootFiles/fns.root")
  else
    `$CLION/generateWAverageFunctions -y #{years.join(",")} -o #{fileName} -n #{nbins}`
    file = TFile.new("rootFiles/#{fileName}")
  end
else
  if fileName.nil?
    puts "Must enter a file name with -F switch, or use the -C switch to create the file"
    exit
  else
    file = TFile.new(fileName)
  end
end

hist = {}; hist_slices = {}; graphs = {}; grs = {}; fns = {}
averaged_graphs = {}
ss = ["or","efg","ftmr","to","ppp"]
range = {}
#order is low edge, high edge, nbins
range["or"] =   [0.1,0.7,nbins]
range["efg"] =  [0.2,0.8,nbins]
range["ftmr"] = [0.0,0.6,nbins]
range["to"] =   [0.05,0.35,nbins]
range["ppp"] =  [0.3,1.3,nbins]

ss.each do |s|
  hist[s] = gROOT.FindObject("hist_#{s}")
  graphs[s] = gROOT.FindObject("graph_#{s}")
  averaged_graphs[s] = gROOT.FindObject("averaged_graph_#{s}")
  #fns[s] = gROOT.FindObject("fns_#{s}")

  # hist_slices[s] = []
  # grs[s] = []
  # range[s][2].times do |i| 
  #   hist_slices[s][i] = gROOT.FindObject("hist_slices_#{s}_#{i}")
  #   #grs[s][i] = gROOT.FindObject("gr_slices_#{s}_#{i}")
  # end
end

cans = {}; can_slices = {}; can_fits = {}
cans["main"] = TCanvas.new("main","Main",0,0,1300,700)
cans["main"].Divide(4,3)

outFile = nil
if outFileName
  outFile = File.new(outFileName,"a")
  outFile.print "#{outYear}"
end

ss.each_with_index do |s, ind|
  cans["main"].cd(2*ind+1)
  hist[s].SetAxisRange(range[s][0],range[s][1],"X")
  hist[s].SetAxisRange(range[s][0],range[s][1],"Y")
  #hist[s].SetTitle("d#{s} vs o#{s}")
  hist[s].Draw("colz")

  #can_slices[s] = TCanvas.new("can_slice_#{s}","Slices #{s}",0,0,1300,700)
  #can_slices[s].Divide(10,17)
  #can_fits[s] = TCanvas.new("can_fits_#{s}","Fits #{s}",0,0,1300,700)
  #can_fits[s].Divide(10,17)

  # range[s][2].times do |i|
  #   can_slices[s].cd(i+1)
  #   hist_slices[s][i].SetAxisRange(range[s][0],range[s][1],"X")
  #   hist_slices[s][i].SetAxisRange(range[s][0],range[s][1],"Y")
  #   hist_slices[s][i].Draw("colz")

  #   #can_fits[s].cd(i+1)
  #   #grs[s][i].SetMarkerStyle(20)
  #   #grs[s][i].Draw("ap")
  # end

  cans["main"].cd(2*ind+2)
  graphs[s].SetMarkerStyle(20)
  #graphs[s].SetTitle("#{s} result vs difference")
  graphs[s].SetMinimum(0)
  graphs[s].SetMaximum(2)
  graphs[s].Draw("ap")

  averaged_graphs[s].SetMarkerColor(4)
  averaged_graphs[s].SetLineColor(4)
  averaged_graphs[s].SetMarkerStyle(20)
  averaged_graphs[s].Draw("psame")

  if verbose
    puts "Function parameters for #{s}:\t\t#{averaged_graphs[s].GetFunction("fn_#{s}").GetParameter(0).to_n(3)}\t#{averaged_graphs[s].GetFunction("fn_#{s}").GetParameter(1).to_n(3)}"
  end

  if outFileName
    outFile.print ",o#{s}.p,#{averaged_graphs[s].GetFunction("fn_#{s}").GetParameter(0).to_n(3)},#{averaged_graphs[s].GetFunction("fn_#{s}").GetParameter(1).to_n(3)}"
  end
end

outFile.puts if outFileName
outFile.close if outFileName

#clean up the temporary file
if (create and fileName.nil?)
  `rm rootFiles/fns.root`
end




##This sets it so that the application ends when a canvas is closed. 
##Without it the application can hang in the background if you 
##don't exit properly.
gApplication.Connect("TCanvas","Closed()","TApplication",gApplication,
                     "Terminate()")
##If runROOT selected the window will stay until closed,  
##If not selected the window goes away after the fits run. 
gApplication.Run if runROOT             
