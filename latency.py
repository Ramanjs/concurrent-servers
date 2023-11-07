from scapy.all import rdpcap
from collections import defaultdict
import matplotlib.pyplot as plt

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

def avg_data(pcap_file):
    packets = rdpcap(pcap_file)

    rtt_results = calculate_rtt(packets)
    byte_counts = calculate_bytes_transferred(packets)

    avg_rtt = 0
    avg_bytes = 0
    avg_time = 0
    cnt = 0
    for flow_key in rtt_results:
        rtt = rtt_results[flow_key]
        bytes_transferred = byte_counts.get(flow_key, 0)
        avg_rtt += rtt[1]
        avg_bytes += bytes_transferred 
        avg_time += rtt[0] 
        cnt += 1

    avg_rtt /= cnt
    avg_time /= cnt
    avg_bytes /= cnt

    return [float(avg_rtt), float(avg_bytes), float(avg_time)]

def get_server_data(files):
    data_list = []
    for file in files:
        data_list.append(avg_data(file))

    return data_list

files = [
         ['fork-data/fork100.pcap', 'fork-data/fork200.pcap', 'fork-data/fork300.pcap', 'fork-data/fork400.pcap', 'fork-data/fork500.pcap'],
         ['thread-data/thread100.pcap', 'thread-data/thread200.pcap', 'thread-data/thread300.pcap', 'thread-data/thread400.pcap', 'thread-data/thread500.pcap'],
         ['select-data/select100.pcap', 'select-data/select200.pcap', 'select-data/select300.pcap', 'select-data/select400.pcap', 'select-data/select500.pcap'],
         ['poll-data/poll100.pcap', 'poll-data/poll200.pcap', 'poll-data/poll300.pcap', 'poll-data/poll400.pcap', 'poll-data/poll500.pcap'],
         ['epoll-data/epoll100.pcap', 'epoll-data/epoll200.pcap', 'epoll-data/epoll300.pcap', 'epoll-data/epoll400.pcap', 'epoll-data/epoll500.pcap'],
         ]

def get_servers(files):
    servers = []
    for file in files:
        servers.append(get_server_data(file))

    return servers

servers = get_servers(files)

def calculate_throughput(data):
    return [((8000*data[i][1]) / data[i][2]) for i in range(len(data))]

concurrent_requests = [500, 1000, 2000, 2500, 3000]

colors = ['red', 'blue', 'green', 'orange', 'purple']
server_names = ['fork', 'thread', 'select', 'poll', 'epoll']

plt.figure(figsize=(10, 6))

i = 0
for server in servers:
    # Calculate throughput for the current server
    throughput = calculate_throughput(server)

    # Plot the throughput for the current server
    plt.plot(concurrent_requests, throughput, label=server_names[i], color=colors[i])
    i += 1

plt.xlabel('Number of Concurrent Requests')
plt.ylabel('Throughput')
plt.title('Throughput of Different Backend Servers')
plt.legend()
plt.grid(True)
plt.show()
