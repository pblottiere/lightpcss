#! /bin/sh

PORT=8080
TABLE="toulouse.path"
COLUMN="geom"

curl http://localhost:$PORT/resource/$TABLE/$COLUMN/read -d"depthBegin=10&depthEnd=11&bounds=[43.595,43.61,0.0,1.4354,1.4575,300.0]&schema=[{\"name\":\"x\",\"size\":4,\"type\":\"signed\"},{\"name\":\"y\",\"size\":4,\"type\":\"signed\"},{\"name\":\"z\",\"size\":4,\"type\":\"signed\"},{\"name\":\"Intensity\",\"size\":2,\"type\":\"unsigned\"},{\"name\":\"Classification\",\"size\":1,\"type\":\"unsigned\"},{\"name\":\"Red\",\"size\":2,\"type\":\"unsigned\"},{\"name\":\"Green\",\"size\":2,\"type\":\"unsigned\"},{\"name\":\"Blue\",\"size\":2,\"type\":\"unsigned\"}]&scale=0.01&offset=[728630,4676727,309]&compress=true"
echo ""
