import requests
import json

if __name__ == "__main__":
    info = requests.get('http://localhost:8080/')
    
    if info.status_code == 200:
        try:
            infod = json.loads(info.text)
            print('MQTT BROKER INFORMATION:\n', infod['MQTT'])
        except ValueError:
            print("Received data aren't in json format")
    else:
        print('Some error occurred')

    devices = requests.get('http://localhost:8080/devices')
    if devices.status_code == 200:
        try:
            devicesd = json.loads(devices.text)
            print("REGISTERED DEVICES:\n", devicesd)
        except ValueError:
            print("Received data aren't in json format")
    else:
        print('Some error occurred')

    did = input('Insert desired device id: ')
    device = requests.get('http://localhost:8080/devices/'+did)
    if device.status_code == 200:
        try:
            deviced = json.loads(device.text)
            print("REQUESTED DEVICE:\n", deviced)
        except ValueError:
            print("Received data aren't in json format")
    else:
        print('Some error occurred, probably the code is invalid')

    users = requests.get('http://localhost:8080/users')
    if users.status_code == 200:
        try:
            usersd = json.loads(users.text)
            print("REGISTERED USERS:\n", usersd)
        except ValueError:
            print("Received data aren't in json format")
    else:
        print('Some error occurred')

    uid = input('Insert desired user id: ')
    user = requests.get('http://localhost:8080/users/'+uid)
    if user.status_code == 200:
        try:
            userd = json.loads(user.text)
            print("REQUESTED USER:\n", userd)
        except ValueError:
            print("Received data aren't in json format")
    else:
        print('Some error occurred, probably the code is invalid')
