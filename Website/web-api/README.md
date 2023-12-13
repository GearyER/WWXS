# WWXS-Backend

API for updating, creating and accessing buoy data.

#### Development:

Be sure to have the Node Package Manager and Docker installed.

First, install all the required node modules using:

```npm install```

To build the image, run:

```docker build -t wwxs-api .```


## Compiling the project
Run `npm install` on both web-ui and web-api root directories. Order does not matter.

Run `ng build` on web-ui root directory.

Run `docker build -t wwxs-ui .` in web-ui root directory.

Run `docker build -t wwxs-api .` in web-api root directory.

Run `docker compose up -d db` in documentation root directory.

Run `docker compose up -d api` in documentation root directory

Run `docker compose up -d ui` in documentation root directory.

You should now be able to go to Docker Desktop and see 3 images all contained in one container. The 3 images being, database image, web-api image and web-ui image. These 3 images can now communicate with one another. Click on the web-ui image and it should direct you to localhost which holds the interface of logging in and seeing buoy data. If you don't have Docker Desktop, user Docker CLI.

If you are unfamiliar with Docker, run through this quick tutorial. [Docker Tutorial](https://docs.docker.com/get-started/).


## Filling the database for testing
To seed the database, edit line 14 of models/index.js to true. This will reset the database and fill it with data from testdata.js every time an instance is started.

To access the database manually, run:

```docker exec -it documentation_db_1 psql -U wwxs```

From there, you can use PostgreSQL commands (including SELECT etc.).
