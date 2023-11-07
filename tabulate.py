import matplotlib.pyplot as plt

files = [
         ['fork-data/fork100.txt', 'fork-data/fork200.txt', 'fork-data/fork300.txt', 'fork-data/fork400.txt', 'fork-data/fork500.txt'],
         ['thread-data/thread100.txt', 'thread-data/thread200.txt', 'thread-data/thread300.txt', 'thread-data/thread400.txt', 'thread-data/thread500.txt'],
         ['select-data/select100.txt', 'select-data/select200.txt', 'select-data/select300.txt', 'select-data/select400.txt', 'select-data/select500.txt'],
         ['poll-data/poll100.txt', 'poll-data/poll200.txt', 'poll-data/poll300.txt', 'poll-data/poll400.txt', 'poll-data/poll500.txt'],
         ['epoll-data/epoll100.txt', 'epoll-data/epoll200.txt', 'epoll-data/epoll300.txt', 'epoll-data/epoll400.txt', 'epoll-data/epoll500.txt'],
         ]


def get_max_values(file):
    txt = open(file, "r")
    lines = txt.readlines()

    max_cpu = 0.0
    max_mem = 0.0
    for line in lines:
        if (line[0] == '2'):
            words = line.split(' ')
            cpu = float(words[-2][:-1])
            mem = float(words[-1][:-1])
            max_cpu = max(max_cpu, cpu)
            max_mem = max(max_mem, mem)
        
    return [max_cpu, max_mem]

def get_row(files):
    row = []
    for file in files:
        row.append(get_max_values(file))

    return row

def get_cpu_rows(files):
    rows = []
    for file in files:
        row = get_row(file) 
        row = [x[0] for x in row]
        rows.append(row)

    return rows

def get_mem_rows(files):
    rows = []
    for file in files:
        row = get_row(file) 
        row = [x[1] for x in row]
        rows.append(row)

    return rows


concurrent_requests = [500, 1000, 2000, 2500, 3000]
servers = ['fork', 'thread', 'select', 'poll', 'epoll']

cpu_rows = get_cpu_rows(files)
mem_rows = get_mem_rows(files)

plt.axis(False)

plt.table(cellText=cpu_rows,
          rowLabels=servers,
          colLabels=concurrent_requests)

plt.tight_layout()
plt.show()
