echo "CBR Delay"
awk -f cbrDelays.awk simple.tr

echo "CBR Throughput"
awk -f cbrTput.awk simple.tr

echo "FTP Delay"
awk -f ftpDelays.awk simple.tr

echo "FTP Throughput"
awk -f ftpTput.awk simple.tr

