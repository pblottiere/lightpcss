# -*- coding: utf-8 -*-
import json

from .database import Session
from .utils import Dimension, Schema, decimal_default

class GreyhoundInfoSchema(Schema):

    def __init__(self):
        Schema.__init__(self)

        self.dims.append(Dimension( "X", "floating", 8 ))
        self.dims.append(Dimension( "Y", "floating", 8 ))
        self.dims.append(Dimension( "Z", "floating", 8 ))
        self.dims.append(Dimension( "Intensity", "unsigned", 2 ))
        self.dims.append(Dimension( "Classification", "unsigned", 1 ))
        self.dims.append(Dimension( "Red", "unsigned", 2 ))
        self.dims.append(Dimension( "Green", "unsigned", 2 ))
        self.dims.append(Dimension( "Blue", "unsigned", 2 ))

class GreyhoundInfo(object):

    def run(self):
        box = Session.boundingbox()
        npoints = Session.numpoints()
        srs = Session.srs()
        schema_json = GreyhoundInfoSchema().json()

        info = json.dumps( {
            "baseDepth" : 6,
            "bounds" : [box['xmin'], box['ymin'], box['zmin'],
                box['xmax'], box['ymax'], box['zmax']],
            "boundsConforming" : [box['xmin'], box['ymin'], box['zmin'],
                box['xmax'], box['ymax'], box['zmax']],
            "numPoints" : npoints,
            "schema" : schema_json,
            "srs" : srs,
            "type" : "octree"}, default = decimal_default )

        return info
