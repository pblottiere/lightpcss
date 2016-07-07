from flask import request
from flask.ext.restplus import Api, Resource, fields

api = Api(
        version='0.1', title='LightPCSS API',
        description='API for accessing LightPCSS',
        )

@api.route("/project")
class Test(Resource):

    def get(self):
        return "COUCOU"
