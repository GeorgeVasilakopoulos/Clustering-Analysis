
from encoder.train import get_dataset, write_dataset
import keras

from argparse import ArgumentParser

def parse_args():
    parser = ArgumentParser(description='AutoEncoder w/ Tensorflow')

    parser.add_argument('-d', type=str, required=True, help='Path to training data')
    parser.add_argument('-q', type=str, required=True, help='Path to query data')
    parser.add_argument('-od', type=str, required=True, help='Path to training data (latent space)')
    parser.add_argument('-oq', type=str, required=True, help='Path to query data (latent space)')

    return parser.parse_args()

if __name__ == "__main__":
    args = parse_args()

    encoder = keras.models.load_model('./encoder/encoder.keras')

    train_data = get_dataset(args.d) / 255.
    test_data = get_dataset(args.q) / 255.

    write_dataset(args.od, encoder.predict(train_data) * 255)
    write_dataset(args.oq, encoder.predict(test_data) * 255)