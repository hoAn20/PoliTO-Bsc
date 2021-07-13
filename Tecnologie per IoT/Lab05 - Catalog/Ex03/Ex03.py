import json
import requests
import time
import sys

def addDevice(device, link):
    resp = requests.post(link, json=device)
    if resp.status_code == 200:
        data = json.loads(resp.text)
        device['id'] = data['id']
        print('Added: ', device)
    else:
        print('Some error occurred, probably in your JSON format')
        device['id'] = ''
    return device

def updateDevice(device, link):
    did = device['id']
    if did == '':
        return addDevice(device, link)
    else:
        resp = requests.put(link + '/' + did, json=device)
        if resp.status_code == 200:
            data = json.loads(resp.text)
            device['id'] = data['id']
            print('Updated: ', device)
        else:
            print('Some error occurred')
        return device

if __name__ == "__main__":
    resp = requests.get('http://localhost:8080/')
    if resp.status_code == 200:
        try:
            info = json.loads(resp.text)
            sublink = info['REST']['device']
        except ValueError:
            print('Wrong JSON format received from GET')
    else:
        print("Some error occurred", resp.status_code)

    device = {
        'end-points': {
            'REST': 'url',
        },
        'resources': 'temperature'
    }
    device = addDevice(device, sublink)
    while True:
        time.sleep(60)
        device = updateDevice(device, sublink)
