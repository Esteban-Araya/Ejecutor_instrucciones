import cv2
import numpy as np
from base64 import b64decode
import io
import os
from PIL import Image


def create_folders():
    folders = ["photos","photos/right","photos/up","photos/down","photos/left","photos/circle"]
  
    for i in folders:
        try:
            os.mkdir(i)
        except:
            pass

def decode_image(image):
    image = b64decode(image)
    image = Image.open(io.BytesIO(image))
    return image

def load_circles(image):
    image = decode_image(image)
    
    image = np.array(image)
    image = cv2.resize(image, (240, 240))
    try:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        for index, rango in enumerate(range(-100,0,1)):
            image[index][rango:240]=255
            image[index][0:-rango]=255
            image[-(index+1)][rango:240]=255
            image[-(index+1)][0:-rango]=255

        cv2.imwrite(f"test.jpeg",image)
    except:
        print("no funco")
        
    
    _,image = cv2.threshold(image,100,255,cv2.THRESH_BINARY)
    cv2.imwrite(f"threshold.jpeg",image)
  
    car = os.listdir("photos/circle")
    car.sort(key=lambda car: os.path.getctime(os.path.join(f"photos/circle", car)), reverse=True)

    try:
        num = int(os.path.splitext(os.path.basename(os.path.join(f"photos/circle",car[0])))[0]) + 1
    except:
        num = 1
    cv2.imwrite(f"photos/circle/{num}.jpeg",image)
        

        
   
def load_arrows(image):
    image = decode_image(image)
    
    image = np.array(image)
    image = cv2.resize(image, (240, 240))
    try:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        for index, rango in enumerate(range(-100,0,1)):
            image[index][rango:240]=255
            image[index][0:-rango]=255
            image[-(index+1)][rango:240]=255
            image[-(index+1)][0:-rango]=255

        cv2.imwrite(f"test.jpeg",image)
    except:
        print("no funco")
        
    
    _,image = cv2.threshold(image,100,255,cv2.THRESH_BINARY)
    cv2.imwrite(f"threshold.jpeg",image)
    
    folders = ["right","down","left","up"]
    for i in folders:
        car = os.listdir(os.path.join(f"photos", i))
        car.sort(key=lambda car: os.path.getctime(os.path.join(f"photos/{i}", car)), reverse=True)

        try:
            num = int(os.path.splitext(os.path.basename(os.path.join(f"photos/{i}",car[0])))[0]) + 1
        except:
            num = 1
        cv2.imwrite(f"photos/{i}/{num}.jpeg",image)
        

        image = cv2.rotate(image, cv2.ROTATE_90_CLOCKWISE)
   