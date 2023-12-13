const authorized = (sequelize, DataTypes) => {
  const Authorized = sequelize.define('authorized', {
    name: {
      type: DataTypes.STRING,
      unique: true
    }
  });

  Authorized.associate = (models) => {
    Authorized.belongsTo(models.User);
    Authorized.belongsTo(models.Buoy);
  };

  return Authorized;
};

module.exports = authorized;

