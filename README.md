# lightpcss
Light Pointcloud Streaming Server

## Requirements

* flask
* flask-restplus
* psycopg2
* pyyaml
* pytest
* pytest-flask
* pygdal
* lazperf

## Usage

### Run

```
uwsgi --yml conf/lightpcss.uwsgi.yml
```

### Unit tests

```
python3 -m pytest
```
