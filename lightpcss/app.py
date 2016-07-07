from flask import request
from flask.ext.restplus import Api, Resource, fields

api = Api(
        version='0.1', title='LightPCSS API',
        description='API for accessing LightPCSS',
        )

@api.route("/lightpcss")
class Test(Resource):

    def get(self):
        return "Light Pointcloud Streaming Server / Oslandia / contact@oslandia.com"
