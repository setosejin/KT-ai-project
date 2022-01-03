import os, re, glob
import cv2
import numpy as np
from sklearn.model_selection import train_test_split

groups_folder_path = './samples/'
categories = ['gizigae', 'not_gizigae', 'not_recog']
num_classes = len(categories)

image_w = 224
image_h = 224
  
X = []
Y = []

for idx, categorie in enumerate(categories):
    label = [0 for i in range(num_classes)]
    label[idx] = 1
    image_dir = groups_folder_path + categorie + '/'
    
    for top, dir, f in os.walk(image_dir):
        for filename in f:
            print(image_dir+filename)
            img = cv2.imread(image_dir+filename)
            img = cv2.resize(img, None, fx=image_w/img.shape[1], fy=image_h/img.shape[0])
            X.append(img/256)
            Y.append(label)
            
X = np.array(X)
Y = np.array(Y)

X_train, X_test, Y_train, Y_test = train_test_split(X, Y)
xy = (X_train, X_test, Y_train, Y_test)

np.save('./img_data.npy', xy)