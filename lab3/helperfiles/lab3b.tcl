#Create a simulator object
set ns [new Simulator]

#Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red

#Open the NAM trace file
set nf [open out.nam w]
$ns namtrace-all $nf

#open the trace file
set f [open simple.tr w]
$ns trace-all $f


#Define a 'finish' procedure
proc finish {} {
        global ns nf f
        $ns flush-trace
        #Close the NAM trace file
        close $nf
	close $f
        #Execute NAM on the trace file
        exec nam out.nam &
#Comment above line out if you don't want to see animation
        exit 0
}

#Create four nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

#Create links between the nodes

#ftp source to router
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
#cbr source to router
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
#router to destination
$ns duplex-link $n2 $n3 3Mb 20ms DropTail

#Set Queue Size of link (n2-n3) to 10
$ns queue-limit $n2 $n3 10

#Give node position (for NAM)
$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link-op $n2 $n3 orient right


#Monitor the queue for link (n2-n3). (for NAM)
$ns duplex-link-op $n2 $n3 queuePos 0.5


#Setup a TCP connection
set tcp [new Agent/TCP]
$tcp set class_ 2
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n3 $sink
$ns connect $tcp $sink
$tcp set fid_ 1

#set TCP sliding window to a fixed size
#CHANGE BOTH FOR CHANGING WINDOW SIZE
$tcp set windowInit_ 20 
$tcp set maxcwnd_ 20

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP


#Setup a UDP connection
set udp [new Agent/UDP]
$ns attach-agent $n1 $udp
set null [new Agent/Null]
$ns attach-agent $n3 $null
$ns connect $udp $null
$udp set fid_ 2

#Setup a CBR over UDP connection (1000 bytes every 10ms)
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set type_ CBR
$cbr set packet_size_ 2400
$cbr set interval_ 0.01
$cbr set random_ false

$ns at 0.0 "$n0 label ftp-source" 
$ns at 0.0 "$n1 label cbr-source" 
$ns at 0.0 "$n2 label router" 
$ns at 0.0 "$n3 label destination" 

#Schedule events for the CBR and FTP agents
$ns at 0.1 "$cbr start"
$ns at 0.1 "$ftp start"
$ns at 4.5 "$ftp stop"
$ns at 4.5 "$cbr stop"

#Detach tcp and sink agents (not really necessary)
$ns at 4.5 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n3 $sink"

#Call the finish procedure after 5 seconds of simulation time
$ns at 5.0 "finish"

#Print CBR packet size and interval
puts "CBR packet size = [$cbr set packet_size_]"
puts "CBR interval = [$cbr set interval_]"

#Run the simulation
$ns run

