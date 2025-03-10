import sys
from collections import defaultdict

def read_adj_list(file_path):
    adj_list = defaultdict(list)
    with open(file_path, 'r') as file:
        for line in file:
            if ':' in line:
                parts = line.split(':')
                node = int(parts[0].strip())
                if len(parts) > 1:
                    edges = list(map(int, parts[1].strip().split()))
                    adj_list[node].extend(edges)
    return adj_list

def write_adj_list(adj_list, file_path):
    with open(file_path, 'w') as file:
        for node in sorted(adj_list.keys()):
            edges = sorted(set(adj_list[node]))
            file.write(f"{node}: {' '.join(map(str, edges))}\n")

def merge_adj_lists(file1, file2, output_file):
    adj_list1 = read_adj_list(file1)
    adj_list2 = read_adj_list(file2)
    merged_adj_list = defaultdict(list, adj_list1)
    
    for node, edges in adj_list2.items():
        merged_adj_list[node].extend(edges)
    
    write_adj_list(merged_adj_list, output_file)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Uso: python merge_adj_lists.py <archivo1> <archivo2> <archivo_salida>")
        sys.exit(1)
    
    file1 = sys.argv[1]
    file2 = sys.argv[2]
    output_file = sys.argv[3]

    merge_adj_lists(file1, file2, output_file)
