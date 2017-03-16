
# Guide to the trace fields:

# 1  Type Identifier:
# “+”: a packet enque event
# “-”: a packet deque event
# “r”: a packet reception event
# “d”: a packet drop (e.g., sent to dropHead_) event

# 2. Time: at which the packet tracing string is created.
# 3-4. Source Node and Destination Node (link level)
# 5. Packet type: tcp/cbr 
# 6. Packet Size: Size of the packet in bytes.
# 7. Flags: A 7-digit flag string: "-------". 
# 8. Flow ID: 1 is ftp and 2 is cbr in our case.
# 9-10. Source Address and Destination Address: addresses of the  CONNECTION end-points in a "node_id.port_number" format. 
# In our case: ftp: (0.0, 3.0),  cbr: (1.0, 3.1)
# 11. Sequence Number
# 12. Packet Unique ID


# Use awk field variables to select the lines you want to process. 
# E.g. this code finds the throughput of the ftp connection by finding the receive rate of these packets at node 3

    ($1 == "r") && ($4==3) &&  ($10 == 3.0) {total_data_received += $6; time_of_last_reception = $2; num_packs++ }
END {
    ftp_flow_start_time = 0.1; # configured in the tcl file;
    total_time_taken = time_of_last_reception -  ftp_flow_start_time ;
    print total_data_received, time_of_last_reception, num_packs;
    print "throughput =",  total_data_received/(total_time_taken*1000), "KB/sec";
    }

	
	
	   