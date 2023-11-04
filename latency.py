from scapy.all import rdpcap
from collections import defaultdict

def calculate_rtt(packet_list):
    tcp_flows = defaultdict(list)
    for packet in packet_list:
        if 'TCP' in packet:
            flow_key = (packet['IP'].src, packet['TCP'].sport, packet['IP'].dst, packet['TCP'].dport)
            if packet['TCP'].flags & 0x12:  # SYN-ACK flags set
                tcp_flows[flow_key].append(packet.time)
            elif packet['TCP'].flags & 0x10:  # ACK flag set
                if flow_key in tcp_flows:
                    tcp_flows[flow_key].append(packet.time)
    
    rtt_results = {}
    for flow_key, times in tcp_flows.items():
        if len(times) >= 2:
            total_time = times[-1] - times[0]
            avg_rtt = total_time / (len(times) - 1) 
            rtt_results[flow_key] = [1000*total_time, 1000*avg_rtt]

    return rtt_results

def calculate_bytes_transferred(packet_list):
    byte_counts = defaultdict(int)
    for packet in packet_list:
        if 'TCP' in packet and 'IP' in packet:
            flow_key = (packet['IP'].src, packet['TCP'].sport, packet['IP'].dst, packet['TCP'].dport)
            byte_counts[flow_key] += len(packet)

    return byte_counts

pcap_file = 'temp.pcap'

packets = rdpcap(pcap_file)

rtt_results = calculate_rtt(packets)
byte_counts = calculate_bytes_transferred(packets)

print("RTT and Bytes Transferred for TCP Flows:")
for flow_key in rtt_results:
    rtt = rtt_results[flow_key]
    bytes_transferred = byte_counts.get(flow_key, 0)
    print(f"Flow: {flow_key} - RTT: {rtt[1]:.6f}ms, Bytes Transferred: {bytes_transferred}, Total Time: {rtt[0]:.6f}")

