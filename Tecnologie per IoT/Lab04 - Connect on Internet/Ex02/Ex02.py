import cherrypy as cp
import json


class Logger(object):
    exposed = True

    def __init__(self):
        self.logs = []

    def GET(self, *uri, **params):
        return json.dumps(self.logs)

    def POST(self, *uri, **params):
        self.logs.append(json.loads(cp.request.body.read()))


if __name__ == "__main__":
    conf = {
        '/': {
            'request.dispatch': cp.dispatch.MethodDispatcher(),
            'tool.session.on': True
        }
    }
    cp.tree.mount(Logger(), "/log", conf)
    cp.config.update({"sever.socket_host": "192.168.1.8"})  # my IP
    cp.config.update({"server.socket_port": 8080})

    cp.engine.start()
    cp.engine.block()
