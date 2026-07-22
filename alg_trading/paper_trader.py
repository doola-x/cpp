import time
import threading
import os
import json
import requests

class OrderBook():
    def __init__(self):
        self.secrets = {}
        with open("paper_t.secret.json", "r") as f:
            self.secrets = json.load(f)
    
    def get_account_info(self):
        url =  self.secrets['alpaca_url'] + '/v2/account'
        headers = {'APCA-API-KEY-ID': self.secrets['api_key'],
                'APCA-API-SECRET-KEY': self.secrets['api_secret']}
        r = requests.get(url, headers=headers)
        return r.json()


ob = OrderBook()
meta = ob.get_account_info()
print(meta['id'])
