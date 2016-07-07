# -*- coding: utf-8 -*-
import json
import decimal

def decimal_default(obj):
    if isinstance(obj, decimal.Decimal):
        return float(obj)
    raise TypeError

class Schema(object):

    def __init__(self):
        self.dims = []

    def json(self):
        json = ""
        for dim in self.dims:
            json += dim.json()

        return json

class Dimension(object):

    def __init__(self, name, typename, size):
        self.name = name
        self.typename = typename
        self.size = size

    def json(self):
        return json.dumps({"name" : self.name,
            "size" : self.size,
            "type" : self.typename})
