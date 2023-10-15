all: web-api/img web-ui/img

run: web-api/img web-ui/img
	docker-compose up

run-detach: web-api/img web-ui/img
	docker-compose up -d

stop:
	docker-compose down

web-api/img: web-api/node_modules
	docker build -t wwxs-api --iidfile web-api/img web-api   

web-ui/img: web-ui/dist
	docker build -t wwxs-ui --iidfile web-ui/img web-ui

web-api/node_modules: web-api
	cd web-api && npm install

web-ui/node_modules: web-ui
	cd web-ui && npm install

web-ui/dist: web-ui/node_modules
	cd web-ui && ng build

web-api:
	mkdir web-api
	git clone git@gitlab.cs.wwu.edu:201920-waterwxstation/web-api.git web-api

web-ui:
	mkdir web-ui
	git clone git@gitlab.cs.wwu.edu:201920-waterwxstation/web-api.git web-api

clean:
	rm -rf web-api web-ui