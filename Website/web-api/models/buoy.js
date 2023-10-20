const buoy = (sequelize, DataTypes) => {
  const Buoy = sequelize.define('buoy', {
    name: {
      type: DataTypes.STRING,
      unique: true
    },
    mac: {
      type: DataTypes.STRING,
      unique: true
    },
    pubKey: {
      type: DataTypes.STRING,
      unique: true
    },
    lastRetrieval: {
      type: DataTypes.DATE
    },
    version: {
      type: DataTypes.STRING
    }
  });

  Buoy.associate = (models) => {
    Buoy.belongsTo(models.Location);
    Buoy.belongsTo(models.Group);
  };

  return Buoy;
};

module.exports = buoy;
