import cherrypy as cp
import json


class Converter(object):
    exposed = True

    def GET(self, *uri, **params):

        # controllo correttezza parametri
        if len(uri) != 3:
            raise cp.HTTPError(400, message="Wrong number of parameters!")
        if uri[1] not in ["K", "C", "F"] or uri[2] not in ["K", "C", "F"]:
            raise cp.HTTPError(400, message="units must be K, C or F")
        try:
            value = float(uri[0])
        except ValueError:
            raise cp.HTTPError(400, message="the value is not a number")
        if self.is_lower_than_zero(uri[1], value):
            raise cp.HTTPError(400, "original temperature is lower than absolute zero")

        # elaborazione json finale
        res = {
            "originalValue": uri[0],
            "originalUnit": uri[1],
            "finalValue": 0,
            "targetUnit": uri[2]
        }
        if params["originalUnit"] == "K":
            res["finalValue"] = self.convert_from_k(uri[2], value)
        elif params["originalUnit"] == "C":
            res["finalValue"] = self.convert_from_c(uri[2], value)
        else:
            res["finalValue"] = self.convert_from_f(uri[2], value)

        return json.dumps(res, indent=4)


    def POST(self, *uri, **params):
        pass

    def PUT(self, *uri, **params):
        pass

    def DELETE(self, *uri, **params):
        pass

    def is_lower_than_zero(self, unit, value):
        if unit == "K":
            return value < 0
        elif unit == "C":
            return (value + 273.15) < 0
        else:
            return ((value - 32) * 5 / 9 + 273.15) < 0

    def convert_from_k(self, unit, value):
        if unit == "C":
            return value - 273.15
        else:
            return (value - 273.15) * 9 / 5 + 32

    def convert_from_c(self, unit, value):
        if unit == "K":
            return value + 273.15
        else:
            return value * 9 / 5 + 32

    def convert_from_f(self, unit, value):
        if unit == "K":
            return (value - 32) * 5 / 9 + 273.15
        else:
            return (value - 32) * 5 / 9


if __name__ == "__main__":
    conf = {
        '/': {
            'request.dispatch': cp.dispatch.MethodDispatcher(),
            'tool.session.on': True
        }
    }

    cp.tree.mount(Converter(), "/converter", conf)
    cp.config.update({"sever.socket_host": "0.0.0.0"})
    cp.config.update({"server.socket_port": 8080})
    cp.engine.start()
    cp.engine.block()
