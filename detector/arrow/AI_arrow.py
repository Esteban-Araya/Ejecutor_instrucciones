from tensorflow.keras.models import load_model
import cv2
import numpy as np
from base64 import b64decode
from PIL import Image
import io
import os

cnn = load_model(os.path.abspath("arrow/AIs/CC10_CC32_CC64_CD128-5_CD64.h5"), compile=False)

def decode_image(image):
    image = b64decode(image)
    image = Image.open(io.BytesIO(image))
    image = np.array(image)
    image = cv2.resize(image, (240, 240))
    
    
    
    try:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        for index, rango in enumerate(range(-85,0,1)):
            image[index][rango:240]=255
            image[index][0:-rango]=255
            image[-(index+1)][rango:240]=255
            image[-(index+1)][0:-rango]=255
        cv2.imwrite("imagegray.jpeg",image)
    except:
        pass
    
    _,image = cv2.threshold(image,100,255,cv2.THRESH_BINARY)

    try:
        
        cv2.imwrite("image.jpeg",image)
       
    except:
        pass
    return image.reshape(-1,240,240,1)



def analyze_arrow(image):
    try:
        image = decode_image(image)
    except:
        print(image)
        return "error" 
    prediction =cnn.predict(image)[0]
    
    prediction = prediction.tolist()
    directions = ["right","up","left","down","circle"]
    print(prediction)
    mean = 0
    for pre in prediction:
        mean += pre
    mean = mean/5 + 0.1 
    max_val = max(prediction)

    if max_val < mean: return "nothing"
    for i,dir in enumerate(directions):

        if max_val == prediction[i]: return dir
    
    return directions[-1]
    
