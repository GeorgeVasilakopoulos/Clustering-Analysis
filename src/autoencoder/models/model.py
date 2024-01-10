
import tensorflow as tf
tf.get_logger().setLevel('INFO')

from tensorflow.keras.layers import Dense, Input, Flatten, Reshape, \
                                    Conv2D, MaxPooling2D, UpSampling2D,\
                                    ReLU, BatchNormalization, Identity

from tensorflow.keras.models import Model, Sequential

import numpy as np

def conv_layer(planes, kernel_size, norm, act, act_params, pool, up):
    assert not (pool and up) 

    return Sequential([
        Conv2D(planes, kernel_size, padding='same'),
        BatchNormalization(axis=-1) if norm else Identity(),
        act(**act_params),
        MaxPooling2D(pool_size=2) if pool else UpSampling2D(size=2) if up else Identity()
    ])

def get_encoder(in_shape=(28, 28, 1), latent_dim=16, planes=[32, 16, 8], norm=False, depth=3, 
            kernel_size=3, act=ReLU, act_params={'max_value': 1.0}):
    
    assert depth > 0 and depth <= 3
    assert len(planes) == depth

    model1 = Sequential([
        conv_layer(planes[i], kernel_size, norm, act, act_params, i + 1 < depth, False)
        for i in range(depth)
    ])

    model2 = Sequential([
        Flatten(), 
        Dense(latent_dim),
        act(**act_params),
    ])

    return Sequential([model1, model2]), model1(Input(shape=in_shape)).get_shape().as_list()[1:]

def get_decoder(shape, latent_dim=16, planes=[32, 16, 8], norm=False, depth=3, 
            kernel_size=3, act=ReLU, act_params={'max_value': 1.0}):
    
    assert depth > 0 and depth <= 4
    assert len(planes) == depth

    model = [
        Dense(np.prod(shape), input_shape=(latent_dim,)),
        Reshape(shape),
        act(**act_params),
    ]

    model += [
        conv_layer(planes[-i-1], kernel_size, norm, act, act_params, False, i + 1 < depth)
        for i in range(depth)
    ]

    model += [
        Conv2D(1, (1, 1), padding='same'),
        act(**act_params)
    ]

    return Sequential(model)


def AutoEncoder(shape=(28, 28, 1), latent_dim=16, planes=[32, 16, 8], norm=False, depth=3, 
                kernel_size=3, act=ReLU, act_params={'max_value': 1.0}):

    encoder, reduced_shape = get_encoder(shape, latent_dim, planes, norm, depth, kernel_size, act, act_params)
    decoder = get_decoder(reduced_shape, latent_dim, planes, norm, depth, kernel_size, act, act_params)

    inputs = Input(shape=shape)
    output = decoder(encoder(inputs))
    return encoder, Model(inputs=inputs, outputs = output)