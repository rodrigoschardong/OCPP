import asyncio
import logging
import json

try:
    import websockets
except ModuleNotFoundError:
    print("This example relies on the 'websockets' package.")
    print("Please install it by running: ")
    print()
    print(" $ pip install websockets")
    import sys

    sys.exit(1)


from ocpp.v201 import ChargePoint as cp
from ocpp.v201 import call

logging.basicConfig(level=logging.INFO)


class ChargePoint(cp):
    async def send_heartbeat(self, interval):
        while True:
            request = call.HeartbeatPayload(
                custom_data = {
                    "Name": "Rodrigo",
                    "Company": "Embrasul",
                    "vendorId": "Teste"
                }
            )
            await self.call(request)
            await asyncio.sleep(interval)

            request = call.HeartbeatPayload()
            await self.call(request)
            await asyncio.sleep(interval)

    async def send_boot_notification(self):
        request = call.BootNotificationPayload(
            charging_station={"model": "Wallbox XYZ", "vendor_name": "anewone"},
            reason="PowerUp",
        )
        response = await self.call(request)

        if response.status == "Accepted":
            print("Connected to central system.")
            await self.send_heartbeat(response.interval)

def read_json_file(file_path):
    try:
        with open(file_path, 'r') as json_file:
            data = json.load(json_file)
            for key, value in data.items():
                print(f"{key}: {value}")
            return data
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
        print("Please create a 'config.json' file from 'config.example.json' with your personal information")
    except json.JSONDecodeError as e:
        print(f"Error decoding JSON: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")

async def main(jsonFilePath):
    serverConfigData = read_json_file(jsonFilePath)
    address = "ws://" + serverConfigData['ip'] + ":" + str(serverConfigData['port']) + '/CP_1'
    async with websockets.connect(
        address, subprotocols = serverConfigData['subprotocols']
    ) as ws:

        charge_point = ChargePoint("CP_1", ws)
        await asyncio.gather(
            charge_point.start(), charge_point.send_boot_notification()
        )


if __name__ == "__main__":
    jsonFilePath = "../config.json"
    # asyncio.run() is used when running this example with Python >= 3.7v
    asyncio.run(main(jsonFilePath))