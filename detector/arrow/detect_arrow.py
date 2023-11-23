import cv2
import numpy as np
from base64 import b64decode
from PIL import Image
import io


def eliminar_puntos_cercanos(approx):
    RANGO = 20
    #approx = list(approx)
    eliminar= []
    
    for i in range(len(approx)):


        for j in range(i+1,len(approx)):
            if approx[i][0][0] + RANGO > approx[j][0][0] > approx[i][0][0] - RANGO:

                if approx[i][0][1] + RANGO > approx[j][0][1]  > approx[i][0][1] - RANGO:
                    eliminar.append(i)
                    #print("se elimino:", approx[i][0] )

    approx = np.delete(approx,eliminar,axis=0)
    
    return approx

def isFlecha2(appr,image,orientacion):
    
    RAN_ATRAS = 40
    RAN_ADELANTE = 50
    kernel = np.ones((5,5),np.uint8)
    #print(appr)
    puntos = len(appr)
    #print(puntos)
    if 10 < puntos or puntos < 6:
        print("no es una flecha, por los lados")
        return False
    
    x,y,w,h = cv2.boundingRect(appr)
    flecha = image[y-17:y+h+17,x-17:x+w+17]
    
    
    flecha = cv2.resize(flecha,(300,300))
    
    try:
        gray = cv2.cvtColor(flecha, cv2.COLOR_BGR2GRAY)
    except:
        gray = flecha
    
    gray = cv2.bitwise_not(gray)
    
    difuminado = cv2.GaussianBlur(gray,(5,5),0)
    
    #difuminado = cv2.medianBlur(difuminado,30)
    cv2.imwrite(f"fotos/flecha{puntos}.jpg",flecha)
    #print("por aca")
    e, a = cv2.threshold(difuminado,170,255,cv2.THRESH_BINARY)
    a = cv2.dilate(a, kernel, iterations=5)
    a = cv2.erode(a, kernel, iterations=3)
    
    _,th = cv2.threshold(gray,170,255,cv2.THRESH_BINARY)
    th = cv2.dilate(th, kernel, iterations=5)
    th = cv2.erode(th, kernel, iterations=3)
    
    
    cnts,_ = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)# OpenCV 4
    cv2.drawContours(flecha, cnts, -1, (255,0,0), 2)
    #cv2.putText(flecha, f"cantidad: {len(approx)}",(100,100),2,0.5,(0,0,255),1,cv2.LINE_AA)
    
    cv2.imwrite("fotos/flecha.jpg",flecha)
    cv2.imwrite("fotos/flechaTH.jpg",th)
    cv2.imwrite("fotos/flechaTH2.jpg",a)
    

    xval = 0
    x = 0
    # print("PASE LA PRUEBA")
    for c in cnts:
        epsilon = 0.01*cv2.arcLength(c,True)
        approx = cv2.approxPolyDP(c,epsilon,True)
        x,y,w,h = cv2.boundingRect(approx)
        
        approx = eliminar_puntos_cercanos(approx)
        # print(type(approx))
        #cv2.rectangle(flecha,(x,y),(x+w,y+h),(250,250,0),2)
        #print("largo: ", len(approx))
       
        pos = 1 
              
        if (orientacion):
            pos = 0

        # print("achu2") 
        xval = sorted(list(approx[:, 0, pos]))
        # print("achu")
        #abajo o izquierda
        if(xval[0] + RAN_ATRAS > xval[1]):
            
            

            if xval[-1] - RAN_ADELANTE > xval[-2]:
                
                if pos == 0:
                    print("Derecha")
                    direction = "Derecha"
                else:
                    print("abajo")
                    direction = "abajo"
                # cv2.imshow(f"flechaPro",flecha)
                # print("PASE LA PRUEBA3")
                # cv2.imshow("cannyFlecha",canny)
                cv2.imwrite(f"fotos/arrow{direction}.jpg",flecha)
                return direction

        #arriba o derecha 
        if(xval[-1] - RAN_ATRAS < xval[-2]):
            
            if xval[0] + RAN_ADELANTE < xval[1]:
                if pos == 0:
                    print("Izquierda")
                    direction = "Izquierda"

                else:
                    print("Arriba")
                    direction = "Arriba"
                   
                # cv2.imshow(f"flechaPro",flecha)
                # print("PASE LA PRUEBA2")
                # # cv2.imshow("cannyFlecha",canny)
                cv2.imwrite(f"fotos/arrow{direction}.jpg",flecha)
                return direction
        cv2.imwrite(f"fotos/NoTarrow{x}.jpg",flecha)
        x += 1
        

    #cv2.imshow(f"noFlecha",flecha)     
    
    # cv2.imshow("cannyFlecha",canny)
    print("no es una flecha")
    
    return False
    
    

    



def devolver_flecha(img):

    image = b64decode(img)
    
    
    image = Image.open(io.BytesIO(image))
    image = np.asarray(image)
    ima = image.copy()
    kernel = np.ones((5,5),np.uint8)
    try:
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    except:
        gray = image
    gray = cv2.bitwise_not(gray)
    difuminado = cv2.GaussianBlur(gray,(5,5),0)
    difuminado = cv2.medianBlur(difuminado,1)
    _,th = cv2.threshold(difuminado,170,255,cv2.THRESH_BINARY)
    th = cv2.dilate(th, kernel, iterations=1)
    th = cv2.erode(th, kernel, iterations=1)
    
    
    cnts,_ = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)# OpenCV 4
    # 
    # 
    
    cv2.drawContours(image, cnts, -1, (255,255,0), 1 )
    
    
    cv2.imwrite("fotos/difuminado.jpg",difuminado)
    cv2.imwrite("fotos/gray.jpg",gray)
    cv2.imwrite("fotos/th.jpg",th)
    
    x = 0
    for c in cnts:
        
        try:
            epsilon = 0.01*cv2.arcLength(c,True)
            approx = cv2.approxPolyDP(c,epsilon,True)
            x,y,w,h = cv2.boundingRect(approx)
            color = (0,0,255)
            if ( 11 >len(approx) > 6):
                cv2.putText(image, f"{len(approx)}",(x,y-25),2,1,(0,0,255),1,cv2.LINE_AA)
                cv2.imwrite(f"fotos/foto.jpg",image)
            
            direction = isFlecha2(approx,ima,w > h)
            cv2.rectangle(image,(x,y),(x+w,y+h),color,2)
            if direction is not False:
                cv2.destroyAllWindows()
                return direction
                
        except:
            
            pass
        x += 1
    
    cv2.destroyAllWindows()
    return "nada"
