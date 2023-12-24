import numpy as np
import tensorflow as tf
from tensorflow.keras import layers, models
from tensorflow.keras.datasets import mnist
import matplotlib.pyplot as plt




(train_data, _),(test_data, _) = mnist.load_data()


train_data.astype('float32')/255.0
test_data.astype('float32')/255.0

train_data = np.reshape(train_data, (len(train_data),28,28,1))
test_data = np.reshape(test_data, (len(test_data),28,28,1))



def encoder(x):
    x = layers.Conv2D(32,(3,3), activation = 'relu', padding='same')(x)
    x = layers.MaxPooling2D((2, 2), padding='same')(x)
    x = layers.Conv2D(64, (3, 3), activation='relu', padding='same')(x)
    x = layers.MaxPooling2D((2, 2), padding='same')(x)
    x = layers.Conv2D(128, (3, 3), activation='relu', padding='same')(x)

    return x




def decoder(x):
    x = layers.Conv2D(128, (3, 3), activation='relu', padding='same')(x)
    x = layers.UpSampling2D((2, 2))(x)
    x = layers.Conv2D(64, (3, 3), activation='relu', padding='same')(x)
    x = layers.UpSampling2D((2, 2))(x)
    x = layers.Conv2D(1, (3, 3), activation='sigmoid', padding='same')(x)

    return x




input_img = tf.keras.Input(shape = (28,28,1),name='encoder_input')

strategy = tf.distribute.MirroredStrategy()
print("Number of devices: {}".format(strategy.num_replicas_in_sync))

with strategy.scope():
    autoencoder = models.Model(input_img,decoder(encoder(input_img)))
    autoencoder.compile(optimizer=tf.keras.optimizers.RMSprop(), loss='mean_squared_error')


autoencoder.summary()
autoencoder.fit(train_data,train_data,epochs=10,batch_size=128,shuffle=True,validation_data=(test_data,test_data))



# Select an image from the test set for testing
image_index = 0
input_image = test_data[image_index]

# Encode the image
recreated = autoencoder.predict(np.expand_dims(input_image, axis=0))



# Plot the original, encoded, and decoded images
plt.figure(figsize=(15, 5))

# Original Image
plt.subplot(1, 2, 1)
plt.imshow(np.squeeze(input_image), cmap='gray')
plt.title('Original Image')
plt.axis('off')


# Decoded Image
plt.subplot(1, 2, 2)
plt.imshow(np.squeeze(recreated), cmap='gray')
plt.title('Decoded Image')
plt.axis('off')

plt.show()



