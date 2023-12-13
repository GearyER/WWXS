const history = (sequelize, DataTypes) => {
  const History = sequelize.define('history', {
    timestamp: {
      type: DataTypes.DATE
    }
  });

  History.associate = (models) => {
    History.belongsTo(models.Location);
    History.belongsTo(models.Buoy);
  };

  return History;
};

module.exports = history;
