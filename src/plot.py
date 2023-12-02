import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import sys

if len(sys.argv) != 2:
    print(f'Usage: python {sys.argv[0]} <plot path>')
    exit(0)

df = pd.read_csv('./output/out.csv')

df_acc   = df[df['Metric'] == 'Accuracy'].drop(columns=['Metric'])
df_af    = df[df['Metric'] == 'AF'].drop(columns=['Metric'])
df_maf   = df[df['Metric'] == 'MAF'].drop(columns=['Metric'])
df_rtime = df[df['Metric'] == 'RTime'].drop(columns=['Metric'])

def plot_curves(sizes, metrics, dfs, y_ticks):
    _, axes = plt.subplots(2, 2, figsize=(16,13))

    def plot(bmin, bmax, xvalues, LSH, Cube, GNNS, MRNG,
            xticks, yticks, xlabel, ylabel, i, j):

        ax = axes[i][j]
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel)

        ax.grid(visible=True)

        cond_lsh  = (LSH >= bmin) & (LSH <= bmax)
        cond_cube = (Cube >= bmin) & (Cube <= bmax)
        cond_gnns = (GNNS >= bmin) & (GNNS <= bmax)
        cond_mrng = (MRNG >= bmin) & (MRNG <= bmax)
        ax.plot(xvalues[cond_lsh], LSH[cond_lsh] , ".-", label="LSH")
        ax.plot(xvalues[cond_cube], Cube[cond_cube], ".-", label="Cube")
        ax.plot(xvalues[cond_gnns], GNNS[cond_gnns], ".-", label="GNNS")
        ax.plot(xvalues[cond_mrng], MRNG[cond_mrng], ".-", label="MRNG")

        ax.legend(loc="best")

        ax.set_xticks(xticks)
        ax.set_yticks(yticks)
        
    for i, ((bmin, bmax), metric, df, ticks) in enumerate(zip([(.2, 1.), (1. , 1.2), (1., 2), (0, 0.8)], metrics, dfs, y_ticks)):
        plot(
            bmin, bmax, sizes, *df.to_numpy().T,
            np.arange(0, 60001, 10000), ticks,
            "Training samples", metric, i // 2, i % 2
        )
        
    plt.savefig(f'./output/plots/plot{sys.argv[1]}', bbox_inches='tight')

plot_curves(
    np.array([100, 500, 1000, 2000, 5000, 10000, 20000, 30000, 40000, 60000]),
    ['Accuracy', 'Approximation Factor', 'Maximum Approximation Factor', 'Relative Time'],
    [df_acc, df_af, df_maf, df_rtime],
    [np.arange(0.2, 1.01, 0.1), np.arange(1., 1.16, .025), np.arange(1., 2.01, .1), np.arange(0.1, 1., .1)]
)