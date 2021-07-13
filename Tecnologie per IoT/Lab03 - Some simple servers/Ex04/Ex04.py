import cherrypy as cp
import os
import json


class Freeboard(object):

    exposed = True

    def GET(self, *uri, **params):
        return open('index.html')

    def PUT(self, *uri, **params):
        pass

    def POST(self, *uri, **params):
        with open('./dashboard/dashboard.json', 'w') as dashboard:
            dashboard.write(params['json_string'])

    def DELETE(self, *uri, **params):
        pass


if __name__ == "__main__":
    conf = {
        '/': {
            'request.dispatch': cp.dispatch.MethodDispatcher(),
            'tools.staticdir.root': os.path.abspath(os.getcwd())
        },
        '/css': {
            'tools.staticdir.on': True,
            'tools.staticdir.dir': './css'
        },
        '/js': {
            'tools.staticdir.on': True,
            'tools.staticdir.dir': './js'
        },
        '/img': {
            'tools.staticdir.on': True,
            'tools.staticdir.dir': './img'
        }
    }
    cp.tree.mount(Freeboard(), '/', conf)

    global_conf = {
        'global': {
            'engine.autoreload.on': False,
            'server.socket_host': '0.0.0.0',
            'server.socket_port': 8080
        }
    }
    cp.config.update(global_conf)

    cp.engine.start()
    cp.engine.block()
