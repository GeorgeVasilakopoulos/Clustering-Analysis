import os
import warnings
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 
warnings.filterwarnings("ignore")

from argparse import ArgumentParser

import numpy as np

from sklearn.model_selection import ParameterGrid
from sklearn.neighbors import NearestNeighbors

from train import get_dataset, train_model

def parse_args():
    parser = ArgumentParser(description='AutoEncoder w/ Tensorflow')

    parser.add_argument('--d', type=str, required=True, help='Path to training data')
    parser.add_argument('--q', type=str, required=True, help='Path to query data')

    return parser.parse_args()

cmd_args = parse_args()

param_grid = [

    {'depth': [2], 'planes': [[16, 8], [16, 32], [32, 16]], 
     'kernel_size': [3, 5], 'lr': [1e-4], 'batch_size': [64, 128],
     'norm': [False],
     'latent_dim': [10, 16, 22]},

    {'depth': [3], 'planes': [[32, 16, 8], [16, 32, 48], [48, 32, 16]], 
     'kernel_size': [3, 5], 'lr': [1e-4], 'batch_size': [64, 128],
     'norm': [False],
     'latent_dim': [10, 16, 22]},
]


train_data = get_dataset(cmd_args.d)
test_data = get_dataset(cmd_args.q)

train_data = train_data.reshape(train_data.shape[0], -1)
test_data = test_data.reshape(test_data.shape[0], -1)


knn = NearestNeighbors(n_neighbors=10, algorithm='brute', metric='euclidean')
knn.fit(train_data)
sample = np.random.choice(test_data.shape[0], size=100, replace=False)
query = test_data[sample, :]

_, true_indices = knn.kneighbors(query)

true_dist = np.stack([np.linalg.norm(q - train_data[i], axis=1) for i, q in zip(true_indices, query)]).reshape(-1)


with open('./results.csv', 'w') as file:
    file.write('Latent Dim\tPlanes\tDepth\tNorm\tKernel Size\tLR\tBatch Size\t')
    file.write('Accuracy\tAF\tMAF\tNeighbours\n')

params = list(ParameterGrid(param_grid))[34:]
print(f'Combinations: {len(params)}')

for i, args in enumerate(params):    
    print(f'\nStarting experiment {i + 1}!')
    args = [cmd_args.d, cmd_args.q, args['latent_dim'], args['planes'], args['depth'], 
            args['norm'], args['kernel_size'], args['lr'], args['batch_size']]
    
    latent_train_data, latent_test_data = train_model(*args, 10)
    
    latent_train_data = latent_train_data.reshape(latent_train_data.shape[0], -1)
    latent_test_data = latent_test_data.reshape(latent_test_data.shape[0], -1)
    
    queryLatent = latent_test_data[sample, :]

    knnLatent = NearestNeighbors(n_neighbors=10, algorithm='brute', metric='euclidean')
    knnLatent.fit(latent_train_data)

    _, latent_indices = knnLatent.kneighbors(queryLatent)
    latent_dist = np.stack([np.linalg.norm(q - train_data[i], axis=1) for i, q in zip(latent_indices, query)]).reshape(-1)

    acc = np.sum(true_indices[:, 0].reshape(-1) == latent_indices[:, 0].reshape(-1)) / np.prod(true_indices[:, 0].shape)
    af  = np.mean(latent_dist / true_dist)
    maf = np.max(latent_dist / true_dist)
    neibs = np.intersect1d(true_indices.reshape(-1), latent_indices.reshape(-1)).shape[0] / np.prod(true_indices.shape)

    with open('./results.csv', 'a') as file:
        for x in args:
            file.write(f'{x}\t')

        file.write(f'{acc}\t{af}\t{maf}\t{neibs}\n')
