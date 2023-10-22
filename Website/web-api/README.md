# Website Backend

API for updating, creating, and accessing buoy data.

## Repository **Overview:**

### Primary Languages/Technology:

- **JavaScript (Node.js)** for our backend. For a guide on getting started with Node, check out this [documentation](https://www.postgresql.org/docs/manuals/).
    - Our codebase uses Node.js version 16.15.0. Please use the commands below to use the correct version:
        
        ```bash
        nvm install 16.15.0
        nvm use 16.15.0
        ```
        
- **Postgres** for our relational database management system. For a guide on getting started with Postgres, check out this [documentation](https://www.postgresql.org/docs/manuals/).

### **Purpose/Functionality:**

- This repository serves as the website API for *updating, creating, and accessing buoy data*.

## **Repository Organization:**

- The repository is organized into the following directories:
    - **bin/www**: Starting and configuring the HTTP server.
    - **helpers**: Provides functionality for authorization and authentication.
    - **models**: Define and associate various models
    - **node_modules**: Contains all installed packages and dependencies, use with `npm`.
    - **public/stylesheets**: Define appearance of page.
    - **routes**: Define various CRUD (Create, Read, Update, Delete) operations for entities.
    - **views**: Set up view structure for pages.

## **Version Control and Workflow:**

Check out our [main documentation](https://github.com/GearyER/WWXS/tree/main#contribution) for more information.

## Development:

### Required Softwares

- git
- Docker
- docker-compose
- Node
- Node Version Manager (nvm)
- Node Package Manager (npm)

### Running

1. Install all the required node modules using:
    
    `npm install`
    
2. To build the image, run:
    
    `docker build -t wwxs-api .`
    

## Compile entire project

Run `npm install` on both web-ui and web-api root directories. Order does not matter.

Run `ng build` on web-ui root directory.

Run `docker build -t wwxs-ui .` in web-ui root directory.

Run `docker build -t wwxs-api .` in web-api root directory.

Run `docker compose up -d db` in documentation root directory.

Run `docker compose up -d api` in documentation root directory

Run `docker compose up -d ui` in documentation root directory.

### Example terminals:

```bash
web-ui % npm install
web-ui % ng build
web-ui % docker build -t wwxs-ui .
```

```bash
web-api % npm install
web-api % docker build -t wwxs-api .
```

```bash
web-documentation % docker compose up -d db
web-documentation % docker compose up -d api
web-documentation % docker compose up -d ui
```

### Docker

You should now be able to go to Docker Desktop and see 3 images all contained in one container.

1. database (db)
2. web-api (api)
3. web-ui (ui)

The 3 images can now communicate with one another. Click on the link under the web-ui image and it should direct you to [localhost:81/](http://localhost:81/) which holds the interface of logging in and seeing buoy data. *If you don't have Docker Desktop, user Docker CLI.*

If you are unfamiliar with Docker, run through this quick tutorial: [Docker Tutorial](https://docs.docker.com/get-started/).

## Debugging

### Filling the database

To seed the database, edit line 14 of models/index.js to true. This will reset the database and fill it with data from testdata.js every time an instance is started.

### Manually accessing database

Run `docker exec -it documentation-db-1 psql -U wwxs`

From there, you can use PostgreSQL commands (including SELECT etc.).

## Acknowledgments

*Please see [main repo](https://github.com/GearyER/WWXS/tree/main) for more information about the project*
