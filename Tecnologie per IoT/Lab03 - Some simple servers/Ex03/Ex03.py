import cherrypy as cp
import json


class Converter(object):
    exposed = True

    def GET(self, *uri, **params):
        pass

    def POST(self, *uri, **params):
        pass

    def PUT(self, *uri, **params):

        # controllo correttezza json nel body
        try:
            req = json.loads(cp.request.body.read())
        except ValueError:
            raise cp.HTTPError(400, message="received string is not json valid format")

        # controllo correttezza parametri
        if len(req) != 3:
            raise cp.HTTPError(400, message="Wrong number of parameters!")
        if "values" not in req.keys():
            raise cp.HTTPError(400, message="value key must be present")
        if "originalUnit" not in req.keys():
            raise cp.HTTPError(400, message="originalUnit key must be present")
        if "targetUnit" not in req.keys():
            raise cp.HTTPError(400, message="targetUnit key must be present")
        if req["originalUnit"] not in ["K", "C", "F"] or req["targetUnit"] not in ["K", "C", "F"]:
            raise cp.HTTPError(400, message="units must be K, C or F")
        try:
            print(type(req['values']))
            values = list(req["values"])
            for i in range(len(values)):
                values[i] = float(values[i])
        except ValueError:
            raise cp.HTTPError(400, message=" is not a number")
        if self.is_lower_than_zero(req["originalUnit"], req["values"]):
            raise cp.HTTPError(400, "original temperature is lower than absolute zero")

        # elaborazione json finale
        res = {
            "originalValue": req["values"],
            "originalUnit": req["originalUnit"],
            "finalValue": [],
            "targetUnit": req["targetUnit"]
        }
        if req["originalUnit"] == "K":
            res["finalValue"] = self.convert_from_k(req["targetUnit"], req["values"])
        elif req["originalUnit"] == "C":
            res["finalValue"] = self.convert_from_c(req["targetUnit"], req["values"])
        else:
            res["finalValue"] = self.convert_from_f(req["targetUnit"], req["values"])

        return json.dumps(res, indent=4)

    def DELETE(self, *uri, **params):
        pass

    def is_lower_than_zero(self, unit, values):
        is_it = True
        for v in values:
            if unit == "K":
                is_it = v < 0
            elif unit == "C":
                is_it = (v + 273.15) < 0
            else:
                is_it = ((v - 32) * 5 / 9 + 273.15) < 0

        return is_it

    def convert_from_k(self, unit, values):
        res = []
        if unit == "C":
            for v in values:
                res.append(v - 273.15)
        else:
            for v in values:
                res.append((v - 273.15) * 9 / 5 + 32)

        return res

    def convert_from_c(self, unit, values):
        res = []
        if unit == "K":
            for v in values:
                res.append(v + 273.15)
        else:
            for v in values:
                res.append(v * 9 / 5 + 32)

        return res

    def convert_from_f(self, unit, values):
        res = []
        if unit == "K":
            for v in values:
                res.append((v - 32) * 5 / 9 + 273.15)
        else:
            for v in values:
                res.append((v - 32) * 5 / 9)

        return res


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
