import itertools
import subprocess


names = ['R', 'T', 'E', 'l']
param_grid = [
    [1, 5, 10, 15],
    [10, 20, 30],
    [30, 40, 50],
    [1, 10, 100, 1000, 2000],
]


config_template = """NearestNeighbors:    1

lsh_k:               4
lsh_L:               5

cube_k:              7
cube_M:              6000
cube_probes:         10

graph_approx:        1
graph_k:             150

"""

config_args = """graph_R:             {R}
graph_T:             {T}
graph_E:             {E}
graph_l:             {l}
"""

for i, params in enumerate(list(itertools.product(*param_grid))):
    config_args_formatted = config_args.format(**dict(zip(names, params)))
    config_content = config_template + config_args_formatted

    with open('../bench.conf', 'w') as config_file:
        config_file.write(config_template + config_args_formatted)

    with open(f'../../output//configs/bench{i}.conf', 'w') as config_file:
        config_file.write(config_args_formatted)

    process = subprocess.run(['../benchmarks.bash', str(i), 'clean'])
