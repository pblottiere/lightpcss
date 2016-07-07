#!/usr/bin/env python
# -*- coding: utf-8 -*-
import pytest

from lightpcss import create_app


@pytest.fixture
def app():
    app = create_app()
    return app


def test_get_lightpcss(client):
    resp = client.get('/lightpcss')
    assert resp.content_type == 'application/json'
    assert resp.status_code == 200
    assert 'Light Pointcloud Streaming Server / Oslandia / contact@oslandia.com' in resp.json
