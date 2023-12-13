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
    },
    password: { // TODO: Don't use plaintext password
      type: DataTypes.STRING
    },
  },
  {
    defaultScope: {
      attributes: { exclude: ['password'] }
    }
  }
  );

  Buoy.associate = (models) => {
    Buoy.belongsTo(models.Location);
    Buoy.belongsTo(models.Group);
    Buoy.belongsTo(models.User); // owner
    // Buoy.belongsTo(models.History);
    // Buoy.belongsTo(models.Manager);
    // Buoy.belongsTo(models.Authorized);
  };

  return Buoy;
};

module.exports = buoy;
