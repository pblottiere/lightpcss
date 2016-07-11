# -*- coding: utf-8 -*-
from struct import pack
import codecs

from . import utils

class PgPointCloud(object):

    def __init__(self, session):
        self.session = session

    def get_points(self, box, dims, offsets, scale, lod):
        return self.__get_points_method1( box, dims, offsets, scale, lod )

    def get_pointn(self, n, box, dims, offset, scale):

        # build params
        poly = utils.boundingbox_to_polygon(box)

        # build sql query
        sql = ("select pc_get(pc_pointn({0}, {1})) as pt from {2} "
            "where pc_intersects({0}, st_geomfromtext('polygon (("
            "{3}))',{4}));"
            .format(self.session.column, n, self.session.table,
                    poly, self.session.srsid()))

        # run the database
        points = self.session.query_aslist(sql)

        # get pgpointcloud schema to retrieve x/y/z position
        schema = utils.Schema()
        schema.parse_pgpointcloud_schema( self.session.schema() )
        xpos = schema.x_position()
        ypos = schema.y_position()
        zpos = schema.z_position()

        # update data with offset and scale
        scaled_points = []
        for pt in points:
            scaled_point = utils.Point()
            scaled_point.x = int((pt[xpos] - offset[0]) / scale)
            scaled_point.y = int((pt[ypos] - offset[1]) / scale)
            scaled_point.z = int((pt[zpos] - offset[2]) / scale)

            scaled_points.append( scaled_point )

        # build a buffer with hexadecimal data
        hexbuffer = []
        for pt in scaled_points:
            hexbuffer.append(self.__hexa_signed_int32(pt.x))
            hexbuffer.append(self.__hexa_signed_int32(pt.y))
            hexbuffer.append(self.__hexa_signed_int32(pt.z))
            hexbuffer.append(self.__hexa_signed_uint16(pt.intensity))
            hexbuffer.append(self.__hexa_signed_uint8(pt.classification))
            hexbuffer.append(self.__hexa_signed_uint16(pt.red))
            hexbuffer.append(self.__hexa_signed_uint16(pt.green))
            hexbuffer.append(self.__hexa_signed_uint16(pt.blue))


    def __get_points_method1(self, box, dims, offsets, scale, lod):
        points = self.get_pointn(1, box, dims, offsets, scale)

    def __hexa_signed_int32(self, val):
        hex = pack('i', val)
        return codecs.encode(hex, 'hex').decode()

    def __hexa_signed_uint16(self, val):
        hex = pack('H', val)
        return codecs.encode(hex, 'hex').decode()

    def __hexa_signed_uint8(self, val):
        hex = pack('c', val)
        return codecs.encode(hex, 'hex').decode()
