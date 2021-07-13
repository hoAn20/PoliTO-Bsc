import cherrypy as cp
import json


class Converter(object):

    exposed = True

    def GET(self, *uri, **params):

        # controllo correttezza parametri
        if len(params) != 3:
            raise cp.HTTPError(400, message="Wrong number of parameters!")
        if "value" not in params.keys():
            raise cp.HTTPError(400, message="value key needed")
        if "originalUnit" not in params.keys():
            raise cp.HTTPError(400, message="originalUnit key needed")
        if "targetUnit" not in params.keys():
            raise cp.HTTPError(400, message="targetUnit key needed")
        if params["originalUnit"] not in ["K", "C", "F"] or params["targetUnit"] not in ["K", "C", "F"]:
            raise cp.HTTPError(400, message="Units must be K, C or F")
        try:
            params["value"] = float(params["value"])
        except ValueError:
            raise cp.HTTPError(400, message="The value is not a number")
        if self.is_lower_than_zero(params["originalUnit"], params["value"]):
            raise cp.HTTPError(400, "original temperature is lower than absolute zero")

        # elaborazione json finale
        res = {
            "originalValue": params["value"],
            "originalUnit": params["originalUnit"],
            "finalValue": 0,
            "targetUnit": params["targetUnit"]
        }
        if params["originalUnit"] == "K":
            res["finalValue"] = self.convert_from_k(params["targetUnit"], params["value"])
        elif params["originalUnit"] == "C":
            res["finalValue"] = self.convert_from_c(params["targetUnit"], params["value"])
        else:
            res["finalValue"] = self.convert_from_f(params["targetUnit"], params["value"])

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
