import psutil
import subprocess
import time

binary_command = ['./thread-server']

output_file = 'output_log.txt'

with open(output_file, 'a') as log_file:
    process = subprocess.Popen(binary_command)

    log_file.write("Time, CPU Utilization (%), Memory Utilization (MB)\n")

    try:
        while True:
            cpu_percent = psutil.cpu_percent(interval=0.1)
            memory_usage = psutil.Process(process.pid).memory_info().rss / (1024 * 1024)  # in MB
            current_time = time.strftime('%Y-%m-%d %H:%M:%S')

            log_file.write(f"{current_time}, {cpu_percent:.2f}, {memory_usage:.2f}\n")
            log_file.flush()
    except KeyboardInterrupt:
        pass
