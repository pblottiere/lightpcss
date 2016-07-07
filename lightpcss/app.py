from flask import request
from flask.ext.restplus import Api, Resource, fields

from .database import Session
from . import greyhound

api = Api(
        version='0.1', title='LightPCSS API',
        description='API for accessing LightPCSS',
        )

# -----------------------------------------------------------------------------
# basic api
# -----------------------------------------------------------------------------
@api.route("/lightpcss")
class Test(Resource):

    def get(self):
        return "Light Pointcloud Streaming Server / Oslandia / contact@oslandia.com"

# -----------------------------------------------------------------------------
# greyhound api
# -----------------------------------------------------------------------------
@api.route("/greyhound/info")
class Info(Resource):

    def get(self):
        return greyhound.GreyhoundInfo().run()
