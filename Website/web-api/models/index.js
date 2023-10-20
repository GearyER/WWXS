const Sequelize = require('sequelize');

const sequelize = new Sequelize(
  process.env.DATABASE,
  process.env.DATABASE_USER,
  process.env.DATABASE_PASSWORD,
  {
    dialect: 'postgres',
    port: process.env.DATABASE_PORT,
    host: process.env.DATABASE_HOST
  }
);

// change this to true to seed the database randomly every time we start the instance
const eraseDatabaseOnSync = true;

const databaseConfig = {
  force: eraseDatabaseOnSync
};

const models = {
  Group: sequelize.import('./group'),
  User: sequelize.import('./user'),
  Buoy: sequelize.import('./buoy'),
  Location: sequelize.import('./location'),
  Data: sequelize.import('./data')
};

Object.keys(models).forEach((key) => {
  if ('associate' in models[key]) {
    models[key].associate(models);
  }
});

module.exports = { sequelize, models, databaseConfig };
