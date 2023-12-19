import pandas as pd
import csv
import numpy as np
import matplotlib.pyplot as plt


def unique(list1):
    # insert the list to the set
    list_set = set(list1)
    # convert the set to the list
    unique_list = (list(list_set))
    return unique_list

def read_wireshark_csv(file_path):
    with open(file_path, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        data = [row for row in reader]
    return data

# Replace 'path_to_your_file.csv' with the path to your exported CSV file
file_path = rf'C:\Users\OtteC\OneDrive\Skrivebord\exp6.csv'
wireshark_data = read_wireshark_csv(file_path)

number_of_packets = 0

def infer_topology(data):
    global number_of_packets
    topology = {}
    for row in data:
        source = row['Source']
        destination = row['Destination']
        info = row['Info']

        if '5678  >  8765 Len=8' in info:
            number_of_packets +=1
            if source not in topology:
                topology[source] = destination
            if destination not in topology:
                topology[destination] = None

    return topology


network_topology = infer_topology(wireshark_data)
sources = network_topology.keys()

source_to_intermediate_nodes = {}

for source in sources:
    for row in wireshark_data:
        if source == row['Source']:
            if len(row['Destination']) > 8:
                if source_to_intermediate_nodes.__contains__(source):
                    source_to_intermediate_nodes[source].append(row['Destination'])
                else:
                    source_to_intermediate_nodes[source] = [row['Destination']]
        else:
            continue
    if source_to_intermediate_nodes.__contains__(source):
        source_to_intermediate_nodes[source] = unique(source_to_intermediate_nodes[source])

def calculate_min_hops(topology, sink):
    def find_path(node, sink, visited):
        if node == sink:
            return 0
        if node in visited:
            return float('inf')  # Avoid loops

        visited.add(node)
        min_hops = float('inf')
        for next_hop in topology.get(node, []):
            hops = find_path(next_hop, sink, visited)
            if hops < min_hops:
                min_hops = hops
        visited.remove(node)
        return min_hops + 1 if min_hops != float('inf') else float('inf')

    min_hops_to_sink = {}
    for node in topology.keys():
        min_hops_to_sink[node] = find_path(node, sink, set())

    return min_hops_to_sink

sink_node = 'fe80::212:7401:1:101'
min_hops = calculate_min_hops(source_to_intermediate_nodes, sink_node)

totalhops=0
for node, hops in min_hops.items():
    print(f"Node {node} requires {hops+1} hops to reach the sink.")
    totalhops+=hops+1

print(totalhops/len(source_to_intermediate_nodes))
print(len(source_to_intermediate_nodes))
print(f"Number of packets {number_of_packets}")