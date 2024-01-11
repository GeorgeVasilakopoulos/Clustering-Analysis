
from models.train import get_dataset, write_dataset
import keras

from argparse import ArgumentParser

import tensorflow as tf
tf.get_logger().setLevel('INFO')

def parse_args():
    parser = ArgumentParser(description='AutoEncoder w/ Tensorflow')

    parser.add_argument('--d', type=str, 
                        default='/mnt/c/Users/10geo/Documents/GitHub/Project/input/train_images', 
                        help='Path to training data')
    parser.add_argument('--q', type=str, 
                        default='/mnt/c/Users/10geo/Documents/GitHub/Project/input/test_images', 
                        help='Path to query data')
    parser.add_argument('--od', type=str, 
                        default='/mnt/c/Users/10geo/Documents/GitHub/Project/input/latent_train_images', 
                        help='Path to training data (latent space)')
    parser.add_argument('--oq', type=str, 
                        default='/mnt/c/Users/10geo/Documents/GitHub/Project/input/latent_test_images', 
                        help='Path to query data (latent space)')

    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()

    latent_dim  = 16
    planes      = [32, 16]
    depth       = 2
    norm        = False
    kernel_size = 3

    lr = 1e-4
    epochs = 10
    batch_size = 64

    encoder = keras.models.load_model('/mnt/c/Users/10geo/Documents/GitHub/Project/src/autoencoder/models/encoder.keras')

    train_data = get_dataset(args.d) / 255.
    test_data = get_dataset(args.q) / 255.

    write_dataset(args.od, encoder.predict(train_data) * 255)
    write_dataset(args.oq, encoder.predict(test_data) * 255)