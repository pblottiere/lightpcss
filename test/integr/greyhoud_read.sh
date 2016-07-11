#! /bin/sh

PORT=5000

#curl "http://localhost:$PORT/greyhound/read&depthBegin=10&depthEnd=11&bounds=\[43.595,43.61,0.0,1.4354,1.4575,300.0\]&schema=\[{\"name\":\"x\",\"size\":4,\"type\":\"signed\"},{\"name\":\"y\",\"size\":4,\"type\":\"signed\"},{\"name\":\"z\",\"size\":4,\"type\":\"signed\"},{\"name\":\"Intensity\",\"size\":2,\"type\":\"unsigned\"},{\"name\":\"Classification\",\"size\":1,\"type\":\"unsigned\"},{\"name\":\"Red\",\"size\":2,\"type\":\"unsigned\"},{\"name\":\"Green\",\"size\":2,\"type\":\"unsigned\"},{\"name\":\"Blue\",\"size\":2,\"type\":\"unsigned\"}\]&scale=0.01&offset=\[43.60333061218262,1.4464976787567139,102.26499938964845\]&compress=true"
curl "http://localhost:$PORT/greyhound/read&depthBegin=10&depthEnd=11&bounds=\[43.595,43.61,0.0,1.4354,1.4575,300.0\]&scale=0.01&offset=\[43.60333061218262,1.4464976787567139,102.26499938964845\]&compress=true"
echo ""
