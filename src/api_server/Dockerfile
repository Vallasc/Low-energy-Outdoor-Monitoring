FROM node:18-alpine3.15

# Create app directory
RUN mkdir -p /usr/src/app/public
WORKDIR /usr/src/app

COPY . .
# Install app dependencies
RUN npm install

EXPOSE 80

CMD node ./src/server.js