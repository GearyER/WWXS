const data = (sequelize, DataTypes) => {
  const Data = sequelize.define('data', {
    timestamp: {
      type: DataTypes.DATE
    },
    surfTemp: {
      type: DataTypes.FLOAT
    },
    surfInsolation: {
      type: DataTypes.FLOAT
    },
    shallowSalinity: {
      type: DataTypes.FLOAT
    },
    shallowTemp: {
      type: DataTypes.FLOAT
    },
    depthTemp: {
      type: DataTypes.FLOAT
    },
    depthTurbidity: {
      type: DataTypes.FLOAT
    }
  });

  Data.associate = (models) => {
    // Data belongs to Location
    Data.belongsTo(models.Location);

    // Data belongs to Buoy
    Data.belongsTo(models.Buoy);
  };

  return Data;
};

module.exports = data;
