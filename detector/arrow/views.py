from django.shortcuts import render
from arrow.AI_arrow import analyze_arrow
from arrow.detect_arrow import devolver_flecha
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt 
from arrow.data.get_photos import load_arrows,create_folders,load_circles
# Create your views here.
create_folders()

@csrf_exempt
def flecha(request):

    direction = devolver_flecha(request.body)

    return JsonResponse({"direction": direction})

@csrf_exempt
def ai_arrow(request):
    direction = analyze_arrow(request.body)
    print(direction)
    return JsonResponse({"direction": direction})

@csrf_exempt
def data(request):
    load_arrows(request.body)
    return JsonResponse({})

def data_circle(request):
    load_circles(request.body)
    return JsonResponse({})