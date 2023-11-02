const history = (sequelize, DataTypes) => {
  const History = sequelize.define('history', {
    startDate: {
      type: DataTypes.DATE
    },
    endDate: {
      type: DataTypes.DATE
    },
  });

  History.associate = (models) => {
    History.belongsTo(models.Buoy);
    History.belongsTo(models.Location);
    History.hasMany(models.Data);
  };

  return History;
};

module.exports = history;
