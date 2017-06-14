# https://stackoverflow.com/questions/32911336/what-is-the-difference-between-json-dumps-and-json-load
import json

def open_json():
    with open('vision_google_cloud.json') as json_data:
        data=json.load(json_data)
    #print(data['faceAnnotations'])
    for r in data['faceAnnotations']:
        x=r['tiltAngle']
        #print(x)
    return x


y=open_json()
print(y)
