# server.py

import aiocoap.resource as resource
import aiocoap
import asyncio

class AlarmResource(resource.Resource):
        """This resource supports the PUT method.
        PUT: Update state of alarm."""

        def __init__(self):
            super().__init__()
            self.state = "OFF"

        async def render_put(self, request):
            self.state = request.payload
            print('Update alarm state: %s' % self.state)

            return aiocoap.Message(code=aiocoap.CHANGED, payload=self.state)

def main():
        # Resource tree creation
        root = resource.Site()
        root.add_resource(['alarm'], AlarmResource())

        asyncio.Task(aiocoap.Context.create_server_context(root, bind=('0.0.0.0', 5683)))

        asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
        main()           