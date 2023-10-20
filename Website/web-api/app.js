const createError = require('http-errors');
const express = require('express');
const path = require('path');
const cookieParser = require('cookie-parser');
const logger = require('morgan');
const cors = require('cors');

const routes = require('./routes');
const { sequelize, models, databaseConfig } = require('./models');

const app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');
app.use(cors()); // TODO: Set up a domain whitelist

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

// context for interaction with db models
app.use(async (req, res, next) => {
  req.context = {
    models,
    loggedIn: null // TODO: I think this is resetting the logged in user for every new API call
  };
  next();
});

app.use('/', routes.render);
app.use('/session', routes.session);
app.use('/user', routes.user);
app.use('/group', routes.group);
app.use('/buoy', routes.buoy);
app.use('/data', routes.data);
app.use('/location', routes.location);

// catch 404 and forward to error handler
app.use((req, res, next) => {
  next(createError(404));
});

// error handler
app.use((err, req, res) => {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = {
  app, sequelize, models, databaseConfig
};
