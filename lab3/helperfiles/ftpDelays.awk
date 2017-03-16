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


#This code is finding the average end-to-end delay of the packets of the ftp connection




#For lines corresponding to enqueue event at node 0, we save the
#enqueueing times in an associative array indexed by packet ID
 ($1 == "+") && ($3 == 0) {
    enq_times[$12] = $2; 
 }

#For lines corresponding to the receive event of the ftp flow at node 3, we save the
#packet reception time also in a similar associative
#array. Additionally, we need to save the IDs of the packets received
#in another array, and count the number of packets received.
 ($1 == "r") && ($4 == 3) && ($10 == 3.0)   { 
    rec_times[$12] = $2; 
    num_packs++; 
    pack[num_packs]=$12;
}
END {


    for (i=1; i <= num_packs; i++) {
	id = pack[i];        
	delay =  rec_times[id] - enq_times[id];
	sum += delay;
#print out raw trace of delays
#Uncomment if you want to see the trace
#	print id,":   ", delay; 
    }
#print out average
    print "Average delay = " sum/num_packs;
}
	
	
	   