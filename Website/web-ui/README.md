# Water Weather Station Web UI

This project was generated with [Angular CLI](https://github.com/angular/angular-cli) version 10.1.3.

## Installation

Run `sudo apt install npm` to install the Node Package Manager.

Run `sudo npm install -g @angular/cli@10.1.3` to install the Angular CLI globally on your system, which will allow you to run the `ng` command.

## Development server

Run `ng serve` for a dev server. Navigate to `http://localhost:4200/`. The app will automatically reload if you change any of the source files.

## Code scaffolding

Run `ng generate component component-name` to generate a new component. You can also use `ng generate directive|pipe|service|class|guard|interface|enum|module`.

## Build

Run `ng build` to build the project. The build artifacts will be stored in the `dist/` directory. Use the `--prod` flag for a production build.

Then, run `docker build -t wwxs-ui .` to build docker image.

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


## Running unit tests

Run `ng test` to execute the unit tests via [Karma](https://karma-runner.github.io).

## Running end-to-end tests

Run `ng e2e` to execute the end-to-end tests via [Protractor](http://www.protractortest.org/).

## Further help

To get more help on the Angular CLI use `ng help` or go check out the [Angular CLI README](https://github.com/angular/angular-cli/blob/master/README.md).
