from argparse import ArgumentParser
import numpy as np

import tensorflow as tf
tf.get_logger().setLevel('INFO')
from tensorflow.keras.layers import Input
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import EarlyStopping

def parse_args():
    parser = ArgumentParser(description='AutoEncoder w/ Tensorflow')

    parser.add_argument('--train_path', type=str, required=True, help='Path to training data')
    parser.add_argument('--test_path', type=str, required=True, help='Path to test data')
    parser.add_argument('--latent_train_path', type=str, help='Path to training data (latent space)')
    parser.add_argument('--latent_test_path', type=str, help='Path to test data (latent space)')

    parser.add_argument('--planes', nargs="*", type=int, default=[32, 16], help='Channels at each stage')
    parser.add_argument('--depth', type=int, default=2, help='Depth of encoder/decoder')
    parser.add_argument('--norm', type=bool, default=False, help='Employ Batch Normalization')
    parser.add_argument('--kernel_size', type=int, default=3, help='Convolution kernel size')

    parser.add_argument('--epochs', type=int, default=10, help='Number of training epochs')
    parser.add_argument('--lr', type=float, default=1e-4, help='Optimizer for training')
    parser.add_argument('--batch_size', type=int, default=64, help='Batch size for training')

    parser.add_argument('--latent_dim', type=int, default=16, help='Latent Dimension')
    parser.add_argument('--save', type=str, default='', help='Save model')

    return parser.parse_args()

def get_dataset(path):
    with open(path, 'rb') as file:
        file.read(4)
        size = int.from_bytes(file.read(4), byteorder='big')
        h = int.from_bytes(file.read(4), byteorder='big')
        w = int.from_bytes(file.read(4), byteorder='big')
        return np.fromfile(file, dtype=np.uint8).reshape((size, h, w, 1))

def write_dataset(path, encoded, magic_num=1234, latent_dim=16, cols=1):
    with open(path, 'wb') as file:
        file.write(magic_num.to_bytes(4, byteorder='big'))
        file.write(encoded.shape[0].to_bytes(4, byteorder='big'))
        file.write(latent_dim.to_bytes(4, byteorder='big'))
        file.write(cols.to_bytes(4, byteorder='big'))
        encoded.astype(np.uint8).tofile(file)

def train_model(train_path, test_path, latent_dim, planes, depth, norm, 
                kernel_size, lr, batch_size, epochs, save=''):
    from model import AutoEncoder

    train_data = get_dataset(train_path) / 255.
    test_data = get_dataset(test_path) / 255.

    assert train_data.shape[1:] == test_data.shape[1:]

    input_shape = train_data.shape[1:]

    assert train_data.shape[1:] == test_data.shape[1:]

    early_stopping = EarlyStopping(monitor='val_loss', patience=3, restore_best_weights=True)

    encoder, autoencoder = AutoEncoder(input_shape, latent_dim, planes, norm, depth, kernel_size)
    autoencoder.compile(optimizer=Adam(learning_rate=lr), loss='mean_squared_error')
    autoencoder.fit(train_data, train_data, epochs=epochs, batch_size=batch_size, 
                    shuffle=True, validation_split=0.25, callbacks=[early_stopping])

    if save != '':
        encoder.save(save)

    encoded_train = encoder.predict(train_data) * 255
    encoded_test = encoder.predict(test_data) * 255

    return encoded_train, encoded_test


if __name__ == "__main__":
    args = parse_args()

    train_path  = args.train_path
    test_path   = args.test_path

    latent_dim  = args.latent_dim
    planes      = args.planes
    depth       = args.depth
    norm        = args.norm
    kernel_size = args.kernel_size

    lr = args.lr
    epochs = args.epochs
    batch_size = args.batch_size

    encoded_train, encoded_test = train_model(train_path, test_path, latent_dim, planes, depth, 
                                              norm, kernel_size, lr, batch_size, epochs, save=args.save)

    write_dataset(args.latent_train_path, encoded_train)
    write_dataset(args.latent_test_path, encoded_test)