# server.py

import logging
import asyncio

from aiocoap import *

logging.basicConfig(level=logging.INFO)

async def main():
    context = await Context.create_client_context()

    payload = b"The quick brown fox /jumps over/ the lazy dog.\n"
    request = Message(code=PUT, payload=payload, uri="coap://192.168.1.229/devices")

    response = await context.request(request).response

    print('Result: %s\n%r'%(response.code, response.payload))

# if name == "main":
asyncio.run(main())          