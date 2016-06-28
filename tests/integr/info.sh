#! /bin/sh

TABLE="toulouse.path"
COLUMN="geom"

curl http://localhost:8080/resource/$TABLE/$COLUMN/info
echo ""
