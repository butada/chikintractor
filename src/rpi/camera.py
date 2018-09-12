#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os

import secret

os.chdir(sys.path[0])


import requests
import json
#import pandas as pd
#import numpy as np
#import matplotlib

#matplotlib.use('Agg') # CUI環境でmatplotlib使いたい場合、指定する
#import matplotlib.pyplot as plt

TOKEN = secret.TOKEN
CHANNEL = 'develop' # チャンネルID

###############
# 画像送信ここから
###############
files = {'file': open("test.jpg", 'rb')}
param = {
    'token':TOKEN, 
    'channels':CHANNEL,
    'filename':"filename",
    'initial_comment': "initial_comment",
    'title': "title"
}
requests.post(url="https://slack.com/api/files.upload",params=param, files=files)

